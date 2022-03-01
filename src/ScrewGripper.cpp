#include "ScrewGripper.h"
#include <servo32u4.h>

#include <Arduino.h>
#include "State.h"
#include "StateManager.h"

#define stateManager StateManager::get()
#define potPin A1

const int potOpen = 500;
const int potClosed = 730;

Servo32U4 servoC;

void ScrewGripper::init(){
    servoC.attach();
    write(1700);
}



int ScrewGripper::read() {
    return analogRead(potPin);
}

void ScrewGripper::write(int us) {
    servoC.writeMicroseconds(us);
}

void ScrewGripper::update() {
    switch(stateManager.getState()) {
        case (GRIP):
            if (read() > potClosed) {
                write(1500);
                stateManager.advanceState();
            } else {
                write(1700);
            }
        break;

        case (RELEASE):
            if (read() < potOpen) {
                write(1500);
                stateManager.advanceState();
            } else {
                write(1300);
            }
        break;

        default:
            write(1500);
            break;
    }
}

