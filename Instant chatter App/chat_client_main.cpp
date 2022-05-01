#include "chat_client_header.h"
#include<iostream>//system call, data types
using namespace std;
#include<sys/socket.h>//socket
#include<unistd.h>//read & write
#include<arpa/inet.h>//ip
#include<stdio.h>//gets function
#include<signal.h>

#define MAX_LEN 1024
#define NUM_COLORS 6

#include <bits/stdc++.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <thread>
#include <mutex>


bool exit_flag=false;
thread t_send, t_recv;

int client_des;

string def_col="\033[0m";
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};

void catch_ctrl_c(int);
string color(int);
int eraseText(int);
void send_message(int);
void recv_message(int);




//initializing default port number
int port =8080;
char logged_name[MAX_LEN];
int main(int argc,char *argv[])
{
ofstream log;
log.open("client_logs",ios::app);
       
if(argc==2)
port=atoi(argv[1]);
	
	
	// socket_creation
	log<<"[+Info] socket is created..!"<<endl;
	client_des=socket(AF_INET,SOCK_STREAM,0); //-1
	if(client_des == -1)
	{
		perror("Socket creation error ");
		log<<"[+error] error in creating socket"<<endl;
		exit(1);
	}


	//sockaddr of sockaddr_in initialization
	log<<"[+Info] Sockaddr initialization..!"<<endl;
	struct sockaddr_in sock_addr_client;
	sock_addr_client.sin_family=AF_INET;//donmain ipv4
	sock_addr_client.sin_port=port;
	sock_addr_client.sin_addr.s_addr=inet_addr("127.0.0.1");

	 
	//connecting
	log<<"[+Info] Connecting to the server..!"<<endl;
	int srever_des=connect(client_des,(struct sockaddr *)&sock_addr_client,sizeof(sock_addr_client));
	if(srever_des == -1)
	{
		perror("Connecting error ");
		log<<"[+Info] Connection error to the server..!"<<endl;
		exit(1);
	}
	else
	{
	//handshaking with server
	log<<"[+Info] handshaking with the client is successful.."<<endl;
 	char msg[MAX_LEN];
 	bzero(msg,sizeof(msg));
 	read(client_des,&msg,sizeof(msg));
 	cout<<msg<<endl;
 	}
	
	
	//class object
 	newUser object;
 	
	cout<<"\nChoose from Menu"<<endl;
	cout<<"--------------------"<<endl;
	cout<<"1 : Existing User"<<endl;
	cout<<"2 : New User"<<endl;
	cout<<"0 : Exit"<<endl;
	int choice;
	cin>>choice;
	
	
	signal(SIGALRM,sig_handler);
	int cnt=3;
	while(1)
	{
		int k=0;
		switch(choice)
		{
			case 0 :	{
						char msg[]="e";
						log<<"[+Info] sending the exit message to server to end loop.."<<endl;
						write(client_des,&msg,sizeof(msg));
						exit(1);
					}
					
			case 1 :	{
						 object.existing_user(cnt);
			 			 string cred=object.existingUserCredInOneString();
			 			//cout<<cred<<endl;
			 			 int len1=cred.length();
						 char char_cred[len1];
						 strcpy(char_cred,cred.c_str());
						 write(client_des,&char_cred,sizeof(char_cred));
						//cout<<a<<endl;	
						 char ret;
						 read(client_des,&ret,sizeof(ret));
						 cout<<ret<<endl;
						 log<<"[+Info] getting the loggin in status from server.."<<endl;
						 if(ret=='1')
						 {						 	
						 	bzero(logged_name,sizeof(logged_name));
						 	read(client_des,&logged_name,sizeof(logged_name));
						 	cout<<"\nLogin Successful ! "<<logged_name<<"\n\n"<<endl;	 
			 				k=1;
						 }
						 else if(ret == '2')
						 {
						 	if(cnt>1)//3 2 1
						 	{
						 	cnt--;
						 	//cout<<"Only "<<cnt<<" Attempts Left !"<<endl;
						 	//cout<<"\nLogin Credentials are Invalid ! \nPlease Enter Correct Details"<<endl;
						 	choice=1;
						 	}
						 	else
						 	{
						 	cout<<"\n\nOOPS! Limits has been exeeded !"<<endl;
						 	cout<<"Please login after 2 minutes !"<<endl;
						 	
						        alarm(5);
						        pause();
						        cnt=3;
						 	choice=1;
						 	}
						 			
						 }
						 else
						 {
						 	cout<<"\nUser not found !\n"<<endl;
						 	choice=2;
					 	 }
				 	}
					break;
					
			case 2 :	{
						log<<"[+Info] Entering the new user into server file.."<<endl;
						object.add_new_User();
						string data=object.newUserDataInOneString();
						int len2=data.length();
						char char_data[len2];
						strcpy(char_data,data.c_str());
						write(client_des,&char_data,sizeof(char_data));
						
						char msg;
						read(client_des,&msg,sizeof(msg));
						log<<"[+Info] Registration successful status.."<<endl;
						if(msg == '3')
						{
					 		cout<<"\n\nRegistration Successful\n"<<endl;
					 		//k=1;
					 		
					 		cout<<"login with your user ID and Password"<<endl;
					 		
					 		while(1){
					 		cout<<"\nChoose from Menu"<<endl;
							cout<<"--------------------"<<endl;
							
							cout<<"1 : Login"<<endl;
							cout<<"0 : Exit"<<endl;

							cin>>choice;
							if(choice!=0 && choice!=1)
							cout<<"Invalid Input"<<endl;
							else
							break;
							}
					 	}
					}
					break;
					
			default :	{cout<<"\nInvalid Entry"<<endl;}
		}
		if(k==1)
		break;
 	}
 	
 	
	log<<"[+Info] checkin for signal (bye or abnormal temination).."<<endl;
        signal(SIGINT, catch_ctrl_c);
	
	
	send(client_des,logged_name,sizeof(logged_name),0);
	
	

	cout<<colors[NUM_COLORS-1]<<"\n\t         ================== Welcome to the chat "<<logged_name<<"  =================="<<endl<<def_col;
	
	
	log<<"[+Info] thread for sending message starts.."<<endl;
	thread t1(send_message, client_des);
	log<<"[+Info] thread for sending receiving starts.."<<endl;
	thread t2(recv_message, client_des);

	t_send=move(t1);
	t_recv=move(t2);

	if(t_send.joinable())
		t_send.join();
	if(t_recv.joinable())
		t_recv.join();
		
	
	
	int a =1;	

	while(a){
 	cout<<"\nChoose from Menu"<<endl;
	cout<<"--------------------"<<endl;
	cout<<"1 : Search chat History"<<endl;
	cout<<"2 : Log Out"<<endl;
	
	int ch;
	cin>>ch;

 	        
 	         switch(ch)
 	         {
 	        
 	          case 1:{
	 	                  string e_name,e_date,combine,data;
	 	          	  cout<<"Enter the name Of user you want to search chat with"<<endl<<"=>";
	 	          	  cin>>e_name;
				  cout<<"Enter the date in dd-mm-yyyy"<<endl<<"=>";
				  cin>>e_date;
				  
				  combine=string(logged_name)+"_"+e_name+"_"+e_date;
				  
				
				  ifstream fin;
				  fin.open(combine,ios::in);
				  
				  while(getline(fin,data))
					{
					cout<<data<<endl;
					}  
 	          	  }      
 	                break;
 	                
 	          case 2 : {
			      a=0;
			      break;			
 	         }
 	                
 	         default :	{cout<<"\nInvalid Entry"<<endl;}
 	         
 	         
 	         } //switch end
 	         
 	}//while end
		
		
		
		
		


	//socket close
	log<<"[+Info] closing the socket desciptor.."<<endl;
 	if(close(client_des) == -1)
	{
		perror("Socket Close error ");
		log<<"[+error] error closing the socket descriptor .."<<endl; 
		exit(1);
	}
	log.close();
  	
	return 0;
}








// Handler for "Ctrl + C"
void catch_ctrl_c(int signal) 
{
	char str[MAX_LEN]="bye";
	send(client_des,str,sizeof(str),0);
	exit_flag=true;
	t_send.detach();
	t_recv.detach();
	//close(client_des);
	exit(signal);
}

string color(int code)
{
	return colors[code%NUM_COLORS];
}

// Erase text from terminal
int eraseText(int cnt)
{
	char back_space=8;
	for(int i=0; i<cnt; i++)
	{
		cout<<back_space;
	}	
	return 0;
}

void send_message(int client_des)//client descriptor
{
cout<<colors[NUM_COLORS-2]<<"\n\t - Enter the first message as 'hi' 'user-name' to who you want to chat with -"<<endl<<def_col;
int count=0,flag=0;
	while(1)
	{
	
	       //cout<<"while"<<endl;
	        if(flag>0)
		cout<<colors[1]<<"You : "<<def_col;
		
		char str[MAX_LEN],temp[MAX_LEN];
		
		string s,s1;
		cin.getline(str,MAX_LEN);
		
		if(strlen(str)==0)
		{
		flag=1;
		continue;
		}
		count++;
		//cout<<"Count status :: "<<count<<endl;
		if(count==1){
		s=to_string(count);
		s1=(char*)str;
		s=s+s1;
		//cout<<"message is  :: "<<s<<endl;
		
		strcpy(temp,s.c_str());
		//cout<<"message is  :: "<<temp<<endl;
		send(client_des,temp,sizeof(temp),0);
		}
		else
		send(client_des,str,sizeof(str),0);
		
		if(strcmp(str,"bye")==0)
		{
			exit_flag=true;
			t_recv.detach();	
			close(client_des);
			return;
		}	
	
	}
}		


// Receive message
void recv_message(int client_des)
{
	while(1)
	{
		if(exit_flag)
			return;
		char name[MAX_LEN], str[MAX_LEN];
		int color_code;
				
		int bytes_received=recv(client_des,name,sizeof(name),0);
		
		//cout<<"NAME is :: "<<name<<endl;	
		if(bytes_received<=0)
			continue;
						
		recv(client_des,&color_code,sizeof(color_code),0);
		//cout<<"color code is :: "<<color_code<<endl;
		
		recv(client_des,str,sizeof(str),0);
		//cout<<"message is :: "<<str<<endl;			
		eraseText(6);
		if(strcmp(name,"#NULL")!=0)
			cout<<color(color_code)<<name<<" : "<<def_col<<str<<endl;
		else
			cout<<color(color_code)<<str<<endl;
		cout<<colors[1]<<"You : "<<def_col;
		fflush(stdout);
	}	
}
































