/**
 *
 * \file ForcePlate.h
 * \author Justin Fong
 * \version 0.1
 * \date 2021-02-23
 * \copyright Copyright (c) 2021
 *
 * \brief  The<code> ForcePlate</ code> class is a force plate object, which measures 4 strain gauages - designed to provide force and COP measurements
 * 
 * This class is designed to work with the sensor system developed at the University of Melbourne's Human Robotics Laboratory
 *
 */

#ifndef FORCEPLATE_H_INCLUDED
#define FORCEPLATE_H_INCLUDED

#include "ForcePlateConstants.h"

#include "Keyboard.h"
#include "Robot.h"
#include "HX711.h"


class ForcePlate : public Robot {
   private:

    HX711* strainGauge;
    Eigen::VectorXd strainForces;
    Eigen::VectorXi strainForcesTPDO;  // Smaller data format for better sending over bus
    bool sensorsOn =  false;
    ForcePlateCommand currCommand = NONE;

    TPDO* tpdo1;
    TPDO* tpdo2;

    RPDO* rpdoCmd;
    void updatePDOs();

   public:
    Keyboard *keyboard;

    ForcePlate();
    ~ForcePlate();

    // Functions which are needed for the Robot Class - they don't do anything at the moment
    bool initialiseJoints() { return true; };
    bool initialiseInputs();
    bool initialiseNetwork() { return true; };  // this one might need to be changed

    Eigen::VectorXd &getStrainReadings();
    Eigen::VectorXi getRawStrainReadings();

    bool configureMasterPDOs();

    void setStrainOffsets(Eigen::VectorXi offsets);

    void updateRobot();

    ForcePlateCommand getCommand();

    void resetCommand();
};

#endif /*ForcePlate.h*/
