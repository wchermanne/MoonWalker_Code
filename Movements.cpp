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

void followPath(args* atab,CtrlStruct *cvs)
{

}

