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


//Class for handeling smart movment commands
class MobilityController {
  public:

  //Simple line following, method takes a pointer to a predicate as a param, will stop once the predicate evaluates to true
    void lineFollow(bool (*predicate)()) {
      while (!predicate()){
        float error = analogRead(leftLightSensor)-analogRead(rightLightSensor);   //Error function is simply difference between light sensors
        chassis.setTwist(10, kLight*error);  //Proportunal controll
      }
      chassis.idle();
    }

    //Turn on a dime till a line is hit, 1 = left, -1 = right
    //TODO: clean this code by removing if and multiplying values by dir
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
    //A litle deadreckoning to move from having the light sensor over a line to having the center of the bot over that line
    void centerOnJunct(){
      chassis.setTwist(10,0);
      delay(1028);
      chassis.idle();
    }

    //Back up a small smount to allow for turning without hitting any mechanisms on walls
    void backFromWall(){
      chassis.setTwist(-10,0);
      delay(1028);
      chassis.idle();
    }

    //TODO: make this not the worst thing ive ever written
    //Drive at a constant speed past c lines on the ground
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

//This shouldnt need a comment but I dont know how agressivly im supposed to comment so im playing it safe
bool alwaysFalse(){
  return false;
}

//returns true when light sensor is over a line that is perpendi- perpondicu- perpr- when the line and the robot make a t shape
bool tLine() {
   
    return (analogRead(leftLightSensor)-analogRead(rightLightSensor)>50) && (analogRead(rightLightSensor) > 600);
}

//Uses ultrasonic, returns true while near a wall
bool atWall() {
  return rangefinder.getDistance() < 25;
}

void setup() {
  // put your setup code here, to run once:
    chassis.init();
    rangefinder.init();
    Serial.begin(115200);
    mobilityController.dimeToLine(-1);
    mobilityController.lineFollow(&tLine);
    mobilityController.centerOnJunct();
    mobilityController.dimeToLine(-1);
    mobilityController.lineFollow(&atWall);




  
}

void loop() {
  // put your main code here, to run repeatedly:
}

