//program to define the class client


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <client.h>
#include <ctype.h>

#define ADDRSERV "127.0.0.1"
#define MAX 1024
#define PORT 8028

int sockfd;                                             
struct sockaddr_in servaddr;
int portnumber,slen,mlen,connectfd;
char ip_address[130];
      
void Client()
{
	connectfd = 0;
	mlen = 0 ;
	portnumber =0 ;
    memset(&servaddr,0,slen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ADDRSERV);
    servaddr.sin_port = htons(PORT);
    slen=sizeof(servaddr);
    sockfd=socket(AF_INET, SOCK_STREAM, 0);										
    if(sockfd < 0)
    {
	      printf("\nSocket can't be created\n");
          exit(EXIT_FAILURE);
    }
}


//function to get socket id
int ToGetSockfd()
{
	return sockfd ;
}


//function to get credentials from the User
void ToGetCredentials(char * credentials)
{
      //char *res;
      //res=(char *)malloc(101);
      char uName[50]="" ;
      char password[50]="";
      printf("Enter the Username :") ;
      fgets(uName,50,stdin);
      if(strcmp(uName,"\n")==0)
      {
      	strcpy(uName,"");
      }
      else
      {
        uName[strlen(uName)-1]='\0';  
      }
      printf("Enter the password :") ;
      fgets(password,50,stdin);
      if(strcmp(password,"\n")==0)
      {
	      strcpy(password,"");
      }
      else
      {
        password[strlen(password)-1]='\0';  
      }
      strcpy(credentials,uName);
      strcat(credentials,":");
      strcat(credentials,password);
}


//function to connect to the server
int ToServerConnect()
{
	connectfd=connect(sockfd,(struct sockaddr *)&servaddr,slen);
    if(connectfd < 0)
    {
    	printf("Socket can't be created\n");
        exit(EXIT_FAILURE);
    }
    return connectfd;
}


//function to send data from the client
int ToSendData(char * str)
{
	char tem[200]="";
	strcpy(tem,str);
    if(strcmp(tem,"bye")==0 || strcmp(tem,"Bye")==0)									//checks for bye if bye close client conncetion
    {												
      	ToCloseClientConnections();
    }
    send(sockfd,tem,strlen(tem),0);
	return 0 ;
}


void CToRecvData(char * recvdata)
{
        char temp[200]="";
        //mlen=recv(connectfd ,temp,sizeof(temp), 0);
        mlen=recv(sockfd ,temp,sizeof(temp), 0);
        strcpy(recvdata,temp);
        if(mlen==0)
        {                  
                ToCloseClientConnections();
        }
}


//function to get the subcommands from the use
void ToGetUserCommands(char * type,char * commands)
{
	while(1)
	{
      	    char subcommand[200]="";
            printf("\nEnter the subcommand :") ;
            fgets(subcommand,200,stdin);
            subcommand[strlen(subcommand)-1]='\0';
            char copysubcommand[200]="";
            strcpy(copysubcommand,subcommand);
            char temp[200]="";
            strcpy(temp,type);
	    	if(strcmp(temp,"authenticated user")==0)										   //check for user type
	    	{			
            	  char comm[200]="";
            	  char input1[200]="",input2[200]="";
            	  char * tok;
            	  tok=strtok(subcommand," ");
            	  strcpy(comm,tok);
            	  tok=strtok(NULL,",");
            	  if(tok!=NULL)
            	  strcpy(input1,tok);
            	  tok=strtok(NULL,",");
            	  if(tok!=NULL)
            	  strcpy(input2,tok);
                  if(strcmp(comm,"ADD")==0)
                  {											
                  	if(strcmp(input1,"")!=0 && strcmp(input2,"")!=0)
                  	{
                        	strcpy(commands,copysubcommand);
                        	break;
                    }
                    else
                    {
                    	printf("ADD command should have two fields\n");
                        continue;
					}
                 }
                 else if(strcmp(comm,"rm")==0)													//checks for remove command
                 {																		
                   	if(strcmp(input1,"")!=0 && strcmp(input2,"")==0)
                   	{
                        	strcpy(commands,copysubcommand);
                        	break;
                    }
                    else
                    {
                        printf("Remove command should have only one field\n"); 
                        continue;
                    }
                 }
                 else if(strcmp(comm,"list")==0)												//checks for list command
                 {											
                   	if(strcmp(input1,"")!=0 && strcmp(input2,"")==0)
                   	{
                   		strcpy(commands,copysubcommand);
                        break;
                    }
                    else
                    {
                    	printf("List command should have one field\n");
                        continue;
                    }
                }
                else if(strcmp(comm,"chgrp")==0)												//checks for the chgrp command
                {											
                   	if(strcmp(input1,"")!=0 && strcmp(input2,"")==0)
                   	{
                   		strcpy(commands,copysubcommand);
                        break;
                   	}
                   	else
                   	{
                   		printf("Enter only Groupname\n");
                   		continue;
                    }
                }
                else if(strcmp(comm,"list")==0)													//checks for the list command
                {											
                   	if(strcmp(input1,"")!=0 && strcmp(input2,"")==0)
                   	{
                   		strcpy(commands,copysubcommand);
                        break;
                   	}
                   	else
                   	{
                   		printf("Enter only pattern\n");
                   		continue;
                    }
                }
                else if(strcmp(comm,"bye")==0 || strcmp(comm,"Bye")==0)
                {	
                   	if(strcmp(input1,"")==0 && strcmp(input2,"")==0)
                   	{
                   		strcpy(commands,copysubcommand);
                        break;
                    }
                    else
                    {
                    	printf("No inputs with bye/Bye\n");
                        continue;
                    }
                }
                else
                {
                	printf("Authenticated users can only use ADD,rm,list,chgrp and bye/Bye subcommands\n");
                	continue;
                }
	      }
	      
	      if(strcmp(temp,"admin")==0)						//checks for the type admin
	      {
            	  char comm[200]="";
            	  char *input1="",*input2="",*input3="";
            	  char * tok;
            	  tok=strtok(subcommand," ");
            	  strcpy(comm,tok);
            	  input1=strtok(NULL,",");
            	  input2=strtok(NULL,",");
            	  input3=strtok(NULL,",");
                  if(strcmp(comm,"addgrp")==0)				//checks for the add group command
                  {											
            	      if(input1!=NULL&&input2==NULL)
            	      {
                  	      strcpy(commands,copysubcommand);
                          break;
                      }
                      else
                      {
                          printf("Enter group name only\n");
                          continue;
                      }
                   }
                   else if(strcmp(comm,"rmgrp")==0)			//checks for the remove group command
                   {											
                   		if(input1!=NULL && input2==NULL)
                   		{
                        	strcpy(commands,copysubcommand);
                        	break;
                        }
                        else
                        {
                        	printf("Enter groupname only\n");
							continue;
                        }
                  }
                    else if(strcmp(comm,"ADD")==0)			//checks for the ADD command
                    {											
                  		if(input1!=NULL && input2!=NULL && input3!=NULL)
                  		{
                        	strcpy(commands,copysubcommand);
                        	break;
                        }
                        else
                        {
                        	printf("Provide three inputs name,phonenumber,filename\n");
                            continue;
                        }
                    }
                   else if(strcmp(comm,"rm")==0)			//checks for the rm command
                    {											
                  		if(input1!=NULL && input2!=NULL && input3==NULL)
                  		{
                        	strcpy(commands,copysubcommand);
                        	break;
                        }
                        else
                        {
                        	printf("Provide two inputs name,filename\n");
                            continue;
                        }
                    }
                    
                    else if(strcmp(comm,"list")==0)			//checks for the rm command
                    {											
                  		if(input1!=NULL && input2!=NULL && input3==NULL)
                  		{
                        	strcpy(commands,copysubcommand);
                        	break;
                        }
                        else
                        {
                        	printf("Provide two inputs name pattern,filename\n");
                            continue;
                        }
                    }
                    
                    else if(strcmp(comm,"bye")==0 || strcmp(comm,"Bye")==0) 	//checks for bye/Bye command 
                	{									
		               	if(input1==NULL && input2==NULL)
		               	{
		               		strcpy(commands,copysubcommand);
                        	break;
		                }
		                else
		                {
		                	printf("No inputs with bye/Bye\n");
		                    continue;
		                }
		            }
                    else
                    {
                     	printf("Admin can give addgrp,rmgrp,list,rm or ADD  subcommand only\n");
                        continue;
                    }
               }
               
			   if(strcmp(temp,"anonymous user")==0)					//checks for anonymous user
			   {											
		              char comm[200]="";
		        	  char *input1="",*input2="";
		        	  char * tok="";
		        	  tok=strtok(subcommand," ");
		        	  strcpy(comm,tok);
		        	  input1=strtok(NULL,",");
		        	  input2=strtok(NULL,",");
		              if(strcmp(comm,"ADD")==0)
		              {											
				          	if(input1!=NULL && input2!=NULL)
				          	{
				                strcpy(commands,copysubcommand);
                        		break;
				            }
				            else
				            {
				            	printf("ADD command should have two fields\n");
				                continue;
							}
		              }
		              else if(strcmp(comm,"bye")==0 || strcmp(comm,"Bye")==0)
		              {									 
				           	if(input1==NULL && input2==NULL)
				           	{
				           		strcpy(commands,copysubcommand);
                        		break;
				            }
				            else
				            {
				            	printf("No inputs with bye/Bye\n");
				                continue;
				            }
				      }
		              else
		              {
		               	printf("Anonymous user should provide only ADD subcommand\n") ;
		                 continue;
		              }
		       }
	}
}



//function to display data received from server
void ToDisplayRecvData(char * recvData)
{
	printf("%s\n",recvData);
}



//function to close connection from the server
int ToCloseClientConnections()
{
      close(sockfd);
      exit(0);
}



