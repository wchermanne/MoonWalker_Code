/*!
 * \file CtrlStruct_gr1.h
 * \brief Controller main structure
 */

#ifndef _CTRL_STRUCT_GR1_H_
#define _CTRL_STRUCT_GR1_H_

//#include "ctrl_io.h"
#include <stdlib.h>
#include "MyIncludes_gr1.h"
#include "Astar_struct_gr1.h"

enum RobotState
{
    BEGINNING,
    CALIBRATION,
    MANOEUVER_START,
    GENERATE_PATH,
    SET_OBJECTIVE,
    FOLLOW_PATH,
    PERFORM_ACTION,
    ISBLOCKED
};



enum RobotNextAction
{
    NO_ACTION,
    TAKE_MODULES,
    TAKE_BALLS,
    EJECT_MODULES,
    EJECT_BALLS,
    HOMOLOGATION
};

enum RobotManoeuverState
{
    WAITMANOEUVER,
    BACKWARDMANOEUVER,
    FORWARDMANOEUVER,
    ROTATEMANOEUVER,
    ROTATEMANOEUVER2,
    DONEMANOEUVER
};

enum ModuleState
{
    WAITMODULE,
    OPEN_GRIPPER,
    GRIPPER_OUT,
    CLOSE_GRIPPER,
    BACKWARD_MODULE,
    GRIPPER_IN,
    OPEN_GRIPPER_2,
    DYNA_ELEVATOR,
    FORWARD_MODULE,
    DONE_MODULE,
    DYNA_ELEVATOR_DOWN,
    ABORT_MODULE
};

enum HomologationState
{
    WAIT_HOMO,
    GRIPPER_OUT_HOMO,
    CLOSE_GRIPPER_HOMO,
    BACKWARD_HOMO,
    TURN_HOMO,
    DROP_MODULE_HOMO
};

enum CalibrationState
{
    WAITCALIB,
    CALIBRATEY,
    FORWARD1,
    ROTATE1,
    CALIBRATEX,
    FORWARD2,
    DONE_CALIB
};

enum BallState
{
  WAITBALL,
  LOWERBALL,
  FORWARDBALL,
  ROTATEBALL,
  BACKBALL,
  RISEBALL,
  DONEBALL
}

typedef struct StructTower
{
    int counter;///< simple variable
    int counterDist;
    int counterAngle;
    double tabDistance[5];
    double tabAngle[5];
    double tabAngleDiff[5];
    double tabFixed[3]; ///< 1D tabular of double
    double dist;
    double previousDistance;
    double previousAngle;
    int previous_rising_index;
    int previous_falling_index;
    double previousMeanDistance;
    double previousMeanAngle;
    double AngleDiff;

    double previousXr;
    double previousYr;
    double currentXr;
    double currentYr;

    ////////////////////// FOR WEBSITE ALSO //////////////////////
    double x_opponent;
    double y_opponent;
    double distOpp;
    double angleOpp;

} StructTower;

// StructWheels
typedef struct StructOdometry
{
    int counter;///< simple variable
    double prev_distance[2];
    double prev_speed[2]; ///< 1D tabular of double for previous speed of both wheels

    double xsiRpoint[3];
    ////////////////////// FOR WEBSITE ALSO //////////////////////
    double x_t;
    double y_t;
    double theta_t;
} StructWheels;

// Controller structure
typedef struct StructControl
{
    double Kp,Ki;
  double sum_error[2]; // sum of all errors
  double currentError[2];
  double previousError[2];
  double previousCommand[2];
  double previousCommandLtd[2];
    double Speed_ref[2];

  double errDist;
  double errAngle;
  int counterNode;
  double command[2];
  double Kt;          // Anti-windup constant
  double Tsample;     // sampling period
  double sum_error_angle;
  double sum_error_dist;

  double prev_sum_error_dist;
  double prev_sum_error_dist_2;
  int isBlocked;
  double t_blocked;
  int counter;
  int isRotating;

    ////////////////////// FOR WEBSITE ALSO //////////////////////
  double lowlevelcommand0;
  double lowlevelcommand1;
    double lowlevelKp;
double lowlevelKi;

  double middlelevelforwardspeed;
  double middlelevelrotatespeed;
  double middlelevelcommand0;
  double middlelevelcommand1;
  double KpDist;
  double KpAngle;
  double KiDist;
  double KiAngle;
  int emergencyStop;
  double Diffx;
  double Diffy;
  double sum_error_angle_controller;
  int isAngleControlled;
}StructControl;



typedef struct StructPathPlanning
{
  Astar *astar;
} StructPathPlanning;


typedef struct Target
{
  double x;
  double y;
  int isTaken;
  int indexTarget;
  int score;
} Target;


typedef struct StructFSM
{
  RobotState robot_state;
  RobotState robot_previous_state;

  RobotNextAction robot_previous_action;
  RobotNextAction robot_next_action;

  RobotManoeuverState start_manoeuver_state;
  HomologationState homologation_state;

  Target **TargetArray;

        ////////////////////// Path planning  //////////////////////
  int isGenerated;
double time_arrived_target;
  int base_reached;
        ////////////////////// Opponent avoidance  //////////////////////
  int opponentDetected;
  int isAvoiding;
  double time_arrived_opp;
  int arrived_opp;


  int sum_indexes;
  int sum_indexes_previous;



      ////////////////////// FOR WEBSITE ALSO //////////////////////
  int robotState;
    int target_reached;
  int indexNextTarget;
  int nextNodeIndex;
  int indexNextAction;
  int isDoneManoeuver;



        ////////////////////// OTHER  //////////////////////

int isDoneBackward1;
int isDoneForward1;
int isDoneRotate1;
int isDoneForward2;
int isDoneForward3;

int isHomologation;
} StructFSM;

typedef struct StructCalibration
{
  double position_x_act;
  double position_y_act;
  int isStocked;
  int isCalibratedX;
  int isCalibratedY;
  int isDoneForward1;
  int isDoneForward2;
  int isDoneRotate1;
  int isDoneRotate2;
  CalibrationState calibrate_state;

} StructCalibration;


typedef struct StructWebsite
{
    int sockfd;
    int TakeModule;
    int TakeBalls;
    int PushModule;
} StructWebsite;

typedef struct StructActions
{
    ModuleState take_modules_state;
    ModuleState eject_modules_state;
    BallState ball_state;
    int nb_modules;
    int nb_modules_ejected;
    int ActionIsDone;
    int ModulesTaken;
    int ModulesEjected;
    int BallsTaken;
    int BallsEjected;
    double time_action_started;
    double time_gripper_open;
    double time_dyna_elevator;

    double time_enter_state;
    int HomologationDone;
    int isDoneForwardBall1;
    int isDoneForwardBall2;
    int isDoneRotateBall;
} StructActions;

typedef struct StructSensors
{
    int left_uswitch;
    int right_uswitch;
    int gripper_uswitch;
    int back_endway;
    int front_endway;
    int photo_chips;

} StructSensors;

/// Main controller structure
typedef struct CtrlStruct
{
	CtrlIn *inputs;   ///< controller inputs
	CtrlOut *outputs; ///< controller outputs

  // Created structures
  StructTower *struct_tower;
  StructOdometry *struct_odometry;
  StructControl *struct_control;
  StructPathPlanning *struct_path_planning;
  StructFSM *struct_fsm;
  StructCalibration *struct_calibration;
  StructWebsite *struct_website;
  StructActions *struct_actions;
  StructSensors *struct_sensors;


} CtrlStruct;


// function prototypes
CtrlStruct* init_CtrlStruct(CtrlIn *inputs, CtrlOut *outputs);
void free_CtrlStruct(CtrlStruct *cvs);

//NAMESPACE_CLOSE();

#endif
