
#include <stdio.h>
#include "MyIncludes_gr1.h"
#include "MyIncludes_Moonwalker.h"
#include "Dynamixel_Functions.h"
#include "uswitch.h"

#ifndef Threads_hpp
#define Threads_hpp

//#include "//NAMESPACE_ctrl.h"

//NAMESPACE_INIT(ctrlGr1); // where X should be replaced by your group number
struct args {
    CtrlStruct *MyStruct;
    MyMotors motorR;
    MyMotors motorL;
    MyOdometers odoR;
    MyOdometers odoL;
    MyDynamixel dynaFrameBalls;
    MyDynamixel dynaElevator;
    MyDynamixel dynaTurnModules;
    MyVannes electrovannes;
    MyTourelle tourelle;
    MySwitch uswitches;
};


void * ThreadMotorR(void *atab);
void * ThreadMotorL(void *atab);
void * ThreadOdoL(void *atab);
void * ThreadOdoR(void *atab);
void * ThreadComputePosition(void *atab);
void * ThreadTourellePosition(void *atab);
void * ThreadAnglesBeacon(void *atab);

//NAMESPACE_CLOSE();
#endif /* Obstacles_gr1_hpp */
