# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  "C"
  "CXX"
  )
# The set of files for implicit dependencies of each language:
set(CMAKE_DEPENDS_CHECK_C
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANcomms/CO_comm_helpers.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANcomms/CO_comm_helpers.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANcomms/CO_command.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANcomms/CO_command.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANcomms/CO_master.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANcomms/CO_master.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANcomms/CO_time.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANcomms/CO_time.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/CANopen.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/CANopen.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/CO_Emergency.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/CO_Emergency.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/CO_HBconsumer.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/CO_HBconsumer.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/CO_NMT_Heartbeat.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/CO_NMT_Heartbeat.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/CO_PDO.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/CO_PDO.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/CO_SDO.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/CO_SDO.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/CO_SDOmaster.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/CO_SDOmaster.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/CO_SYNC.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/CO_SYNC.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/CO_trace.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/CO_trace.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/crc16-ccitt.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/crc16-ccitt.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/socketCAN/CO_Linux_tasks.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/socketCAN/CO_Linux_tasks.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/socketCAN/CO_OD_storage.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/socketCAN/CO_OD_storage.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/stack/socketCAN/CO_driver.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/stack/socketCAN/CO_driver.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/objDict/CO_OD.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/objDict/CO_OD.c.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/IO/iobb.c" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/IO/iobb.c.o"
  )
set(CMAKE_C_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_C
  "BASE_DIRECTORY=."
  "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO"
  "STATE_MACHINE_INCLUDE=\"AlexMachine.h\""
  "STATE_MACHINE_TYPE=AlexMachine"
  )

# The include file search paths:
set(CMAKE_C_TARGET_INCLUDE_PATH
  "../src/apps/AlexMachine/TrajectoryGenerator"
  "../src/apps/AlexMachine/stateMachine"
  "../src/apps/AlexMachine/stateMachine/states"
  "../src/core/CANopen/CANcomms"
  "../src/core/CANopen/CANopenNode"
  "../src/core/CANopen/CANopenNode/stack"
  "../src/core/CANopen/CANopenNode/stack/socketCAN"
  "../src/core/CANopen/objDict"
  "../src/core"
  "../src/core/network"
  "../src/core/robot"
  "../src/core/stateMachine"
  "../src/hardware/IO"
  "../src/hardware/drives"
  "../src/hardware/platforms/Alex"
  "../src/hardware/platforms/Alex/IO"
  "../src/hardware/platforms/Alex/joints"
  "../src/hardware/platforms/ForcePlate"
  "../src/hardware/platforms/LoggingRobot"
  "../src/hardware/platforms/M3"
  "../src/hardware/platforms/X2"
  "../lib"
  "../lib/yaml-cpp/include"
  )
set(CMAKE_DEPENDS_CHECK_CXX
  "/home/ubuntu/Desktop/CSALEX2022/lib/FLNL/baseSocket.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/lib/FLNL/baseSocket.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/lib/FLNL/client.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/lib/FLNL/client.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/lib/FLNL/server.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/lib/FLNL/server.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/TrajectoryGenerator/AlexTrajectoryGenerator.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/TrajectoryGenerator/AlexTrajectoryGenerator.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/AlexMachine.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/AlexMachine.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/BackStepLeft.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/BackStepLeft.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/BackStepRight.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/BackStepRight.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/DebugState.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/DebugState.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/ErrorState.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/ErrorState.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/ExoTestState.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/ExoTestState.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/InitState.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/InitState.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/InitialSitting.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/InitialSitting.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/LeftForward.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/LeftForward.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/RightForward.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/RightForward.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/Sitting.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/Sitting.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/SittingDwn.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/SittingDwn.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/Standing.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/Standing.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/StandingUp.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/StandingUp.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/SteppingFirstLeft.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/SteppingFirstLeft.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/SteppingLastLeft.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/SteppingLastLeft.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/SteppingLastRight.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/SteppingLastRight.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/SteppingLeft.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/SteppingLeft.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/SteppingLeftStair.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/SteppingLeftStair.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/SteppingLeftStairDown.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/SteppingLeftStairDown.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/SteppingRight.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/SteppingRight.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/SteppingRightStair.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/SteppingRightStair.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/apps/AlexMachine/stateMachine/states/SteppingRightStairDown.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/apps/AlexMachine/stateMachine/states/SteppingRightStairDown.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/RPDO.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/RPDO.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/CANopen/CANopenNode/TPDO.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/CANopen/CANopenNode/TPDO.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/application.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/application.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/logging.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/logging.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/main.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/main.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/robot/CANDevice.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/robot/CANDevice.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/robot/Drive.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/robot/Drive.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/robot/InputDevice.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/robot/InputDevice.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/robot/Joint.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/robot/Joint.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/robot/Robot.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/robot/Robot.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/stateMachine/Event.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/stateMachine/Event.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/stateMachine/State.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/stateMachine/State.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/stateMachine/StateMachine.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/stateMachine/StateMachine.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/core/stateMachine/Transition.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/core/stateMachine/Transition.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/IO/ForcePlateSensor.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/IO/ForcePlateSensor.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/IO/HX711.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/IO/HX711.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/IO/Joystick.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/IO/Joystick.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/IO/Keyboard.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/IO/Keyboard.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/IO/RobotousRFT.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/IO/RobotousRFT.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/drives/CopleyDrive.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/drives/CopleyDrive.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/drives/KincoDrive.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/drives/KincoDrive.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/drives/SchneiderDrive.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/drives/SchneiderDrive.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/platforms/Alex/AlexRobot.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/platforms/Alex/AlexRobot.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/platforms/Alex/IO/ALEXCrutchController.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/platforms/Alex/IO/ALEXCrutchController.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/platforms/Alex/IO/Buttons.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/platforms/Alex/IO/Buttons.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/platforms/Alex/joints/AlexJoint.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/platforms/Alex/joints/AlexJoint.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/platforms/ForcePlate/ForcePlate.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/platforms/ForcePlate/ForcePlate.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/platforms/LoggingRobot/LoggingRobot.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/platforms/LoggingRobot/LoggingRobot.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/platforms/M3/JointM3.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/platforms/M3/JointM3.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/platforms/M3/RobotM3.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/platforms/M3/RobotM3.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/platforms/X2/X2ForceSensor.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/platforms/X2/X2ForceSensor.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/platforms/X2/X2Joint.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/platforms/X2/X2Joint.cpp.o"
  "/home/ubuntu/Desktop/CSALEX2022/src/hardware/platforms/X2/X2Robot.cpp" "/home/ubuntu/Desktop/CSALEX2022/build/CMakeFiles/AlexMachine_APP.dir/src/hardware/platforms/X2/X2Robot.cpp.o"
  )
set(CMAKE_CXX_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_CXX
  "BASE_DIRECTORY=."
  "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO"
  "STATE_MACHINE_INCLUDE=\"AlexMachine.h\""
  "STATE_MACHINE_TYPE=AlexMachine"
  )

# The include file search paths:
set(CMAKE_CXX_TARGET_INCLUDE_PATH
  "../src/apps/AlexMachine/TrajectoryGenerator"
  "../src/apps/AlexMachine/stateMachine"
  "../src/apps/AlexMachine/stateMachine/states"
  "../src/core/CANopen/CANcomms"
  "../src/core/CANopen/CANopenNode"
  "../src/core/CANopen/CANopenNode/stack"
  "../src/core/CANopen/CANopenNode/stack/socketCAN"
  "../src/core/CANopen/objDict"
  "../src/core"
  "../src/core/network"
  "../src/core/robot"
  "../src/core/stateMachine"
  "../src/hardware/IO"
  "../src/hardware/drives"
  "../src/hardware/platforms/Alex"
  "../src/hardware/platforms/Alex/IO"
  "../src/hardware/platforms/Alex/joints"
  "../src/hardware/platforms/ForcePlate"
  "../src/hardware/platforms/LoggingRobot"
  "../src/hardware/platforms/M3"
  "../src/hardware/platforms/X2"
  "../lib"
  "../lib/yaml-cpp/include"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  "/home/ubuntu/Desktop/CSALEX2022/build/lib/yaml-cpp/CMakeFiles/yaml-cpp.dir/DependInfo.cmake"
  )

# Fortran module output directory.
set(CMAKE_Fortran_TARGET_MODULE_DIR "")
