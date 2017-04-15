/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#define MCP2515_RXB0D0 0x66
#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"

void StructSensors_init(CtrlStruct *cvs)
{

    cvs->struct_sensors->back_endway =1;
    cvs->struct_sensors->front_endway =0;
    cvs->struct_sensors->left_uswitch =0;
    cvs->struct_sensors->right_uswitch =0;
    cvs->struct_sensors->gripper_uswitch =0;
    cvs->struct_sensors->photo_chips = 0;
}

MySwitch::MySwitch(MyMCP2515 *myCan, int address)
{
    this_can = myCan;
    this_address = address;// 0x608 for writing and 0x600 for reading
}
MySwitch::~MySwitch()
{

}

void MySwitch::setLed(bool activate){
    this_can->doInit();
    time_sleep(0.1);
    if(activate)
    {
        makeData(data,GPLAT+offset,mask_led_switch,mask_led_switch,0x00,true);
        this_can->doSendMsg(this_address,data,3,0x00);
        printf("Led on \n");
    }
    else
    {
        makeData(data,GPLAT+offset,mask_led_switch,0x00,0x00,true);
        this_can->doSendMsg(this_address,data,3,0x00);
        printf("Led off \n");
    }
}

void MySwitch::readSwitch(CtrlStruct *cvs){ // To modify!!!
    this_can->doInit();
    char tab[8];
    for(int i=0; i<8; i++)
    {
        tab[i] = 0;
    }
    makeData(data,0x34,0x3f,0x3f,0x00,true);
    this_can->doSendMsg(0x608,data,3,0x00);
    time_sleep(0.1);
    //time_sleep(0.6);
    //makeData(data,0x01+offset,0xff,0x00,0x00,true);
    //this_can->doSendMsg(0x608,data,3,0x00);
    makeData(data,0x11+offset,0x08,0x08,0x00,true); // remove messages from motors
    this_can->doSendMsg(0x608,data,3,0x00);

    time_sleep(0.1);
     //makeData(data,GPLAT+offset, 0xff, 0xff,0x00,true);// release brake
    this_can->doSendMyMsg(0x608,tab,0,0x00);
    time_sleep(0.1);
    this_can->doReceiveMsg();
    int message = this_can->doReceiveSwitch();
    if (message == 99)
    {

    }
    else
    {
        //printf("VALUE %d \n \n", message);
    }

    switch(message) {
        case 2:
            {if(cvs->struct_sensors->left_uswitch == 0)
            {
                cvs->struct_sensors->left_uswitch = 1;
                makeData(data,0x01+offset,0x02,0x00,0x00,true);
                //this_can->doSendMsg(this_address,data,3,0x00);
            }
            else
            {
               cvs->struct_sensors->left_uswitch = 0;
                makeData(data,0x01+offset,0x02,0xff,0x00,true);
                //this_can->doSendMsg(this_address,data,3,0x00);
            }
            //printf("u_switch_left = %d \n \n",cvs->struct_sensors->left_uswitch );
            //time_sleep(1);
            break;}
        case 4:
            {if(cvs->struct_sensors->right_uswitch == 0)
            {
                cvs->struct_sensors->right_uswitch = 1;
                makeData(data,0x01+offset,0x04,0x00,0x00,true);
                //this_can->doSendMsg(this_address,data,3,0x00);
            }
            else
            {
               cvs->struct_sensors->right_uswitch = 0;
                makeData(data,0x01+offset,0x04,0xff,0x00,true);
                //this_can->doSendMsg(this_address,data,3,0x00);
            }
            //printf("u_switch_right = %d \n \n",cvs->struct_sensors->right_uswitch );
            //time_sleep(1);
            break;}
        case 6:
            {
                //printf("both are pressed \n");
                cvs->struct_sensors->right_uswitch = 0;
                cvs->struct_sensors->left_uswitch = 0;
                break;
            }
        case 8:
            {if(cvs->struct_sensors->front_endway == 0)
            {
                cvs->struct_sensors->front_endway= 1;
                makeData(data,0x01+offset,0x08,0x00,0x00,true);
                this_can->doSendMsg(this_address,data,3,0x00);
            }
            else
            {
               cvs->struct_sensors->front_endway = 0;
                makeData(data,0x01+offset,0x08,0xff,0x00,true);
                this_can->doSendMsg(this_address,data,3,0x00);
            }
            //printf("front_endway = %d",cvs->struct_sensors->front_endway);
            //time_sleep(1);
            break;}
              case 16:
            {if(cvs->struct_sensors->gripper_uswitch== 0)
            {
                cvs->struct_sensors->gripper_uswitch= 1;
                makeData(data,0x01+offset,0x10,0x00,0x00,true);
                this_can->doSendMsg(this_address,data,3,0x00);
            }
            else
            {
               cvs->struct_sensors->gripper_uswitch = 0;
                makeData(data,0x01+offset,0x10,0xff,0x00,true);
                this_can->doSendMsg(this_address,data,3,0x00);
            }
            //printf("gripper= %d",cvs->struct_sensors->gripper_uswitch);
            //time_sleep(1);
            break;}
        case 32:
            {if(cvs->struct_sensors->back_endway == 0)
            {
                cvs->struct_sensors->back_endway= 1;
                makeData(data,0x01+offset,0x20,0x00,0x00,true);
                this_can->doSendMsg(this_address,data,3,0x00);
            }
            else
            {
               cvs->struct_sensors->back_endway = 0;
                makeData(data,0x01+offset,0x20,0xff,0x00,true);
                this_can->doSendMsg(this_address,data,3,0x00);
            }
            //printf("back_endway = %d",cvs->struct_sensors->back_endway);
            //time_sleep(1);
            break;}
    }
}

void MySwitch::init()
{
    this_can->doInit();

    makeData(data,0x1f,0x3f,0x3f,0x00,true); // Activate GPIO in read mode
    this_can->doSendMsg(0x608,data,3,0x00);
    time_sleep(0.1);
    makeData(data,0x01+offset,0xff,0x00,0x00,true); // active rising-edge detection
    //this_can->doSendMsg(0x608,data,3,0x00);
    time_sleep(0.1);
    makeData(data,0x11+offset,0x09,0x09,0x00,true); // remove messages from motors
    this_can->doSendMsg(0x708,data,3,0x00);

    time_sleep(0.1);

}

void MySwitch::blink(){
    setLed(true);
    time_sleep(0.5);
    setLed(false);
    time_sleep(0.5);
    setLed(true);
    time_sleep(0.5);
    setLed(false);
    time_sleep(0.5);
    setLed(true);
    time_sleep(0.5);
    setLed(false);
    time_sleep(0.5);
    setLed(true);
    time_sleep(0.5);
    setLed(false);
    time_sleep(0.5);
    setLed(true);
    time_sleep(0.5);
    setLed(false);
    time_sleep(0.5);
    setLed(true);
    time_sleep(0.5);
    setLed(false);
    time_sleep(0.5);
    setLed(true);
    time_sleep(0.5);
    setLed(false);
    time_sleep(0.5);
}
