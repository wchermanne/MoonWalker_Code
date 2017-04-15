/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   globals.h
 * Author: verbi
 *
 * Created on 7 décembre 2016, 14:06
 */

#ifndef GLOBALS_H
#define GLOBALS_H


extern unsigned int id_address; // Rd
extern unsigned int baudrate_address;//Rd
extern unsigned int cw_limit_address;//Rd/Wr --> NOT in degrees!
extern unsigned int ccw_limit_address;//Rd/Wr --> NOT in degrees!
extern unsigned int led_address;//Rd/Wr
extern unsigned int goal_position_address;//Rd/Wr --> in degrees!
extern unsigned int moving_speed_address;//Rd/Wr
extern unsigned int torque_limit_address;//Rd/Wr
extern unsigned int current_position_address;//Rd --> in degrees!
extern unsigned int current_speed_address;//Rd
extern unsigned int current_load_address;//Rd
extern unsigned int voltage_address;//Rd
extern unsigned int temperature_address;//Rd
extern unsigned int isMoving_address;//Rd

//SPI register
extern unsigned int spi_dynamixel_write1;
extern unsigned int spi_dynamixel_write2;
extern unsigned int spi_dynamixel_read1;
extern unsigned int spi_dynamixel_read2;



void makeData( char* data,  char data0,  char data1, 
			 char data2, char data3, bool forCAN);
int spi2data (char* data);
int maxdata(int data1, int data2);




#endif /* GLOBALS_H */

