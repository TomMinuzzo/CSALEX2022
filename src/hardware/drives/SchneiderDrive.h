
/**
 * \file SchneiderDrive.h
 * \author Justin Fong
 * \brief  An implementation of the Drive Object, specifically for the Schneider Drive
 *
 * This class enables low level functions to the system. It does limited error
 * checking.
 * \version 0.1
 * \date 2020-04-07
 * \version 0.1
 * \copyright Copyright (c) 2020
 *
 */
#ifndef SCHNEIDERDRIVE_H_INCLUDED
#define SCHNEIDERDRIVE_H_INCLUDED
#include "Drive.h"

/**
 * \brief An implementation of the Drive Object, specifically for Schneider-branded devices (currently used on the X2 Exoskeleton)
 *
 */
class SchneiderDrive : public Drive {
   public:
    /**
         * \brief Construct a new Schneider Drive object
         *
         * \param NodeID CANopen Node ID
         */
    SchneiderDrive(int NodeID);

    /**
         * \brief Destroy the Schneider Drive object
         *
         */
    ~SchneiderDrive();
    /**
         * Initialises the drive (SDO start message)
         *
         * \return True if successful, False if not
         */
    bool init();
    /**
     * \todo Move jointMinMap and jointMaxMap to set additional parameters (bit 5 in 0x6041 makes updates happen immediately)
     *
     */
    /**
         * Sets the drive to Position control with default parameters (through SDO messages)
         *
         * Note: Should be overloaded to allow parameters to be set
         *
         * \return true if successful
         * \return false if not
         */
    bool initPosControl(motorProfile posControlMotorProfile);
    /**
         * Sets the drive to Velocity control with default parameters (through SDO messages)
         *
         * Note: Should be overloaded to allow parameters to be set
         *
         * \return true if successful
         * \return false if not
         */
    bool initVelControl(motorProfile velControlMotorProfile);

    /**
         * Sets the drive to Torque control with default parameters (through SDO messages)
         *
         * Note: Should be overloaded to allow parameters to be set
         *
         * \return true if successful
         * \return false if not
         */
    bool initTorqueControl();
    /**
          * \brief Overloaded method from Drive, specifically for Schneider Drive implementation.
          *     Generates the list of commands required to configure Position control in CANopen motor drive
          *
          * /param Profile Velocity, value used by position mode motor trajectory generator.
          *            Units: 0.1 counts/sec
          *            Range:0 - 500,000,000
          * /param Profile Acceleration, value position mode motor trajectory generator will attempt to achieve.
          *            Units: 10 counts/sec^2
          *            Range:0 - 200,000,000
          * /param Profile Deceleration, value position mode motor trajectory generator will use at end of trapezoidal profile.
          *             see programmers manual for other profile types use.
          *            Units: 10 counts/sec^2
          *            Range:0 - 200,000,000
          *
          *    NOTE: More details on params and profiles can be found in the CANopne CiA 402 series specifications:
          *           https://www.can-cia.org/can-knowledge/canopen/cia402/
          *
          */

    std::vector<std::string> generatePosControlConfigSDO(motorProfile positionProfile);
    /**
          * \brief Overloaded method from Drive, specifically for Schneider Drive implementation.
          *     Generates the list of commands required to configure Velocity control in CANopen motor drive
          *
          * /param Profile Acceleration, value Velocity mode motor trajectory generator will attempt to achieve.
          *            Units: 10 counts/sec^2
          *            Range:0 - 200,000,000
          * /param Profile Deceleration, value Velocity mode motor trajectory generator will use at end of trapezoidal profile.
          *             see programmers manual for other profile types use.
          *            Units: 10 counts/sec^2
          *            Range:0 - 200,000,000
          *
          *    NOTE: More details on params and profiles can be found in the CANopne CiA 402 series specifications:
          *           https://www.can-cia.org/can-knowledge/canopen/cia402/
          *
          */
    std::vector<std::string> generateVelControlConfigSDO(motorProfile velocityProfile);
    /**
          * \brief Overloaded method from Drive, specifically for Schneider Drive implementation.
          *     Generates the list of commands required to configure Torque control in CANopen motor drive
          *
          *    NOTE: More details on params and profiles can be found in the CANopne CiA 402 series specifications:
          *           https://www.can-cia.org/can-knowledge/canopen/cia402/
          *
          */
    std::vector<std::string> generateTorqueControlConfigSDO();

    /**
          * \brief Generates the SDO commands to set the current position as offset
          *
          * /param offset, joint position value to be at the homing position [encoder count]
          *
          *
          */
    std::vector<std::string> generatePositionOffsetSDO(int offset);

    /**
          * \brief Set the current position as offset
          *
          * /param offset, joint position value to be at the homing position [encoder count]
          *
         * \return true if successful
         * \return false if not
         */
    bool setPositionOffset(int offset);

    /**
     * \brief Sends SDO value to set the tracking error window (the value of the tracking error)
     *
     * /param window Size of the error allowed before error is thrown
     *
     * \return true if successful
     * \return false if not
     */
    bool setTrackingWindow(INTEGER32 window);

    /**
     * \brief Sends SDO value to set the fault mask 
     *
     * /param mask value of the mask (see Schneider's CANOpenProgrammer's Manual Index 0x2182, p. 69)
     *
     * \return true if successful
     * \return false if not
     */
    bool setFaultMask(UNSIGNED32 mask);


    /** OVERWRITING BASE CLASS
           * \brief Initialises a standard set of PDOs for the use of the drive. These are:
           *
           *   TPDO1: COB-ID 180+{NODE-ID}: Status Word (0x6041), Send on Internal Event Trigger
           *   TPDO2: COB-ID 280+{NODE-ID}: Actual Position (0x6064), Actual Velocity (0x606C), Sent every SYNC Message
           *   
           * REMOVING  TPDO3: COB-ID 380+{NODE-ID}: Actual Torque (0x607C), Sent every SYNC MEssage
           *
           *   RPDO3: COB-ID 300+{NODE-ID}: Target Position (0x607A), Applied immediately when received
           * REMOVING  RPDO4: COB-ID 400+{NODE-ID}: Target Velocity (0x60FF), Applied immediately when received
           * REMOVING  RPDO5: COB-ID 500+{NODE-ID}: Target Torque (0x6071), Applied immediately when received
           *
           * \return true
           * \return false
           */
    virtual bool initPDOs();

};
#endif
