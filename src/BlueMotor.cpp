#include <Arduino.h>
#include <BlueMotor.h>
#include <Romi32U4.h>

//Yeah our brains are pretty big
#define pwmbound(x) ((x)>400?400:((x)<-400?-400:x))

int oldValue = 0;
int newValue;
long count = 0;
unsigned time = 0;

int qeOld = 0;
int qeNew = 0;

const float kP = 0.25;

//A flattned matrix for relating the old and new values as indecies to the direction of encoding, (0) represents an uh-oh-spaghettio state
//I chode (0) so that it gets ignored but I keep track of which states are bad so I can swap it out with a flag for testing
int EncoderMatrix [16] = {0,-1,1,(2),1,0,(0),-1,-1,(0),0,1,(0),1,-1,0};

BlueMotor::BlueMotor()
{
}

void BlueMotor::setup()
{
    pinMode(PWMOutPin, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    TCCR1A = 0xA8; //0b10101000; //gcl: added OCR1C for adding a third PWM on pin 11
    TCCR1B = 0x11; //0b00010001;
    ICR1 = 400;
    OCR1C = 0;

    attachInterrupt(digitalPinToInterrupt(ENCA), isr, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCB), isr, CHANGE);
    reset();
}

long BlueMotor::getPosition()
{
    return count;
}

void BlueMotor::reset()
{
    count = 0;
}


//Lab 4 func because we are bored
void BlueMotor::isr()
{
    qeNew = digitalRead(ENCA) * 2 + digitalRead(ENCB);  //Get decimal value 0-3 coresponding to current state
    count += EncoderMatrix[qeNew*4+qeOld]; //Convert old and new values into a single index for flattened matrix using M A T H
    qeOld=qeNew;
}

//Lab 3 func
// void BlueMotor::isr()
// {
//     count++;
// }

void BlueMotor::setEffort(int effort)
{
    if (effort < 0)
    {
        setEffort(-effort, true);
    }
    else
    {
        setEffort(effort, false);
    }
}

void BlueMotor::setEffort(int effort, bool clockwise)
{
    if (clockwise)
    {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
    }
    else
    {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
    }
    OCR1C = constrain(effort, 0, 400);
}



void BlueMotor::moveTo(long target)  //Move to this encoder position within the specified
{                                    //tolerance in the header file using proportional control
                                     //then stop
    Serial.println(target-count);
    while (abs((target-count)) > tolerance) {
        int error = target-count;
        setEffort(kP*error+(error>0?100:-100));
    }

    
    Serial.println(count);
    setEffort(0);
}
