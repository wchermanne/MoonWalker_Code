#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"


void Manoeuver_Start(args *atab, CtrlStruct *cvs)
{
  double xnextpos,ynextpos;
  switch (cvs->struct_fsm->start_manoeuver_state)
  {
    case WAITMANOEUVER:
        printf(RED"WAITMANOEUVER\n"RESET);
    cvs->struct_fsm->start_manoeuver_state=BACKWARDMANOEUVER;
    break;

    case BACKWARDMANOEUVER:
        printf(RED"BACKMANOEUVER\n"RESET);

    // // printf(RED "I'm in the FORWARD1 state\n" RESET);
    if(atab->MyStruct->inputs->color==BLUE) // Ceux en haut
    {
         printf(CYN"BLUE\n"RESET);
      xnextpos= -400;
      ynextpos= -350;

    }
    else if(atab->MyStruct->inputs->color==YELLOW) // Ceux en bas
    {
          printf(RED"YELLOW\n"RESET);

      xnextpos= -400;
      ynextpos= 350;
    }
    Forward_distance(atab,cvs,cvs->struct_fsm->isDoneForward1,xnextpos,ynextpos);

    if(cvs->struct_fsm->isDoneForward1==1)
    {
    cvs->struct_fsm->isDoneForward1=0;
   cvs->struct_fsm->start_manoeuver_state =ROTATEMANOEUVER;
    }
    break;

    case ROTATEMANOEUVER:

    Rotate_to_angle(atab,cvs,PI);
    if(cvs->struct_fsm->isDoneRotate1==1)
    {
     cvs->struct_fsm->isDoneRotate1=0;
    cvs->struct_fsm->start_manoeuver_state =FORWARDMANOEUVER;
    }
    break;

    case FORWARDMANOEUVER:
                printf(CYN"FORWARDMANOEUVER\n"RESET);

            if(cvs->inputs->color==BLUE) // Ceux en haut
    {
                        printf(CYN"BLUE\n"RESET);

      xnextpos= -800;
      ynextpos= -350;

    }
    else if(cvs->inputs->color==YELLOW) // Ceux en bas
    {
    printf(RED"YELLOW\n"RESET);

      xnextpos= -800;
      ynextpos= 350;
    }
    Forward_distance(atab,cvs,cvs->struct_fsm->isDoneForward1,xnextpos,ynextpos);

    if(cvs->struct_fsm->isDoneForward1==1)
    {
    cvs->struct_fsm->isDoneForward1=0;
    cvs->struct_fsm->start_manoeuver_state =DONEMANOEUVER;
    }
    break;

    case DONEMANOEUVER:
    atab->motorR.setSpeed(0);
    atab->motorL.setSpeed(0);
    cvs->struct_fsm->isDoneManoeuver =1;
    break;
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
}

void Rotate_relative(args *atab,CtrlStruct *cvs,double angle)
{
  double finalAngle = cvs->struct_odometry->theta_t + angle;

  if(cvs->struct_odometry->theta_t-finalAngle>7*PI/180)
  {
    printf("Rotate counterclock!\n");

    atab->motorR.setSpeed(-15);
    atab->motorL.setSpeed(15);

  }
  else if (cvs->struct_odometry->theta_t-finalAngle<-7*PI/180)
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
}



void followPath(args* atab,CtrlStruct *cvs)
{

    KpKi = Kp_Ki_Computation(0.05,0.01);
         printf("Kp calculated! \n");
    //////////////////// Controllers ////////////////////
    FromHighToMiddleLevel(cvs,command);
    if(command[0] > 500)
    {
      command[0] = 500;
    }
    else if (command[0] < -500)
    {
      command[0] = -500;
    }
    if(command[1] > 2)
    {
      command[1] = 2;
      command[0] = 0;
    }
    else if (command[1] < -2)
    {
      command[1] = -2;
      command[0] = 0;
    }
    if (command[1] > 1 || command[1] <-1)
    {
      cvs->struct_control->isRotating =1;
    }
    else
    {
      cvs->struct_control->isRotating =0;
    }
    MiddleLevelController(cvs,command[0]/1000,command[1],command);
    LowLevelController(cvs,command, KpKi[0], KpKi[1],command);
    atab->motorR.setSpeed(command[0]);
    atab->motorL.setSpeed(command[1]);

    printf("Command of the right_wheel: %f\n",command[0]);
    printf("Command of the left_wheel: %f\n",command[1]);

}
