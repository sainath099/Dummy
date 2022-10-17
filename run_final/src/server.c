/*  program to define the server   */

#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include "server.h"
#include "user.h"

char SAUTH[500]="/home/kali/Desktop/run_final/db/authentication.txt";

int sockfd;
struct sockaddr_in servaddr ;
struct sockaddr_in cliaddr ;
int slen = 0 ,connectfd = 0, mlen = 0 ;
pid_t pid;
int opt=1;

void load()
{
	FILE *fp;
    fp=fopen(SAUTH,"r");
	if(fp==NULL)
	{
		printf("\nauthentication file doesn't exist\n");
		exit(0);
	}
	fclose(fp);
}

//function to create socket 

int ToCreateSocket()
{
            slen=sizeof(struct sockaddr_in);
            memset(&servaddr,0,slen);
            memset(&cliaddr,0,slen);
            servaddr.sin_family = AF_INET; 										//To use Internet addressing family
            servaddr.sin_addr.s_addr = inet_addr(ADDRSERV);
            servaddr.sin_port = htons(PORT);
            sockfd=socket(AF_INET, SOCK_STREAM, 0); 							//creating the socket
            if (setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt))) 
			{
				perror("setsockopt");
				exit(EXIT_FAILURE);
			}                                                                    
            if(sockfd < 0)
            {
              printf("\nSocket creation has not been done\n");
            }
            return 0;
}



//function to bind the socket
int ToBindSocket()
{
            int ret;
            ret=bind(sockfd, (struct sockaddr *)&servaddr, slen);                //binding the socket
            if(ret < 0)
            {
                    printf("\nBinding has not been done\n");
            }
            return 0;
}



//function to call listening to the port 8028
int ToListen()
{
            int ret;
            ret=listen(sockfd, 5);                                              //listening to the port 8028
            if(ret< 0)
            {
                    printf("\nSocket is not Listening\n");
            }
            return 0;
}



//function to accept connection and add concurrency by using child process
void ToAcceptConnections()
{
	while(1)
	{
		connectfd=accept(sockfd,(struct sockaddr*)&servaddr,(socklen_t *)&slen);
		if(connectfd < 0)
		{
			printf("Conncetion is not established\n");
		}
		else
		{
			printf("Got a connection\n");
		}
		pid = fork();                                                 //creating the child process   
	    if (pid == 0)
	    {
	    	char filename[200]="" ;
	    	char type[200]="";
	    	ToAuthenticateUser(type);                                						//calling the authenticate function to check the type of user
			char type1[200]="";        
			if( strcmp("admin",type)!=0 && strcmp(type,"")!=0)
			{
				strcpy(type1,"authenticated user") ;
			}
			else if(strcmp(type,"")==0)
			{
				strcpy(type1,"anonymous user") ;
			}
			else if(strcmp(type,"admin")==0)
			{
				strcpy(type1,"admin") ;
			}
			
			mlen=send(connectfd,type1,strlen(type1),0);              						//sending the type of user to the client
            
            while(1)
            {
            	char recvdata[1024] ="";
            	SToRecvData(recvdata);                                                   	//calling the function to receive data
				if(strcmp(type1,"authenticated user")==0)									//checking the user is authenticated
				{
					AuthenticatedUserFunctionalities(recvdata,filename,type);				//calling the funtion to perform authenticated user functionalities
                }
                if(strcmp(type1,"anonymous user")==0)										//checking whether the user is anonymous
                {                                                            				
                	AnonymousFunctions(recvdata,filename);									//calling the function to perform anonymous user functionalities
                }
                if(strcmp(type1,"admin")==0)
                {														 					//checking whether the user is admin
                	AdminFunction(recvdata,filename);										//calling the function to perform admin functionalities
                }
            }
		}
        signal(SIGCHLD,SIG_IGN);                                                            //signal to avoid Zombie process
	    close(connectfd);
	}	
}


void AuthenticatedUserFunctionalities(char *recvdata,char *filename ,char * type)
{
        char comm[200]="";
        strcpy(comm,recvdata);
		char input1[200]="",input2[200]="";
		char * tok="";
		tok=strtok(comm," ");
		strcpy(comm,tok);
		tok = strtok(NULL,",");
		if(tok!=NULL)
		{
			strcpy(input1,tok);
			tok = strtok(NULL,",");
			if(tok!=NULL)
			strcpy(input2,tok);
		}
		
		if(strcmp(comm,"ADD")==0)
		{
			int res=AuthenticatedAddData(input1,input2,filename);							//calling the athenticated adddata function of user 
			if(res==1)
			{
				char positive[200]="Contact added" ;
				send(connectfd,positive,strlen(positive),0);     							//sending the message to the client
		    }
			else
			{
				char negative[200]="Authenticated user can't add the data to the public group";
				send(connectfd,negative,strlen(negative),0);
			}
		}
		
		else if(strcmp(comm,"chgrp")==0)													//checking if the command is change group
		{
			
			ToChgrp(input1,type);
			strcpy(filename,input1);
			if(strcmp(filename,"no")==0)
			{
				char neg[200]="The user doesnt belong to the group";
				send(connectfd,neg,strlen(neg),0);
			}
			else
			{
				char pos[200]="Group changed" ;
				send(connectfd,pos,strlen(pos),0);
		    }
		}
		
		else if(strcmp(comm,"rm")==0)														//checking if the command is remove contact
		{
		     int res=ToRemoveContact(input1,filename); 										//calling the user function to remove the contact
		     if(res == 1)
		     {
		  	      char pos[200] = "Contact removed " ;
		          send(connectfd,pos,strlen(pos),0);
		     }
		     else
		     {
		          char neg[200]="Entered contact name is not present";
		          send(connectfd,neg,strlen(neg),0);
		     }
		}
		
		else if(strcmp(comm,"list")==0)														//checking if the command is list
		{														
		     char concat[1024]="";
		     int res=Tolistcontact(input1,filename,concat); 								//calling the user function to list the contacts
		     if(res == 1)
		     {
		          send(connectfd,concat,strlen(concat),0);
		     }
		     else
		     {
		          char neg[200]="No contacts with given pattern";
		          send(connectfd,neg,strlen(neg),0);
		     }
		}
}





//function to implement anonymous user functionalities

void AnonymousFunctions(char * recvdata ,char * filename)
{
      char command[200]="";
      char input1[200]="",input2[200]="";
      char subcommand[256]="";
      strcpy(subcommand,recvdata);
      char * tok=strtok(subcommand," ");
      strcpy(command,tok);
      tok=strtok(NULL,",");
      strcpy(input1,tok);
      tok=strtok(NULL,",");
      strcpy(input2,tok);
      if(strcmp(command,"ADD")==0)
      {                                                          
         strcpy(filename,"");
         int res=ToAddData(input1,input2,filename);
         if(res == 1)
         {
      	   char positive[200] = "Contact added" ;
           send(connectfd,positive,strlen(positive),0);
         }
         else
         {
         	char negative[200]="Contact is not added" ;
            send(connectfd,negative,strlen(negative),0);
         }
     }
}


//function to implement admin functionalities
void AdminFunction(char * recvdata,char * filename)
{
      char command[100]="";
      char input1[100]="",input2[100]="",input3[100]="";
      char subcommand[500]="";
      strcpy(subcommand,recvdata);
      char * tok="";
	  tok=strtok(subcommand," ");
	  strcpy(command,tok);
	  tok = strtok(NULL,",");
	  if(tok!=NULL)
	  {
			strcpy(input1,tok);
			tok = strtok(NULL,",");
			if(tok!=NULL)
			strcpy(input2,tok);
			tok = strtok(NULL,",");
			if(tok!=NULL)
			strcpy(input3,tok);
	  }
      if(strcmp(command,"addgrp")==0)
      {
            strcpy(filename,input1);
            int res = ToAddGrp(filename);
            if(res==1)
            {
             	char positive[50] = "Group added " ;
                send(connectfd,positive,strlen(positive),0);
            }
            else
            {
            	char negative[50]= "Error while adding the group " ;
            	send(connectfd,negative,strlen(negative),0);
            }
      }
      else if(strcmp(command,"rmgrp")==0)
      {
		  	strcpy(filename,input1);
		  	int res = ToRemoveGrp(filename);							
		  	
			if(res==1)
			{
		  		char positive[50]= "Group removed " ;
		  	    send(connectfd,positive,strlen(positive),0);
		  	}
		  	else
		  	{
		  		char negative[50]="No group is present with that name" ;
		  	    send(connectfd,negative,strlen(negative),0);
		  	}
      }
      else if(strcmp(command,"ADD")==0)
      {
            strcpy(filename,input3);
            int res = ToAddData(input1,input2,filename);
            if(res==1)
            {
                  char positive[50]="contact added to the given group";	
                  send(connectfd,positive,strlen(positive),0);
            }
            else
            {
            	  char negative[100]="Group doesn't exist ,Add the group to add the data" ;
                  send(connectfd,negative,strlen(negative),0);
            }
      }
      else if(strcmp(command,"rm")==0)
      {
            strcpy(filename,input2);
            int res = AdminRemoveContact(input1,filename);
            if(res==1)
            {
                  char positive[50]="contact removed from the given group";	
                  send(connectfd,positive,strlen(positive),0);
            }
            else if(res==2)
            {
            	  char negative[100]="Group doesn't exist" ;
                  send(connectfd,negative,strlen(negative),0);
            }
            else
            {
            	  char negative[100]="No contacts are present with given name";
            	  send(connectfd,negative,strlen(negative),0);
            }
      }
      else if(strcmp(command,"list")==0)														//checking if the command is list
		{														
		     char concat[1024]="";
		     strcpy(filename,input2);
		     int res=Adminlistcontact(input1,filename,concat); 								//calling the user function to list the contacts
		     if(res == 1)
		     {
		          send(connectfd,concat,strlen(concat),0);
		     }
		     else if(res ==2)
		     {
		     	char neg[200]="Group doesn't exist";
		         send(connectfd,neg,strlen(neg),0);
		     }
		     else
		     {
		          char neg[200]="No contacts with given name pattern";
		          send(connectfd,neg,strlen(neg),0);
		     }
		}
}


//function to receive data
void SToRecvData(char * recvdata)
{
        char temp[200]="";
        mlen=recv(connectfd ,temp,sizeof(temp), 0);
        //mlen=recv(sockfd ,temp,sizeof(temp), 0);
        strcpy(recvdata,temp);
        if(strcmp(temp,"bye")==0 || strcmp(temp,"Bye")==0)
        {                  
                close(connectfd);
                exit(0);
        }
}


//function to authenticate user
void ToAuthenticateUser(char * type)
{
        char receivedata[1024];
        memset(receivedata,0,1024);
        recv(connectfd ,receivedata,1024 , 0);
        if(strcmp(receivedata,":")==0)
        {
        	strcpy(type ,"");
        }
        else
        {
		    char username[50]="",password[50]="" ;
		    char recvdata[1024];
		    memset(recvdata,0,1024);
		    strcpy(recvdata,receivedata);
			strcpy(username,strtok(recvdata,":"));
			strcpy(password,strtok(NULL,":"));
		    FILE *fp;
			char * line;
			int size = 1024;
			line = (char *)malloc(1024);
			fp=fopen("/home/kali/Desktop/remote_phonebook/db/authentication.txt","r");
			if(fp!=NULL)
			{
		     	while(fgets(line,size,fp))
		     	{
		     		char t[1024]="";
		     		strcpy(t,line);
		     		t[strlen(t)-1]='\0';
					char u[50]="" ,p[50]="" ;
					strcpy(u,strtok(t,":"));
					if(strcmp(u,username)==0)
					{
						strcpy(p,strtok(NULL,":"));
						if(strcmp(p,password)==0)
						{
							strcpy(type,u);
							break;
						}
					}	
				}
			} 
			fclose(fp);
			free(line);
    	}
}
