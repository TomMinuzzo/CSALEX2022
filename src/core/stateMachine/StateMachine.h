/**
 * \file StateMachine.h
 * \author William Campbell
 * \version 0.1
 * \date 2019-09-24
 * For more detail on the architecture and mechanics of the state machine class see: https://embeded.readthedocs.io/en/latest/StaeMachines/.
 * \copyright Copyright (c) 2019
 *
 */
/**
 *  @defgroup stateMachine State Machine module
 * A group of abstract classes, used to build event-driven state machines.
 */
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

class State;

#include "logging.h"
#include "State.h"
#include "spdlog/helper/LogHelper.h"

/**
 * @ingroup stateMachine
 * \brief Abstract class representing a state machine. Includes a number of State and Transition objects
 *
 */
class StateMachine {
   public:
    /**
     * \brief Construct a new State Machine object
     *
     */
    StateMachine(void);
    /**
     * \brief Default destructor
     *
     */
    virtual ~StateMachine(){};
    /**
     * \brief Sets the current state. Note: No check made
     *
     * \param i Pointer to the desired current state.
     */
    void initialize(State *i);

    /**
     * \brief Returns a pointer to the current state
     *
     * \return State* Pointer to the current state
     */
    State *getCurState(void);

    /**
     * \brief Calls the entry method of the current state
     *
     */
    void activate(void);

    /**
     * \brief Processes the state machine. For each possible transition, checks if that transition should be made
     *  If no, calls during() on the current state
     *  If yes, calls exit() on the current state, entry() and then during() on the new state.
     *
     */
    virtual void update(void);

    /**
     * \brief Custom initialisation of the state machine
     *
     */
    virtual void init(void) = 0;

    /**
     * \brief End the state machine execution state
     *
     */
    virtual void end(void) = 0;

   protected:
    /**
     * \brief Pointer to the current state
     *
     */
    State *currentState;

    bool initialised = false;

    /**
     * \brief Custom spdlogger allowing to conveniently log Eigen Vectors (among other things)
     * Required to be initialised in the derived state machine init()
     */
    LogHelper logHelper;

};

/**
 * \brief Macros to quickly create event objects given their names as input.
 * useage: EventObject ( MyEvent ) * myEvent;
 */
#define EventObject(_name_)             \
    class _name_;                       \
    friend class _name_;                \
    class _name_ : public Event {       \
       public:                          \
        _name_(StateMachine *m,         \
               const char *name = NULL) \
            : Event(m, name){};         \
        bool check(void);               \
    };                                  \
    _name_
/**
 * \brief Macro to create statemachine transitions.
 *  Add a tranition object to the from states arch list
 * _to_ a specific state object, triggered by the occurence of _event_.
 *
 */
#define NewTransition(_from_, _event_, _to_) \
    _from_->addArc(new Transition(_to_, _event_))

#endif  //STATEMACHINE_H
