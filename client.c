

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "client.h"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

void Client_init()
{
    int sockfd, portno = 8124, n;//51717, n;
    char serverIp[] = "192.168.43.88";//"130.104.174.73";
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    char recu[256];
    int data;

    printf( "contacting %s on port %d\n", serverIp, portno );

    if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
    {
                error("ERROR");
    }

    if ( ( server = gethostbyname( serverIp ) ) == NULL )
    {
                error("ERROR");
    }

    bzero( (char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
                error("ERROR");

    }
}

void sendData( int sockfd, int x ) {
    int n;

    char buffer[32];
    sprintf( buffer, "%d\n", x );
    if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
    {
        //error( const_cast<char *>( "ERROR writing to socket") );
        error("ERROR");
        //printf("ERRORRRRR");
    }
    buffer[n] = '\0';
}
/*
void sendDouble( int sockfd, double x ) {
    int n;

    char buffer[32];
    sprintf( buffer, "%f\n", x );
    if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
        error( const_cast<char *>( "ERROR writing to socket") );
    buffer[n] = '\0';
}
*/
void sendString( int sockfd, char str[]) {
    int n;
    int i;
    char buffer[64];
    sprintf(buffer,"%s\n",str);
    if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
    {
        error("ERROR");
    }
        //error( const_cast<char *>( "ERROR writing to socket") );
    buffer[n] = '\0';
}

int getData( int sockfd ) {
    char buffer[32];
    int n;

    if ( (n = read(sockfd,buffer,31) ) < 0 )
    {
        error("ERROR");

    }
       // error( const_cast<char *>( "ERROR reading from socket") );
    buffer[n] = '\0';
    return atoi( buffer );
}
void getDataSTR( int sockfd, char dest[] ) {
    char buffer[32];
    int n;

    if ( (n = read(sockfd,buffer,31) ) < 0 )
    {
        error("ERROR");
    }
        //error( const_cast<char *>( "ERROR reading from socket") );
    buffer[n] = '\0';
    sprintf(dest,"%s",buffer);
}
/*
double getDataDouble( int sockfd ) {
    char buffer[32];
    int n;

    if ( (n = read(sockfd,buffer,31) ) < 0 )
        error( const_cast<char *>( "ERROR reading from socket") );
    buffer[n] = '\0';
    return atof( buffer );
}

int main(int argc, char *argv[])
{
    int sockfd, portno = 8124, n;//51717, n;
    char serverIp[] = "192.168.43.88";//"130.104.174.73";
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    char recu[256];
    int data;

    if (argc < 3) {
        // error( const_cast<char *>( "usage myClient2 hostname port\n" ) );
        printf( "contacting %s on port %d\n", serverIp, portno );
        // exit(0);
    }
    if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
        error( const_cast<char *>( "ERROR opening socket") );

    if ( ( server = gethostbyname( serverIp ) ) == NULL )
        error( const_cast<char *>("ERROR, no such host\n") );

    bzero( (char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error( const_cast<char *>( "ERROR connecting") );

    //sendString( sockfd, (char *)"Coucou");
    //getDataSTR( sockfd,recu );
    //printf("recu : %s\n",recu);
    sendDouble( sockfd, 5.67);
    //data = getData( sockfd );
    int i = 0;
    int j;
    while (1)
    {
        //sendDouble( sockfd, 5.67);

        getDataSTR( sockfd,recu);
        if(recu == "Salut")
        {

        }
        else
        {
           printf("recu : %s\n",recu );
        }
        for (i=0; i<100; i++)
        {    char toSend[64]; //= "id=1:" + i;
            sprintf(toSend, "id=1:  %d", i);
            sendString( sockfd,toSend);
            for (j=0; j<1000000; j++)
            {

            }
            char toSend2[64]; //= "id=1:" + i;
            sprintf(toSend2, "id=2:  %d", i*2);
            sendString( sockfd,toSend2);
            for (j=0; j<1000000; j++)
            {

            }
            getDataSTR( sockfd,recu);
        }



    }


    close( sockfd );
    return 0;
}
*/
