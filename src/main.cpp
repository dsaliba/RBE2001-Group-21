#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"


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

void setup()
{
  Serial.begin(9600);
  motor.setup();
  motor.reset();


  // Serial.println(motor.getPosition());
  // motor.setEffort(0);
  // delay(50);
  // motor.setEffort(-400);
  // delay(3000);
  // Serial.println(motor.getPosition());
  // delay(3000);
  // Serial.println(motor.getPosition());
  // motor.setEffort(0);
  // Serial.print("Time (ms)");
  // Serial.print("   ");
  // Serial.print("Position");
  // Serial.print("    ");
  // Serial.println("speedInRPM");
  motor.moveTo(CPR*3);
  delay(500);
  motor.moveTo(0);

}


void loop()
{
  if (buttonB.isPressed()) {
    motor.reset();
    motor.moveTo(CPR);
  }
  if (buttonC.isPressed()) {
    motor.reset();
    motor.moveTo(-CPR);
  }
  // timeToPrint = millis() + sampleTime;
  // oldPosition = motor.getPosition();
  // while (buttonB.isPressed())
  // {
  //   // The button is currently pressed.
  //   motor.setEffort(motorEffort);
  //   if ((now = millis()) > timeToPrint)
  //   {
  //     timeToPrint = now + sampleTime;
  //     newPosition = motor.getPosition();
  //     //Not sure if the casting I do here is required but im scared of integer division, (refer to the first 3 lines of Heaart on Ice)
  //     speedInRPM = ((float)(newPosition-oldPosition)/270.0)/((float)sampleTime/60000.0);  
  //     Serial.print(now);
  //     Serial.print("          ");
  //     Serial.print(newPosition);
  //     Serial.print("          ");
  //     Serial.println(speedInRPM);
  //     oldPosition = newPosition;
      
  //   }
    
  // }
    
  // motor.setEffort(0);
}