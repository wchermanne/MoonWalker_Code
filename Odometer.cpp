/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Motors.cpp
 * Author: matthieu
 *
 * Created on March 6, 2017, 4:42 PM
 */
#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"

#define PI 3.141592653
#define nTicksTour 2708.0

/*buld of the motor. address corresponds to the one of the CAN's PCB (ex: Minibot = 0x708)*/
MyOdometers::MyOdometers( MyDE0Nano *nano, int side)
{
    this_nano = nano;
    this_side = side;// 1 for the right motor, 2 for the left motor

}
MyOdometers::~MyOdometers()
{

}

double MyOdometers::getOdometersPosition()
{
    double nTicks;
    //unsigned char buf[4] = {0x00, 0x00, 0x00, 0x00};
    char buf[4] = {0x00, 0x00, 0x00, 0x00};
    if(this_side == 1) {
    makeData(buf, 0x00, 0x00, 0x00, 0x00, false);
    this_nano->readWriteReg(READ, 0x09, (unsigned char*)buf, 4); // register read of PosEgdeTicks
    int right_odometer = spi2data(buf); // converting char value into int value //minus sign for MW
    nTicks = right_odometer;
    //printf("right odometer position : %f\n",(((double) nTicks)/nTicksTour*360));
    //printf("right odometer position : %f\n",(((double) nTicks)));
    }
    else if (this_side == 2) {
    makeData(buf, 0x00, 0x00, 0x00, 0x00, false);
    this_nano->readWriteReg(READ, 0x08, (unsigned char*)buf, 4);
    int left_odometer = spi2data(buf); // register read of PosEgdeTicks //minus sign for MW
    nTicks = left_odometer;
    //printf("Left odometer position : %f\n",(((double) nTicks)/nTicksTour*360));
    //printf("Left odometer position : %f\n",(((double) nTicks)));
    }
    time_sleep(0.01);
    return nTicks/nTicksTour*360;
    // dist parcourue et vitesse
}

double MyOdometers::getOdometersSpeed(int number_of_samples)
{
    //printf("getSpeedWithMean \n");
    double pos1, pos2;
    long int time_start, time_end;
    struct timeval  tv;
    int address_of_SPI, i, speed;
    double diff_pos = 0.0;
    if(this_side == 1)
    {
        address_of_SPI = 0x09;
    }
    else if(this_side ==2)
    {
        address_of_SPI = 0x08;
    }
    else
    {
        printf("This motor is not defined");
        i = number_of_samples+1;
    }

    //unsigned char buf[4] = {0x00, 0x00, 0x00, 0x00};
    char buf[4] = {0x00, 0x00, 0x00, 0x00};

    for(i=0;i<number_of_samples; i++)
    {

        makeData(buf, 0x00, 0x00, 0x00, 0x00, false);
        this_nano->readWriteReg(READ, address_of_SPI, (unsigned char*)buf, 4);
        gettimeofday(&tv, NULL);
        time_start =  (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;// register read of PosEgdeTicks
        speed = spi2data(buf); // converting char value into int value //minus sign for MW
        pos1 = (double) speed;

        time_sleep(0.01);

        makeData(buf, 0x00, 0x00, 0x00, 0x00, false);
        this_nano->readWriteReg(READ, address_of_SPI, (unsigned char*)buf, 4);
        gettimeofday(&tv, NULL);
        time_end =  (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;// register read of PosEgdeTicks
        speed = spi2data(buf); // converting char value into int value //minus sign for MW
        pos2 = (double) speed;

        diff_pos +=(pos2-pos1)*2*PI/((time_end-time_start)*nTicksTour);

    }
    return (diff_pos/number_of_samples)*1000; // minus sign for MW only
}


