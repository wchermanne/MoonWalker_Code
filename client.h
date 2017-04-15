//
//  Calibration_gr1.hpp
//
//
//  Created by Matthieu Fontaine on 28/03/17.
//
//

#ifndef client_h
#define client_h

#include <stdio.h>

void error(char *msg);
void sendData( int sockfd, int x );
void sendString( int sockfd, char str[]);
int getData( int sockfd );
void getDataSTR( int sockfd, char dest[]);
void Client_init();


#endif
