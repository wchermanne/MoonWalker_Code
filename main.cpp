

#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"


int main(int argc, char** argv) {

    /////////// Initializations from MoonWalker     ///////////
    /////////// Initializations from MoonWalker     ///////////
    /////////// Initializations from MoonWalker     ///////////

    gpioInitialise();
    MyMCP2515 *MyCAN = new MyMCP2515();
    MyCAN->doInit();
    MyDE0Nano *nano = new MyDE0Nano();
    args *atab = (args*) malloc(sizeof(args));
    MyTourelle tourelle(MyCAN, nano, 0x508);
    MyMotors motorsright(MyCAN, nano, 0x708,1);
    MyMotors motorsleft(MyCAN, nano, 0x708,2);
    MyOdometers odoR(nano,1);
    MyOdometers odoL(nano,2);
    MyVannes electrovannes(MyCAN,0x408);
    MyDynamixel dynaElevator(0x04,nano);
    MyDynamixel dynaTurnModules(0x06,nano);
    MyDynamixel dynaFrameBalls(0x08,nano);
    MySwitch uswitches(MyCAN,0x608);
    nano->reset();
    /////////// Initializations from Robotics     ///////////
    /////////// Initializations from Robotics     ///////////
    /////////// Initializations from Robotics     ///////////

    CtrlIn *In;
    CtrlOut *Out;

    In = (CtrlIn*) malloc(sizeof(CtrlIn));
    Out = (CtrlOut*) malloc(sizeof(CtrlOut));

    CtrlStruct *MyStruct;


    ///////////////// TO MODIFY WITH THE SITE? ///////////////

    double xsiR[2];

    MyStruct = init_CtrlStruct(In,Out);
    time_sleep(0.2);
    printf("MyStruct initialized\n");
    atab->motorR = motorsright;
    atab->motorL = motorsleft;
    atab->MyStruct = MyStruct;
    atab->odoL = odoL;
    atab->odoR = odoR;
    atab->dynaFrameBalls= dynaFrameBalls;
    atab->dynaElevator= dynaElevator;
    atab->dynaTurnModules= dynaTurnModules;
    atab->electrovannes= electrovannes;
    atab->uswitches= uswitches;
    atab->ts_can = 0;
    /////////////////// SOME INITIALIZATIONS OF SOME INPUTS ///////////////////
    /////////////////// SOME INITIALIZATIONS OF SOME INPUTS ///////////////////
    /////////////////// SOME INITIALIZATIONS OF SOME INPUTS ///////////////////

    MyStruct->inputs->r_wheel_speed = 0.0;
    MyStruct->inputs->l_wheel_speed = 0.0;
    MyStruct->inputs->color=YELLOW;
    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////

    /////////// Threads     ///////////
    /////////// Threads     ///////////
    /////////// Threads     ///////////
    pthread_t threadMotorRight,threadMotorLeft, threadOdoRight, threadOdoLeft, threadPosCompute, threadForSensor;
    int retRight = pthread_create(&threadMotorRight, NULL, ThreadMotorR, (void*)atab);
    int retLeft = pthread_create(&threadMotorLeft, NULL, ThreadMotorL, (void*)atab);
    int retOdoRight = pthread_create(&threadOdoRight, NULL, ThreadOdoR, (void*)atab);
    int retOdoLeft = pthread_create(&threadOdoLeft, NULL, ThreadOdoL, (void*)atab);
    int retPosCompute = pthread_create(&threadPosCompute, NULL, ThreadComputePosition, (void*)atab);
   // int retSensor = pthread_create(&threadForSensor, NULL, ThreadSensor, (void*)atab);
    if(retRight || retLeft || retOdoRight || retOdoLeft || retPosCompute )
    {
        printf("failed in one/several thread creation\n");
    }

    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
    double duration;
    double sec=0;
    clock_t start,now,diff;
    start = clock();
    Astar_init(MyStruct);

    //Create_path(MyStruct->struct_path_planning->astar,17,14);




/*
/////////////////////////////// Site web init ///////////////////////////////
/////////////////////////////// Site web init ///////////////////////////////
/////////////////////////////// Site web init ///////////////////////////////

    char toSend[512];
    printf("ASTAR OK2\n");

    char recu[256];
    int sockfd, portno = 8124, n;//51717, n;
    char serverIp[] = "192.168.43.88";//"130.104.174.73";
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    //char recu[256];
    int data;
    printf("ASTAR OK3\n");

    if (argc < 3) {
        // error( const_cast<char *>( "usage myClient2 hostname port\n" ) );
        printf( "contacting %s on port %d\n", serverIp, portno );
            printf("Test1\n");

        // exit(0);
    }
        printf("Test2\n");

    if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
    {
                //error( const_cast<char *>( "ERROR opening socket") );
                printf("Blablabla\n");
    }
        printf("Test3\n");

    if ( ( server = gethostbyname( serverIp ) ) == NULL )
    {
                        printf("Blaiblibli\n");

    }
        //error( const_cast<char *>("ERROR, no such host\n") );
    printf("Test4\n");

    bzero( (char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
                error( const_cast<char *>( "ERROR connecting") );
    }
    printf("
           ASTAR OK4\n");
*/
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////

        //lowerElevator(dynaElevator);
        //dynaElevator.PrintRegisters();
        //upperElevator(dynaElevator);
        dynaElevator.LedOn();
       /* motorsright.initMotor();
        motorsleft.initMotor();*/
    //uswitches.blink();
    //electrovannes.blink();
   while(1)
     {
         /////////////// Just to assign the time ////////////////
        now=clock();
        diff=now-start;
        sec=(double) (diff)/(CLOCKS_PER_SEC);
        MyStruct->inputs->t=sec;
       // printf(GRN "t= %f\n" RESET,MyStruct->inputs->t);
         ////////////////////////////////////////////////////
/*

     FromHighToMiddleLevel(MyStruct,MyStruct->struct_control->command);
      if(MyStruct->struct_control->command[1] > 3*PI/4 )
    {
        MyStruct->struct_control->command[1] = 3*PI/4;
        MyStruct->struct_control->command[0] = 0;
    }
    else if (MyStruct->struct_control->command[1] < -3*PI/4 )
    {
        MyStruct->struct_control->command[1] = -3*PI/4;
        MyStruct->struct_control->command[0] = 0;
    }
    MiddleLevelController(MyStruct,MyStruct->struct_control->command[0]/1000, MyStruct->struct_control->command[1], MyStruct->struct_control->Speed_ref);


     printf("command = %f",MyStruct->struct_control->command[0]);
     printf("\t %f \n",MyStruct->struct_control->command[1]);

     // TO VERIFY
      /*if(MyStruct->struct_control->Speed_ref[1] > 2)
  {
    MyStruct->struct_control->Speed_ref[1] = 2;
    MyStruct->struct_control->Speed_ref[0] = 0;
  }
  else if (MyStruct->struct_control->Speed_ref[1] < -2)
  {
    MyStruct->struct_control->Speed_ref[1]= -2;
    MyStruct->struct_control->Speed_ref[0] = 0;
  }
*/

/*


     LowLevelController(MyStruct,MyStruct->struct_control->Speed_ref,KpKi[0]/6,KpKi[1]/6,MyStruct->struct_control->command);
     motorsright.setSpeed(MyStruct->struct_control->command[0]);
     motorsleft.setSpeed(MyStruct->struct_control->command[1]);
     printf("command[0] = %f",MyStruct->struct_control->command[0]);
     printf("command[1]\t %f \n",MyStruct->struct_control->command[1]);

    printf(BLU "Position x = %f \n",MyStruct->struct_odometry->x_t);
    printf("Position y = %f \n",MyStruct->struct_odometry->y_t);
    printf("Position theta = %f\n" RESET,MyStruct->struct_odometry->theta_t);
*/




    if(MyStruct->inputs->t>90.0)
    {
        funnyAction(electrovannes);
        Stop_Everything(atab); // This function is in the FSM_gr1 file
    }
    else
    {
        //motorsleft.setBrake(false);
        //motorsright.setBrake(false);
        /*if(atab->ts_can==1)
        {
            motorsleft.setSpeed(22);
            motorsright.setSpeed(22);
            atab->ts_can=0;
        }*/
        uswitches.readSwitch(MyStruct);

     //matchFSM(atab,MyStruct,electrovannes,dynaElevator,dynaTurnModules);
/*unsigned char buf[4] = {0x00, 0x00, 0x00, 0x00};
int photo_chips_previous;
while(1)
{
        nano->readWriteReg(READ, 12, buf, 4);
        atab->MyStruct->struct_sensors->photo_chips = (int) buf[3];
        //printf("photo_chips in the while = %d \n", atab->MyStruct->struct_sensors->photo_chips);
        photo_chips_previous = atab->MyStruct->struct_sensors->photo_chips;
    upperElevator(dynaElevator);
    while(atab->MyStruct->struct_sensors->photo_chips==photo_chips_previous)
    {
        nano->readWriteReg(READ, 12, buf, 4);
        atab->MyStruct->struct_sensors->photo_chips = (int) buf[3];
        //printf("photo_chips in the while = %d \n", atab->MyStruct->struct_sensors->photo_chips);

    }
    stopElevator(dynaElevator);
    printf("------------------------------end of the while-----------------------------------------");
    time_sleep(3);
    //atab->MyStruct->struct_sensors->photo_chips=0;

}*/

/*
        while(1)
        {
              gripperIn(atab->electrovannes);
            time_sleep(3);
            closeGripper(atab->electrovannes);
            time_sleep(2);
            printf("pince closed = %d \n",atab->MyStruct->struct_sensors->gripper_uswitch);

            openGripper(atab->electrovannes);
            time_sleep(2);
            printf("pince open = %d \n",atab->MyStruct->struct_sensors->gripper_uswitch);
        }

*/



        //lowerElevator(dynaElevator);
        //catchModules(atab,4);
        //displayOdometry(MyStruct);

    }

    //uswitches.blink();
    //electrovannes.blink();
    //uswitches.readSwitch(1,1);
    //time_sleep(0.5);
    //uswitches.readSwitch(MyStruct);

    /*uswitches.readSwitch(3,1);
    time_sleep(0.5);
    uswitches.readSwitch(4,1);
    time_sleep(0.5);
    uswitches.readSwitch(5,1);
    time_sleep(0.5);
    uswitches.readSwitch(6,1);
    time_sleep(0.5);*/
    /*uswitches2.readSwitch(1,1);
    time_sleep(0.5);
    uswitches2.readSwitch(2,1);
    time_sleep(0.5);
    uswitches2.readSwitch(3,1);
    time_sleep(0.5);
    uswitches2.readSwitch(4,1);
    time_sleep(0.5);
    uswitches2.readSwitch(5,1);
    time_sleep(0.5);
    uswitches2.readSwitch(6,1);*/
/*
if(MyStruct->struct_calibration->isDoneRotate1==0)
    {
        //printf("I should rotate\n");
        //Rotate_to_angle(atab,MyStruct,2*PI/3);
    }
if(MyStruct->struct_calibration->isDoneForward1==0)
    {
        printf("I should go forward \n");
       Forward_distance(atab,MyStruct,0,nextX,nextY);
    }
else{
    motorsleft.setSpeed(0.0);
    motorsright.setSpeed(0.0);
}
*/



/************************************************************************************
*************************************************************************************
*************************************************************************************
************************************* WEBSITE START *********************************
*************************************************************************************
*************************************************************************************
*************************************************************************************/
  /*
    ///////////////////// ROBOT STATE /////////////////////
    ///////////////////// ROBOT STATE /////////////////////
    ///////////////////// ROBOT STATE /////////////////////

switch (MyStruct->struct_fsm->robot_state)
    {

    case BEGINNING:
        MyStruct->struct_fsm->robotState=0;
    break;

    case SET_OBJECTIVE:
        MyStruct->struct_fsm->robotState=1;
    break;

    case GENERATE_PATH:
        MyStruct->struct_fsm->robotState=2;
    break;

    case FOLLOW_PATH:
        MyStruct->struct_fsm->robotState=3;
    break;

    case PERFORM_ACTION:
        MyStruct->struct_fsm->robotState=4;
    break;

    case ISBLOCKED:
        MyStruct->struct_fsm->robotState=5;
    break;
    }

        ///////////////////// ROBOT NextAction /////////////////////
        ///////////////////// ROBOT NextAction /////////////////////
        ///////////////////// ROBOT NextAction /////////////////////

    switch (MyStruct->struct_fsm->robot_next_action)
    {
    case NO_ACTION:
        MyStruct->struct_fsm->indexNextAction=0;
        printf("I'm in the NO_ACTION state\n");
    break;

    case TAKE_MODULES:
        MyStruct->struct_fsm->indexNextAction=1;
        printf("I'm in the TAKE_MODULES state\n");
    break;

    case TAKE_BALLS:
        MyStruct->struct_fsm->indexNextAction=2;
        printf("I'm in the TAKE_BALLS state\n");
    break;

    case EJECT_MODULES:
        MyStruct->struct_fsm->indexNextAction=3;
        printf("I'm in the EJECT_MODULES state\n");
    break;

    case EJECT_BALLS:
        MyStruct->struct_fsm->indexNextAction=4;
        printf("I'm in the EJECT_BALLS state\n");
    break;
    }
/*
/////////////////// BOX FSM ////////////////////
int robotstate=MyStruct->struct_fsm->robotState;
int indexNextTarget=MyStruct->struct_fsm->indexNextTarget;
int indexNextAction=MyStruct->struct_fsm->indexNextAction;
int ActionIsDone=MyStruct->struct_fsm->ActionIsDone;
int target_reached=MyStruct->struct_fsm->target_reached;
///////////////////////////////////////////////

/////////////////// BOX Odometry ////////////////////

double xodo=MyStruct->struct_odometry->x_t;
double yodo=MyStruct->struct_odometry->y_t;
double thetaodo=MyStruct->struct_odometry->theta_t;
////////////////////////////////////////////////////////

/////////////////// BOX PathPlanning ////////////////////
int nextNodeIndex=MyStruct->struct_fsm->nextNodeIndex;
int path=0;
double diffx= MyStruct->struct_control->Diffx;
double diffy= MyStruct->struct_control->Diffy;


////////////////////////////////////////////////////////

/////////////////// BOX High Level  ////////////////////

double errDist=MyStruct->struct_control->errDist;
double errAngle=MyStruct->struct_control->errAngle;
double KpDist=MyStruct->struct_control->KpDist;
double KpAngle=MyStruct->struct_control->KpAngle;
////////////////////////////////////////////////////////

/////////////////// BOX Middle Level  ////////////////////
double ForwardSpeed=MyStruct->struct_control->middlelevelforwardspeed;
double AngularSpeed=MyStruct->struct_control->middlelevelrotatespeed;
double middlelevelcommand0=MyStruct->struct_control->middlelevelcommand0;
double middlelevelcommand1=MyStruct->struct_control->middlelevelcommand1;

  double truerightspeed=MyStruct->inputs->r_odo_speed;

  double trueleftspeed=MyStruct->inputs->l_odo_speed;

////////////////////////////////////////////////////////


/////////////////// BOX Low Level  ////////////////////
  double lowlevelKp=MyStruct->struct_control->lowlevelKp;
  double lowlevelKi=MyStruct->struct_control->lowlevelKi;
  double lowlevelcommand0=MyStruct->struct_control->lowlevelcommand0;
  double lowlevelcommand1=MyStruct->struct_control->lowlevelcommand1;

  int emergencyStop=MyStruct->struct_control->emergencyStop;
////////////////////////////////////////////////////////



//sprintf(toSend, "id=2:  %f ", MyStruct->struct_odometry->x_t);
sprintf(toSend, "id=1: %d %d %d %d %d %f %f %f %d %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d",robotstate,indexNextTarget,indexNextAction,ActionIsDone,target_reached, xodo,yodo,(thetaodo)*180/PI,nextNodeIndex,path,diffx,diffy,errDist,(errAngle)*180/PI,KpDist,KpAngle,ForwardSpeed,AngularSpeed,middlelevelcommand0,middlelevelcommand1,truerightspeed,trueleftspeed,lowlevelKp,lowlevelKi,lowlevelcommand0,lowlevelcommand1,emergencyStop);
//sprintf(toSend, "id=1:  %f %f %f ", (MyStruct->inputs->t)*70,(MyStruct->inputs->t)*70,MyStruct->struct_odometry->theta_t);
//sprintf(toSend2, "id=2:  %f %f %f ", (MyStruct->inputs->t)*70,(MyStruct->inputs->t)*70,MyStruct->struct_odometry->theta_t);

sendString(sockfd,toSend);
//sendString(sockfd,toSend);

time_sleep(0.1);
printf("Pos x:%f\n",MyStruct->struct_odometry->x_t);
printf("Pos y:%f\n",MyStruct->struct_odometry->y_t);
printf("Pos Theta:%f\n",MyStruct->struct_odometry->theta_t);

/************************************************************************************
*************************************************************************************
*************************************************************************************
************************************* WEBSITE FINISH *********************************
*************************************************************************************
*************************************************************************************
*************************************************************************************/

    }
}
