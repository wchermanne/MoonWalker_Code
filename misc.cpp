/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   misc.cpp
 * Author: verbi
 *
 * Created on 6 décembre 2016, 21:12
 */

#include <cstdlib>
#include "misc.h"
using namespace std;

/*
 *
 */
void genBuf(unsigned char* buf, unsigned char data0, unsigned char data1,
			unsigned char data2, unsigned char data3)
{
	buf[0] = data0;
	buf[1] = data1;
	buf[2] = data2;
	buf[3] = data3;
}

double average(double* buf, int length)
{
	double accumulator=0.0;
	for(int i =0; i< length; i++)
	{
		accumulator += buf[i];
	}
	return accumulator/(double)length;
}

//	struct timeval tp;
//	gettimeofday(&tp, NULL);
//	long int ms_start = tp.tv_sec * 1000 + tp.tv_usec / 1000;
//
//
//	while(motorL.getPos()> -13800*100)
//	{}
//
//	gettimeofday(&tp, NULL);
//	long int ms_end = tp.tv_sec * 1000 + tp.tv_usec / 1000;
//
//	cout << ms_end-ms_start << endl;


