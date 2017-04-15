//
//  Tower_gr1.hpp
//
//
//  Created by William Chermanne on 20/02/17.
//
//
/*!
 * \file filename_grX.h
 * \brief File description
 */

#include <stdio.h>
#ifndef Tower_gr1_hpp
#define Tower_gr1_hpp
#include "MyIncludes_gr1.h"

//#include "NAMESPACE_ctrl.h"

//NAMESPACE_INIT(ctrlGr1); // where X should be replaced by your group number

// add your extra code (prototypes, macros...) here
void StructTower_init(CtrlStruct *cvs);
double angle_opponent(CtrlStruct *cvs);
double distance_opponent(CtrlStruct *cvs);
double *anglesTable_fixed(CtrlStruct *cvs);
void tower_display(CtrlStruct *cvs);
double cotan(double a);
double adjust_value_to_bounds( double cot ,double cot_max );
double weighted_dist_min(double angle_opp,double dist_min);
void opponent_position(CtrlStruct *cvs,double distance_opponent, double angle_opponent);
double outliersDetection(CtrlStruct *cvs, double distance);


//NAMESPACE_CLOSE();

#endif // end of header guard
