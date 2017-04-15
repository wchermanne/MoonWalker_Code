//
//  FSM_gr1.hpp
//
//
//  Created by William Chermanne on 6/03/17.
//
//

#ifndef FSM_gr1_hpp
#define FSM_gr1_hpp

#include <stdio.h>
#include "MyIncludes_gr1.h"
#include "MyIncludes_Moonwalker.h"

#define BASIS 0
#define BLUE_MID_FUSEE 1
#define YELLOW_MID_FUSEE 2

#define BLUE_DOWN_FUSEE 3
#define YELLOW_UP_FUSEE 4

#define BLUE_BALLS 5
#define YELLOW_BALLS 6

#define MIDDLE_RAMP 7
#define LOWER_RAMP 8
#define UPPER_RAMP 9

//NAMESPACE_INIT(ctrlGr1);
void StructFSM_init(CtrlStruct *cvs);
void matchFSM(args* atab,CtrlStruct *cvs,MyVannes electrovannes,MyDynamixel dynaElevator,MyDynamixel dynaTurnModules);
void StructFSM_free(CtrlStruct *cvs);
int updateObjective(CtrlStruct *cvs);
void Stop_Everything(args* atab);
void followPath(args* atab,CtrlStruct *cvs);

//NAMESPACE_CLOSE();

#endif /* FSM_gr1_hpp */
