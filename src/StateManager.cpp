#include "StateManager.h"
#include "State.h"
#include <Arduino.h>

//This class handels the construciton of our loop, a singleton class is used which handels the current state and state advancement
//boolean flags are also kept in this class in order to allow more data to be stored on a state by state basis.
//state manager also handels timer logic

#define length 5

//18
// const State routine[length]{
//     RELEASE,
//     PERMISSION,
//     FOLLOWTOSTAGING,
//     GRIP,
//     PERMISSION,
//     FB25,
//     BACKFROMWALL,
//     QUARTERTURNLEFT,
//     QUARTERTURNLEFT,
//     QUARTERTURNLEFT,
//     QUARTERTURNLEFT,
//     PERMISSION,
//     FOLLOWTOSTAGING,
//     FBFLAT,
//     CENTERONJUNCT,
//     RELEASE,
//     BACKFROMWALL,
//     BACKFROMWALL  
//};

const State routine[length]{
    PERMISSION,
    FB25,
    PERMISSION,
    FBFLAT,
    PERMISSION
};


//List of states in order for the master routine
// const State routine[length]{
//     BACKFROMWALL,
//     DIMETOLINELEFT,
//     PERMISSION,
//     FOLLOWTOLINE,
//     CENTERONJUNCT,
//     DIMETOLINELEFT,
//     FOLLOWTOWALL,
//     IDLE
// };

// const State routine[length] {
//     PERMISSION,
//     GRIPCONFIG
// };

// const State routine[length] {
//     RELEASE,
//     PERMISSION,
//     GRIP,
//     PERMISSION
// };


//BOT #1 (33 Instructions)
// const State routine[length] {
//     PERMISSION,
//     GRIP,
//     BACKFROMWALL,
//     DIMETOLINELEFT,
//     FOLLOWTOLINE,
//     CENTERONJUNCT,
//     DIMETOLINELEFT,
//     FOLLOWTOWALL,
//     FBFLAT,
//     RELEASE,
//     PERMISSION,
//     GRIP,
//     BACKFROMWALL,
//     FB45,
//     DIMETOLINELEFT,
//     FOLLOWTOLINE,
//     DIMETOLINELEFT,
//     CENTERONJUNCT,
//     DIMETOLINERIGHT,
//     FOLLOWTOWALL,
//     PERMISSION,
//     RELEASE,
//     BACKFROMWALL,
//     DIMETOLINELEFT,
//     FOLLOWTOLINE,
//     CENTERONJUNCT,
//     DIMETOLINELEFT,
//     FOLLOWTOWALL,
//     QUARTERTURNLEFT,
//     FOLLOWTOLINE,
//     CENTERONJUNCT,
//     FOLLOWTOLINE,
//     IDLE
// };

int index = 0;
long targetTime = 0;
bool timerGoing = false;

StateManager::StateManager()
{

    index = 0;
}

bool flagA = false;
bool flagB = false;

State StateManager::getState()                       //Get current state
{
    return routine[index];
}

void StateManager::advanceState()                        //Advance to next state in routine (NOTE: this technically loops after completion but ususal routines are ended with an IDLE state)
{
    flagA = false;
    flagB = false;

    index = (index + 1) % length;
}

void StateManager::startTimer(long t)                         //Start a timer given a delay in millis
{
    targetTime = t + millis();
    timerGoing = true;
}

bool StateManager::checkTimer()                                      //Check if the current delay has been reached, return true if so
{
    if (!timerGoing) 
    {
        return false;
    }
    if (millis() >= targetTime)
    {
        timerGoing = false;
        return true;
    }
    return false;
}

bool StateManager::timerStarted()                                     //Check if there is a current active timer
{
    return timerGoing;
}

int StateManager::getTimerRemaining()     //Get remaining timer value
{
    return targetTime-millis();
}
