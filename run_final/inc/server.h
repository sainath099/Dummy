//program to have the header file for server class

#include <stdio.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h>

#define ADDRSERV "127.0.0.1"
#define MAX 256
#define PORT 8028
												//default destructor
		void load();	    										//load the data from the file

		int ToCreateSocket();											//create the socket
		int ToBindSocket();											//to bind the socket	
		int ToListen(); 											//to listen to the socket 
		
		void ToAcceptConnections();										//to accept connections
        void SToRecvData(char *);											//to receive data and commands from client
        void ToAuthenticateUser();										//to authenticate the user
		

		void AuthenticatedUserFunctionalities(char *,char * ,char * );			//to perform authenticated user functionalities
		void AnonymousFunctions(char *recvdata,char * filename);						//to perform anonymous user functionalities
		void AdminFunction(char *recvdata,char * filename);							//to perform admin user functionalities
		





