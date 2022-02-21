
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

#define stateManager StateManager::get()

const float kLight = 0.15;

Chassis chassis;
Rangefinder rangefinder(rangeEcho, rangeTrig);

void MobilityController::pause(){
  chassis.idle();
}

    void MobilityController::init(){
      chassis.init();
      rangefinder.init();
    }

    void MobilityController::update() {

      switch(stateManager.getState()) {

        case (FOLLOWTOLINE):
          if ((analogRead(leftLightSensor)-analogRead(rightLightSensor)>50) && (analogRead(rightLightSensor) > 600)) {
              stateManager.advanceState();
          } else {
              float error = analogRead(leftLightSensor)-analogRead(rightLightSensor); 
              chassis.setTwist(10, kLight*error); 
          }
        break;

        case (FOLLOWTOWALL):
          if (rangefinder.getDistance() < 12.7) {
              stateManager.advanceState();
          } else {
              float error = analogRead(leftLightSensor)-analogRead(rightLightSensor); 
              chassis.setTwist(10, kLight*error); 
          }
        break;

        case (DIMETOLINELEFT):
            if (!stateManager.flagA){
                if (!(analogRead(leftLightSensor) < 600)){
                  stateManager.flagA = true;
                }
                chassis.setMotorEfforts(-30, 30);
            } else {
              if (!(analogRead(rightLightSensor) < 150)) {
                stateManager.advanceState();
              }
              chassis.setMotorEfforts(-30, 30); 
            }
        break;

        case (DIMETOLINERIGHT):
          if (!stateManager.flagA){
                if (!(analogRead(rightLightSensor) < 600)){
                  stateManager.flagA = true;
                }
                chassis.setMotorEfforts(30, -30);
            } else {
              if (!(analogRead(leftLightSensor) < 150)) {
                stateManager.advanceState();
              }
              chassis.setMotorEfforts(30, -30); 
            }
        break;

        case (CENTERONJUNCT):
        if (!stateManager.flagA) {
          chassis.setTwist(10, 0);
          stateManager.startTimer(900);
          stateManager.flagA = true;
        }
        if (stateManager.checkTimer()) {
          chassis.idle();
          stateManager.advanceState();
        }
        break;

        case (BACKFROMWALL):
          if (!stateManager.flagA) {
          chassis.setTwist(-10, 0);
          stateManager.startTimer(526);
          stateManager.flagA = true;
        }
        if (stateManager.checkTimer()) {
          chassis.idle();
          stateManager.advanceState();
        }
        break;


        case (IDLE):
        default:
          chassis.idle();
      }
    }




// class MobilityController { 
//   public: 
//     void init() {
//       chassis.init();
//       rangefinder.init();
//     }

//     void lineFollow(bool (*predicate)()) { 
//       while (!predicate()){ 
//         float error = analogRead(leftLightSensor)-analogRead(rightLightSensor); 
//         chassis.setTwist(10, kLight*error); 
//       } 
//       chassis.idle(); 
//     } 
 
//     void dimeToLine(int dir) { 
//       if (dir == 1) { 
//         while (analogRead(leftLightSensor) < 600){ 
//             chassis.setMotorEfforts(-30, 30); 
//         } 
//         while (analogRead(rightLightSensor) < 150){ 
//             chassis.setMotorEfforts(-30, 30); 
//         } 
//       } else { 
//         while (analogRead(rightLightSensor) < 600){ 
//             chassis.setMotorEfforts(30, -30); 
//         } 
//         while (analogRead(leftLightSensor) < 150){ 
//             chassis.setMotorEfforts(30, -30); 
//         } 
//       } 
//       chassis.idle(); 
//     } 
 
 
//     //TODO make this not terrible 
//     void centerOnJunct(){ 
//       chassis.setTwist(10,0); 
//       delay(1028); 
//       chassis.idle(); 
//     } 
 
//     void backFromWall(){ 
//       chassis.setTwist(-10,0); 
//       delay(1028); 
//       chassis.idle(); 
//     } 
 
    
// }; 