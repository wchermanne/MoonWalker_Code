/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"


/*build of the tourelle. address corresponds to the one of the CAN's PCB (ex: Minibot = 0x508)*/
MyTourelle::MyTourelle(MyMCP2515 *myCan, MyDE0Nano *nano, int address)
{
    this_can = myCan;
    this_nano = nano;
    this_address = address;
    ticks_number_per_rotation = 25355;
    beacon_radius = BEACON_RADIUS;

}
MyTourelle::~MyTourelle() //sait pas du tout a quoi ca sert...
{}
/*set the brake if activate is true, release them is it is false*/
void MyTourelle::setBrake(bool activate) {
    if(activate)
    {
        makeData(data,GPLAT+offset,mask_brake,mask_brake,0x00,true);
        this_can->doSendMsg(this_address,data,3,0x00);
    }
    else
    {
        makeData(data,GPLAT+offset,mask_brake,0x00,0x00,true);
        this_can->doSendMsg(this_address,data,3,0x00);
    }
}

/*turn on the led if activate is true, off if false */
void MyTourelle::setLed(bool activate){

    if(activate)
    {
        makeData(data,GPLAT+offset,mask_led,mask_led,0x00,true);
        this_can->doSendMsg(this_address,data,3,0x00);
    }
    else
    {
        makeData(data,GPLAT+offset,mask_led,0x00,0x00,true);
        this_can->doSendMsg(this_address,data,3,0x00);
    }
}

/*set the speed of the tourelle (positive: CCW, negative:CW)*/
void MyTourelle::setSpeed(int speed){

    makeData(data,GPLAT+offset, mask_brake,0x00,0x00,true);// release brake
    this_can->doSendMsg(this_address,data,3,0x00);
    time_sleep(0.01);

    makeData(data,T1CON+offset, 0xb3,0x80,0x00,true);//T1CON
   this_can->doSendMsg(this_address,data, 3,0x00);
    time_sleep(0.01);

    makeData(data,PR1+offset, 0xff, 0xff,0x00,true);//PR1
    this_can->doSendMsg(this_address,data, 3,0x00);
    time_sleep(0.01);

    makeData(data,PWM1+offset, 0xff,duty_zero + speed,0x00,true);//PWM1 set to speed
    this_can->doSendMsg(this_address,data, 3,0x00);
    time_sleep(0.01);
}
/*
 This function return the number of beacon seen during 1 rotation
 */
int MyTourelle::getNumberBeacon()
{
    int rising_edge_detected = nb_rising();
    int falling_edge_detected = nb_rising();
    int output = maxdata(rising_edge_detected, falling_edge_detected);
    return output;

}

/*
 *This function return the angle in RAD and dist in meter of the last beacon based on a mid-value between the first positive edge and last neg edge detected
 *
 * =>  registers have to be defined in the DE0 nano => register 0x0a and 0x0b for example
 */
double* MyTourelle::getDataBeacon()
{
    double *angle_beacon = new double[2];
    //unsigned char buf[4] = {0x00, 0x00, 0x00, 0x00};
    char buf[4] = {0x00, 0x00, 0x00, 0x00};
    makeData(buf, 0x00, 0x00, 0x00, 0x00, false);
    this_nano->readWriteReg(READ, 0x0a, (unsigned char*)buf, 4); // register read of PosEgdeTicks
    int pos_edge_value = spi2data(buf); // converting char value into int value
    printf("pos_edge_value = %d \n", pos_edge_value);
    makeData(buf, 0x00, 0x00, 0x00, 0x00, false);
    this_nano->readWriteReg(READ, 0x0b, (unsigned char*)buf, 4);
    int neg_edge_value = spi2data(buf); // register read of NegEgdeTicks
    printf("neg_edge_value = %d \n", neg_edge_value);
    printf("nTicks = %d \n", ticks_number_per_rotation);
    double angle_pos = (((double) pos_edge_value)/((double) ticks_number_per_rotation))*2*PI;
    printf("angle_pos = %f \n", angle_pos);
    double angle_neg = (((double)neg_edge_value)/((double)ticks_number_per_rotation))*2*PI;
    printf("angle_neg = %f \n", angle_pos);

    angle_beacon[0] = (angle_pos + angle_neg)/2;
    angle_beacon[1] = beacon_radius/sin((angle_neg-angle_pos)/2);

    return angle_beacon;


}

/*
 *This function return the angles in RAD of the last beacon first positive edge and last neg edge detected
 *
 * =>  registers have to be defined in the DE0 nano => register 0x0a and 0x0b for example
 */
double* MyTourelle::getAnglesBeacon()
{
    double *angle_beacon = new double[2];
    //unsigned char buf[4] = {0x00, 0x00, 0x00, 0x00};
    char buf[4] = {0x00, 0x00, 0x00, 0x00};
    makeData(buf, 0x00, 0x00, 0x00, 0x00, false);
    this_nano->readWriteReg(READ, 0x0a, (unsigned char*)buf, 4); // register read of PosEgdeTicks
    int pos_edge_value = spi2data(buf); // converting char value into int value
    //printf("pos_edge_value = %d \n", pos_edge_value);
    makeData(buf, 0x00, 0x00, 0x00, 0x00, false);
    this_nano->readWriteReg(READ, 0x0b, (unsigned char*)buf, 4);
    int neg_edge_value = spi2data(buf); // register read of NegEgdeTicks
    //printf("neg_edge_value = %d \n", neg_edge_value);
    //printf("nTicks = %d \n", ticks_number_per_rotation);
    double angle_pos = (((double) pos_edge_value)/((double) ticks_number_per_rotation))*2*PI;
    //printf("angle_pos = %f \n", angle_pos);
    double angle_neg = (((double)neg_edge_value)/((double)ticks_number_per_rotation))*2*PI;
    //printf("angle_neg = %f \n", angle_pos);

    angle_beacon[0] = angle_pos;
    angle_beacon[1] = angle_neg;

    return angle_beacon;
}

/*
 This function return the number of rising edge seen during 1 rotation
 => a register has to be defined in the DE0 nano => register 0x0c for example
 */
int MyTourelle::nb_rising()
{
    char buf[4] = {0x00, 0x00, 0x00, 0x00};
    //makeData(buf, 0x00, 0x00, 0x00, 0x00, false);
    this_nano->readWriteReg(READ, 0x0c,(unsigned char*) buf, 4);
    return ((int) buf[3]);

}
/*
 This function return the number of rising edge seen during 1 rotation
 => a register has to be defined in the DE0 nano => register 0x0d for example
 */
int MyTourelle::nb_falling()
{
    char buf[4] = {0x00, 0x00, 0x00, 0x00};
    //makeData(buf, 0x00, 0x00, 0x00, 0x00, false);
    this_nano->readWriteReg(READ, 0x0d, (unsigned char*) buf, 4);
    return ((int) buf[3]);

}
/*
 This function return the position of the tower in radian
 => a register has to be defined in the DE0 nano => register 0x03 for example
 */
double MyTourelle::getPosition()
{
    char buf[4] = {0x00, 0x00, 0x00, 0x00};
    makeData(buf, 0x00, 0x00, 0x00, 0x00, false);
    this_nano->readWriteReg(READ, 0x03, (unsigned char*)buf, 4); // register read of count_laser_cod_a
    int nbr_tickz = spi2data(buf); // converting char value into int value

    double position = (((double)nbr_tickz)/((double) ticks_number_per_rotation))*2*PI;

    return position;
}
