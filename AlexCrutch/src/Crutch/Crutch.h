/*
* Crutch Object running on pocket Beagle
*
*
*/
// VECTOR INDEX FOR nextMOTION lists

#ifndef CRUTCH_H_DEFINED
#define CRUTCH_H_DEFINED

#define LEFT_FORWARD 0
#define RIGHT_FORWARD 1
#define STANDING 2
#define SITTING 3

#define CLK_FREQ 2000        // Operating frequency of the crutch 'run' function
#define LONG_PRESS_LENGTH 2  // Number of seconds to hold button to execute long press logic

#define _NOLCD
#define _KEYBOARD
// #define _NOROBOT

#include <array>
#include <map>
#include <string>
#include <vector>

#include "LCD.h"

#ifdef _KEYBOARD
#include "Keyboard.h"
#endif

using namespace std;

enum class RobotMode {
    NORMALWALK, /**< 0 */
    SITDWN,     /**< 1 */
    STNDUP,     /**< 2 */
    UPSTAIR,    /**< 3 */
    DWNSTAIR,   /**< 4 */
    TILTUP,     /**< 5 */
    TILTDWN,    /**< 6 */
    RAMPUP,     /**< 7 */
    RAMPDWN,    /**< 8 */
    BKSTEP,     /**< 9 */
    FTTG,       /**< 10 */
    UNEVEN,     /**< 11 */
    INITIAL     /**< 12 */
};

// Incompatible state:Movement Pairs:
// Sitting -> can only stand
// Feet Together -> only when not in standing

enum SMState { Init,         /**< 0 */
               InitSitting,  /**< 1 */
               LeftForward,  /**< 2 */
               RightForward, /**< 3 */
               Standing,     /**< 4 */
               Sitting,      /**< 5 */
               SittingDown,  /**< 6 */
               StandingUp,   /**< 7 */
               StepFirstL,   /**< 8 */
               StepFirstR,   /**< 9 */
               StepLastL,    /**< 10 */
               StepLastR,    /**< 11 */
               StepL,        /**< 12 */
               StepR,        /**< 13 */
               BackStepR,    /**< 14 */
               BackStepL,    /**< 15 */
               Error         /**< 16 */
};

enum Stage { Default,   /**< 0 */
             SitStand,  /**< 1 */
             UnevenGnd, /**< 2 */
             Stairs,    /**< 3 */
             Tilt,      /**< 4 */
             Ramp       /**< 5 */
};

static std::map<SMState, bool> stateStationaryStatus = {
    {Error, true},
    {Init, true},
    {InitSitting, false},
    {LeftForward, true},
    {RightForward, true},
    {Standing, true},
    {Sitting, true},
    {SittingDown, false},
    {StandingUp, false},
    {StepFirstL, false},
    {StepFirstR, false},
    {StepLastL, false},
    {StepLastR, false},
    {StepL, false},
    {StepR, false},
    {BackStepL, false},
    {BackStepR, false}
};

/*Look Up table to convert between nextMotion selections and OD int outputs to exo BBB*/
static std::map<RobotMode, std::string> movementToString = {
    {RobotMode::NORMALWALK, "Normal"},
    {RobotMode::BKSTEP, "Backstep"},
    {RobotMode::FTTG, "Feet Together"},
    {RobotMode::UPSTAIR, "Up Stairs"},
    {RobotMode::DWNSTAIR, "Down Stairs"},
    {RobotMode::TILTUP, "Up Tilt"},
    {RobotMode::TILTDWN, "Down Tilt"},
    {RobotMode::RAMPUP, "Up Ramp"},
    {RobotMode::RAMPDWN, "Down Ramp"},
    {RobotMode::UNEVEN, "Uneven"},
    {RobotMode::SITDWN, "Sit Down"},
    {RobotMode::STNDUP, "Stand Up"},
    {RobotMode::INITIAL, "Initial Sit"}
};

static std::map<SMState, std::string> stateToString = {
    {Error, "Error"},
    {Init, "Init"},
    {InitSitting, "InitSitting"},
    {LeftForward, "Left Forward"},
    {RightForward, "Right Forward"},
    {Standing, "Standing"},
    {Sitting, "Sitting"},
    {SittingDown, "Sitting Down"},
    {StandingUp, "Standing Up"},
    {StepFirstL, "Step 1st Left"},
    {StepFirstR, "Step 1st Right"},
    {StepLastL, "Step Last L"},
    {StepLastR, "Step Last R"},
    {StepL, "Step Left"},
    {StepR, "Step Right"},
    {BackStepL, "Back Step left"},
    {BackStepR, "Back Step Right"}
};

/** MAP BEFORE RnD list changes (Just need to uncomment and delete SitStand stage to use)
// Note: Every stage MUST have RobotMode::STNDUP in it
static std::map<Stage, std::vector<RobotMode>> stageMovementList = {
    {Default, {RobotMode::NORMALWALK, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::TILTUP, RobotMode::TILTDWN, RobotMode::UNEVEN, RobotMode::STNDUP, RobotMode::SITDWN}},
    {UnevenGnd, {RobotMode::NORMALWALK, RobotMode::TILTDWN, RobotMode::STNDUP, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::NORMALWALK, RobotMode::TILTUP, RobotMode::UNEVEN, RobotMode::SITDWN}},
    {Stairs, {RobotMode::NORMALWALK, RobotMode::FTTG, RobotMode::BKSTEP, RobotMode::NORMALWALK, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::STNDUP, RobotMode::TILTUP, RobotMode::TILTDWN, RobotMode::UNEVEN, RobotMode::SITDWN}},
    {Tilt, {RobotMode::NORMALWALK, RobotMode::NORMALWALK, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::STNDUP, RobotMode::TILTUP, RobotMode::TILTDWN, RobotMode::UNEVEN, RobotMode::SITDWN}},
    {Ramp, {RobotMode::NORMALWALK, RobotMode::DWNSTAIR, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::STNDUP, RobotMode::TILTUP, RobotMode::TILTDWN, RobotMode::UNEVEN, RobotMode::SITDWN}}};
**/

// Note: Every stage MUST have RobotMode::STNDUP in it
static std::map<Stage, std::vector<RobotMode>> stageMovementList = {
    // Original Default one {Default, {RobotMode::NORMALWALK, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::TILTUP, RobotMode::TILTDWN, RobotMode::RAMPUP, RobotMode::RAMPDWN, RobotMode::UNEVEN, RobotMode::STNDUP, RobotMode::SITDWN}},
    {Default, {RobotMode::NORMALWALK, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::UNEVEN, RobotMode::STNDUP, RobotMode::SITDWN}},
    {SitStand, {RobotMode::NORMALWALK, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::SITDWN, RobotMode::STNDUP}},
    {UnevenGnd, {RobotMode::NORMALWALK, RobotMode::BKSTEP, RobotMode::UNEVEN, RobotMode::STNDUP}},
    {Stairs, {RobotMode::NORMALWALK, RobotMode::FTTG, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::STNDUP}},
    {Tilt, {RobotMode::NORMALWALK, RobotMode::NORMALWALK, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::STNDUP, RobotMode::TILTUP, RobotMode::TILTDWN, RobotMode::UNEVEN, RobotMode::SITDWN}},
    {Ramp, {RobotMode::NORMALWALK, RobotMode::BKSTEP, RobotMode::STNDUP, RobotMode::RAMPUP, RobotMode::RAMPDWN}}};

class Crutch {
   private:
    /* Jagged array for Current state, next motion relationship */
    // walking, standing and sitting w/ their Next motion lists
    // vector<vector<std::string>> nextMotion{
    //     {"RobotMode::NORMALWALK", "feet together", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
    //     {"sit Down", "RobotMode::NORMALWALK", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
    //     {"Stand Up"}};
    /* data */
    SMState currState;
    SMState lastState;

    RobotMode nextMove;
    RobotMode lastNextMove;

    Stage stage;
    Stage lastStage;

    int index;

    // Button Variables
    bool nextBut;
    bool prevNextBut;          // for debounce
    int initNextButCount = 0;  // for long press timer
    bool lastBut;
    bool prevLastBut;          // for debounce
    int initLastButCount = 0;  // for long press timer
    bool goBut;
    int pressTime;  // for recording length of button press

    bool waitGoRelease = true;

    std::string nextButPath = "/sys/class/gpio/gpio58/value";
    std::string lastButPath = "/sys/class/gpio/gpio59/value";
    std::string goButPath = "/sys/class/gpio/gpio57/value";

    std::map<int, int> enterMap;
    std::map<int, int> exitMap;
    std::map<int, std::array<int, 11>> stageMap;
    std::map<int, int> indexMap;

    void updateButtons();
    bool checkButton(std::string path);
    bool isStationaryState(SMState state);

    void updateStageEnter();
    void updateStageExit();

    void decrementIndex();
    void decrementIndexSafely();
    void incrementIndex();
    void incrementIndexSafely();
    bool isBadMovement();

    void longNextButLogic();
    void longLastButLogic();

#ifdef _KEYBOARD
    Keyboard *kb;
    bool longLastBut;
    bool longNextBut;
    bool prevLongNextBut;
    bool prevLongLastBut;
#endif

   public:
    Crutch(/* args */);
    ~Crutch();
    void run();
    void printCSNM();
    LCD *lcd;
    void initCrutch();
    void printVector(vector<vector<std::string>> const &mat);

    //OD interfaces
    void setHeartBeat(int val);
    void setNextMotion(int val);
    void setGreenButon(int val);
    int getCurrentMotion();
    int getCurrentState();
    //For Testing w.o. object Dicitonary
    void crutchTest();
    void incrementCount();
    int counter;
    int stateIndex;
    void testOD();
};

#endif
