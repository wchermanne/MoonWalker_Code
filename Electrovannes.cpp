/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"



MyVannes::MyVannes(MyMCP2515 *myCan, int address)
{
    this_can = myCan;
    this_address = address;// 1 for the right motor, 2 for the left motor
}
MyVannes::~MyVannes()
{

}

void MyVannes::setLed(bool activate){
    this_can->doInit();
    time_sleep(0.15);
    if(activate)
    {
        makeData(data,GPLAT+offset,mask_led_vanne,mask_led_vanne,0x00,true);
        this_can->doSendMsg(this_address,data,3,0x00);
        printf("Led on \n");
    }
    else
    {
        makeData(data,GPLAT+offset,mask_led_vanne,0x00,0x00,true);
        this_can->doSendMsg(this_address,data,3,0x00);
        printf("Led off \n");
    }
    time_sleep(0.15);
}

void MyVannes::setVanne(int vanne, int value){
    this_can->doInit();// because WTF
    time_sleep(0.1);

    switch(vanne) {
        case 1:
            makeData(data,GPLAT+offset,mask_vanne_1,((mask_vanne_1)&(value*0xff)),0x00,true);
            this_can->doSendMsg(this_address,data,3,0x00);
            break;
        case 2:
            makeData(data,GPLAT+offset,mask_vanne_2,((mask_vanne_2)&(value*0xff)),0x00,true);
            this_can->doSendMsg(this_address,data,3,0x00);
            break;
        case 3:
            makeData(data,GPLAT+offset,mask_vanne_3,((mask_vanne_3)&(value*0xff)),0x00,true);
            this_can->doSendMsg(this_address,data,3,0x00);
            break;
        case 4:
            makeData(data,GPLAT+offset,mask_vanne_4,((mask_vanne_4)&(value*0xff)),0x00,true);
            this_can->doSendMsg(this_address,data,3,0x00);
            break;
    }
}

void MyVannes::takeModules_part1()
{
	setVanne(VALVE_GRIPPER, false); // ouvrir la pince
	setVanne(VALVE_MODULES, true); // sortir la pince
	time_sleep(2); // attendre que la pince soit sortie
	setVanne(VALVE_GRIPPER, true); // fermer la pince
	time_sleep(2); // attendre que le module soit pris
}

void MyVannes::takeModules_part2()
{
	setVanne(VALVE_MODULES, false); // rentrer la pince
	time_sleep(2); // attendre que la pince soit rentree
	setVanne(VALVE_GRIPPER, false); // ouvrir la pince pour lacher le module
	time_sleep(0.5);
	/*time_sleep(3); // attendre que la pince soit sortie
	//setVanne(2,true);
	time_sleep(1);
	//setVanne(2,false);*/
}

void MyVannes::takeOres()
{
	setVanne(VALVE_FRAME, true); // monter le frame
	time_sleep(8); //  attendre que le dynamixel mette le frame a lhorizontal
	setVanne(VALVE_FRAME, false); // descendre le frame
	time_sleep(5); // attendre que le frame soit descendu
	setVanne(VALVE_FRAME, true); // remonter le frame avec les boules
	time_sleep(5); // attendre que le frame soit remonte
	time_sleep(5); // attendre le dynamixel mette le frame a la verticale pour ejecter les boules
}

void MyVannes::funnyAction()
{
	setVanne(VALVE_FUNNY, true); // lacher la pression
	time_sleep(20.5);
	setVanne(VALVE_FUNNY, false); // refermer la vanne
}

void MyVannes::resetVannes()
{
    setVanne(VALVE_FRAME,false);
    time_sleep(1);
    setVanne(VALVE_GRIPPER,false);
    time_sleep(1);
    setVanne(VALVE_MODULES,false);
    time_sleep(1);
    setVanne(VALVE_FUNNY,false);
     time_sleep(1);
}

void MyVannes::blink(){
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


