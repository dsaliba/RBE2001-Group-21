#pragma once
#include "State.h"

class StateManager {
    public:
     static StateManager &get() { static StateManager stateManager; return stateManager; }
        
        StateManager();
        State getState();
        void advanceState();
        void startTimer(long t);
        bool checkTimer();
        bool timerStarted();
        int getTimerRemaining();

        
        bool flagA = false;
        bool flagB = false;

};
