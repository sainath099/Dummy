//program to have a header file for client class
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
void Client();
int ToGetSockfd();				//to get sock id
void ToGetCredentials(char *);			//to get credentials
int ToServerConnect( );				//to connect to the server
int ToSendData(char *);			//to send data to the server
//char * CToRecvData();				//to receive data from the server
void CToRecvData(char *);
void ToGetUserCommands(char *, char *);		//to get user subcommands
void ToDisplayRecvData(char * );		//to display records from the server
int ToCloseClientConnections();			//to close the client connections

        	
