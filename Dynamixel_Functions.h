/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Functions.h
 * Author: Euroclear
 *
 * Created on February 28, 2017, 6:43 PM
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

class MyDynamixel{
public:
    MyDynamixel(unsigned int id, MyDE0Nano *nano);//, unsigned char buf[4]);
    virtual ~MyDynamixel();
    void SendMessage(unsigned int id,unsigned int length,unsigned int instr,unsigned int P0,unsigned int P1,unsigned int P2,MyDE0Nano *nano);//,unsigned char buf[4]);
    void ReadMessage(unsigned int id,unsigned int address,unsigned int nBytes);
    void LedOn();
    void LedOff();
    void EndlessTurn(int speed, const char *direction);
    void absoluteRotate(int speed, double angle);
    void PrintRegisters();
    double GetRegister(unsigned int address,unsigned int nBytes);
    void relativeRotate(int speed, double relativeAngle);
    void TakeBalls();
    void ReleaseBalls();



private:
    unsigned int this_id;
    MyDE0Nano *this_nano;
    unsigned char this_buf[4];
};



#endif /* FUNCTIONS_H */

