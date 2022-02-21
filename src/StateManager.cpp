#include "StateManager.h"
#include "State.h"
#include <Arduino.h> 

#define length 8


const State routine [length] {
            BACKFROMWALL,
            DIMETOLINELEFT,
            PERMISSION,
            FOLLOWTOLINE,
            CENTERONJUNCT,
            DIMETOLINELEFT,
            FOLLOWTOWALL,
            IDLE
};

        int index = 0;
        long targetTime =0;
        bool timerGoing = false;


         StateManager::StateManager() {
        
            index = 0;
        }

        bool flagA = false;
        bool flagB = false;

        State StateManager::getState() {
            return routine[index];
        }

        void StateManager::advanceState() {
            flagA = false;
            flagB = false;
        
            index = (index+1)%length;
        }

        void StateManager::startTimer(long t) {
            targetTime = t + millis();
            timerGoing = true;
        }  

        bool StateManager::checkTimer() {
            if (millis() >= targetTime) {
                timerGoing = false;
                return true;
            }
            return false;
        }

        bool StateManager::timerStarted() {
            return timerGoing;
        }





