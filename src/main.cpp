#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"
#include "MobilityController.h"
#include "StateManager.h"

#include "RemoteConstants.h"
#include "IRdecoder.h" 

BlueMotor motor;
Romi32U4ButtonB buttonB;
Romi32U4ButtonC buttonC;
long timeToPrint = 0;
long now = 0;
long newPosition = 0;
long oldPosition = 0;
long sampleTime = 100;
int speedInRPM = 0;
int CPR = 540;
int motorEffort = 400;
bool paused = false;

MobilityController mobilityController;
IRDecoder decoder(2); 


void setup()
{
  Serial.begin(9600);
  motor.setup();
  motor.reset();
  mobilityController.init();
  decoder.init(); 
  

}

void update() {
  mobilityController.update();
  
  switch(StateManager::get().getState()) {
    case (PERMISSION):
      if (decoder.getKeyCode() == remoteEnterSave) {
        StateManager::get().advanceState();
      }
    break;
  };
}

void loop()
{
  if (decoder.getKeyCode() == remotePlayPause) {
    paused = !paused;
    mobilityController.pause();
    Serial.println(paused);
  }

  if (!paused) {
    update();
  } 
  
}

