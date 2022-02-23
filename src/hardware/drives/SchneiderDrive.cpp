/**
 * @brief An implementation of the Drive Object, specifically for the Schneider Drive
 *
 */
#include "SchneiderDrive.h"

#include <iostream>

SchneiderDrive::SchneiderDrive(int NodeID) : Drive::Drive(NodeID) {
    this->NodeID = NodeID;
}
SchneiderDrive::~SchneiderDrive() {
    spdlog::debug("SchneiderDrive Deleted");
}

bool SchneiderDrive::init() {
    return false;
}

bool SchneiderDrive::initPosControl(motorProfile posControlMotorProfile) {
    spdlog::debug("Node     ID {} Initialising Position Control", NodeID);

    sendSDOMessages(generatePosControlConfigSDO(posControlMotorProfile));
    /**
     * \todo Move jointMinMap and jointMaxMap to set additional parameters (bit 5 in 0x6041 makes updates happen immediately)
     *
     */
    return true;
}

bool SchneiderDrive::initVelControl(motorProfile velControlMotorProfile) {
    //spdlog::debug("NodeID {} Initialising Velocity Control", NodeID);
    /**
     * \todo create velControlMOTORPROFILE and test on exo
     * \todo Tune velocity loop gain index 0x2381 to optimize V control
     *
    */
    //sendSDOMessages(generateVelControlConfigSDO(velControlMotorProfile));
    return false;
}

bool SchneiderDrive::initTorqueControl() {
    //spdlog::debug("NodeID {} Initialising Torque Control", NodeID);
    //sendSDOMessages(generateTorqueControlConfigSDO());

    return false;
}

std::vector<std::string> SchneiderDrive::generatePosControlConfigSDO(motorProfile positionProfile) {
    return Drive::generatePosControlConfigSDO(positionProfile); /*<!execute base class function*/
};

std::vector<std::string> SchneiderDrive::generateVelControlConfigSDO(motorProfile velocityProfile) {
    return Drive::generateVelControlConfigSDO(velocityProfile); /*<!execute base class function*/
};

std::vector<std::string> SchneiderDrive::generateTorqueControlConfigSDO() {
    return Drive::generateTorqueControlConfigSDO(); /*<!execute base class function*/
}

std::vector<std::string> SchneiderDrive::generatePositionOffsetSDO(int offset) {
    // Define Vector to be returned as part of this method
    std::vector<std::string> CANCommands;
    // Define stringstream for ease of constructing hex strings
    std::stringstream sstream;

    // set mode of operation
    sstream << "[1] " << NodeID << " write 0x6060 0 i8 6";
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());
    // set the home offset
    sstream << "[1] " << NodeID << " write 0x607C 0 i32 "<< std::dec << offset;
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());
    // set homing method to 0
    sstream << "[1] " << NodeID << " write 0X6098 0 i8 0";
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());
    // set control word to start homing
    sstream << "[1] " << NodeID << " write 0x6040 0 u16 0x0f";
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());
    // set control word to start homing
    sstream << "[1] " << NodeID << " write 0x6040 0 u16 0x1f";
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());
    return CANCommands;
}

bool SchneiderDrive::setPositionOffset(int offset) {
    spdlog::debug("NodeID {} Setting Position Offset", NodeID);

    sendSDOMessages(generatePositionOffsetSDO(offset));

    return true;


}


bool SchneiderDrive::setTrackingWindow(INTEGER32 window) {
    spdlog::debug("NodeID {} Tracking Window", NodeID);

    std::vector<std::string> CANCommands;
    // Define stringstream for ease of constructing hex strings
    std::stringstream sstream;
    sstream << "[1] " << NodeID << " write 0x2120 0 i32 " << std::dec << window;
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    sendSDOMessages(CANCommands);

    return true;
}

bool SchneiderDrive::setFaultMask(UNSIGNED32 mask) {
    spdlog::debug("NodeID {} Fault mask set to {0:x}", NodeID, mask);

    std::vector<std::string> CANCommands;
    // Define stringstream for ease of constructing hex strings
    std::stringstream sstream;
    sstream << "[1] " << NodeID << " write 0x2182 0 i32 " << std::dec << mask;
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    sendSDOMessages(CANCommands);

    return true;
}


bool Drive::initPDOs() {
    spdlog::debug("Drive::initPDOs");

    // Calculate COB_ID. If TPDO:
    //int COB_ID = 0x100 * PDO_Num + 0x80 + NodeID;
    int TPDO_Num = 1;
    spdlog::debug("Set up STATUS_WORD TPDO on Node {}", NodeID);
    if (sendSDOMessages(generateTPDOConfigSDO(TPDO_MappedObjects[TPDO_Num], TPDO_Num, TPDO_COBID[TPDO_Num] + NodeID, 0xFF)) < 0) {
        spdlog::error("Set up STATUS_WORD TPDO FAILED on node {}", NodeID);
        return false;
    } 

    spdlog::debug("Set up ACTUAL_POS and ACTUAL_VEL TPDO on Node {}", NodeID);
    TPDO_Num = 2;
    if (sendSDOMessages(generateTPDOConfigSDO(TPDO_MappedObjects[TPDO_Num], TPDO_Num, TPDO_COBID[TPDO_Num] + NodeID, 0x01)) < 0) {
        spdlog::error("Set up ACTUAL_POS and ACTUAL_VEL TPDO FAILED on node {}", NodeID);
        return false;
    } 

    /*
    spdlog::debug("Set up ACTUAL_TOR TPDO on Node {}", NodeID);
    TPDO_Num = 3;
    if (sendSDOMessages(generateTPDOConfigSDO(TPDO_MappedObjects[TPDO_Num], TPDO_Num, TPDO_COBID[TPDO_Num] + NodeID, 0x01)) < 0) {
        spdlog::error("Set up ACTUAL_TOR TPDO FAILED on node {}", NodeID);
        return false;
    } 
    */

    // Calculate COB_ID. If RPDO:
    //int COB_ID = 0x100 * (PDO_Num+1) + NodeID;
    spdlog::debug("Set up CONTROL_WORD RPDO on Node {}", NodeID);
    int RPDO_Num = 1;
    if (sendSDOMessages(generateRPDOConfigSDO(RPDO_MappedObjects[RPDO_Num], RPDO_Num, RPDO_COBID[RPDO_Num] + NodeID, 0xff)) < 0) {
        spdlog::error("Set up CONTROL_WORD RPDO FAILED on node {}", NodeID);
        return false;
    } 
    spdlog::debug("Set up TARGET_POS RPDO on Node {}", NodeID);
    RPDO_Num = 2;
    if (sendSDOMessages(generateRPDOConfigSDO(RPDO_MappedObjects[RPDO_Num], RPDO_Num, RPDO_COBID[RPDO_Num] + NodeID, 0xff)) < 0) {
        spdlog::error("Set up TARGET_POS RPDO FAILED on node {}", NodeID);
        return false;
    }
    /*
    spdlog::debug("Set up TARGET_VEL RPDO on Node {}", NodeID);
    RPDO_Num = 3;
    if (sendSDOMessages(generateRPDOConfigSDO(RPDO_MappedObjects[RPDO_Num], RPDO_Num, RPDO_COBID[RPDO_Num] + NodeID, 0xff)) < 0) {
        spdlog::error("Set up ARGET_VEL RPDO FAILED on node {}", NodeID);
        return false;
    } 
    spdlog::debug("Set up TARGET_TOR RPDO on Node {}", NodeID);
    RPDO_Num = 4;
    if (sendSDOMessages(generateRPDOConfigSDO(RPDO_MappedObjects[RPDO_Num], RPDO_Num, RPDO_COBID[RPDO_Num] + NodeID, 0xff)) < 0) {
        spdlog::error("Set up TARGET_TOR RPDO FAILED on node {}", NodeID);
        return false;
    } 
    return true;
    }
    */

