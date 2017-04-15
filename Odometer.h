/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Odometer.h
 * Author: matthieu
 *
 * Created on March 13, 2017, 11:27 PM
 */
#include "MyMCP2515.h"
#include "MyDE0Nano.h"
#ifndef ODOMETER_H
#define ODOMETER_H

class MyOdometers{
public:     MyOdometers( MyDE0Nano *nano, int side);
    virtual ~MyOdometers();
    double getOdometersPosition();
    double getOdometersSpeed(int number_of_samples);
private:
    MyDE0Nano *this_nano;
    int this_side;
    char data[3];
};

#endif /* ODOMETER_H */

