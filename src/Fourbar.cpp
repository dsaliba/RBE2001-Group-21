#include "Fourbar.h"
#include <Arduino.h>
#include "BlueMotor.h"

#include "StateManager.h"

#define stateManager StateManager::get()

const long pos25 = -6765;

const long pos45 = -5425;

const long tolerance = 15;


BlueMotor fbMotor;

void Fourbar::init(){
    fbMotor.setup();
}

void Fourbar::setPosition(long pos) {
    fbMotor.moveTo(pos);
}

long Fourbar::getPosition(){
    return fbMotor.getPosition();
}


void Fourbar::update() {
    switch (stateManager.getState())
    {
    case (FBFLAT):
        // fbMotor.moveTo(0);
        // if (abs(fbMotor.getPosition()-0) < tolerance) {
        //     stateManager.advanceState();
        // }
        if (fbMotor.moveOneStepTo(0)) {
            stateManager.advanceState();
        }
        break;

    case (FB25):
        // fbMotor.moveTo(pos25);
        // if (abs(fbMotor.getPosition()-pos25) < tolerance) {
        //     stateManager.advanceState();
        // }
        if (fbMotor.moveOneStepTo(pos25)) {
            stateManager.advanceState();
        }
        break;
    
    case (FB45):
        // fbMotor.moveTo(pos45);
        // if (abs(fbMotor.getPosition()-pos45) < tolerance) {
        //     stateManager.advanceState();
        // }
        if (fbMotor.moveOneStepTo(pos45)) {
            stateManager.advanceState();
        }
        break;
    
    default:
        break;
    }
}

void Fourbar::pause() {
    fbMotor.setEffort(0);
}