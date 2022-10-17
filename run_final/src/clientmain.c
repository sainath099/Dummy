//program to define the server main function
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include "client.h"
int sockid ;
void signalHandler(int signal)
{
        if(signal==SIGINT)
        {
                send(sockid,"bye",3,0);
                exit(0);
        }
}

//main function to execute client

int main () 
{
        signal(SIGINT,signalHandler);						//signal to cut the client from server
        char recvdata[200]="",credentials[200]="",commands[200]="";
        Client();											//creating a client
        sockid=ToGetSockfd();								//calling the getsockfd of client 
        ToGetCredentials(credentials);						//calling the getcredentials functions of client
        ToServerConnect() ;									//calling the serverconnect function to connect to the server
        ToSendData(credentials);							//sending the credentials to server 
        CToRecvData(recvdata);								//receiving the message from the server using recvData function
        char type[200]="";
        strcpy(type,recvdata);							
        ToDisplayRecvData(recvdata);						//calling the funtion to display the data
        while(1)
        {
                strcpy(commands,"");
                ToGetUserCommands(type,commands) ;	//calling the function to get the usercommands
                if(strcmp("",commands)!=0)
                {
               	   ToSendData(commands);					//sending the user commands to the server				
                   CToRecvData(recvdata);					//receiving the data from the server
                   ToDisplayRecvData(recvdata);				//displaying the received data from the server
                }
                else
                {
                        printf("Subcommand can't be empty");
                        continue;
                }
        }
        return EXIT_SUCCESS;
}

