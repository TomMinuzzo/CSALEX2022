/**
 * The <code>Joint</code> class is a abstract class which represents a joint in a
 * <code>Robot</code> objec. This class can be used to represent all types of joints,
 * including actuated, non-actuated, revolute, prismatic, etc.
 *
 *
 * Version 0.1
 * Date: 07/04/2020
 *
 */
#include "Joint.h"

Joint::Joint(int jointID, double jointMin, double jointMax, const std::string& n) : id(jointID), name(n), qMin(jointMin), qMax(jointMax), actuated(false) {
    position = 0;
    velocity = 0;
    torque = 0;
}

Joint::Joint(int jointID, double jointMin, double jointMax, double q0, const std::string& n) : id(jointID), name(n), qMin(jointMin), qMax(jointMax), actuated(false) {
    position = q0;
    velocity = 0;
    torque = 0;
}

Joint::Joint(int jointID, double jointMin, double jointMax, Drive *jointDrive, const std::string& n) : id(jointID), name(n), qMin(jointMin), qMax(jointMax), actuated(true) {
    position = 0;
    velocity = 0;
    torque = 0;
    this->drive = jointDrive;
}

Joint::Joint(int jointID, double jointMin, double jointMax, double q0, Drive *jointDrive, const std::string& n) : id(jointID), name(n), qMin(jointMin), qMax(jointMax), actuated(true) {
    position = q0;
    velocity = 0;
    torque = 0;
    this->drive = jointDrive;
    calibrated = false;
}

Joint::~Joint() {
    spdlog::debug("Joint object deleted");
}
int Joint::getId() {
    return id;
}

double Joint::getPosition() {
    return position;
}
double Joint::getVelocity() {
    return velocity;
}
double Joint::getTorque() {
    return torque;
}

void Joint::printStatus() {
    std::cout << "Joint " << name << "(ID " << id << ") @ pos " << getPosition() << " deg" << std::endl;
}

bool Joint::configureMasterPDOs(){
    return drive->configureMasterPDOs();
}

    // Methods if joint is actuated

bool Joint::updateValue() {
    position = updatePosition();
    velocity = updateVelocity();
    torque = updateTorque();

    return true;
}

ControlMode Joint::setMode(ControlMode driveMode_, motorProfile profile) {
    if (actuated) {
        if (driveMode_ == CM_POSITION_CONTROL) {
            if (drive->initPosControl(profile)) {
                driveMode = driveMode_;
                return CM_POSITION_CONTROL;
            }
        } else if (driveMode_ == CM_VELOCITY_CONTROL) {
            if (drive->initVelControl(profile)) {
                driveMode = driveMode_;
                return CM_VELOCITY_CONTROL;
            }
        } else if (driveMode_ == CM_TORQUE_CONTROL) {
            if (drive->initTorqueControl()) {
                driveMode = driveMode_;
                return CM_TORQUE_CONTROL;
            }
        }
    }
    return CM_UNACTUATED_JOINT;
}

ControlMode Joint::setMode(ControlMode driveMode_) {
    if (actuated) {
        if (driveMode_ == CM_POSITION_CONTROL) {
            if (drive->initPosControl()) {
                driveMode = driveMode_;
                return CM_POSITION_CONTROL;
            }
        } else if (driveMode_ == CM_VELOCITY_CONTROL) {
            if (drive->initVelControl()) {
                driveMode = driveMode_;
                return CM_VELOCITY_CONTROL;
            }
        } else if (driveMode_ == CM_TORQUE_CONTROL) {
            if (drive->initTorqueControl()) {
                driveMode = driveMode_;
                return CM_TORQUE_CONTROL;
            }
        }
    }
    return CM_UNACTUATED_JOINT;
}

setMovementReturnCode_t Joint::setPosition(double desQ) {
    if (actuated) {
        if (std::isfinite(desQ)) {
            if (driveMode == CM_POSITION_CONTROL) {
                if(id == 4 || id == 5){
                    std::cout << "[Joint:setPosition] Actuate Ankle " << id << " to " << desQ + q0 << std::endl;
                }
                else{
                    drive->setPos(jointPositionToDriveUnit(desQ + q0));
                    drive->posControlConfirmSP();
                }
                return SUCCESS;
            } else {
                return INCORRECT_MODE;
            }
        }
        else {
            spdlog::error("Joint {} set position to incorrect value ({})", id, desQ);
            return OUTSIDE_LIMITS;
        }
    }
    return UNACTUATED_JOINT;
}

setMovementReturnCode_t Joint::setVelocity(double velocity) {
    if (actuated) {
        if (std::isfinite(velocity)) {
            if (driveMode == CM_VELOCITY_CONTROL) {
                drive->setVel(jointVelocityToDriveUnit(velocity));
                return SUCCESS;
            } else {
                return INCORRECT_MODE;
            }
        } else {
            spdlog::error("Joint {} set velocity to incorrect value ({})", id, velocity);
            return OUTSIDE_LIMITS;
        }
    }
    return UNACTUATED_JOINT;
}

setMovementReturnCode_t Joint::setTorque(double torque) {
    if (actuated) {
        if (std::isfinite(torque)) {
            if (driveMode == CM_TORQUE_CONTROL) {
                drive->setTorque(jointTorqueToDriveUnit(torque));
                return SUCCESS;
            }
            return INCORRECT_MODE;
        }
        else {
            spdlog::error("Joint {} set torque to incorrect value ({})", id, torque);
            return OUTSIDE_LIMITS;
        }
    }
    return UNACTUATED_JOINT;
}

// Updating functions to access joint-level commands
void Joint::setPositionOffset(double qcalib = 0) {
    q0 = driveUnitToJointPosition(drive->getPos()) - qcalib;
    calibrated = true;
}

double Joint::updatePosition() {
    if (actuated) {
        return driveUnitToJointPosition(drive->getPos()) - q0;
    }
    return 0;
}

double Joint::updateVelocity() {
    if (actuated) {
        return driveUnitToJointVelocity(drive->getVel());
    }
    return 0;
}

double Joint::updateTorque() {
    if (actuated) {
        return driveUnitToJointTorque(drive->getTorque());
        spdlog::info("this happens");
    }
    spdlog::info("this other thing happens");

    return 0;
}

// Drive configuration methods
bool Joint::start() {
    if (actuated) {
        return drive->start();
    }
    else{
        return false;
    }

}

void Joint::resetErrors() {
    if (actuated) {
        drive->resetErrors();
    }
}

void Joint::readyToSwitchOn() {
    if (actuated) {
        drive->readyToSwitchOn();
    }
}

bool Joint::enable() {
    if (actuated) {
        if (drive->getState() == READY_TO_SWITCH_ON) {
            drive->enable();
            return true;
        }
    }
    return false;
}

bool Joint::disable() {
    if (actuated) {
        drive->readyToSwitchOn();  //Ready to switch on is also power off state
        return true;
    }
    return false;
}


// For Position Control
bool Joint::setPosControlContinuousProfile(bool continuous) {
    if (drive->getState() == ENABLED  && driveMode  == CM_POSITION_CONTROL && actuated) {
        return (drive->posControlSetContinuousProfile(continuous));
    }
    spdlog::error("SetPosControlContinuous: Drive is not enabled, in incorrect mode or not actuated");
    return false;
}