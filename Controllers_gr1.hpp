//
//  Controllers_gr1.hpp
//
//
//  Created by William Chermanne on 27/02/17.
//
//

#include <stdio.h>
#include "MyIncludes_gr1.h"
#ifndef Controllers_gr1_hpp
#define Controllers_gr1_hpp

//#include "namespace_ctrl.h"

//NAMESPACE_INIT(ctrlGr1);

void StructControl_init(CtrlStruct *cvs);// Initialiser la structure controle
void displayControllers(CtrlStruct *cvs);

void LowLevelController(CtrlStruct *cvs, double *ref_speed, double Kp,  double Ki, double *command);
 void MiddleLevelController(CtrlStruct *cvs,double vref,double wref,double *Speed_ref);

void PI_controller(double *ref_speed, CtrlStruct *cvs, double Kp, double Ki, double *command);


void Position_controller(CtrlStruct *cvs,double errDist, double errAngle,double KpDist, double KpAngle, double *command);

void FromHighToMiddleLevel(CtrlStruct *cvs,double *command);

double Limiter(double input);

double* Kp_Ki_Computation(double overshoot, double time_response);

void StructControl_free(CtrlStruct *cvs);
void IsBlocked(CtrlStruct *cvs);

//NAMESPACE_CLOSE();

#endif /* Controllers_gr1_hpp */
