#include <Arduino.h> 
#include <Chassis.h> 
#include <Rangefinder.h> 
 
 
 
//Bindings 
#define leftLightSensor A3 
#define rightLightSensor A2 
#define rangeEcho 3 
#define rangeTrig 6 
 
Chassis chassis; 
Rangefinder rangefinder(rangeEcho, rangeTrig); 
 
 
 
//PID constants 
const float kLight = 0.15; 
 
 
 
class MobilityController { 
  public: 
    void lineFollow(bool (*predicate)()) { 
      while (!predicate()){ 
        float error = analogRead(leftLightSensor)-analogRead(rightLightSensor); 
        chassis.setTwist(10, kLight*error); 
      } 
      chassis.idle(); 
    } 
 
    void dimeToLine(int dir) { 
      if (dir == 1) { 
        while (analogRead(leftLightSensor) < 600){ 
            chassis.setMotorEfforts(-30, 30); 
        } 
        while (analogRead(rightLightSensor) < 150){ 
            chassis.setMotorEfforts(-30, 30); 
        } 
      } else { 
        while (analogRead(rightLightSensor) < 600){ 
            chassis.setMotorEfforts(30, -30); 
        } 
        while (analogRead(leftLightSensor) < 150){ 
            chassis.setMotorEfforts(30, -30); 
        } 
      } 
      chassis.idle(); 
    } 
 
 
    //TODO make this not terrible 
    void centerOnJunct(){ 
      chassis.setTwist(10,0); 
      delay(1028); 
      chassis.idle(); 
    } 
 
    void backFromWall(){ 
      chassis.setTwist(-10,0); 
      delay(1028); 
      chassis.idle(); 
    } 
 
    void drivePastLinesToT(int c) { 
      int passed = 0; 
      bool overline = false; 
      int timer = 0; 
      while (passed < c) { 
          chassis.setTwist(15, 0); 
          if ((analogRead(leftLightSensor)-analogRead(rightLightSensor)>50) && (analogRead(rightLightSensor) > 700)) { 
            if (!overline) { 
              overline = true; 
              passed ++; 
            }  
          } 
          timer ++; 
          if ((analogRead(leftLightSensor)-analogRead(rightLightSensor)>50) && (analogRead(rightLightSensor) <150) && timer > 50) { 
            overline = false; 
            timer = 0; 
          } 
      } 
      chassis.idle(); 
    } 
}; 
 
 
MobilityController mobilityController; 
 
bool alwaysFalse(){ 
  return false; 
} 
 
bool tLine() { 
    
    return (analogRead(leftLightSensor)-analogRead(rightLightSensor)>50) && (analogRead(rightLightSensor) > 600); 
} 
 
bool atWall() { 
  return rangefinder.getDistance() < 12.7; 
} 
 
void setup() { 
  // put your setup code here, to run once: 
    chassis.init(); 
    rangefinder.init(); 
    Serial.begin(115200); 
    // mobilityController.drivePastLinesToT(3); 
   // delay(10); 
   // mobilityController.centerOnJunct(); 
   // mobilityController.dimeToLine(1); 
   // mobilityController.lineFollow(&tLine); 
   // mobilityController.centerOnJunct(); 
   // mobilityController.dimeToLine(-1); 
   // mobilityController.lineFollow(&tLine); 
   // mobilityController.centerOnJunct(); 
   // mobilityController.dimeToLine(-1); 
    //mobilityController.lineFollow(&atWall); 
   // delay(500); 
   // mobilityController.dimeToLine(1); 
   // rangefinder.getDistance(); 
   // mobilityController.lineFollow(&tLine); 
   // mobilityController.centerOnJunct(); 
   // mobilityController.dimeToLine(-1); 
   // mobilityController.lineFollow(&atWall); 
   // mobilityController.backFromWall(); 
    //mobilityController.dimeToLine(1); 
   // rangefinder.getDistance(); 
   // mobilityController.lineFollow(&tLine); 
   // mobilityController.centerOnJunct(); 
   // mobilityController.lineFollow(&tLine); 
   // mobilityController.centerOnJunct(); 
   // mobilityController.lineFollow(&tLine); 
   // mobilityController.centerOnJunct(); 
    //mobilityController.dimeToLine(1); 
    //mobilityController.lineFollow(&atWall); 
    //chassis.turnFor(-90, 50); 
   // delay(6000); 
   // mobilityController.lineFollow(&tLine); 
   // mobilityController.centerOnJunct(); 
   // mobilityController.dimeToLine(1); 
   mobilityController.dimeToLine(- 1); 
   rangefinder.getDistance(); 
 mobilityController.lineFollow(&tLine); 
 mobilityController.centerOnJunct();
 mobilityController.dimeToLine(-1); 
 mobilityController.lineFollow(&atWall); 

 
   
} 
 
void loop() { 
  // put your main code here, to run repeatedly: 
} 