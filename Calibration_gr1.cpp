//
//  Calibration_gr1.cpp
//
//
//  Created by Matthieu Fontaine on 28/03/17.
//
//
#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"


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

void Calibrate_Start(CtrlStruct *cvs)
{
   printf("Calibrate_Start to implement\n");
}

void Calibrate_Match(CtrlStruct *cvs)
{
  printf("Calibrated_Match to implement\n");
}


void Calibrate_y(CtrlStruct *cvs)
{
  printf("Calibrate_y to implement\n");
}

void Calibrate_x(CtrlStruct *cvs)
{
  printf("Calibrate_x to implement\n");
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
        cvs->struct_control->sum_error_dist = 0.0;
        cvs->struct_control->sum_error_angle = 0.0;
        atab->motorR.setSpeed(0.0);
        atab->motorR.setSpeed(0.0);
      }
    }

void Rotate_to_angle(args *atab,CtrlStruct *cvs,double angle)
{
  if(cvs->struct_odometry->theta_t-angle>7*PI/180)
  {
    printf("Rotate counterclok!\n");

    atab->motorR.setSpeed(-15);
    atab->motorL.setSpeed(15);

  }
  else if (cvs->struct_odometry->theta_t-angle<-7*PI/180)
  {
    printf("Rotate clok!\n");
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
