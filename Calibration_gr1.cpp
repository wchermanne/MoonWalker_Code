//
//  Calibration_gr1.cpp
//
//
//  Created by Matthieu Fontaine on 28/03/17.
//
//
#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"
#define PI 3.14159265358979323


void StructCalibration_init(CtrlStruct *cvs)
{
  cvs->struct_calibration->position_x_act=0;
  cvs->struct_calibration->position_y_act=0;
  cvs->struct_calibration->isStocked=0;
  cvs->struct_calibration->isCalibratedX=0;
  cvs->struct_calibration->isCalibratedY=0;
  cvs->struct_calibration->isDoneForward1=0;
  cvs->struct_calibration->isDoneForward2=0;
  cvs->struct_calibration->isDoneRotate1=0;
  cvs->struct_calibration->isDoneRotate2=0;
}

void Calibrate_Start(CtrlStruct *cvs, args *atab)
{
    int isDoneForward1 = cvs->struct_calibration->isDoneForward1;
    int isDoneForward2 = cvs->struct_calibration->isDoneForward2;
    int isDoneRotate1 = cvs->struct_calibration->isDoneRotate1;
  double xnextpos,ynextpos;
//  int myID = cvs->inputs->robot_id;
  switch (cvs->struct_calibration->calibrate_state)
  {
    case WAITCALIB:
    printf(RED "Waiting for calibration...\n" RESET);
    StructCalibration_init(cvs);
    cvs->struct_calibration->calibrate_state = CALIBRATEY;
    break;

    case CALIBRATEY:
    printf(GRN "Calibrating Y...\n" RESET);
    Calibrate_y(atab,cvs);
    if(cvs->struct_calibration->isCalibratedY==1)
    {
      cvs->struct_calibration->calibrate_state =FORWARD1;
    }
    break;

    case FORWARD1:
    Forward_distance(atab,cvs,isDoneForward1,-100,600);
    if(cvs->struct_calibration->isDoneForward1==1)
    {
      cvs->struct_calibration->calibrate_state =ROTATE1;
      cvs->struct_calibration->isDoneForward1=0;
      cvs->struct_calibration->isDoneForward2=0;
      cvs->struct_fsm->isDoneForward1=0;
    }
    break;

    case ROTATE1:
    printf(GRN "Rotating towards base...\n" RESET);
    if(cvs->inputs->color == YELLOW){
        Angle_controller(atab,cvs,PI);
    }
    else if(cvs->inputs->color == BLUE){
        Angle_controller(atab,cvs,-PI);
    }
    if(cvs->struct_calibration->isDoneRotate1==1)
    {
      cvs->struct_calibration->calibrate_state =CALIBRATEX;
      cvs->struct_calibration->isDoneRotate1==0;
    }
    break;

    case CALIBRATEX:
    printf(RED "Calibrating X...\n" RESET);
    Calibrate_x(atab,cvs);
    if(cvs->struct_calibration->isCalibratedX==1)
    {
      atab->motorR.setSpeed(0);
      atab->motorL.setSpeed(0);
      cvs->struct_calibration->calibrate_state =FORWARD2;
    }
    break;

    case FORWARD2:
    Forward_distance(atab,cvs,isDoneForward2,-800,600);
    if(cvs->struct_calibration->isDoneForward2==1)
    {
      cvs->struct_calibration->calibrate_state =DONE_CALIB;
      cvs->struct_calibration->isDoneForward1=0;
      cvs->struct_calibration->isDoneForward2=0;
      cvs->struct_fsm->isDoneForward1=0;
    }
    break;

    case DONE_CALIB:
    printf(RED "Calibration done.\nAll systems on standby.\n\nAwaiting start signal...\n" RESET);
    atab->motorR.setSpeed(0);
    atab->motorL.setSpeed(0);
    break;
  }

}

void Calibrate_Match(CtrlStruct *cvs)
{
  printf("Calibrated_Match to implement\n");
  // no can do!
}


void Calibrate_y(args *atab,CtrlStruct *cvs)
{
  if(cvs->struct_calibration->isCalibratedY==0)
  {
    int my_right_switch =  cvs->struct_sensors->right_uswitch;
    int my_left_switch = cvs->struct_sensors->left_uswitch;
    int isDone = 0;
    double yt_calib, theta_t_calib;
    if(cvs->inputs->color == YELLOW)
    {
      yt_calib = 1440; //
      theta_t_calib = PI/2;
    }
    else if(cvs->inputs->color == BLUE)
    {
      yt_calib = -1440; // position du centre des roues
      theta_t_calib = -PI/2;
    }
    else
    {
      // // printf("Error : robot is undefined");
      isDone =1;
    }
    if(!isDone)
    {
      atab->motorR.setSpeed(20);
      atab->motorL.setSpeed(20);

      if (my_right_switch && my_left_switch)
      {
        atab->motorR.setSpeed(0);
        atab->motorL.setSpeed(0);
        cvs->struct_odometry->y_t = yt_calib ;
        cvs->struct_odometry->theta_t = theta_t_calib;

        isDone =1;
        cvs->struct_calibration->isCalibratedY=1;
      }
    }
  }
}

void Calibrate_x(args *atab, CtrlStruct *cvs)
{
    if(cvs->struct_calibration->isCalibratedX==0)
  {
    int my_right_switch =  cvs->struct_sensors->right_uswitch;
    int my_left_switch = cvs->struct_sensors->left_uswitch;
    double xt_calib, theta_t_calib;

    xt_calib=-940;
    theta_t_calib=PI;
    int isDone = 0;
    if(!isDone)
    {
      atab->motorR.setSpeed(20);
      atab->motorL.setSpeed(20);
      if (my_right_switch && my_left_switch)
      {
        atab->motorR.setSpeed(0);
        atab->motorL.setSpeed(0);
        cvs->struct_odometry->x_t = xt_calib ;
        cvs->struct_odometry->theta_t = theta_t_calib;
        isDone =1;
        cvs->struct_calibration->isCalibratedX=1;
      }
    }
  }
}

void Forward_distance(args *atab, CtrlStruct *cvs,int isDone,double nextPosition_x,double nextPosition_y)
{
  double errDist;
  double errAngle;
  double *command=cvs->struct_control->command;

  // Position actuelle du robot
  double position_x=cvs->struct_odometry->x_t;
  double position_y=cvs->struct_odometry->y_t;

  double diffx= nextPosition_x-position_x;
  double diffy= nextPosition_y-position_y;

  double arctan=atan2(diffy,diffx);
  double odo_angle= cvs->struct_odometry->theta_t;

  if(arctan<0)
  {
    arctan+=2*PI;
  } // Remettre entre 0 et 2*PI

  if(odo_angle<0)
  {
    odo_angle+=2*PI;
  }
  if(arctan > odo_angle)
  {
    if(arctan - odo_angle > PI)
    {
      errAngle = (2*PI) -(arctan - odo_angle);
      errAngle = -errAngle;
    }
    else
    {
      errAngle = arctan - odo_angle;
    }
  }
  else
  {

    if(odo_angle - arctan > PI)
    {
      errAngle = (2*PI) -(odo_angle- arctan);
      errAngle = errAngle;
    }
    else
    {
      errAngle = odo_angle- arctan;
      errAngle = -errAngle;
    }
  }

 errDist = EuclidianDistance(nextPosition_x,nextPosition_y,position_x,position_y);

  if(errDist>15.0)
  {

    Position_controller(cvs,errDist,errAngle,0.53, 1,command);
    MiddleLevelController(cvs,command[0]/1000,command[1],command);
    LowLevelController(cvs, command, 50, 1061,command);
    atab->motorR.setSpeed(command[0]);
    atab->motorL.setSpeed(command[1]);
  }

  else
  {
    cvs->struct_fsm->isDoneForward1=1;
    cvs->struct_calibration->isDoneForward1=1;
    cvs->struct_calibration->isDoneForward2=1;
    cvs->struct_control->sum_error_dist = 0.0;
    cvs->struct_control->sum_error_angle = 0.0;
    atab->motorR.setSpeed(0.0);
    atab->motorR.setSpeed(0.0);
  }


}

void Angle_controller(args *atab, CtrlStruct *cvs, double angle_dest) // to be modified in order to remove 3 last args + isDoneRotate =1
{
    double command[2];
    double arctan = angle_dest;
    double command_low[2];
    double odo_angle= cvs->struct_odometry->theta_t;
    double KpAngle = 1;
    double KiAngle =1;
    double errAngle = 0.0;
    if(arctan<0)
    {
        arctan+=2*PI;
    } // Remettre entre 0 et 2*PI

    if(odo_angle<0)
    {
        odo_angle+=2*PI;
    }
    if(arctan > odo_angle)
    {

        if(arctan - odo_angle > PI)
        {
            errAngle = (2*PI) -(arctan - odo_angle);
            errAngle = -errAngle;
        }
        else
        {
            errAngle = arctan - odo_angle;
        }
    }
    else
    {

        if(odo_angle - arctan > PI)
        {
            errAngle = (2*PI) -(odo_angle- arctan);
            errAngle = errAngle;
        }
        else
        {
            errAngle = odo_angle- arctan;
            errAngle = -errAngle;
        }
    }
    double delta_t = 0.01;
    double command_w;
//ref_speed[0] - curr_speed_right; // speed error right wheel definition
    double curr_error_angle = errAngle;//ref_speed[1] - curr_speed_left; // speed error left wheeldefinition
    cvs->struct_control->sum_error_angle_controller += curr_error_angle;
    command_w = KpAngle*errAngle + KiAngle*cvs->struct_control->sum_error_angle_controller*delta_t; // w

    if(errAngle >5*PI/180)
    {
    command[0] = 0;
    command[1] = command_w;
    MiddleLevelController(cvs,command[0],command[1],command);
    LowLevelController(cvs, command, 15, 1961, command);
    atab->motorR.setSpeed(command[0]);
    atab->motorL.setSpeed(command[1]);
    }
    else
    {
        cvs->struct_control->isAngleControlled =1;
        cvs->struct_calibration->isDoneRotate1=1;
        cvs->struct_control->sum_error_angle_controller =0;
        atab->motorR.setSpeed(0);
        atab->motorL.setSpeed(0);
    }
}


void Rotate_to_angle(args *atab,CtrlStruct *cvs,double angle)
{
  if(cvs->struct_odometry->theta_t-angle>7*PI/180)
  {
    printf("Rotate counterclock!\n");

    atab->motorR.setSpeed(-15);
    atab->motorL.setSpeed(15);

  }
  else if (cvs->struct_odometry->theta_t-angle<-7*PI/180)
  {
    printf("Rotate clock!\n");
    atab->motorR.setSpeed(15);
    atab->motorL.setSpeed(-15);
  }
  else
  {
    printf("No rotate!\n");
    atab->motorR.setSpeed(0);
    atab->motorL.setSpeed(0);
    cvs->struct_fsm->isDoneRotate1=1;
  }
  /* double odo_angle = cvs->struct_odometry->theta_t;

  double arctan=odo_angle-angle;
  double errAngle;

  if(arctan<0)
  {
  arctan+=2*PI;
} // Remettre entre 0 et 2*PI

if(odo_angle<0)
{
odo_angle+=2*PI;
}
if(arctan > odo_angle)
{

if(arctan - odo_angle > PI)
{
errAngle = (2*PI) -(arctan - odo_angle);
errAngle = -errAngle;
}
else
{
errAngle = arctan - odo_angle;
}
}
else
{

if(odo_angle - arctan > PI)
{
errAngle = (2*PI) -(odo_angle- arctan);
errAngle = errAngle;
}
else
{
errAngle = odo_angle- arctan;
errAngle = -errAngle;
}
if(errAngle>ERR_ANGLE)
{
Position_controller(cvs,0,errAngle,0.7, 1.5,command);
}*/
}




//NAMESPACE_CLOSE();
