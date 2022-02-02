/**
 * /file CalibrateState.cpp
 * /author Justin Fong
 * /brief Calibration - takes sensors and offsets sensors by NUM_CALIBRATE_READINGS second average
 * /version 0.1
 * /date 2021-1-21
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "CalibrateState.h"

CalibrateState::CalibrateState(StateMachine *m, LoggingRobot *robot, const char *name) : State(m, name), robot(robot) {
    spdlog::info("CalibrateState Created");
};
void CalibrateState::entry(void) {
    spdlog::info("CalibrateState entry");
    spdlog::info("Calibrating....");

    Eigen::VectorXd force = robot->getCrutchReadings();
    readings = Eigen::ArrayXXd::Zero(NUM_CALIBRATE_READINGS, force.size());

    robot->zeroForcePlate();
    robot->startCrutchSensors();
    currReading =0;
};

void CalibrateState::during(void){
    // Collect data and save
    if (currReading< NUM_CALIBRATE_READINGS){
        //Eigen::VectorXd force = robot->getCrutchReadings();
        readings.row(currReading) = robot->getCrutchReadings();
    }
    currReading = currReading+1;
};

void CalibrateState::exit(void) {
    // Take average of the matrices
    Eigen::VectorXd offsets = Eigen::VectorXd::Zero(readings.cols());

    // Set offsets for crutches
    for (int i = 0; i < readings.cols(); i++) {
        offsets[i] = readings.col(i).sum()/NUM_CALIBRATE_READINGS;
        spdlog::info("Crutch Offset {}", offsets[i]);
    }
    robot->setCrutchOffsets(offsets);
    robot->stopCrutchSensors();

    spdlog::info("CalibrateState Exit");
};

int CalibrateState::getCurrReading(void) {
    return currReading;
};