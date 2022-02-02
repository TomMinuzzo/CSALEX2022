/**
 * /file IdleState.h
 * /author Justin Fong
 * /brief Idle state - logger is doing nothing
 * /version 0.1
 * /date 2021-1-21
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef FORCEPLATE_IDLESTATE_H
#define FORCEPLATE_IDLESTATE_H

#include "ForcePlate.h"
#include "State.h"

class IdleState : public State {
   public:
    ForcePlate *robot;
    IdleState(StateMachine *m, ForcePlate *robot, const char *name = NULL);

    void entry(void);
    void during(void);
    void exit(void);
};
#endif