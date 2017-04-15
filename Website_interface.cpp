

#include "MyIncludes_gr1.h"
#include "MyIncludes_Moonwalker.h"




/* This function initializes the StructWheels structure
 *
 * param[in] : cvs controller main structure
 */

void StructWebsite_init(CtrlStruct *cvs)
{
    cvs->struct_website->TakeModule=0;
    cvs->struct_website->TakeBalls=1;
    cvs->struct_website->PushModule=1;
}

void StructWebsite_getData(CtrlStruct *cvs)
{
    cvs->struct_website->TakeModule=0; // A remplacer par le data site web
    cvs->struct_website->TakeBalls=1; // A remplacer par le data site web
    cvs->struct_website->PushModule=1;
}

void StructWebsite_SendData(CtrlStruct *cvs,int sockfd)
{
    printf(GRN "SEND DATA.......\n"RESET);
    printf(CYN "SEND DATA.......\n"RESET);
    printf(RED "SEND DATA.......\n"RESET);
    int actionState;
    int robotState;
    int nextTarget;
    ///////////////////// ROBOT STATE /////////////////////
    ///////////////////// ROBOT STATE /////////////////////
    ///////////////////// ROBOT STATE /////////////////////

    switch (cvs->struct_fsm->robot_state)
    {
    ///////////////////// ROBOT STATE /////////////////////
    ///////////////////// ROBOT STATE /////////////////////
    ///////////////////// ROBOT STATE /////////////////////
    case BEGINNING:
        robotState=0;
        printf("I'm in the BEGINNING state\n");
    break;

    case SET_OBJECTIVE:
        robotState=1;
        printf("I'm in the SET_OBJECTIVE state\n");
    break;

    case GENERATE_PATH:
        robotState=2;
        printf("I'm in the GENERATE_PATH state\n");
    break;

    case FOLLOW_PATH:
        robotState=3;
        printf("I'm in the FOLLOW_PATH state\n");
    break;

    case PERFORM_ACTION:
        robotState=4;
        printf("I'm in the PERFORM_ACTION state\n");
    break;

    case ISBLOCKED:
        robotState=5;
        printf("I'm in the ISBLOCKED state\n");
    break;
    }

    ///////////////////// ROBOT NEXT ACTION /////////////////////
    ///////////////////// ROBOT NEXT ACTION /////////////////////
    ///////////////////// ROBOT NEXT ACTION /////////////////////

    char toSend[10];

//sprintf(toSend, "id=1:  %d %d %d", 42,29,18);

//sprintf(toSend, "id=1:  %f %f %f ", cvs->struct_odometry->x_t,cvs->struct_odometry->y_t,cvs->struct_odometry->theta_t);

//sendString(sockfd,toSend);

    printf(GRN " DATA SENT......\n"RESET);
    printf(CYN "DATA SENT.......\n"RESET);
    printf(RED "DATA SENT.......\n"RESET);
    printf(RED "ALMOST BACK IN THE MAIN\n"RESET);


/*
What data to send?

Box Astar: nextNodeIndex = cvs->struct_fsm->nextNodeIndex;
Box Astar: path??

Box FSM:
cvs->struct_fsm->robot_next_action;
cvs->struct_fsm->indexNextTarget;
cvs->struct_fsm->robot_state;
cvs->struct_fsm->target_reached;

Box Controllers
:
  cvs->struct_control->Kp; // Coefficient for the P controller
  cvs->struct_control->Ki; // Coefficient for I controller
  cvs->struct_control->errDist;
  cvs->struct_control->errAngle

  cvs->struct_control->lowlevelcommand0;
  cvs->struct_control->lowlevelcommand1;

  cvs->struct_control->middlelevelcommand0;
  cvs->struct_control->middlelevelcommand1;
    double lowlevelcommand0;
  double lowlevelcommand1;
  double lowlevelKp;
  double lowlevelKi;

  double middlelevelcommand0;
  double middlelevelcommand1;
  double middlelevelKpforward;
  double middlelevelKprotate;

*/
}

void MyDemo(CtrlStruct *cvs)
{
    if(cvs->struct_website->TakeModule==1)
    {
        printf("About to take a module!!!\n");
    }
}
