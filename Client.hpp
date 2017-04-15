#ifndef CLIENT_HPP_INCLUDED
#define CLIENT_HPP_INCLUDED
void error(char *msg);
void sendData( int sockfd, int x );
void sendString( int sockfd, char str[]);
int getData( int sockfd );
void getDataSTR( int sockfd, char dest[]);
void Client_init();
#endif
