
#include <Arduino.h>
#include <Chassis.h>
#include <Rangefinder.h>
#include "State.h"
#include "StateManager.h"
#include <MobilityController.h>

#define leftLightSensor A3
#define rightLightSensor A2
#define rangeEcho 3
#define rangeTrig 6

//Quality of life definition for referencing the state manager singleton
#define stateManager StateManager::get()

const float kLight = 0.15;

Chassis chassis;
Rangefinder rangefinder(rangeEcho, rangeTrig);

//Define behavior for when paused
//TODO: pausing during certain routines will break the overall path, decide if this needs to be fixed
void MobilityController::pause()
{
  chassis.idle();
}

//Call init of memeber classes
void MobilityController::init()
{
  chassis.init();
  rangefinder.init();
}

//Loop update method, checks for what to do based on current state
void MobilityController::update()
{


  switch (stateManager.getState())    //Get current state from state manager and switch over it
  {

  case (FOLLOWTOLINE):  //Linefollow until a perpendicular junction is met
    if ((analogRead(leftLightSensor) - analogRead(rightLightSensor) > 50) && (analogRead(rightLightSensor) > 600))  //True if at junction
    {
      stateManager.advanceState();
    }
    else
    { //Proportunal Line following
      float error = analogRead(leftLightSensor) - analogRead(rightLightSensor);
      chassis.setTwist(10, kLight * error);
    }
    break;

  case (FOLLOWTOWALL):    //Line follow until the ultrasonic sensor detects something close
    if (rangefinder.getDistance() < 12.7) //Arbitary value chosen via testing
    {
      stateManager.advanceState();
    }
    else
    { //Proportional Line following
      float error = analogRead(leftLightSensor) - analogRead(rightLightSensor);
      chassis.setTwist(10, kLight * error);
    }
    break;

  case (DIMETOLINELEFT):   //Turn in place until the light sensor is centered over another line (can be used for 90 or 180 degree turns depending on field position)
    //FlagA is used to track which stage this routine we are on, all flag values are set to false at state transitions
    if (!stateManager.flagA)
    { //Turn until the left side has crossed a line then change flag state
      if (!(analogRead(leftLightSensor) < 600))
      {
        stateManager.flagA = true;
      }
      chassis.setMotorEfforts(-30, 30);
    }
    else
    { //Turn until right side aproaches a line (the result of these two blocks is centering over the line)
      if (!(analogRead(rightLightSensor) < 150))
      {
        stateManager.advanceState();
      }
      chassis.setMotorEfforts(-30, 30);
    }
    break;

  case (DIMETOLINERIGHT):   //Identical to the previous state but turning in the oposite direction (see previous comments)
    if (!stateManager.flagA)
    {
      if (!(analogRead(rightLightSensor) < 600))
      {
        stateManager.flagA = true;
      }
      chassis.setMotorEfforts(30, -30);
    }
    else
    {
      if (!(analogRead(leftLightSensor) < 150))
      {
        stateManager.advanceState();
      }
      chassis.setMotorEfforts(30, -30);
    }
    break;

  case (CENTERONJUNCT):     //Move from having light sensor over a line to having the center of the wheels over the line
    if (!stateManager.flagA)  //Right after advancing to this state configure timer
    {
      chassis.setTwist(10, 0);
      stateManager.startTimer(900);
      stateManager.flagA = true;
    }
    if (stateManager.checkTimer())
    { //After diriving straight for 900 ms advance state (value found through testing)
      chassis.idle();
      stateManager.advanceState();
    }
    break;

  case (BACKFROMWALL):    //Back up a small arbitrary amount, used to get away from the barn before turning so fourbar has clearence to turn
    if (!stateManager.flagA)  //Same timer logic as CENTERONJUNCT but driving backwards
    {
      chassis.setTwist(-10, 0);
      stateManager.startTimer(526);
      stateManager.flagA = true;
    }
    if (stateManager.checkTimer())
    {
      chassis.idle();
      stateManager.advanceState();
    }
    break;

  case (IDLE):    //IDLE state, idles chassis
  default:        //NOTE: IDLE is the default state, this means if the robot is in a state the mobility controller doesnt recognise
                  //such as a state for grasping the panel, the chassis knows to idle
    chassis.idle();
  }
}
