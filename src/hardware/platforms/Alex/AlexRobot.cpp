#include "AlexRobot.h"

static volatile sig_atomic_t exitHoming = 0;

AlexRobot::AlexRobot(AlexTrajectoryGenerator *tj) {
    trajectoryGenerator = tj;
    spdlog::debug("AlexRobot Created");
    
    // Set to something other than Init... as this is what it will be set to later
    setCurrentState(AlexState::Debug);

    // This is the default name accessed from the MACRO. If ROS is used, under demo machine robot name can be set
    // by setRobotName() to the ros node name. See X2DemoMachine::init()
    robotName_ = XSTR(X2_NAME);

#ifdef NOROBOT
    simJointPositions_ = Eigen::VectorXd::Zero(ALEX_NUM_JOINTS);
    simJointVelocities_ = Eigen::VectorXd::Zero(ALEX_NUM_JOINTS);
    simJointTorques_ = Eigen::VectorXd::Zero(ALEX_NUM_JOINTS);
#endif

    // Initializing the parameters to zero
    x2Parameters.m = Eigen::VectorXd::Zero(ALEX_NUM_JOINTS);
    x2Parameters.l = Eigen::VectorXd::Zero(ALEX_NUM_JOINTS);
    x2Parameters.s = Eigen::VectorXd::Zero(ALEX_NUM_JOINTS);
    x2Parameters.I = Eigen::VectorXd::Zero(ALEX_NUM_JOINTS);
    x2Parameters.c0 = Eigen::VectorXd::Zero(ALEX_NUM_JOINTS);
    x2Parameters.c1 = Eigen::VectorXd::Zero(ALEX_NUM_JOINTS);
    x2Parameters.c2 = Eigen::VectorXd::Zero(ALEX_NUM_JOINTS);
    initialiseJoints();
    initialiseInputs();
}

AlexRobot::~AlexRobot() {
    freeMemory();
    spdlog::debug("AlexRobot deleted");
}


void AlexRobot::resetErrors() {
    spdlog::debug("Clearing errors on all motor drives ");
    for (auto p : joints) {
        // Put into ReadyToSwitchOn()
        p->resetErrors();
    }
}


bool AlexRobot::initPositionControl() {
    spdlog::debug("Initialising Position Control on all joints ");
    bool returnValue = true;
    for (auto p : joints) {
        if (p->setMode(CM_POSITION_CONTROL, posControlMotorProfile) != CM_POSITION_CONTROL) {
            // Something back happened if were are here
            spdlog::error("Something bad happened");
            returnValue = false;
        }
        // Put into ReadyToSwitchOn()
        p->readyToSwitchOn();
    }

    // Pause for a bit to let commands go
    usleep(2000);
    for (auto p : joints) {
        p->enable();
    }

#ifdef SIM
    controllerSwitchMsg_.request.start_controllers = {"position_controller"};
    controllerSwitchMsg_.request.stop_controllers = {"velocity_controller", "torque_controller"};
    controllerSwitchMsg_.request.strictness = 1;
    controllerSwitchMsg_.request.start_asap = true;
    controllerSwitchMsg_.request.timeout = 0.0;

    if (controllerSwitchClient_.call(controllerSwitchMsg_)) {
        spdlog::info("Switched to position controller");
    } else {
        spdlog::error("Failed switching to position controller");
        returnValue = false;
    }
#endif

    return returnValue;
}


bool AlexRobot::initVelocityControl() {
    spdlog::debug("Initialising Velocity Control on all joints ");
    bool returnValue = true;
    for (auto p : joints) {
        if (p->setMode(CM_VELOCITY_CONTROL, velControlMotorProfile) != CM_VELOCITY_CONTROL) {
            // Something back happened if were are here
            spdlog::error("Something bad happened");
            returnValue = false;
        }
        // Put into ReadyToSwitchOn()
        p->readyToSwitchOn();
    }

    // Pause for a bit to let commands go
    usleep(10000);
    for (auto p : joints) {
        p->enable();
    }

#ifdef SIM
    controllerSwitchMsg_.request.start_controllers = {"velocity_controller"};
    controllerSwitchMsg_.request.stop_controllers = {"position_controller", "torque_controller"};
    controllerSwitchMsg_.request.strictness = 1;
    controllerSwitchMsg_.request.start_asap = true;
    controllerSwitchMsg_.request.timeout = 0.0;

    if (controllerSwitchClient_.call(controllerSwitchMsg_)) {
        spdlog::info("Switched to velocity controller");
    } else {
        spdlog::error("Failed switching to velocity controller");
        returnValue = false;
    }
#endif

    return returnValue;
}

bool AlexRobot::initTorqueControl() {
    spdlog::debug("Initialising Torque Control on all joints ");
    bool returnValue = true;
    for (auto p : joints) {
        if (p->setMode(CM_TORQUE_CONTROL) != CM_TORQUE_CONTROL) {
            // Something back happened if were are here
            spdlog::error("Something bad happened");
            returnValue = false;
        }
        // Put into ReadyToSwitchOn()
        p->readyToSwitchOn();
    }

    // Pause for a bit to let commands go
    usleep(2000);
    for (auto p : joints) {
        p->enable();
    }

#ifdef SIM
    controllerSwitchMsg_.request.start_controllers = {"torque_controller"};
    controllerSwitchMsg_.request.stop_controllers = {"position_controller", "velocity_controller"};
    controllerSwitchMsg_.request.strictness = 1;
    controllerSwitchMsg_.request.start_asap = true;
    controllerSwitchMsg_.request.timeout = 0.0;

    if (controllerSwitchClient_.call(controllerSwitchMsg_)) {
        spdlog::info("Switched to torque controller");
    } else {
        spdlog::error("Failed switching to torque controller");
        returnValue = false;
    }
#endif

    return returnValue;
}

bool AlexRobot::setPosControlContinuousProfile(bool continuous){
    bool returnValue = true;
    for (auto p : joints) {
        if(!(p->setPosControlContinuousProfile(continuous))){
            returnValue = false;
        }
    }
    return returnValue;
}


setMovementReturnCode_t AlexRobot::setPosition(Eigen::VectorXd positions) {
    int i = 0;
    setMovementReturnCode_t returnValue = SUCCESS;
    for (auto p : joints) {
        if (i < ALEX_ACTUAL_NUM_JOINTS){
            #ifdef NOROBOT
            spdlog::debug("Joint {}, Target {}, Current {}", i, positions[i], simJointPositions_(i));
            #else
            spdlog::debug("Joint {}, Target {}, Current {}", i, rad2deg(positions[i]), rad2deg(((AlexJoint *)p)->getPosition()));
            #endif

            setMovementReturnCode_t setPosCode = ((AlexJoint *)p)->setPosition(positions[i]);
            if (setPosCode == INCORRECT_MODE) {
                spdlog::error("Joint {} is not in Position Control ", p->getId());
                returnValue = INCORRECT_MODE;
            } else if (setPosCode != SUCCESS) {
                // Something bad happened
                spdlog::error("Joint {} Unknown Error", p->getId());
                returnValue = UNKNOWN_ERROR;
            }
        }
        i++;
    }

#ifdef SIM
    std::vector<double> positionVector(ALEX_NUM_JOINTS);

    for (int i = 0; i < ALEX_NUM_JOINTS; i++) {
        positionVector[i] = positions[i];
    }

    positionCommandMsg_.data = positionVector;
    positionCommandPublisher_.publish(positionCommandMsg_);
#elif NOROBOT
    simJointPositions_ = positions;
#endif

    return returnValue;
}

setMovementReturnCode_t AlexRobot::setVelocity(Eigen::VectorXd velocities) {
    int i = 0;
    setMovementReturnCode_t returnValue = SUCCESS;
    for (auto p : joints) {
        if (i < ALEX_ACTUAL_NUM_JOINTS){
            setMovementReturnCode_t setPosCode = ((AlexJoint *)p)->setVelocity(velocities[i]);
            if (setPosCode == INCORRECT_MODE) {
                spdlog::error("Joint {} is not in Velocity Control", p->getId());
                returnValue = INCORRECT_MODE;
            } else if (setPosCode != SUCCESS) {
                // Something bad happened
                spdlog::error("Joint {} Unknown Error", p->getId());
                returnValue = UNKNOWN_ERROR;
            }
            i++;
        }
    }

#ifdef SIM
    std::vector<double> velocityVector(ALEX_NUM_JOINTS);

    for (int i = 0; i < ALEX_NUM_JOINTS; i++) {
        velocityVector[i] = velocities[i];
    }

    velocityCommandMsg_.data = velocityVector;
    velocityCommandPublisher_.publish(velocityCommandMsg_);
#endif

    return returnValue;
}

setMovementReturnCode_t AlexRobot::setTorque(Eigen::VectorXd torques) {
    int i = 0;
    setMovementReturnCode_t returnValue = SUCCESS;
    for (auto p : joints) {
        if (i < ALEX_ACTUAL_NUM_JOINTS){
            setMovementReturnCode_t setPosCode = ((AlexJoint *)p)->setTorque(torques[i]);
            if (setPosCode == INCORRECT_MODE) {
                spdlog::error("Joint {} is not in Torque Control", p->getId());
                returnValue = INCORRECT_MODE;
            } else if (setPosCode != SUCCESS) {
                // Something bad happened
                spdlog::error("Joint {} Unknown Error", p->getId());
                returnValue = UNKNOWN_ERROR;
            }
        }
        i++;
    }

#ifdef SIM
    std::vector<double> torqueVector(ALEX_NUM_JOINTS);

    for (int i = 0; i < ALEX_NUM_JOINTS; i++) {
        torqueVector[i] = torques[i];
    }

    torqueCommandMsg_.data = torqueVector;
    torqueCommandPublisher_.publish(torqueCommandMsg_);
#endif

    return returnValue;
}

Eigen::VectorXd &AlexRobot::getPosition() {
#ifndef NOROBOT
    return Robot::getPosition();
#else
    spdlog::info("test");
    return simJointPositions_;
#endif
}

Eigen::VectorXd &AlexRobot::getVelocity() {
#ifndef NOROBOT
    return Robot::getVelocity();
#else
    return simJointVelocities_;
#endif
}

Eigen::VectorXd &AlexRobot::getTorque() {
#ifndef NOROBOT
    return Robot::getTorque();
#else
    return simJointTorques_;
#endif
}

// Homing Functionality. Drives the Joints in a given direction at a low speed until it stops moving (i.e. hits a stop)
// This position is then set as the joint limit. 
bool AlexRobot::homing(std::vector<int> homingDirection, float thresholdTorque, float delayTime,
                     float homingSpeed, float maxTime) {
    std::vector<bool> success(ALEX_NUM_JOINTS, false);
    std::chrono::steady_clock::time_point time0;
    this->initVelocityControl();
    signal(SIGINT, signalHandler); // check if ctrl + c is pressed

    for (int i = 0; i < ALEX_ACTUAL_NUM_JOINTS; i++) {
        if (homingDirection[i] == 0) continue;  // skip the joint if it is not asked to do homing

        Eigen::VectorXd desiredVelocity(ALEX_ACTUAL_NUM_JOINTS);
        std::chrono::steady_clock::time_point firstTimeHighTorque;  // time at the first time joint exceed thresholdTorque
        bool highTorqueReached = false;

        desiredVelocity[i] = homingSpeed * homingDirection[i] / std::abs(homingDirection[i]);  // setting the desired velocity by using the direction
        time0 = std::chrono::steady_clock::now();

        spdlog::info("Homing Joint {} ...", i);

        while (success[i] == false &&
                exitHoming == 0 &&
               std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time0).count() < maxTime * 1000) {
            this->updateRobot();  // because this function has its own loops, updateRobot needs to be called
            this->setVelocity(desiredVelocity);
            usleep(10000);

            if (std::abs(this->getTorque()[i]) >= thresholdTorque) {  // if high torque is reached
                highTorqueReached = true;
                firstTimeHighTorque = std::chrono::steady_clock::now();
                while (std::chrono::duration_cast<std::chrono::milliseconds>  // high torque should be measured for delayTime
                       (std::chrono::steady_clock::now() - firstTimeHighTorque).count() < delayTime * 1000 &&
                        exitHoming == 0) {
                    this->updateRobot();
                    usleep(10000);

                    if (std::abs(this->getTorque()[i]) < thresholdTorque) {  // if torque value reach below thresholdTorque, goes back
                        spdlog::debug("Torque drop", this->getTorque()[i]);
                        highTorqueReached = false;
                        break;
                    }
                }
            }
            success[i] = highTorqueReached;
        }

        if (success[i]) {
            spdlog::info("Homing Succeeded for Joint {} .", i);
            usleep(10000);
            if (i == ALEX_LEFT_HIP || i == ALEX_RIGHT_HIP) {  // if it is a hip joint

                // zeroing is done depending on the limits on the homing direction
                if (homingDirection[i] > 0)
                    ((AlexJoint *)this->joints[i])->setPositionOffset(AlexJointLimits.hipMax);
                else
                    ((AlexJoint *)this->joints[i])->setPositionOffset(AlexJointLimits.hipMin);
            } else if (i == ALEX_LEFT_KNEE || i == ALEX_RIGHT_KNEE) {  // if it is a knee joint
                // zeroing is done depending on the limits on the homing direction
                if (homingDirection[i] > 0)
                    ((AlexJoint *)this->joints[i])->setPositionOffset(AlexJointLimits.kneeMax);
                else
                    ((AlexJoint *)this->joints[i])->setPositionOffset(AlexJointLimits.kneeMin);
            }

        } else {
            spdlog::error("Homing Failed for Joint {} .", i);
        }
    }
    // Checking if all commanded joint successfully homed
    for (int i = 0; i < ALEX_NUM_JOINTS; i++) {
        if (homingDirection[i] == 0) continue;  // skip the joint if it is not asked to do homing
        if (success[i] == false) return false;
    }
    return true;  // will come here if all joints successfully homed
}



void AlexRobot::signalHandler(int signum) {
    exitHoming = 1;
    std::raise(SIGTERM); //Clean exit
}

bool AlexRobot::initialiseInputs() {
    spdlog::info("initinputs");
    inputs.push_back(keyboard = new Keyboard());
    inputs.push_back(buttons = new Buttons());
    // Should also Construct an Alex Crutch Input here
    inputs.push_back(pb = new ALEXCrutchController());
    return true;
}

bool AlexRobot::initializeRobotParams(std::string robotName) {

    // need to use address of base directory because when run with ROS, working directory is ~/.ros
    std::string baseDirectory = XSTR(BASE_DIRECTORY);
    std::string relativeFilePath = "/config/x2_params.yaml";

    YAML::Node params = YAML::LoadFile(baseDirectory + relativeFilePath);

    // if the robotName does not match with the name in x2_params.yaml
    if(!params[robotName]){
        spdlog::error("Parameters of {} couldn't be found in {} !", robotName, baseDirectory + relativeFilePath);
        spdlog::error("All parameters are zero !");

        return false;
    }

    // getting the parameters from the yaml file
    for(int i = 0; i<ALEX_NUM_JOINTS; i++){
        x2Parameters.m[i] = params[robotName]["m"][i].as<double>();
        x2Parameters.l[i] = params[robotName]["l"][i].as<double>();
        x2Parameters.s[i] = params[robotName]["s"][i].as<double>();
        x2Parameters.I[i] = params[robotName]["I"][i].as<double>();
        x2Parameters.c0[i] = params[robotName]["c0"][i].as<double>();
        x2Parameters.c1[i] = params[robotName]["c1"][i].as<double>();
        x2Parameters.c2[i] = params[robotName]["c2"][i].as<double>();
    }
   /* for(int i = 0; i<X2_NUM_FORCE_SENSORS; i++) {
        x2Parameters.cuffWeights[i] = params[robotName]["cuff_weights"][i].as<double>();
        x2Parameters.forceSensorScaleFactor[i] = params[robotName]["force_sensor_scale_factor"][i].as<double>();
    }*/

    return true;
}

void AlexRobot::freeMemory() {
    for (auto p : joints) {
        spdlog::debug("Delete Joint ID: {}", p->getId());
        delete p;
    }
    for (auto p : motorDrives) {
        spdlog::debug("Delete Drive Node: {}", p->getNodeID());
        delete p;
    }
    for (auto p : inputs) {
        spdlog::debug("Deleting Input");
        delete p;
    }
}

void AlexRobot::startNewTraj() {
    // Index Resetting
    currTrajProgress = 0;
    clock_gettime(CLOCK_MONOTONIC, &prevTime);
}

bool AlexRobot::moveThroughTraj() {
    bool returnValue = true;
    timespec currTime;
    clock_gettime(CLOCK_MONOTONIC, &currTime);

    double elapsedSec = currTime.tv_sec - prevTime.tv_sec + (currTime.tv_nsec - prevTime.tv_nsec) / 1e9;
    double trajTimeUS = trajectoryGenerator->getStepDuration();
    prevTime = currTime;

    // This should check to make sure that the "GO" button is pressed.
    if (getGo()) {
        currTrajProgress += elapsedSec;
        double fracTrajProgress = currTrajProgress / trajTimeUS;
        std::vector<double> setPoints = trajectoryGenerator->getSetPoint(fracTrajProgress);
        Eigen::VectorXd eigSetPoints = Eigen::Map<Eigen::Matrix<double, ALEX_NUM_JOINTS, 1>>(setPoints.data());

        //spdlog::info("{}, {:03.2f}, {:03.2f}", fracTrajProgress, rad2deg(setPoints[0]), rad2deg(setPoints[1]));
        returnValue = (setPosition(eigSetPoints) == SUCCESS);
    } 

    return returnValue;
}

bool AlexRobot::initialiseJoints() {
    for (int id = 0; id < ALEX_NUM_JOINTS; id++) {
        motorDrives.push_back(new CopleyDrive(id + 1));
        // The X2 has 2 Hips and 2 Knees, by default configured as 2 hips, then 2 legs int jointID, double jointMin, double jointMax, JointDrivePairs jdp, Drive *drive
        if (id == ALEX_LEFT_HIP || id == ALEX_RIGHT_HIP) {
            joints.push_back(new AlexJoint(id, AlexJointLimits.hipMin, AlexJointLimits.hipMax, ALEXhipJDP, motorDrives[id]));
        } else if (id == ALEX_LEFT_KNEE || id == ALEX_RIGHT_KNEE) {
            joints.push_back(new AlexJoint(id, AlexJointLimits.kneeMin, AlexJointLimits.kneeMax,  ALEXkneeJDP, motorDrives[id]));
        } else {  // is an ankle  ->  CHANGE DRIVE to Schneider drives NOT COPLEY
           // Drives.push_back(new SchneiderDrive(id + 1));
           // joints.push_back(new AlexJoint(id, jointMinMap[id], jointMaxMap[id], Drives[id], ankleParam));
           
           // DO NOTHING FOR NOW - WILL NEED TO ADD ANKLES
        }
        
    }
    return true;
}

bool AlexRobot::initialiseNetwork() {
    spdlog::debug("AlexRobot::initialiseNetwork()");
#ifndef VIRTUAL
    bool status;
    for (auto joint : joints) {
        status = joint->initNetwork();
        if (!status)
            return false;
    }
#endif
    return true;
}

bool AlexRobot::setJointOverloadBehaviour(UNSIGNED32 mask, double window) {
    spdlog::debug("AlexRobot::setJointOverloadBehaviour()");
#ifndef VIRTUAL
    bool status;
    for (auto joint : joints) {
        status = ((AlexJoint *)joint)->setOverloadBehaviour(mask, window);
        if (!status)
            return false;
    }
#endif
    return true;
}

void AlexRobot::updateRobot() {
    Robot::updateRobot();
    currentMovement = pb->getNextMovement();
}

double AlexRobot::getCurrTrajProgress() {
    return currTrajProgress;
}

std::vector<double> AlexRobot::getJointStates() {
    std::vector<double> robotJointspace;
    int i = 0;
    for (auto joint : joints) {
        #ifdef NOROBOT
            robotJointspace.push_back(simJointPositions_(i));
        #else
            robotJointspace.push_back(joint->getPosition());
        #endif
        i++;
    }
    #ifdef NOROBOT
        spdlog::info("Current State: {:03.2f}, {:03.2f}, {:03.2f}, {:03.2f}", rad2deg(simJointPositions_(0)), rad2deg(simJointPositions_(1)), rad2deg(simJointPositions_(2)), rad2deg(simJointPositions_(3)));
    #endif 
    return robotJointspace;
}

void AlexRobot::setCurrentMotion(RobotMode mode) {
    currentMovement = static_cast<int>(mode);
}

RobotMode AlexRobot::getCurrentMotion() {
    return static_cast<RobotMode>(currentMovement);
}

void AlexRobot::setNextMotion(RobotMode mode) {
    pb->setNextMovement(static_cast<UNSIGNED8>(mode));
}

RobotMode AlexRobot::getNextMotion() {
    return static_cast<RobotMode>(pb->getNextMovement());
}

void AlexRobot::setCurrentState(AlexState state) {
    currentState = static_cast<UNSIGNED8>(state);
}

bool AlexRobot::getGo() {
    return pb->getGo();
}

void AlexRobot::setResetFlag(bool value) {
   // resetTrajectory = value;
   // Do nothing
}

bool AlexRobot::getResetFlag() {
    return resetTrajectory;
}

bool AlexRobot::disableJoints() {
    bool tmp = true;
    for (auto p : joints) {
        if (((Joint *)p)->disable() == false) {
            spdlog::error("Drive {} failed to be disabled!", p->getId());
            tmp = false;
        }
    }

    return tmp;
}

bool AlexRobot::configureMasterPDOs() {
    spdlog::info("AlexRobot::ConfigureMasterPDOs");

    void *currStateVar[1] = {(void *) &currentState};
    UNSIGNED16 currStateSize[1] = {1};
    currStateTPDO = new TPDO(0x211, 0xFF, currStateVar, currStateSize, 1);

    void *currMovVar[1] = {(void *) &currentMovement};
    UNSIGNED16 currMovSize[1] = {1};
    currMovTPDO = new TPDO(0x212, 0xFF, currMovVar, currMovSize, 1);

    return Robot::configureMasterPDOs();
}

#ifdef VIRTUAL
void AlexRobot::setVirtualPosition(double angle) {
    for (auto joint : joints) {
        ((AlexJoint *)joint)->setPosition(angle);
    }
}
#endif 