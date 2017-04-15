//
//  Calibration_gr1.hpp
//
//
//  Created by Matthieu Fontaine on 28/03/17.
//
//

#ifndef Calibration_gr1_hpp
#define Calibration_gr1_hpp

#include <stdio.h>
#include "MyIncludes_gr1.h"
#include "MyIncludes_Moonwalker.h"
#include "Threads.hpp"


//NAMESPACE_INIT(ctrlGr1);
void Calibrate_Start(CtrlStruct *cvs,args *atab);
void Calibrate_y(args *atab,CtrlStruct *cvs);
void Forward_distance(args *atab, CtrlStruct *cvs,int isDone,double nextPosition_x,double nextPosition_y);
void Rotate_to_angle(args *atab,CtrlStruct *cvs,double angle);
void Calibrate_x(args *atab,CtrlStruct *cvs);
void StructCalibration_init(CtrlStruct *cvs);
void Calibrate_Match(CtrlStruct *cvs);
void Angle_controller(args *atab, CtrlStruct *cvs, double angle_dest);

//NAMESPACE_CLOSE();

#endif /* Calibration_gr1_hpp */
