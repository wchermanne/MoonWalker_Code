#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"

static int ts;
static int tsOdo;


void * ThreadMotorR(void *atab)
{

    args *In_pthread = (args*) atab;
    while(1){
        if(tsOdo==0)
        {
            ts = 1;
            In_pthread->MyStruct->inputs->r_wheel_speed =  (In_pthread->odoR.getOdometersSpeed(1)); //(In_pthread->motorR.getSpeed());
            //printf("right : %f \n",In_pthread->MyStruct->inputs->r_wheel_speed);
            tsOdo=1;
        }
        else
        {
            //printf("ERROR : mutex locking failed \n ");
        }
    }
}
void * ThreadMotorL(void *atab)
{
    args *In_pthread = (args*) atab;
    while(1)
    {
        if(tsOdo==0)//ts==1)
        {
            ts = 0;
            In_pthread->MyStruct->inputs->l_wheel_speed = (In_pthread->odoL.getOdometersSpeed(1));//(In_pthread->motorL.getSpeed()); //
            //printf("left : %f \t",In_pthread->MyStruct->inputs->l_wheel_speed);
            tsOdo=1;
        }
        else
        {
            //printf("ERROR : mutex locking failed \n ");
        }
    }
}

void * ThreadOdoL(void *atab)
{
    struct timeval  tv;
    args *In_pthread = (args*) atab;
    while(1)
    {
        if(tsOdo==0)
        {
            In_pthread->MyStruct->inputs->l_odo_speed = (In_pthread->odoL.getOdometersSpeed(1));
            gettimeofday(&tv,NULL);
            In_pthread->MyStruct->inputs->current_TL = ((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);//1000;
            //printf("Vitesse Odo left = %f \n",In_pthread->MyStruct->inputs->l_odo_speed);

            tsOdo=1;
        }
        else
        {
            //printf("ERROR : mutex locking failed \n ");
        }
    }
}

void * ThreadOdoR(void *atab)
{
    struct timeval  tv;
    args *In_pthread = (args*) atab;
    while(1)
    {
        if(tsOdo==0)
        {
            In_pthread->MyStruct->inputs->r_odo_speed = (In_pthread->odoR.getOdometersSpeed(1));
            gettimeofday(&tv,NULL);
            In_pthread->MyStruct->inputs->current_TR = ((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);//1000;
            //printf("Vitesse Ofo right = %f \n",In_pthread->MyStruct->inputs->r_odo_speed);
            tsOdo=1;
        }
        else
        {
            //printf("ERROR : mutex locking failed ");
        };
    }
}

void * ThreadComputePosition(void *atab)
{
    args *In_pthread = (args*) atab;
    while(1)
    {
        if(tsOdo==1)
        {
            xsiRWheels(In_pthread->MyStruct,0);
            computePosition(In_pthread->MyStruct);
            tsOdo = 0;
            //printf("Position : x = %f \t",In_pthread->MyStruct->struct_odometry->x_t);
            //printf("y = %f \n",In_pthread->MyStruct->struct_odometry->y_t);
        }
        else
        {
            //printf("ERROR : mutex locking failed ");
        }
    }
}

void * ThreadTourellePosition(void *atab)
{
    args *In_pthread = (args*) atab;
    while(1)
    {
        In_pthread->MyStruct->inputs->tower_pos = (In_pthread->tourelle.getPosition());

    }
}

void * ThreadAnglesBeacon(void *atab)
{
    args *In_pthread = (args*) atab;
    while(1)
    {
        double * edge_angles = In_pthread->tourelle.getAnglesBeacon();
        In_pthread->MyStruct->inputs->last_rising[9]= * edge_angles;
        In_pthread->MyStruct->inputs->last_falling[9] = *(edge_angles)+1;

    }
}

