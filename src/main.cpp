#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"
#include "MobilityController.h"
#include "StateManager.h"
#include "Gripper.h"
#include "ScrewGripper.h"
#include "Fourbar.h"

#include "RemoteConstants.h"
#include "IRdecoder.h"


#include <servo32u4.h>


BlueMotor motor;
Romi32U4ButtonB buttonB;
Romi32U4ButtonC buttonC;
Romi32U4ButtonA buttonA;

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
Gripper gripper;
Fourbar fourbar;

//BlueMotor ffbMotor;



IRDecoder decoder(2);



//Call necisary innit methods
void setup()
{
  Serial.begin(9600);
  motor.setup();
  motor.reset();
  mobilityController.init();
  gripper.init();
  decoder.init();
  fourbar.init();
  //ffbMotor.setup();

}



//Method for calling the statemachine updates of all the subsystems as well as the surface level update loop (handels things like waiting for permision)
void update()
{
  mobilityController.update(); //Call mobility update
  gripper.update();
  fourbar.update();

  switch (StateManager::get().getState())
  {
  case (PERMISSION): //Wait for permision given via IR remote
    if (decoder.getKeyCode() == remoteEnterSave)
    {
      Serial.println("go");
      StateManager::get().advanceState();
    }
    break;

  case (BUTTONPERMISSION):
    if (buttonA.isPressed()) {
      StateManager::get().advanceState();
    }
  };
}

void loop()
{

  if (decoder.getKeyCode() == remoteSetup) {
    Serial.print("State: ");
    Serial.println(StateManager::get().getState());
  }

  // if (decoder.getKeyCode()== remoteUp) {
  //   gripper.up();
  // }

  // if (decoder.getKeyCode() == remoteDown) {
  //   gripper.down();
  // }
  
  //Check pausing logic
  if (decoder.getKeyCode() == remotePlayPause)
  { 
    paused = !paused;
    mobilityController.pause();
    fourbar.pause();

    Serial.println(paused);
  }

  // if (buttonB.isPressed()) {
  //   ffbMotor.setEffort(400);
  // } else if (buttonC.isPressed()){
  //   ffbMotor.setEffort(-400);
  // } else {
  //   ffbMotor.setEffort(0);
  // }


  // if (buttonA.isPressed()) {
  //   Serial.println(ffbMotor.getPosition());
  // }

  //call update if unpaused
  if (!paused)
  {
    update();
  } else {
    
  }
}
