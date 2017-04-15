/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#define MCP2515_RXB0D0 0x66
#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"

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

void MySwitch::readSwitch(int mySwitchReg, int value){ // To modify!!!
    this_can->doInit();
    //this_can->doDebug();
    time_sleep(0.1);
    makeData(data,0x1f,0x3f,0x3f,0x00,true);
   this_can->doSendMsg(0x608,data,3,0x00);

    switch(mySwitchReg) {
        case 1:
            makeData(data,GPLAT+offset,mask_switch_1 ,((mask_switch_1 )&(value*0xff)),0x01,true);
            //this_can->doSendMsg(this_address,data,3,0x00);
            this_can->doReceiveMsg();
            //this_can->doDebug();
            //this_can->doHandleIRQ(3);
            printf("data 1:%f \n",spi2data(data));
            break;
        case 2:
            makeData(data,GPLAT+offset,mask_switch_2,((mask_switch_2)&(value*0xff)),0x01,true);
            //this_can->doSendMsg(this_address,data,3,0x00);
           this_can->doReceiveMsg();
            //this_can->doDebug();
            //this_can->doHandleIRQ(3);
            printf("data 2:%f \n",spi2data(data));
            break;
        case 3:
            makeData(data,GPLAT+offset,mask_switch_3,((mask_switch_3)&(value*0xff)),0x01,true);
            this_can->doSendMsg(this_address,data,3,0x00);
            //this_can->doReceiveMsg();
            //this_can->doDebug();
            this_can->doHandleIRQ(3);

            printf("data 3:%f \n",spi2data(data));
            break;
        case 4:
            makeData(data,GPLAT+offset,mask_switch_4,((mask_switch_4)&(value*0xff)),0x01,true);
            this_can->doSendMsg(this_address,data,3,0x00);
            this_can->doHandleIRQ(3);
            //this_can->doDebug();
            //this_can->doHandleIRQ(3);
            printf("data 4:%f \n",spi2data(data));
            break;
        case 5:
            makeData(data,GPLAT+offset,mask_switch_5,((mask_switch_5)&(value*0xff)),0x01,true);
            this_can->doSendMsg(this_address,data,3,0x00);
            //this_can->doReceiveMsg();
            this_can->doHandleIRQ(3);
            printf("data 5:%f \n",spi2data(data));
            break;
        case 6:
            makeData(data,GPLAT+offset,mask_switch_6,((mask_switch_6)&(value*0xff)),0x01,true);
            this_can->doSendMsg(this_address,data,3,0x00);
            printf("data 6:%f \n",spi2data(data));
             this_can->doHandleIRQ(3);
            break;
    }
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


