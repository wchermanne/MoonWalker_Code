#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <wiringPiSPI.h>
#include <pigpio.h>
#include <bitset>
#include <ctime>
#include <sys/time.h>
#include "MyDE0Nano.h"
#include "misc.h"
#include "Dynamixel_Functions.h"
#include <iomanip>
#include "globals.h"
#include "math.h"
#include <string.h>
using namespace std;

#define angle_frame 54
#define tolerance_angle 2 //en deg

MyDynamixel::MyDynamixel(unsigned int id, MyDE0Nano *nano)//, unsigned char buf[4])
{
    this_id = id;
    this_nano = nano;
    //this_buf = buf;

}
MyDynamixel::~MyDynamixel()
{

}
void MyDynamixel::SendMessage(unsigned int id,unsigned int length,unsigned int instr,unsigned int P0,unsigned int P1,unsigned int P2, MyDE0Nano *nano)//, unsigned char buf[4])
{
    /*
     * SendMessage(id,length,instr,P0,P1,P2,MyDE0Nano *nano,unsigned char buf[4])
     */
    int check = (~(this_id+length+instr+P0+P1+P2)) & 0x000000FF;

    genBuf(this_buf,check, instr, length, this_id);
    nano->readWriteReg(WRITE, spi_dynamixel_write1, this_buf, 4);
    genBuf(this_buf, 0x00, P2, P1, P0);
    nano->readWriteReg(WRITE, spi_dynamixel_write2, this_buf, 4);
    time_sleep(0.05);
}

void MyDynamixel::ReadMessage(unsigned int id,unsigned int address,unsigned int nBytes)
{
    /*
     * Send a message for asking the value of a register and printing this value
     * ReadMessage(0x06, 0x19 ,0x01 ,nano, buf) --> read 1 byte of the value of register at address 0x19 (= LED) of dynamixel with ID = 0x06
     */
    SendMessage(this_id,0x04,0x02,address,nBytes,0x00,this_nano);
    this_nano->readWriteReg(READ, spi_dynamixel_read1, this_buf, 4);
    printf("data1:  0x%02x   0x%02x  0x%02x  0x%02x \n", this_buf[0],this_buf[1],this_buf[2],this_buf[3] );
    this_nano->readWriteReg(READ, spi_dynamixel_read2, this_buf, 4);
    printf("data2:  0x%02x   0x%02x  0x%02x  0x%02x \n", this_buf[0],this_buf[1],this_buf[2],this_buf[3] );
    time_sleep(0.1);
}

void MyDynamixel::LedOn()
{
    /*
     * LedOn(nano, buf); --> turns the LED of dynamixel with id 0x08 ON
     */
    printf("\n	==========   Dynamixel 0x%02x : LED ON   ========== \n", this_id);
    SendMessage(this_id,0x04,0x03,led_address,0x01,0x00,this_nano);
    cout << "Dynamixel 0x0" << this_id << " : " << "led ON " << endl;
}

void MyDynamixel::LedOff()
{
    /*
     * LedOff(nano, buf); --> turns the LED of dynamixel with id 0x08 OFF
     */
    printf("\n	==========   Dynamixel 0x%02x : LED OFF   ========== \n", this_id);
    SendMessage(this_id,0x04,0x03,led_address,0x00,0x00,this_nano);
    cout << "Dynamixel 0x0" << this_id << " : " << "led OFF " << endl;
}

void MyDynamixel::EndlessTurn(int speed, const char *direction)
{
    /*
     * speed must be set to a value between 0x00 and 0x3FF
     * EndlessTurn(0x3ff, nano, buf); --> sets the max speed to dynamixel with id 0x08
     */
     int condition1 = (strcmp(direction, "CW") == 0); // true si direction = CW
     int condition2 = (strcmp(direction, "CCW") == 0); // true si direction = CCW


    printf("\n	==========   Dynamixel 0x%02x : ENDLESS TURN   ========== \n", this_id);
    int speedL = speed & 0x000000FF;
    int speedH = ((speed & 0x00000F00)>>8)+4*condition1;
    double percent = 100*speed/1023.0;

    SendMessage(this_id,0x05,0x03,cw_limit_address,0x00,0x00,this_nano); // CW angle limit set to 0
    SendMessage(this_id,0x05,0x03,ccw_limit_address,0x00,0x00,this_nano); // CCW angle limit set to 0
    SendMessage(this_id,0x05,0x03,moving_speed_address,speedL,speedH,this_nano); // set speed
    printf("Dynamixel 0x%02x : endless %s turn at speed: 0x%03x (= %d/1023 = %.2f%% of max speed)\n", this_id, direction, speed, speed, percent);
}

void MyDynamixel::absoluteRotate(int speed, double angle)
{
    /*
     * absoluteRotate(0x090,150, nano, buf) --> dynamixel with ID = 0x06 moves at position 150(deg) at speed 0x090
     * function only ends when th position is reached!
     */
    printf("\n	==========   Dynamixel 0x%02x : ABSOLUTE ROTATE   ========== \n", this_id);
    SendMessage(this_id,0x05,0x03,cw_limit_address,0x00,0x00,this_nano); // 0x000 --> CW angle limit set to 30
    SendMessage(this_id,0x05,0x03,ccw_limit_address,0xff,0x03,this_nano); // 0x3ff --> CCW angle limit set to 300
    int speedL = speed & 0x000000FF;
    int speedH = (speed & 0x00000F00)>>8;
    double percent = 100*speed/1023.0;
    int position = angle*1023.0/300;
    int positionL = position & 0x000000FF;
    int positionH = (position & 0x00000F00)>>8;
    while(angle>300.0 || angle<0.0)
    {
        // the angle is not included i the interval [0-300](deg) or [0 - 1023](position) --> program has to be blocked in this state
        printf("Invalid angle! \n");
        time_sleep(5);
    }
    SendMessage(this_id,0x05,0x03,moving_speed_address,speedL,speedH,this_nano); // set speed
    SendMessage(this_id,0x05,0x03,goal_position_address,positionL,positionH,this_nano); // set position
    printf("Dynamixel 0x%02x : rotation at speed: 0x%03x (= %d/1023 = %.2f%% of max speed)\n", this_id, speed, speed, percent);
    printf("Dynamixel 0x%02x : going to position: 0x%03x (= %.2f degrees) \n", this_id, position, angle);
    while(GetRegister(isMoving_address ,0x01) || fabs(GetRegister(current_position_address ,0x02)-angle)>tolerance_angle) // while it is still moving
    {
        double currentposition = GetRegister(current_position_address ,0x02);
        double error =  fabs(currentposition-angle);
        printf("error = %.2f \n", error);
        // wait untill it stops moving and reahces its demanded position (1 degree of error)
    }
    printf("Dynamixel 0x%02x : Arrived at position: = %.2f degrees \n", this_id, GetRegister(current_position_address ,0x02));
    time_sleep(0.1); // pour souffler un peu :)
}

void MyDynamixel::relativeRotate(int speed, double relativeAngle)
{
    /*
     * relativeAngle in degrees!
     * relativeRotate(0x090, 150, nano, buf); --> rotates dynamixel 0x06 from an angle of 0x1ff ( = 150 degrees) at speed 0x090
     *	 function only ends when th position is reached!
     */
    printf("\n	==========   Dynamixel 0x%02x : RELATIVE ROTATE   ========== \n", this_id);
    SendMessage(this_id,0x05,0x03,cw_limit_address,0x00,0x00,this_nano); // 0x000 --> CW angle limit set to 30
    SendMessage(this_id,0x05,0x03,ccw_limit_address,0xff,0x03,this_nano); // 0x3ff --> CCW angle limit set to 300
    int speedL = speed & 0x000000FF;
    int speedH = (speed & 0x00000F00)>>8;
    double percent = 100*speed/1023.0;
    //int currentPosition =  //position is a value between 0x00 and 0x3ff

    double currentAnglePrevious = GetRegister(current_position_address, 0x02);
    double currentAngle = GetRegister(current_position_address, 0x02);
    double tolerance = 0.3;
    while(fabs(currentAngle-currentAnglePrevious)> tolerance)
    {
        // erreur sur currentAngle --> 1 passge dans while, erreur sur currentAnglePrevious --> 2 passages
        printf("Mistake \n");
        currentAnglePrevious = currentAngle;
        currentAngle = GetRegister(current_position_address, 0x02);
    }
    printf("Correct \n");
    printf("Dynamixel %.2f : currentAngle \n",currentAngle);
    printf("Dynamixel %.2f : currentAnglePrevious \n", currentAnglePrevious);

    double finalAngle = currentAngle+relativeAngle;
    int finalPosition = finalAngle*1023.0/300;
    int positionL = finalPosition & 0x000000FF;
    int positionH = (finalPosition & 0x00000F00)>>8;
    if(finalAngle>300.0 || finalAngle<0.0)
    {
        // the angle is not included i the interval [0-300](deg) or [0 - 1023](position) --> program has to be blocked in this state
        printf("finalAngle = %f \n", finalAngle);
        printf("Invalid angle! \n");
        time_sleep(5);
    }
    printf("finalPosition = %d \n", finalPosition);
    SendMessage(this_id,0x05,0x03,moving_speed_address,speedL,speedH,this_nano); // set speed
    SendMessage(this_id,0x05,0x03,goal_position_address,positionL,positionH,this_nano); // set position
    /*printf("Dynamixel 0x%02x : rotation at speed: 0x%03x (= %d/1023 = %.2f%% of max speed)\n", this_id, speed, speed, percent);
    printf("Dynamixel 0x%02x : starting from position: = %.2f degrees \n", this_id, currentAngle);
    printf("Dynamixel 0x%02x : travelling a distance of = %.2f degrees \n", this_id, relativeAngle);
    printf("Dynamixel 0x%02x : going to position: 0x%03x (= %.2f degrees) \n", this_id, finalPosition, finalAngle); */
    while(GetRegister(isMoving_address ,0x01) || fabs(GetRegister(current_position_address ,0x02)-finalAngle)>tolerance_angle) // while it is still moving or error too high
    {
        double error =  fabs(GetRegister(current_position_address ,0x02)-finalAngle);
        printf("error = %.2f \n", error);
    }
    printf("Dynamixel 0x%02x : Arrived at position: = %.2f degrees \n", this_id, GetRegister(current_position_address ,0x02));
    time_sleep(0.1); // pour souffler un peu :)
}

double MyDynamixel::GetRegister(unsigned int address,unsigned int nBytes)
{
    /*
     * Send a message for asking the value of a register and returning the value of the register
     * don't forget to specify the number of bytes wanted (0x01 or 0x02)
     * a = GetRegister(led_address ,0x01 ,nano, buf) --> returns the current value of the led
     */
    SendMessage(this_id,0x04,0x02,address,nBytes,0x00,this_nano);
    //nano->readWriteReg(READ, spi_dynamixel_read1, buf, 4);
    //printf("data1:  0x%02x   0x%02x  0x%02x  0x%02x \n", buf[0],buf[1],buf[2],buf[3] );
    this_nano->readWriteReg(READ, spi_dynamixel_read2, this_buf, 4);
    //printf("data2:  0x%02x   0x%02x  0x%02x  0x%02x \n", buf[0],buf[1],buf[2],buf[3] );
    time_sleep(0.1);
    if(address == goal_position_address || address == current_position_address)
    {
        // renvoie les angles degrees!
        return (256*this_buf[2]+this_buf[3])/1023.0*300;
    }
    else
    {
        if(nBytes == 0x01)
        {
            return this_buf[3]; // from data 2
        }
        else if(nBytes == 0x02)
        {
            return (256*this_buf[2]+this_buf[3]); // from data 2
        }
    }
}

void MyDynamixel::PrintRegisters()
{
    /*
     * PrintRegisters(nano,buf); --> read all important registers of dynamixel 0x08
     */
    printf("\nID: \n");
    ReadMessage(this_id,id_address ,0x01);
    printf("Baudrate: \n");
    ReadMessage(this_id,baudrate_address ,0x01);
    printf("CW limit angle: \n");
    ReadMessage(this_id,cw_limit_address ,0x02);
    printf("CCW limit angle: \n");
    ReadMessage(this_id,ccw_limit_address , 0x02);
    printf("LED value: \n");
    ReadMessage(this_id, led_address,0x01);
    printf("Goal position: \n");
    ReadMessage(this_id, goal_position_address,0x02);
    printf("Moving speed: \n");
    ReadMessage(this_id,moving_speed_address ,0x02);
    printf("Torque limit: \n");
    ReadMessage(this_id, torque_limit_address,0x02);
    printf("Current position: \n");
    ReadMessage(this_id,current_position_address ,0x02);
    printf("Current speed: \n");
    ReadMessage(this_id, current_speed_address,0x02);
    printf("Current load: \n");
    ReadMessage(this_id,current_load_address ,0x02);
    printf("Voltage: \n");
    ReadMessage(this_id, voltage_address,0x01);
    printf("Temperature: \n");
    ReadMessage(this_id, temperature_address,0x01);
    printf("Is Moving: \n");
    ReadMessage(this_id,isMoving_address ,0x01);
}

void MyDynamixel::TakeBalls()
{
    /*
    *  Rotates the frame to take the balls
    *  ATTENTION prevu pour dynamixel du frame -> 0x08
    *  TakeBalls(nano,buf);
    */
    relativeRotate(0x090, angle_frame);

}

void MyDynamixel::ReleaseBalls()
{
    /*
    *  Rotates the frame to release the balls
    *  ATTENTION prevu pour dynamixel du frame -> 0x08
    *  ReleaseBalls(nano,buf);
    */
    relativeRotate(0x090, -angle_frame);
}
