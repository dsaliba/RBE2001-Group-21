#include "Gripper.h"
#include <servo32u4.h>

#include <Arduino.h>
#include "State.h"
#include "StateManager.h"

#define stateManager StateManager::get()
#define potPin A1

int pos = 1500;

int openPos = 1350;
int closePos = 1700;
int openPot = 280;
//int closePot = 320;
int closePot = 300;
const int tolerance = 3;

//199:1000
//294:1500
//380:2000

Servo32U4 servo;

void Gripper::init()
{
    servo.attach();
    write(1500);
}

void Gripper::update()
{
    switch (stateManager.getState())
    {
    case (GRIP): //Close gripper
        if (!stateManager.flagA) {
            stateManager.flagA = true;
            stateManager.startTimer(1500);
        }
        if (read() > closePot-tolerance) //Gripper is closed
        {   
            pos = closePos;
            write(closePos);
            stateManager.advanceState();
        }
        else
        {
            if (stateManager.checkTimer()) {
                Serial.println("yikes");
                write(openPos);
                pos = openPos;
                stateManager.advanceState();
            }
            //Switch to det reckoned timer
            // if (read() - map(pos, openPos, closePos, openPot, closePot) > 50)   //If there is too much resistance to closing, open
            // {
            //     Serial.println("yikes");
            //     write(openPos);
            //     pos = openPos;
            //     stateManager.advanceState();
            // }
        }
            Serial.println(read());
            pos += 50;
            write(pos);
            delay(50);
            
        break;
    case (RELEASE):     //Open gripper
        write(openPos);
        pos = openPos;
        stateManager.advanceState();
        break;
    case (GRIPCONFIG):
        int sweep = 1800;
        int lastpot = read();
        while(abs(lastpot-read()) < 10) {
            sweep += 10;
            write(sweep);
            delay(100);
        }

        lastpot = -1;
        while (abs(lastpot-read()) > 4) {
            lastpot = read();
            sweep += 40;
            write(sweep);
            delay(200);
        }
        closePot = lastpot-5;
        closePos = sweep -45;

        sweep-=40;

        lastpot = -1;
        while (abs(lastpot-read()) > 4) {
            lastpot = read();
            sweep -= 40;
            write(sweep);
            delay(200);
        }
        openPot = lastpot;
        openPos = sweep +40;

        write(openPos);

        Serial.print("Open: ");
        Serial.print(openPot);
        Serial.print(" : ");
        Serial.println(openPos);
        Serial.print("Close: ");
        Serial.print(closePot);
        Serial.print(" : ");
        Serial.println(closePos);

        stateManager.advanceState();

        break;
    }
}

void Gripper::write(int us) //Write position to gripper (between 1000 and 2000)
{
    servo.writeMicroseconds(us);
}

int Gripper::read() //Reaq position from potentiometer
{
    return analogRead(potPin);
}

void Gripper::up() {
    pos += 50;
    write(pos);
    Serial.print(pos);
    Serial.print(": ");
    Serial.println(read());
}

void Gripper::down() {
    pos -= 50;
    write(pos);
    Serial.print(pos);
    Serial.print(": ");
    Serial.println(read());
}

// 1500: 274
// 1600: 288
// 1700: 306
// 1800: 324
// 1900: 343
// 2000: 360