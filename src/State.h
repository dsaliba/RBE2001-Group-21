#pragma once

//Globally define state enum for use in multiple classes (see classes for more indepth definitions)
enum State {
    IDLE,                   //Idle all subsystems
    FOLLOWTOLINE,           //Line follow until a junciton is reached
    FOLLOWTOMID,           //Line follow until ultrasonic detects an object
    FOLLOWTOSTAGING,
    FOLLOWTO25,
    FOLLOWTO45,
    FOLLOWTODEPO,
    DIMETOLINELEFT,         //Turn inplace left until the light sensor is centered on a line
    DIMETOLINERIGHT,        //Turn inplace right until the light sensor is centered on a line
    CENTERONJUNCT,          //Drive forward until the center of the wheels is over the point the light sensor was previously over
    BACKFROMWALL,           //Back up a small arbitrary amount
    QUARTERTURNLEFT,        //Turn left 90 degrees without needing a line
    QUARTERTURNRIGHT,       //Turn right 90 degrees without needing a line
    UNHOOK,
    PERMISSION,             //Wait until an IR enter signal is given before advancing state
    CALIBRATION,
    BUTTONPERMISSION,
    GRIP,                   //Close Gripper
    GRIPCONFIG,             //Maintance routine for generating gripper constants BLOCKING
    RELEASE,                 //Open Gripper
    FBFLAT,
    FB45,
    FB25,
    FBUNLATCH45,
    FBUNLATCH

};

