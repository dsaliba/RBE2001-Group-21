#pragma once

//Globally define state enum for use in multiple classes (see classes for more indepth definitions)
enum State {
    IDLE,                   //Idle all subsystems
    FOLLOWTOLINE,           //Line follow until a junciton is reached
    FOLLOWTOWALL,           //Line follow until ultrasonic detects an object
    DIMETOLINELEFT,         //Turn inplace left until the light sensor is centered on a line
    DIMETOLINERIGHT,        //Turn inplace right until the light sensor is centered on a line
    CENTERONJUNCT,          //Drive forward until the center of the wheels is over the point the light sensor was previously over
    BACKFROMWALL,           //Back up a small arbitrary amount
    PERMISSION              //Wait until an IR enter signal is given before advancing state
};

