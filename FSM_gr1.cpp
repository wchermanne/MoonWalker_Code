//
//  FSM_gr1.cpp
//
//
//  Created by William Chermanne on 6/03/17.
//
//
#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"

#define NBR_TARGETS 10


//NAMESPACE_INIT(ctrlGr1);

/* This function initializes the StructFSMstructure
*
* param[in] : cvs controller main structure
*/

void StructFSM_init(CtrlStruct *cvs)
{
  // printf(RED "I'm initializing the FSM \n" RESET);
  cvs->struct_fsm->indexNextTarget=0;
    cvs->struct_fsm->indexNextAction=0;

  cvs->struct_fsm->TargetArray = (Target**) malloc(NBR_TARGETS*sizeof(Target));

  cvs->struct_fsm->robot_state=BEGINNING;
  cvs->struct_fsm->robot_next_action=NO_ACTION;
  cvs->struct_fsm->robot_previous_state=BEGINNING;
  cvs->struct_fsm->robot_previous_action=NO_ACTION;

  RobotNextAction robot_next_action;

  cvs->struct_fsm->isGenerated=0;
  cvs->struct_fsm->opponentDetected=0;
  cvs->struct_fsm->sum_indexes=0;
  cvs->struct_fsm->sum_indexes_previous=0;
  cvs->struct_fsm->time_arrived_target=0.0;

  cvs->struct_fsm->target_reached=0;
  cvs->struct_fsm->base_reached=0;
  cvs->struct_fsm->isAvoiding = 0;
  cvs->struct_fsm->arrived_opp = 1;
  cvs->struct_fsm->time_arrived_opp =0;
  cvs->struct_fsm->nextNodeIndex=0;
  cvs->struct_fsm->isDoneBackward1=0;
  cvs->struct_fsm->isDoneForward1=0;
  cvs->struct_fsm->isDoneRotate1=0;
  cvs->struct_fsm->isDoneManoeuver =0;
cvs->struct_fsm->isDoneRotate1=0;
cvs->struct_fsm->isDoneForward2=0;
cvs->struct_fsm->isDoneForward3=0;

cvs->struct_fsm->a=0;


  for (int i = 0; i < NBR_TARGETS; i++)
  {
    cvs->struct_fsm->TargetArray[i] = (Target*) malloc(sizeof(Target));
    cvs->struct_fsm->TargetArray[i]->isTaken=0;
  }
    if(cvs->inputs->color=BLUE)
    {
        cvs->struct_fsm->TargetArray[BASIS]->x=-850;
        cvs->struct_fsm->TargetArray[BASIS]->y=-620;
    }
    else
    {
        cvs->struct_fsm->TargetArray[BASIS]->x=-850;
        cvs->struct_fsm->TargetArray[BASIS]->y=620;
    }

  cvs->struct_fsm->TargetArray[BASIS]->indexTarget=BASIS;
  cvs->struct_fsm->TargetArray[BASIS]->score=0;

  cvs->struct_fsm->TargetArray[BLUE_MID_FUSEE]->x=-900;
  cvs->struct_fsm->TargetArray[BLUE_MID_FUSEE]->y=-350;
  cvs->struct_fsm->TargetArray[BLUE_MID_FUSEE]->indexTarget=BLUE_MID_FUSEE;
  cvs->struct_fsm->TargetArray[BLUE_MID_FUSEE]->score=0;

  cvs->struct_fsm->TargetArray[YELLOW_MID_FUSEE]->x=-900;
  cvs->struct_fsm->TargetArray[YELLOW_MID_FUSEE]->y=-350;
  cvs->struct_fsm->TargetArray[YELLOW_MID_FUSEE]->score=0;
  cvs->struct_fsm->TargetArray[YELLOW_MID_FUSEE]->indexTarget=YELLOW_MID_FUSEE;

  cvs->struct_fsm->TargetArray[YELLOW_UP_FUSEE]->x=350;
  cvs->struct_fsm->TargetArray[YELLOW_UP_FUSEE]->y=1400;
  cvs->struct_fsm->TargetArray[YELLOW_UP_FUSEE]->indexTarget=YELLOW_UP_FUSEE;
  cvs->struct_fsm->TargetArray[YELLOW_UP_FUSEE]->score=0;

  cvs->struct_fsm->TargetArray[BLUE_DOWN_FUSEE]->x=350;
  cvs->struct_fsm->TargetArray[BLUE_DOWN_FUSEE]->y=-1400;
  cvs->struct_fsm->TargetArray[BLUE_DOWN_FUSEE]->score=0;
  cvs->struct_fsm->TargetArray[BLUE_DOWN_FUSEE]->indexTarget=BLUE_DOWN_FUSEE;


  cvs->struct_fsm->TargetArray[BLUE_BALLS]->x=-350;
  cvs->struct_fsm->TargetArray[BLUE_BALLS]->y=-800;
  cvs->struct_fsm->TargetArray[BLUE_BALLS]->score=0;
  cvs->struct_fsm->TargetArray[BLUE_BALLS]->indexTarget=BLUE_BALLS;

  cvs->struct_fsm->TargetArray[YELLOW_BALLS]->x=-350;
  cvs->struct_fsm->TargetArray[YELLOW_BALLS]->y=800;
  cvs->struct_fsm->TargetArray[YELLOW_BALLS]->score=0;
  cvs->struct_fsm->TargetArray[YELLOW_BALLS]->indexTarget=YELLOW_BALLS;

  cvs->struct_fsm->TargetArray[MIDDLE_RAMP]->x=0;
  cvs->struct_fsm->TargetArray[MIDDLE_RAMP]->y=0;
  cvs->struct_fsm->TargetArray[MIDDLE_RAMP]->score=2;
  cvs->struct_fsm->TargetArray[MIDDLE_RAMP]->indexTarget=MIDDLE_RAMP;


  cvs->struct_fsm->TargetArray[LOWER_RAMP]->x=800;
  cvs->struct_fsm->TargetArray[LOWER_RAMP]->y=-300;
  cvs->struct_fsm->TargetArray[LOWER_RAMP]->score=0;
  cvs->struct_fsm->TargetArray[LOWER_RAMP]->indexTarget=LOWER_RAMP;


  cvs->struct_fsm->TargetArray[UPPER_RAMP]->x=800;
  cvs->struct_fsm->TargetArray[UPPER_RAMP]->y=300;
  cvs->struct_fsm->TargetArray[UPPER_RAMP]->score=0;
  cvs->struct_fsm->TargetArray[UPPER_RAMP]->indexTarget=UPPER_RAMP;

}


void matchFSM(args* atab,CtrlStruct *cvs,MyVannes electrovannes,MyDynamixel dynaElevator,MyDynamixel dynaTurnModules)
{
  // printf(RED "======================== MATCH FSM ======================== \n" RESET);
  // printf(RED "======================== MATCH FSM ======================== \n" RESET);
  // printf(RED "======================== MATCH FSM ======================== \n" RESET);

  CtrlIn *ivs;
  CtrlOut *ovs;
  StructOdometry *odometry;

  ivs = cvs->inputs;
  ovs = cvs->outputs;
  odometry = cvs->struct_odometry;

  double xStart,yStart;
  double xTarget,yTarget;
  int StartNode;
  int TargetNode;
  int i=1;
  double *KpKi;
  double r_wheel_speed;
  double l_wheel_speed;
  double *tempxsirpoint;
  double Kp,Ki;
  bool found;
  int maxScore=0;
  int indexNextTarget;
  double *command=cvs->struct_control->command;
  double error;
  double sumIsTaken;
  //double dist_opp=distance_opponent(cvs);
  //double angle_opp= angle_opponent(cvs);
  int match_target_non_walkable;
  /*if(dist_opp==1000)
  {
    dist_opp=cvs->struct_tower->previousMeanDistance;
  }
  if(angle_opp==1000)
  {
    angle_opp=cvs->struct_tower->previousMeanAngle;
  }
  */
  //opponent_position(cvs,dist_opp,angle_opp);


  switch (cvs->struct_fsm->robot_state)
  {

    ////////////////////////////  NEW STATE   ////////////////////////////
    ////////////////////////////  NEW STATE   ////////////////////////////
    ////////////////////////////  NEW STATE   ////////////////////////////

    case BEGINNING:

    printf(RED "I'm in the BEGINNING state\n" RESET);
    Astar_init(cvs);
    StructControl_init(cvs);
    RefreshPath(cvs->struct_path_planning->astar);
    if(cvs->struct_fsm->isDoneManoeuver == 0)
    {
            cvs->struct_fsm->robot_state =MANOEUVER_START;

    }
    else
    {
        cvs->struct_fsm->robot_state =SET_OBJECTIVE;
    }
    break;

    case MANOEUVER_START:
      if(cvs->struct_fsm->isDoneManoeuver == 0)
      {
         Manoeuver_Start(atab,cvs);

      }
      else
      {
          cvs->struct_fsm->robot_state =PERFORM_ACTION;
          cvs->struct_fsm->robot_next_action=TAKE_MODULES;
      }



    break;
    ////////////////////////////  NEW STATE   ////////////////////////////
    ////////////////////////////  NEW STATE   ////////////////////////////
    ////////////////////////////  NEW STATE   ////////////////////////////
    case SET_OBJECTIVE:
     printf(GRN "I'm in the SET_OBJECTIVE state\n" RESET);
    for (int i = 0; i < NBR_TARGETS; i++)
    {
      if(cvs->struct_fsm->TargetArray[i]->score>maxScore && cvs->struct_fsm->TargetArray[i]->isTaken==0)
      {
        maxScore=cvs->struct_fsm->TargetArray[i]->score;
        indexNextTarget=cvs->struct_fsm->TargetArray[i]->indexTarget;
      }
    }
    if(indexNextTarget!=0) // Just to select the targets
    {
        cvs->struct_fsm->indexNextTarget=indexNextTarget;
        cvs->struct_fsm->robot_state =GENERATE_PATH;

        // CAS OU ON DOIT CHOPPER UN MODULE
        if((indexNextTarget==(BLUE_MID_FUSEE))||(indexNextTarget==(YELLOW_MID_FUSEE))||(indexNextTarget==(BLUE_DOWN_FUSEE))||(indexNextTarget==YELLOW_UP_FUSEE))
        {
        cvs->struct_fsm->robot_next_action=TAKE_MODULES;
        }

        // CAS OU ON DOIT EJECTER UN MODULE
        if((indexNextTarget==(MIDDLE_RAMP))||(indexNextTarget==(UPPER_RAMP))||(indexNextTarget==(LOWER_RAMP)))
        {
        cvs->struct_fsm->robot_next_action=EJECT_MODULES;
        }
        // CAS OU ON DOIT CHOPPER DES BOULES
        if((indexNextTarget==(BLUE_BALLS))||(indexNextTarget==(YELLOW_BALLS)))
        {
        cvs->struct_fsm->robot_next_action=TAKE_BALLS;
        }

    }
    else
    {
        cvs->struct_fsm->robot_state =SET_OBJECTIVE;
    }
    break;

    ////////////////////////////  NEW STATE   ////////////////////////////
    ////////////////////////////  NEW STATE   ////////////////////////////
    ////////////////////////////  NEW STATE   ////////////////////////////


    case GENERATE_PATH:

    printf(GRN "I'm in the GENERATE_PATH state\n" RESET);
    indexNextTarget=cvs->struct_fsm->indexNextTarget;
    printf("Index next target in generate path:%d\n",indexNextTarget);
    xTarget=cvs->struct_fsm->TargetArray[indexNextTarget]->x;
    yTarget=cvs->struct_fsm->TargetArray[indexNextTarget]->y;
    printf("xTarget:%f\n",xTarget);
    printf("yTarget:%f\n",yTarget);

    if(cvs->struct_fsm->isGenerated==0)
    {
        xStart=cvs->struct_odometry->x_t;
        yStart=cvs->struct_odometry->y_t;
        printf("xStart:%f\n",xStart);
        printf("yStart:%f\n",yStart);
      ////// Create the path //////
      if(cvs->struct_fsm->isAvoiding == 0)
      {
        StartNode=FindNearestNode(cvs->struct_path_planning->astar,xStart,yStart);
        printf("StartNode:%d\n",StartNode);
      }
      else
      {
        StartNode = cvs->struct_fsm->nextNodeIndex;
        cvs->struct_fsm->isAvoiding =0;
      }
      TargetNode= FindNearestNode(cvs->struct_path_planning->astar,xTarget,yTarget);
      printf("TargetNode:%d\n",TargetNode);
      Create_path(cvs->struct_path_planning->astar, StartNode,TargetNode);
      cvs->struct_fsm->isGenerated=1;
    }

    cvs->struct_fsm->robot_state =FOLLOW_PATH;

    break;


    ////////////////////////////  NEW STATE   ////////////////////////////
    ////////////////////////////  NEW STATE   ////////////////////////////
    ////////////////////////////  NEW STATE   ////////////////////////////

    case FOLLOW_PATH:
     printf(BLU "I'm in the FOLLOW_PATH state\n" RESET);

    CtrlIn *ivs;
    CtrlOut *ovs;
    StructOdometry *odometry;

    ivs = cvs->inputs;
    ovs = cvs->outputs;
    odometry = cvs->struct_odometry;
    command=cvs->struct_control->command;

    indexNextTarget=cvs->struct_fsm->indexNextTarget;

///////////////// JUST TO PRINT ///////////////////
///////////////// JUST TO PRINT ///////////////////
///////////////// JUST TO PRINT ///////////////////

    switch (indexNextTarget)
    {
    case BASIS:
           printf(RED "indexNextTarget=BASIS\n");
    break;
    case BLUE_MID_FUSEE:
           printf(RED "indexNextTarget=BLUE_MID_FUSEE\n");
    break;

    case YELLOW_MID_FUSEE:
           printf(RED "indexNextTarget=YELLOW_MID_FUSEE\n");
    break;

    case BLUE_DOWN_FUSEE:
           printf(RED "indexNextTarget=BLUE_DOWNP_FUSEE\n");
    break;


    case YELLOW_UP_FUSEE:
           printf(RED "indexNextTarget=YELLOW_UP_FUSEE\n");
    break;

    case BLUE_BALLS:
           printf(RED "indexNextTarget=BLUE_BALLS\n");
    break;

    case YELLOW_BALLS:
           printf(RED "indexNextTarget=YELLOW_BALLS\n");
    break;

    case MIDDLE_RAMP:
           printf(RED "indexNextTarget=MIDDLE_RAMP\n");
    break;

    case UPPER_RAMP:
           printf(RED "indexNextTarget=UPPER_RAMP\n");
    break;

    case LOWER_RAMP:
           printf(RED "indexNextTarget=LOWER_RAMP\n");
    break;
    }
///////////////// FINISH PRINT ///////////////////
///////////////// FINISH PRINT ///////////////////
///////////////// FINISH PRINT ///////////////////
    xTarget=cvs->struct_fsm->TargetArray[indexNextTarget]->x;
    yTarget=cvs->struct_fsm->TargetArray[indexNextTarget]->y;
    printf("xTarget : %f\n",xTarget);
    printf("yTarget : %f\n",yTarget);

  if(cvs->struct_path_planning->astar->length_path ==1)
  {

    cvs->struct_fsm->isGenerated=0;
    cvs->struct_fsm->robot_state =BEGINNING;
  }


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


  if(cvs->struct_control->isBlocked == 1) // We are blocked
  {
    cvs->struct_control->t_blocked = cvs->inputs->t;
    cvs->struct_fsm->robot_state =ISBLOCKED;
  }
  error=EuclidianDistance(xTarget,yTarget,cvs->struct_odometry->x_t,cvs->struct_odometry->y_t);

  if(error<DISTANCE_ERROR && cvs->struct_fsm->target_reached==0) // We have reached the target
  {
     printf(GRN "TARGET REACHED\n" RESET);
     printf(GRN "TARGET REACHED\n" RESET);
     printf(GRN "TARGET REACHED\n" RESET);
     printf(GRN "TARGET REACHED\n" RESET);
     printf(GRN "TARGET REACHED\n" RESET);
     printf(GRN "TARGET REACHED\n" RESET);
    cvs->struct_fsm->time_arrived_target=cvs->inputs->t; // To set the time
    if(indexNextTarget==MIDDLE_RAMP && cvs->struct_fsm->isDoneRotate1==0)
    {
        Rotate_to_angle(atab,cvs,0);
    }
    else
    {
        cvs->struct_fsm->target_reached=1;
        atab->motorR.setSpeed(0);
        atab->motorL.setSpeed(0);
        cvs->struct_fsm->robot_state =PERFORM_ACTION;

    }
  }

  break;


 case PERFORM_ACTION:
    printf("I'm in the PERFORM_ACTION state\n");
    switch (cvs->struct_fsm->robot_next_action)
    {
    case NO_ACTION:
        printf("I'm in the PERFORM_ACTION state and I must do NO_ACTION\n");
    break;

    case TAKE_MODULES:
        printf("I'm in the PERFORM_ACTION state and I must do TAKE_MODULES\n");
        //cvs->struct_fsm->robot_state =BEGINNING;
        if(cvs->struct_actions->ModulesTaken==0 &&  cvs->struct_fsm->time_arrived_target< TIME_OUT_TAKE_MODULES)
        {
            takeModules(atab,4);
        }
        if(cvs->struct_actions->ModulesTaken==1 || cvs->inputs->t - cvs->struct_fsm->time_arrived_target> TIME_OUT_TAKE_MODULES) // Time out
        {
            cvs->struct_actions->ActionIsDone=1;
        }
    break;

    case TAKE_BALLS:
        printf("I'm in the PERFORM_ACTION state and I must do TAKE_BALLS\n");
        if(cvs->struct_actions->BallsTaken==0 &&  cvs->struct_fsm->time_arrived_target< TIME_OUT_TAKE_BALLS)
        {
            //takeBalls(atab,cvs,electrovannes,dynaElevator,dynaTurnModules);
        }
        if(cvs->struct_actions->BallsTaken==1 || cvs->inputs->t - cvs->struct_fsm->time_arrived_target> TIME_OUT_TAKE_BALLS) // Time out
        {
            cvs->struct_actions->ActionIsDone=1;
        }
    break;

    case EJECT_MODULES:
        printf("I'm in the PERFORM_ACTION state and I must do EJECT_MODULES\n");
        if(cvs->struct_actions->ModulesEjected==0 &&  cvs->struct_fsm->time_arrived_target< TIME_OUT_EJECT_MODULES)
        {
            ejectModules(atab,4);
        }
        if(cvs->struct_actions->ModulesEjected==1 || cvs->inputs->t - cvs->struct_fsm->time_arrived_target> TIME_OUT_EJECT_MODULES) // Time out
        {
            cvs->struct_actions->ActionIsDone=1;
        }
    break;

    case EJECT_BALLS:
        printf("I'm in the PERFORM_ACTION state and I must do EJECT_BALLS\n");
        if(cvs->struct_actions->BallsEjected==0 &&  cvs->struct_fsm->time_arrived_target< TIME_OUT_EJECT_MODULES)
        {
            //ejectModules(atab);
        }
        if(cvs->struct_actions->BallsTaken==1 || cvs->inputs->t - cvs->struct_fsm->time_arrived_target> TIME_OUT_EJECT_BALLS) // Time out
        {
            cvs->struct_actions->ActionIsDone=1;
        }
    break;
    }
    if(cvs->struct_actions->ActionIsDone==1)
    {
        cvs->struct_actions->ActionIsDone=0;
        cvs->struct_fsm->robot_state =BEGINNING;
    }
    else
    {
        cvs->struct_fsm->robot_state =PERFORM_ACTION;
    }
  break;

  ////////////////////////////  NEW STATE   ////////////////////////////
  ////////////////////////////  NEW STATE   ////////////////////////////
  ////////////////////////////  NEW STATE   ////////////////////////////
  case ISBLOCKED:


  ivs = cvs->inputs;
  ovs = cvs->outputs;
  odometry = cvs->struct_odometry;
  command=cvs->struct_control->command;
   printf(BLU "I'm in the ISBLOCKED state\n" RESET);
  if(cvs->inputs->t-cvs->struct_control->t_blocked<2)
  {
    double *command=cvs->struct_control->command;
    MiddleLevelController(cvs,-0.1,0,command);
    LowLevelController(cvs,command, 15, 50,command);
    ovs->wheel_commands[R_ID] = command[0];
    ovs->wheel_commands[L_ID] = command[1];
    cvs->struct_fsm->robot_state=ISBLOCKED;
  }
  else
  {
    cvs->struct_fsm->robot_state=FOLLOW_PATH;
    cvs->struct_control->isBlocked=0;
    cvs->struct_control->isRotating =0;
  }

  break;


  default:
  // printf("Error: unknown state : %d !\n", cvs->struct_fsm->robot_state);
  exit(EXIT_FAILURE);
}

}

int updateObjective(CtrlStruct *cvs)
{
  printf("updateObjective to implement! \n");
}




void StructFSM_free(CtrlStruct *cvs)
{
  for (int i = 0; i < NBR_TARGETS; i++)
  {
    free(cvs->struct_fsm->TargetArray[i]);
  }
  free(cvs->struct_fsm->TargetArray);
}
//NAMESPACE_CLOSE();
