#include "Crutch.h"

#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>

#include "CANopen.h"

Crutch::Crutch(/* args */) {
    std::cout << "Crutch object created" << std::endl;
#ifndef _NOLCD
    lcd = new LCD();
    lcd->setup();
#endif

    stage = Default;
    index = 0;

#ifdef _KEYBOARD
    kb = new Keyboard();
#endif
}

Crutch::~Crutch() {
    lcd->~LCD();

    std::cout << "Crutch object deleted" << std::endl;
}

void Crutch::initCrutch() {
    lastState = Init;
    currState = Init;

    lastNextMove = RobotMode::DWNSTAIR;  // This is irrelevant - just needed to make sure that it prints to the screen on the first press
    nextMove = RobotMode::NORMALWALK;
#ifndef _NOLCD
    lcd->commControlOn();
#endif
}

void Crutch::run() {
    // Take the current state from the object dictionary - this is passed on from the OD on the Main Beaglebone
    currState = static_cast<SMState>(CO_OD_RAM.currentState);

    // Checking current state for use without exo
#ifdef _KEYBOARD
    if (kb->getX()) {
        std::cout << "Current State: " << stateToString[currState] << std::endl;
    }
#endif

    // This is a record of the number of iterations of this device
    incrementCount();

    // Poll the buttons
    updateButtons();

    // This should go into updateButtons
    // Start timer when button is pressed
    if (nextBut && !prevNextBut) {
        initNextButCount = counter;
    }
    if (lastBut && !prevLastBut) {
        initLastButCount = counter;
    }

    //std::cout << std::dec << (counter - initNextButCount) / CLK_FREQ << " " << std::dec << (counter - initLastButCount) / CLK_FREQ << std::endl;

    // Reset next move if both buttons held for more than 5 seconds
    if (nextBut &&  (counter - initNextButCount) / CLK_FREQ > 5 && lastBut && (counter - initLastButCount) / CLK_FREQ > 5){
        // Reset next move
        CO_OD_RAM.nextMovement = 0;
        lastNextMove = nextMove;
        nextMove = RobotMode::NORMALWALK;
        

        CO_OD_RAM.currentState = static_cast<int> (SMState::Error);
        currState = SMState::Error;
        std::cout << "RESET" << std::endl;;
    }
 

    if (currState == SMState::Init) {
        if (nextBut && lastBut) {
            nextMove = RobotMode::INITIAL;
            //std::cout << "Start Buttons Pressed" << std::endl;
            CO_OD_RAM.nextMovement = static_cast<uint16_t>(nextMove);
#ifdef _NOROBOT
            //CO_OD_RAM.currentState = static_cast<SMState>(Standing);  // Arbitrary state
#endif
        }

#ifdef _KEYBOARD
        if (kb->getW()) {
            nextMove = RobotMode::INITIAL;
            std::cout << "Start Buttons Pressed" << std::endl;
            CO_OD_RAM.nextMovement = static_cast<uint16_t>(nextMove);
            //CO_OD_RAM.currentState = static_cast<SMState>(Standing);  // Arbitrary state, use for testing with keyboard
        }
#endif

        if (nextMove == RobotMode::INITIAL) {
            if (goBut) {
                //std::cout << "Send GO" << std::endl;
            }
            CO_OD_RAM.goButton = static_cast<uint16_t>(goBut);
        }
    }
    // If current State is a stationary State
    else if (isStationaryState(currState)) {
        if (!isStationaryState(lastState)) {
            // We have just finished a move - don't do anything until go is released
            std::cout << "Finished Move" << std::endl;
            waitGoRelease = true;
            CO_OD_RAM.goButton = static_cast<uint16_t>(false);

            // Triggers (for e.g.) when exo has just stood after sitting - don't want next movement to be standing again
            if (isBadMovement()) {
                incrementIndex();
                nextMove = stageMovementList[stage][index];
            }
        }
        if (waitGoRelease) {
            // Waiting for Go Button to be released
            if (!goBut) {
                std::cout << "Go Released!" << std::endl;
                waitGoRelease = false;
            }
        } else {
            // Keyboard input for long-press logic
#ifdef _KEYBOARD
            if (!longNextBut && prevLongNextBut && (currState != SMState::Error)) {
                longNextButLogic();
                nextMove = stageMovementList[stage][index];
            }
            if (!longLastBut && prevLongLastBut && (currState != SMState::Error)) {
                longLastButLogic();
                nextMove = stageMovementList[stage][index];
            }
            prevLongNextBut = longNextBut;
            prevLongLastBut = longLastBut;
#endif
            // Execute logic when button is released (Note: Do not want to execute logic when in error state)
            if (!nextBut && prevNextBut && (currState != SMState::Error)) {
                // Upon release, determine how many seconds the button was pressed for
                pressTime = (counter - initNextButCount) / CLK_FREQ;

                if (pressTime > LONG_PRESS_LENGTH) {
                    // If the button has been pressed for longer than the long press threshold,
                    // execute long press logic
                    longNextButLogic();
                } else {
                    // If the button has not been pressed long enough, execute short press logic
                    incrementIndex();
                }
                // Update any stage or movement index changes
                nextMove = stageMovementList[stage][index];
            }
            if (!lastBut && prevLastBut && (currState != SMState::Error)) {
                // Upon release, determine how many seconds the button was pressed for
                pressTime = (counter - initLastButCount) / CLK_FREQ;

                if (pressTime > LONG_PRESS_LENGTH) {
                    // If the button has been pressed for longer than the long press threshold,
                    // execute long press logic
                    longLastButLogic();
                } else {
                    // If the button has not been pressed long enough, execute short press logic
                    decrementIndex();
                }
                // Update any stage or movement index changes
                nextMove = stageMovementList[stage][index];
            }

            // Check if the Go Button has been pressed
            if (goBut) {
                //std::cout << "Go Pressed" << std::endl;
                if (nextMove == static_cast<RobotMode>(CO_OD_RAM.currentMovement)) {
                    // If the movement set on the ExoBeagle is the same as the one on the screen,
                    // Send the go button status on the OD to true
                    CO_OD_RAM.goButton = static_cast<uint16_t>(true);
                } else {
                    // If they are not the same, then send the next movement across to the exoskeleton
                    // and do not update the go button on the exo side
                    //std::cout << "Next Move Sent" << std::endl;
                    CO_OD_RAM.nextMovement = static_cast<uint16_t>(nextMove);
                }
            } else {
                // If the GoButton is not pressed, set the go button to false always
                CO_OD_RAM.goButton = static_cast<uint16_t>(false);
            }
        }
    } else {
        // If not in a stationary state, just map the GoButton to to the ExoBeagle OD
        CO_OD_RAM.goButton = static_cast<uint16_t>(goBut);
    }

    prevNextBut = nextBut;
    prevLastBut = lastBut;

#ifndef _NOLCD
    if (!lcd->isQueueEmpty()) {
        lcd->sendNextCommand();
    }
#endif
}

void Crutch::printCSNM() {
    // Only update the screen if we are in a stationary state
    if (currState != lastState) {
// If the state has changed - update the screen to show the state
#ifndef _NOLCD
        lcd->printCurrState(stateToString[currState]);
        if (isStationaryState(currState)) {
            lcd->backlightOn();
        } else {
            lcd->backlightOff();
        }

#endif
        //sleep(1);
        std::cout << "Curr State: " << stateToString[currState] << std::endl;
        lastState = currState;
    }
    if (nextMove != lastNextMove) {
// If the selected move has changed, update the selected move
#ifndef _NOLCD
        lcd->printNextMove(movementToString[nextMove]);
#endif
        //sleep(1);
        std::cout << "Next Move: " << movementToString[nextMove] << std::endl;
        lastNextMove = nextMove;
    }

    if (lastStage != stage) {
#ifndef _NOLCD
        lcd->printStage(stage);
#endif
        std::cout << "Stage: " << stage << std::endl;
        lastStage = stage;
    }
}

void Crutch::setHeartBeat(int val) {
}

void Crutch::setNextMotion(int val) {
    // CO_OD_RAM.nextMovement = val;
}
void Crutch::setGreenButon(int val) {
    // CO_OD_RAM.goButton = val;
}
int Crutch::getCurrentMotion() {
    // retrun CO_OD_RAM.currentMotion;
    return currState;
}

int Crutch::getCurrentState() {
    return currState;
}

void Crutch::incrementCount() {
    counter++;
}
// TEST BBB OD
void Crutch::testOD() {
    std::cout << "Current state" << stateToString[currState] << std::endl;
}

void Crutch::printVector(vector<vector<std::string>> const &mat) {
    for (vector<std::string> row : mat) {
        for (std::string val : row) {
            cout << val << " ";
        }
        cout << '\n';
    }
}

bool Crutch::isStationaryState(SMState state) {
    return stateStationaryStatus[state];
}

void Crutch::updateButtons() {
#ifdef _KEYBOARD
    kb->updateInput();
    nextBut = kb->getA();
    lastBut = kb->getS();
    longNextBut = kb->getE();
    longLastBut = kb->getQ();

    if (kb->getD()) {
        goBut = !goBut;
        if (goBut) {
            std::cout << "Go Pressed" << std::endl;
        } else {
            std::cout << "Go Unpressed" << std::endl;
        }
    }

#else
    nextBut = checkButton(nextButPath);
    lastBut = checkButton(lastButPath);
    goBut = checkButton(goButPath);
    // std::cout << "NextBut: " << nextBut << ", LastBut: " << lastBut << ", Trigger: " << goBut << std::endl;
#endif
}

bool Crutch::checkButton(std::string path) {
    char value;
    std::ifstream stream(path);
    stream >> value;
    stream.close();

    // GPIO are configured as pull up. Value is pulled down to 0 when the buttons are pressed.
    if (value == '0') {
        return true;
    } else {
        return false;
    }
}

// For backward cycling between movements
void Crutch::decrementIndex() {
    index = (index < 1) ? index = stageMovementList[stage].size() - 1 : index - 1;
    decrementIndexSafely();  // For skipping unwanted movements
}

// For forward cycling between movements
void Crutch::incrementIndex() {
    index = (index + 1) % stageMovementList[stage].size();
    incrementIndexSafely();  // For skipping unwanted movements
}

/**
 * \brief For backward cycling between stages:
 * 
 * - Decrement stage number (NOTE: Assumes Stage enum type is CONTINUOUS)
 * - Reset movement index to the beginning of that stage's list
 * - TODO: Check the movement it's reset to is safe given the current state
 * 
 */
void Crutch::longLastButLogic() {
    stage = (stage < 1) ? stage = Stage(stageMovementList.size() - 1) : Stage(stage - 1);
    index = 0;
    incrementIndexSafely();
}

/**
 * \brief For forward cycling between stages:
 * 
 * - Increment stage number (NOTE: Assumes Stage enum type is CONTINUOUS)
 * - Reset movement index to the beginning of that stage's list
 * - TODO: Check the movement it's reset to is safe given the current state
 * 
 */
void Crutch::longNextButLogic() {
    stage = Stage((stage + 1) % stageMovementList.size());
    index = 0;
    incrementIndexSafely();
}

/** 
 * \brief Cycles index up when currently is an unwanted movement
 * 
*/
void Crutch::incrementIndexSafely() {
    while (isBadMovement()) {
        index = (index + 1) % stageMovementList[stage].size();
    }
}

/**
 * \brief Cycles index down when currently is an unwanted movement
 * 
 */
void Crutch::decrementIndexSafely() {
    while (isBadMovement()) {
        index = (index < 1) ? index = stageMovementList[stage].size() - 1 : index - 1;
    }
}

/**
 * \brief Checks if the current movement is not wanted given the current state
 * 
 *  - If sitting, only option is to stand up, search for that entry in the list
 *  - Only allow stand up in the sitting state
 *  - To only allow sitting when feet are together
 *  - To prevent a Feet Together Movement when feet are already together 
 */
bool Crutch::isBadMovement() {
    return (currState == SMState::Sitting && stageMovementList[stage][index] != RobotMode::STNDUP) ||
           (currState != SMState::Sitting && stageMovementList[stage][index] == RobotMode::STNDUP) ||
           (stageMovementList[stage][index] == RobotMode::SITDWN && currState != SMState::Standing) ||
           (currState == SMState::Standing && stageMovementList[stage][index] == RobotMode::FTTG);
}