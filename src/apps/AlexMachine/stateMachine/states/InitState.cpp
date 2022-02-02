#include "InitState.h"

void InitState::entry(void)
{
    spdlog::info("ALEX Software Begins");
#ifdef KEYBOARD
    std::cout << "========================" << endl
              << " PRESS D to start program or A to Home" << endl
              << "========================" << endl;
#endif
#ifdef VIRTUAL
    std::cout << "========================" << endl
              << " Press S to enter Debug state" << endl
              << "========================" << endl;
#endif
    //Initialize OD entries - Must be something other then Initial -> must be sent by crutch @ startup
    robot->setCurrentState(AlexState::Init);
    robot->setCurrentMotion(RobotMode::NORMALWALK);
    robot->setNextMotion(RobotMode::NORMALWALK);
    //robot->pb.printMenu();
    // entry flag must be set to true by a green button release
    robot->setResetFlag(false);
}

void InitState::during(void)
{
    //Virtual crutch - changing OD.nm
    // RobotMode modeSelected = robot->pb.updateController(robot->keyboard.getE(), robot->keyboard.getW(), robot->keyboard.getX());
    // if (modeSelected == RobotMode::INITIAL) {
    //     std::cout << "output:" << robot->pb.printRobotMode(modeSelected) << std::endl;
    // }
    updateCrutch();
    updateFlag();
}
void InitState::exit(void)
{
    robot->initPositionControl();
    robot->setPosControlContinuousProfile(true);

    PilotParameters Brad_parameters = {
        .lowerleg_length = 0.44,
        .upperleg_length = 0.44,
        .ankle_height = 0.12,
        .foot_length = 0.30,
        .hip_width = 0.43,
        .torso_length = 0.4,
        .buttocks_height = 0.05};
    trajectoryGenerator->setPilotParameters(Brad_parameters);



    #ifdef NOROBOT

    Eigen::VectorXd positions(4);
    positions(0)= deg2rad(90);
    positions(1) =deg2rad(100);
    positions(2)= deg2rad(90);
    positions(3) =deg2rad(100);
    robot->setPosition(positions);
#endif
}
