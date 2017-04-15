#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"

void StructActions_init(CtrlStruct *cvs)
{
    cvs->struct_actions->nb_modules=0;
    cvs->struct_actions->ActionIsDone=0;
    cvs->struct_actions->ModulesTaken=0;
    cvs->struct_actions->ModulesEjected=0;
    cvs->struct_actions->BallsTaken=0;
    cvs->struct_actions->BallsEjected=0;
    cvs->struct_actions->time_action_started=0;
    cvs->struct_actions->take_modules_state=WAITMODULE;
    cvs->struct_actions->time_gripper_open=0;
    cvs->struct_actions->time_dyna_elevator=0;
    cvs->struct_actions->time_dyna_elevator=0;
    cvs->struct_actions->HomologationDone=0;
    cvs->struct_actions->eject_modules_state=WAITMODULE;
    cvs->struct_actions->nb_modules_ejected=0;


}
void funnyAction(MyVannes electrovannes)
{
    electrovannes.setVanne(2, true);
    time_sleep(0.1);
    electrovannes.setVanne(2, false);
}



void closeGripper(MyVannes electrovannes)
{
    electrovannes.setVanne(VALVE_GRIPPER, true); // fermer la pince
}

void openGripper(MyVannes electrovannes)
{
    electrovannes.setVanne(VALVE_GRIPPER, false); // ouvrir la pince
}

void gripperOut(MyVannes electrovannes)
{
    electrovannes.setVanne(VALVE_MODULES, true); // sortir la pince
}
void gripperIn(MyVannes electrovannes)
{
    electrovannes.setVanne(VALVE_MODULES, false); // rentrer la pince
}

void catchModules(args* atab,int modulesToCatch)
{
    /*int photoswitch;
    int switch_gripper;
    int gripperIsIn;
    int gripperIsOut;*/
    double xnextpos1,ynextpos1;
    switch (atab->MyStruct->struct_actions->take_modules_state)
    {
    case WAITMODULE:
        printf("I'm in the WAITMODULE STATE state\n");
        if(atab->MyStruct->struct_actions->nb_modules==modulesToCatch)
        {
            printf("%f modules are taken",modulesToCatch);
            atab->MyStruct->struct_actions->take_modules_state=WAITMODULE;
            atab->MyStruct->struct_actions->ActionIsDone=1;
            atab->MyStruct->struct_actions->ModulesTaken=1;
        }
        else
        {
            atab->MyStruct->struct_actions->take_modules_state=OPEN_GRIPPER;
        }
        break;

    case OPEN_GRIPPER:
        printf("I'm in the OPEN_GRIPPER STATE\n");
        openGripper(atab->electrovannes);
        atab->MyStruct->struct_actions->take_modules_state=GRIPPER_OUT;
        atab->MyStruct->struct_actions->time_enter_state=atab->MyStruct->inputs->t;
        // Time wait
        break;



    case GRIPPER_OUT:
        printf("I'm in the GRIPPER_OUT state\n");

        gripperOut(atab->electrovannes);
        printf("%d\n", (atab->MyStruct->struct_sensors->front_endway));
        if((atab->MyStruct->struct_sensors->front_endway)==1)
        {
            printf("HELLLOOO\n");
            atab->MyStruct->struct_actions->take_modules_state=CLOSE_GRIPPER;
            atab->MyStruct->struct_actions->time_enter_state=atab->MyStruct->inputs->t; // le temps auquel on rentre dans l'état suivant: CLOSE_GRIPPER
        }
        else
        {
            if((atab->MyStruct->inputs->t-atab->MyStruct->struct_actions->time_enter_state>TIME_GRIPPER_OUT))
            {
                printf("Time Out reached in the GRIPPER OUT state");
                // Ca a foiré-> La glissère n'est pas bien sortie --> abort
                // Reculer un peu? Tant qu'on a pas le fin de course?
                atab->MyStruct->struct_actions->take_modules_state=ABORT_MODULE; // quitte la fonction catchmodules et rentre tous les mécanismes
            }
            else
            {
                printf("Just a test");
                atab->MyStruct->struct_actions->take_modules_state=GRIPPER_OUT;
                // On doit attendre que la pince sorte complètement :-)
            }
        }

      break;

    case CLOSE_GRIPPER:
        printf("I'm in the CLOSE_GRIPPER state\n");
        if(atab->MyStruct->inputs->t-atab->MyStruct->struct_actions->time_enter_state<TIME_GRIPPER_CLOSED) //Je ferme la pince tant que j'ai pas dépassé le timeque j'estimeetre necassaire pourfermer la pince
        {
            closeGripper(atab->electrovannes); // Fermer la pince
        }
        else // j'ai atteint le temps quej'estime être nécessaire pour fermer la pince
        {
            if((atab->MyStruct->struct_sensors->gripper_uswitch)==0) // J'ai le module :-)
            {
                atab->MyStruct->struct_actions->take_modules_state=BACKWARD_MODULE;
                atab->MyStruct->struct_actions->time_enter_state=atab->MyStruct->inputs->t;
                printf("I got the module! \n");
            }
            else // J'ai pas le module
            {
                atab->MyStruct->struct_actions->take_modules_state=ABORT_MODULE; // quitte la fonction catchmodules et rentre tous les mécanismes
                printf("I missed the module! \n");
            }
        }

        break;

    case BACKWARD_MODULE:
        printf("I'm in the BACKWARD_MODULE state\n");

    // Checker si en reculant le switch de la gripper se met pas à 1
    xnextpos1=atab->MyStruct->struct_odometry->x_t+100; // X_FRONT+100
    ynextpos1=atab->MyStruct->struct_odometry->y_t;     // Y_FRONT

    //xnextpos1=X_FRONT_MODULES+100; --> ne fonctionne pas :/
    //ynextpos1=Y_FRONT_MODULES; --> ne fonctionne pas :/
    printf("IsDoneForward1 %d\n",atab->MyStruct->struct_fsm->isDoneForward1);
    while(atab->MyStruct->struct_fsm->isDoneForward1==0)
    {
        printf("WHILE IsDoneForward1 %d\n",atab->MyStruct->struct_fsm->isDoneForward1);

        Forward_distance(atab,atab->MyStruct,atab->MyStruct->struct_fsm->isDoneForward1,xnextpos1,ynextpos1);
        if((atab->MyStruct->struct_sensors->gripper_uswitch)==1)
        {
        printf("IIIIFFFF %d\n");

            // En reculant, j'ai laché le module.
            //atab->MyStruct->struct_actions->take_modules_state=FORWARD_MODULE; // Je réavance et je relance la séquence
            atab->MyStruct->struct_actions->take_modules_state=ABORT_MODULE; // quitte la fonction catchmodules et rentre tous les mécanismes
        }
    }

    if((atab->MyStruct->struct_sensors->gripper_uswitch)==0 &&  atab->MyStruct->struct_fsm->isDoneForward1==1) // Si le switch de la pince est toujours à 0 j'ai pas pomé le module
    {
        printf("FinalIsDoneForward1 %d\n",atab->MyStruct->struct_fsm->isDoneForward1);
        printf("microswitch button %d\n",(atab->MyStruct->struct_sensors->gripper_uswitch));

        atab->MyStruct->struct_actions->take_modules_state=GRIPPER_IN; // Je passe à l'état suivant
        atab->MyStruct->struct_fsm->isDoneForward1=0;
    }
    break;


    case GRIPPER_IN:
    printf("I'm in the GRIPPER_IN state\n");
    time_sleep(0.5);
    break;

    if(switch_gripper==1)
    {
        // En rentrant ma pince, j'ai laché le module.
        atab->MyStruct->struct_actions->take_modules_state=FORWARD_MODULE; // Je réavance et je relance la séquence
        openGripper(atab->electrovannes); // J'ouvre la pince pour pas niquer les chips
        atab->MyStruct->struct_actions->nb_modules++; // Sinon il croit qu'il en a pas pris vu que je fais nb_modules ++ quand il l'a totalement rentrée
        // Je veux qu'il croit qu'il en a pris un vu que celui qu'on a pomé est perdu à tout jamais

    }
    else
    {
        gripperIn(atab->electrovannes);
        if(gripperIsIn==1)
        {
            atab->MyStruct->struct_actions->take_modules_state=OPEN_GRIPPER_2;
            atab->MyStruct->struct_actions->time_enter_state=atab->MyStruct->inputs->t;
            atab->MyStruct->struct_actions->nb_modules++;
        }
        else
        {
            if((atab->MyStruct->inputs->t-atab->MyStruct->struct_actions->time_enter_state>TIME_GRIPPER_IN))
            {
                // Ca a foiré, bloqué, quand j'ai rentré la pince :-(
                // What to do?
                // Ouvrir la pince,sortir la pince,monter modules
                // Rentrer la pince
            }
            else
            {
                atab->MyStruct->struct_actions->take_modules_state=GRIPPER_IN;
                // On doit attendre
            }
        }
    }
    break;

    case OPEN_GRIPPER_2: // Je suis jamais sensé arriver ici si j'ai pas le module
    printf("I'm in the OPEN_GRIPPER_2 state\n");
    openGripper(atab->electrovannes);
    atab->MyStruct->struct_actions->take_modules_state=DYNA_ELEVATOR;
    atab->MyStruct->struct_actions->time_enter_state=atab->MyStruct->inputs->t;
    break;

    case DYNA_ELEVATOR:
    //if((atab->MyStruct->inputs->t-atab->MyStruct->struct_actions->time_enter_state>TIME_DYNA_ELEVATOR))
    atab->dynaElevator.EndlessTurn(0x150,"CW"); // monter l'ascenseur
    atab->MyStruct->struct_actions->take_modules_state=FORWARD_MODULE;
    atab->MyStruct->struct_actions->time_enter_state=atab->MyStruct->inputs->t;
    break;

    case FORWARD_MODULE:
    xnextpos1=X_FRONT_MODULES;
    ynextpos1=Y_FRONT_MODULES;

    while(atab->MyStruct->struct_fsm->isDoneForward1==0)
    {
        if(photoswitch==1)
        {
            atab->dynaElevator.EndlessTurn(0x00,"CW"); // arreter l'ascenseur
        }
        Forward_distance(atab,atab->MyStruct,atab->MyStruct->struct_fsm->isDoneForward1,xnextpos1,ynextpos1);
    }
    if(photoswitch==1) // Over protection
    {
        atab->dynaElevator.EndlessTurn(0x00,"CW"); // arreter l'ascenseur
        if(atab->MyStruct->struct_fsm->isDoneForward1==0)
        {
            atab->MyStruct->struct_actions->take_modules_state=DONE_MODULE;
        }
    }
    else
    {
        atab->MyStruct->struct_actions->take_modules_state=FORWARD_MODULE;
    }


    break;

    case DONE_MODULE:
    if(atab->MyStruct->struct_actions->nb_modules==modulesToCatch)
    {
        atab->MyStruct->struct_actions->take_modules_state=WAITMODULE;
    }
    else
    {
        atab->MyStruct->struct_actions->take_modules_state=OPEN_GRIPPER;
    }
    break;


    case ABORT_MODULE:
        // on annule tout
        printf("I'm in the ABORT_MODULE state\n");
        openGripper(atab->electrovannes);
        gripperIn(atab->electrovannes);
        atab->MyStruct->struct_actions->ModulesTaken=1; // permet de quitter la fonction catchmodules
    break;
    }

}



void takeModules(args *atab,int ModulesToTake)
/*
    Prend les 4 modules dans le robot lorque celui ci est devant le fusee
    Normalement, on utilisera catchModules mais
*/
{
    atab->MyStruct->struct_actions->time_action_started=atab->MyStruct->inputs->t;
    printf("Time action started: %f\n",atab->MyStruct->struct_actions->time_action_started);
    int timeSleeps[3];
    timeSleeps[0]=4.1;
    timeSleeps[1]=4.4;
    timeSleeps[2]=4.3;
    double xnextpos1,ynextpos1,xnextpos2,ynextpos2,xnextpos3,ynextpos3;
    int SwitchGripper;
    atab->dynaElevator.absoluteRotate(0x50,0);
    time_sleep(2); // attendre que la pince soit sortie
    for(int i = 0; i<ModulesToTake-1; i++)
    {
        printf("In the for : %d \n",i);
        openGripper(atab->electrovannes); // ouvrir la pince
        time_sleep(2); // attendre que la pince soit sortie
        gripperOut(atab->electrovannes);
        time_sleep(2); // attendre que la pince soit sortie
        closeGripper(atab->electrovannes); // fermer pince
        time_sleep(1); // attendre que le module soit pris


// A partir de maintenant, je checke si le uSwitch de la pince est à 1. Si oui, je recule, je réavance, et je recommence la séquence.
// Sinon, je recommence comme c'était.
        //SwitchGripper=atab->uswitches.readSwitch()
        if(SwitchGripper==1)
        {
            //////////////////* Realign */////////////////////
            xnextpos1= -800;
            ynextpos1=-350;


            while(atab->MyStruct->struct_fsm->isDoneForward1==0)
            {
                Forward_distance(atab,atab->MyStruct,atab->MyStruct->struct_fsm->isDoneForward1,xnextpos1,ynextpos1);
            }
            atab->MyStruct->struct_fsm->isDoneForward1=0;

            xnextpos2=atab->MyStruct->struct_odometry->x_t-100;
            ynextpos2=atab->MyStruct->struct_odometry->y_t;

            while(atab->MyStruct->struct_fsm->isDoneForward1==0)
            {
                Forward_distance(atab,atab->MyStruct,atab->MyStruct->struct_fsm->isDoneForward1,xnextpos2,ynextpos2);
            }
            atab->MyStruct->struct_fsm->isDoneForward1=0;
            ModulesToTake++;
        }
        else
        {
            xnextpos1=atab->MyStruct->struct_odometry->x_t+100;
            ynextpos1=atab->MyStruct->struct_odometry->y_t;

            while(atab->MyStruct->struct_fsm->isDoneForward1==0)
            {
                Forward_distance(atab,atab->MyStruct,atab->MyStruct->struct_fsm->isDoneForward1,xnextpos1,ynextpos1);
            }
            atab->MyStruct->struct_fsm->isDoneForward1=0;

            gripperIn(atab->electrovannes); // rentrer la pince
            time_sleep(2); // attendre que la pince soit rentree
            openGripper(atab->electrovannes); // Ouvrir la pince
            time_sleep(0.5);

            atab->dynaElevator.EndlessTurn(0x150,"CW"); // monter l'ascenseur
            time_sleep(timeSleeps[i]);
            atab->dynaElevator.EndlessTurn(0x00,"CW"); // arreter l'ascenseur


            xnextpos2=atab->MyStruct->struct_odometry->x_t-100;
            ynextpos2=atab->MyStruct->struct_odometry->y_t;

            while(atab->MyStruct->struct_fsm->isDoneForward1==0)
            {
                Forward_distance(atab,atab->MyStruct,atab->MyStruct->struct_fsm->isDoneForward1,xnextpos2,ynextpos2);
            }
            atab->MyStruct->struct_fsm->isDoneForward1=0;
        }
    }
    // 4 eme module --> ne PAS ouviri la pince ni monter l'ascenseur
    openGripper(atab->electrovannes); // ouvrir la pince
// ouvrir la pince
    gripperOut(atab->electrovannes);
    time_sleep(2); // attendre que la pince soit sortie
    closeGripper(atab->electrovannes);
    time_sleep(1); // attendre que le module soit pris


    xnextpos3=atab->MyStruct->struct_odometry->x_t+100;
    ynextpos3=atab->MyStruct->struct_odometry->y_t;

    while(atab->MyStruct->struct_fsm->isDoneForward1==0)
    {
        Forward_distance(atab,atab->MyStruct,atab->MyStruct->struct_fsm->isDoneForward1,xnextpos3,ynextpos3);
    }
    atab->MyStruct->struct_fsm->isDoneForward1=0;
    gripperIn(atab->electrovannes); // rentrer la pince
    atab->MyStruct->struct_actions->ModulesTaken=1;
}

void lowerElevator(MyDynamixel dynaElevator)
/*
        redescendre l'ascenseur afin de pouvoir redemarrer la sequence
*/
{
    dynaElevator.EndlessTurn(0x150,"CCW");
}

void upperElevator(MyDynamixel dynaElevator)
/*
        monter l'ascenseur
*/
{

    dynaElevator.EndlessTurn(0x150,"CW");
}


void stopElevator(MyDynamixel dynaElevator)
/*
        monter l'ascenseur
*/
{

    dynaElevator.EndlessTurn(0x00,"CW");
}

void ejectModules(args* atab,int ModulesToEject)
/*
      deposer les 4 modules dans la zone lorsque le robot est devant celle-ci
*/
{
    atab->motorL.setSpeed(0);
    atab->motorR.setSpeed(0);
    gripperOut(atab->electrovannes); // sortir la pince
    time_sleep(1);
    openGripper(atab->electrovannes); // ouvrir la pince
    time_sleep(1);
    for(int i = 0 ; i<ModulesToEject-1; i++)
    {
        gripperIn(atab->electrovannes); // Rentrer la pince
        atab->dynaElevator.EndlessTurn(0x150,"CCW"); // descendre l'ascenseur
        time_sleep(4);
        gripperOut(atab->electrovannes); // Sortir la pince
        time_sleep(1);
    }
    gripperIn(atab->electrovannes); // rentrer la pince
    atab->MyStruct->struct_actions->ModulesEjected=1;
}


void Homologation(args* atab)
{
  double xnextpos1,ynextpos1;
  switch (atab->MyStruct->struct_actions->homologation_state)
  {
    case WAIT_HOMO:
    atab->MyStruct->struct_actions->homologation_state=CATCH_MODULE_HOMO;

    break;

    case GRIPPER_OUT_HOMO:
    if(ModulesTaken==0)
    {
      catchModules(atab,1);
    }
    else
    {
      atab->MyStruct->struct_actions->homologation_state=TURN_HOMO;
    }
    break;

    case CLOSE_GRIPPER_HOMO:
    if(ModulesTaken==0)
    if(atab->MyStruct->inputs->t-atab->MyStruct->struct_actions->time_enter_state<TIME_GRIPPER_CLOSED) //Je ferme la pince tant que j'ai pas dépassé le timeque j'estimeetre necassaire pourfermer la pince
    {
        closeGripper(atab->electrovannes); // Fermer la pince
    }
    else
    {
      atab->MyStruct->struct_actions->homologation_state=BACKWARD_HOMO;
    }
    break;


    case BACKWARD_HOMO:
    xnextpos1=atab->MyStruct->struct_odometry->x_t+100; // X_FRONT+100
    ynextpos1=atab->MyStruct->struct_odometry->y_t;     // Y_FRONT

    printf("IsDoneForward1 %d\n",atab->MyStruct->struct_fsm->isDoneForward1);
    while(atab->MyStruct->struct_fsm->isDoneForward1==0)
    {
        printf("WHILE IsDoneForward1 %d\n",atab->MyStruct->struct_fsm->isDoneForward1);
        Forward_distance(atab,atab->MyStruct,atab->MyStruct->struct_fsm->isDoneForward1,xnextpos1,ynextpos1);
    }
    if (atab->MyStruct->struct_fsm->isDoneForward1==1)
    {
      atab->MyStruct->struct_actions->homologation_state=TURN_HOMO;
    }
    break;

    case TURN_HOMO:
    while(atab->MyStruc->struct_fsm->isDoneRotate1==0)
    {
      if(atab->MyStruct->inputs->color==YELLOW)
      {
      Rotate_to_angle(atab,atab->Mystrut,-PI/4)
      }
      else
      {
      Rotate_to_angle(atab,atab->Mystrut,PI/4)
      }
    }
    if(atab->MyStruc->struct_fsm->isDoneRotate1==1)
    {
      atab->MyStruct->struct_actions->homologation_state=DROP_MODULE_HOMO;
    }
    break;

    case DROP_MODULE_HOMO:
    if(atab->MyStruct->struct_actions->ModulesEjected==0)
    {
    openGripper(atab->electrovannes);
    }
    else
    {
      atab->MyStruct->struct_actions->homologation_state=DONE_HOMO;
    }
    break;

    case DONE_HOMO:
    {
      atab->MyStruct->struct_actions->ActionIsDone=1;
    }

  }

}


void ejectModulesFSM(args* atab,int ModulesToEject)
{
  case WAITMODULE:
      printf("I'm in the WAITMODULE STATE state\n");
      if(atab->MyStruct->struct_actions->nb_modules_ejected==ModulesToEject)
      {
          printf("%d modules are taken",ModulesToEject);
          atab->MyStruct->struct_actions->take_modules_state=WAITMODULE;
          atab->MyStruct->struct_actions->ActionIsDone=1;
          atab->MyStruct->struct_actions->ModulesEjected=1;
      }
      else
      {
          atab->MyStruct->struct_actions->take_modules_state=OPEN_GRIPPER;
      }
  break;

  case OPEN_GRIPPER:
      printf("I'm in the OPEN_GRIPPER STATE\n");
      openGripper(atab->electrovannes);
      atab->MyStruct->struct_actions->take_modules_state=GRIPPER_OUT;
      atab->MyStruct->struct_actions->time_enter_state=atab->MyStruct->inputs->t;
      // Time wait
      break;
  case GRIPPER_OUT:
          printf("I'm in the GRIPPER_OUT state\n");

          gripperOut(atab->electrovannes);
          printf("%d\n", (atab->MyStruct->struct_sensors->front_endway));
          if((atab->MyStruct->struct_sensors->front_endway)==1)
          {
              printf("HELLLOOO\n");
              atab->MyStruct->struct_actions->take_modules_state=GRIPPER_IN;
              atab->MyStruct->struct_actions->time_enter_state=atab->MyStruct->inputs->t; // le temps auquel on rentre dans l'état suivant: CLOSE_GRIPPER
          }
          else
          {
              if((atab->MyStruct->inputs->t-atab->MyStruct->struct_actions->time_enter_state>TIME_GRIPPER_OUT))
              {
                  printf("Time Out reached in the GRIPPER OUT state");
                  // Ca a foiré-> La glissère n'est pas bien sortie --> abort
                  // Reculer un peu? Tant qu'on a pas le fin de course?
                  atab->MyStruct->struct_actions->take_modules_state=ABORT_MODULE; // quitte la fonction catchmodules et rentre tous les mécanismes
              }
              else
              {
                  printf("Just a test");
                  atab->MyStruct->struct_actions->take_modules_state=GRIPPER_OUT;
                  // On doit attendre que la pince sorte complètement :-)
              }
          }

  break;

  case GRIPPER_IN:
  printf("I'm in the GRIPPER_IN state\n");
  time_sleep(0.5);

  gripperIn(atab->electrovannes);
      if(gripperIsIn==1)
      {
          atab->MyStruct->struct_actions->take_modules_state=DYNA_ELEVATOR_DOWN;
          atab->MyStruct->struct_actions->time_enter_state=atab->MyStruct->inputs->t;
          atab->MyStruct->struct_actions->nb_modules_ejected++;
      }
      else
      {
          if((atab->MyStruct->inputs->t-atab->MyStruct->struct_actions->time_enter_state>TIME_GRIPPER_IN))
          {
              // Ca a foiré, bloqué, quand j'ai rentré la pince :-(
              // What to do?

          }
          else
          {
              atab->MyStruct->struct_actions->take_modules_state=GRIPPER_IN;
          }
      }
  }
  break;

  case DYNA_ELEVATOR_DOWN:
  printf("I'm in the GRIPPER_IN state\n");
  time_sleep(0.5);
  atab->dynaElevator.EndlessTurn(0x150,"CCW"); // descendre l'ascenseur
  if((atab->MyStruct->inputs->t-atab->MyStruct->struct_actions->time_enter_state>TIME_DYNA_ELEVATOR_DOWN))
    atab->dynaElevator.EndlessTurn(0x00,"CCW"); // arreter l'ascenseur

  }
  break;

  case DONE_MODULE:
      if(atab->MyStruct->struct_actions->nb_modules_ejected==4)
      {
          atab->MyStruct->struct_actions->take_modules_state=WAITMODULE;
      }
      else
      {
          atab->MyStruct->struct_actions->take_modules_state=OPEN_GRIPPER;
      }
  break;

  case ABORT_MODULE:
      printf("I'm in the ABORT_MODULE state\n");
      openGripper(atab->electrovannes);
      gripperIn(atab->electrovannes);
      atab->MyStruct->struct_actions->ModulesEjected=1; // permet de quitter la fonction catchmodules
  break;
}

void Stop_Everything(args* atab)
{
    atab->motorR.setSpeed(0);
    atab->motorL.setSpeed(0);
    // +Set dynamixel to 0
    // +Set allActuators to 0
}
