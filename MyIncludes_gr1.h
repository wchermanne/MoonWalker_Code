//
//  MyIncludes_gr1.h
//
//
//  Created by William Chermanne on 20/03/17.
//
//

#ifndef MyIncludes_gr1_h
#define MyIncludes_gr1_h


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"

#define CARRE(X) (X)*(X)

#define NBR_NODES 42
#define NBR_ADJ 5 // 5 if Harcode, else 8

#define DIST_MIN 25
#define ERROR_DISTANCE 50

#define NBR_STATES 3
#define PI 3.14159265358979323846
#define CONSIDERATION_RADIUS 100.0 // in cm
#define RADIUS_OPPONENT 30.0 // in cm
#define CONSIDERATION_ANGLE 70.0// in degrees
#define DISTANCE_ERROR 50.0 // in mm
#define BLUE 0
#define YELLOW 1
#define BEACON_RADIUS 40.0 // in cm

#define VALVE_GRIPPER 1
#define VALVE_FUNNY 2
#define VALVE_FRAME 3
#define VALVE_MODULES 4

// ALL TIME OUTS
#define TIME_OUT_TAKE_MODULES 60
#define TIME_OUT_EJECT_MODULES 60
#define TIME_OUT_TAKE_BALLS 60
#define TIME_OUT_EJECT_BALLS 60
#define TIME_GRIPPER_CLOSED 10
#define TIME_GRIPPER_OUT 10
#define TIME_GRIPPER_IN  10
#define TIME_DYNA_ELEVATOR 0.5

#define X_FRONT_MODULES -800;
#define Y_FRONT_MODULES 350;









#include "CtrlStruct_gr1.h"

#include "Astar_gr1.hpp"
#include "Astar_struct_gr1.h"
#include "ctrl_main_gr1.h"

#include "ctrl_io.h"
#include "Calibration_gr1.hpp"
#include "Controllers_gr1.hpp"
#include "ctrl_main_gr1.h"

#include "Keyboard_gr1.hpp"
#include "FSM_gr1.hpp"
#include "UsefulFunctions_gr1.hpp"
#include "Obstacles_gr1.hpp"
#include "Odometry_gr1.hpp"
#include "Tower_gr1.hpp"
#include <math.h>
#include <stdlib.h>
#include <string.h>


#endif /* MyIncludes_gr1_h */
