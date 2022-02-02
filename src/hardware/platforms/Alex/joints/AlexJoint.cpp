/**
 * @file AlexJoint.cpp
 * @author William Campbell
 * @brief 
 * @version 0.1
 * @date 2020-06-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "AlexJoint.h"

#include <iostream>



AlexJoint::AlexJoint(int jointID, double jointMin, double jointMax, JointDrivePairs jdp, Drive *drive) : Joint(jointID, jointMin, jointMax, drive) {
    spdlog::debug("Joint Created, JOINT ID: {}", this->id);
    JDSlope = (jdp.drivePosB - jdp.drivePosA) / (jdp.jointPosB - jdp.jointPosA);
    JDIntercept =jdp.drivePosA -  JDSlope * jdp.jointPosA ;
  
    // Do nothing else
}

bool AlexJoint::updateValue() {
    #ifndef VIRTUAL
    position = driveUnitToJointPosition(drive->getPos());
    velocity = driveUnitToJointVelocity(drive->getVel());
    torque = driveUnitToJointTorque(drive->getTorque());
    
    // FOR TESTING w/o real robot -> set current pos to last setPosition
#endif
#ifdef VIRTUAL
    position = lastQCommand;
#endif

    return true;
}

int AlexJoint::jointPositionToDriveUnit(double jointPosition) {

    return JDSlope * jointPosition + JDIntercept;
}

double AlexJoint::driveUnitToJointPosition(int driveValue) {
    return (driveValue - JDIntercept) / JDSlope;
}

int AlexJoint::jointVelocityToDriveUnit(double jointVelocity) {
    return (JDSlope * jointVelocity) * 10;
}

double AlexJoint::driveUnitToJointVelocity(int driveValue) {
    return ((driveValue) / (JDSlope * 10));
}

int AlexJoint::jointTorqueToDriveUnit(double jointTorque) {
    return jointTorque / (MOTOR_RATED_TORQUE * REDUCTION_RATIO / 1000.0);
}

double AlexJoint::driveUnitToJointTorque(int driveValue) {
    return driveValue * (MOTOR_RATED_TORQUE * REDUCTION_RATIO / 1000.0);
}

bool AlexJoint::initNetwork() {
    spdlog::debug("Joint::initNetwork()");
    if (drive->initPDOs()) {
        return true;
    } else {
        return false;
    }
    // For testing
    // return true;
}

double AlexJoint::getPosition() {
    return position;
}
double AlexJoint::getVelocity() {
    return velocity;
}
double AlexJoint::getTorque() {
    return torque;
}

void AlexJoint::setPositionOffset(double offset) {
    if(this->id==ALEX_LEFT_HIP || this->id==ALEX_RIGHT_HIP || this->id==ALEX_LEFT_KNEE  ||this->id==ALEX_RIGHT_KNEE ){
        ((CopleyDrive *)drive)->setPositionOffset(-jointPositionToDriveUnit(offset));
    }
    else{
        ((SchneiderDrive *)drive)->setPositionOffset(-jointPositionToDriveUnit(offset));
    }
}

bool AlexJoint::setOverloadBehaviour(UNSIGNED32 mask, double window) {
    if(this->id==ALEX_LEFT_HIP || this->id==ALEX_RIGHT_HIP || this->id==ALEX_LEFT_KNEE  ||this->id==ALEX_RIGHT_KNEE ){
        ((CopleyDrive *)drive)->setFaultMask(mask);
        ((CopleyDrive *)drive)->setTrackingWindow(abs(window*JDSlope));
    }
    else{   
        ((SchneiderDrive *)drive)->setFaultMask(mask);
        ((SchneiderDrive *)drive)->setTrackingWindow(abs(window*JDSlope));
    }
    return true;
}
