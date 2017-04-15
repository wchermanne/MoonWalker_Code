/*
 * this function creates a tab based on the argugments from char0 to char3
 * forCAN is true when data is a tab of length 3
 */

#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"
unsigned int id_address = 0x03;
unsigned int baudrate_address = 0x04;
unsigned int cw_limit_address = 0x06;
unsigned int ccw_limit_address = 0x08;
unsigned int led_address = 0x19;
unsigned int goal_position_address = 0x1e;
unsigned int moving_speed_address = 0x20;
unsigned int torque_limit_address = 0x22;
unsigned int current_position_address = 0x24;
unsigned int current_speed_address = 0x26;
unsigned int current_load_address = 0x28;
unsigned int voltage_address = 0x2a;
unsigned int temperature_address = 0x2b;
unsigned int isMoving_address = 0x2e;

//SPI register
unsigned int spi_dynamixel_write1 = 0x01;
unsigned int spi_dynamixel_write2 = 0x02;
unsigned int spi_dynamixel_read1 = 0x05;
unsigned int spi_dynamixel_read2 = 0x06;

void makeData( char* data,  char data0,  char data1,
			 char data2, char data3, bool forCAN)
{
    if(forCAN)
    {
        data[0] = data0;
        data[1] = data1;
        data[2] = data2;

    }
    else
    {
        data[0] = data0;
        data[1] = data1;
        data[2] = data2;
        data[3] = data3;
    }
}

int spi2data (char* data)
{
    int output = ((int) data[3] + (256*data[2]) + (256*256*data[1]) + (256*256*256*data[0]-2*data[0]%128*4294967296));
}


int maxdata(int data1, int data2)
{
    int max;
    if(data1 >= data2)
    {
        max = data1;
    }
    else
    {
        max = data2;
    }
    return max;
}

