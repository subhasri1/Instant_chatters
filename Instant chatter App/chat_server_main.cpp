#include "chat_server_header.h"
#include<iostream>//system call, data types
using namespace std;
#include<sys/socket.h>//socket
#include<unistd.h>//read & write
#include<arpa/inet.h>//ip
#include<fstream>//filehandling
#include<vector>//map STL
//#include<cstring>//strcpy
#include<stdlib.h>//atoi
#include<fcntl.h>
#include<sys/stat.h>

#define MAX_LEN 1024

#include <bits/stdc++.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <thread>
#include <mutex>

#define NUM_COLORS 6



//structure to store clients details along with its thread
struct terminal
{
	int id;
	string name;
	int socket;
	thread th;
};


static  int l=0;//static variable

string name_login_user;
string file_name="";
string file_name2="";
ofstream fout;//object of ofstream class to write on the file
ofstream fout2;

vector<terminal> clients;

string def_col="\033[0m";//for heading

string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m","\033[36m"};
int seed=0;
mutex cout_mtx,clients_mtx;


string color(int code);
void set_name(int id, char name[]);//for givin the client name
void shared_print(string str, bool endLine);//for proper sync of cout statements
int broadcast_message(string message, int sender_id);//sending name and message to all the user except client
int broadcast_message(int num, int sender_id);//sending id to all user except to ownself
void end_connection(int id);//for endiing the thread

void handle_client(int cli_des, int id);//thread handler for handling client
int broadcast_message_online(string person_to_talk, int sender_id, string name);//sending name to the person who the user choose to talk
int broadcast_message_online(int num, int sender_id, string name);//sending ID to person who the user choose to talk with
int broadcast(string  person_to_talk,int id,string message);//sending message to person who the user choose to talk to

//initialize the default port
int port=8080;
int main(int argc,char *argv[])
{

ofstream log;
log.open("server_logs",ios::app);

if(argc==2)
port=atoi(argv[1]);
	
	if(argc == 2)
	{
		port = atoi(argv[1]);
	}
	// socket_creation
	log<<"[+Info] socket is created..!"<<endl;
	int sock_des=socket(AF_INET,SOCK_STREAM,0); //-1
	if(sock_des == -1)
	{
		perror("Socket creation error ");
		log<<"[+error] error in creating socket"<<endl;
		exit(1);
	}
	
	
	//sockaddr of sockaddr_in initialization
	log<<"[+Info] Sockaddr initialization..!"<<endl;
	struct sockaddr_in sock_addr_server;
	sock_addr_server.sin_family=AF_INET;//donmain ipv4
	sock_addr_server.sin_port=port;
	sock_addr_server.sin_addr.s_addr=inet_addr("127.0.0.1");
	 
	 
	//binding
	log<<"[+Info] Binding.."<<endl;
	int b = bind(sock_des,(struct sockaddr *)&sock_addr_server,sizeof(sock_addr_server));
	if(b == -1)
	{
		perror("Binding error ");
		log<<"[+error] error in Binding socket"<<endl;
		exit(1);
	}
	 
	 
	//listening
	log<<"[+Info] Listening.."<<endl;
	int l = listen(sock_des,10);
	if(l == -1)
	{
		perror("Listening error ");
		log<<"[+error] error in Listening to socket"<<endl;
		exit(1);
	}
	

	vector<user_details> userCred;
	user_cred_file_to_vector(userCred);
	
	
	cout<<"Server are listening on port Number "<<sock_addr_server.sin_port<<endl;
	log<<"[+Info] Server are listening on port Number.."<<sock_addr_server.sin_port<<endl;
	
	while(1)
	{
	 		 	
	 	//acceptance
	 	log<<"[+Info] Accepting the client.."<<endl;
	 	struct sockaddr_in sock_addr_client;
		socklen_t len =sizeof(sock_addr_client);
	 	int cli_des=accept(sock_des,(struct sockaddr *)&sock_addr_client,&len);
	 	if(cli_des == -1)
		{
			perror("Clint acceptance error ");
			log<<"[+error] error accepting the client"<<endl;
			exit(1);
		}
		else
		{
	 	//handshaking with client
	 	char msg[]="Successful Connection established from the Server";
	 	log<<"[+Info] handshaking with the client is successful.."<<endl;
	 	write(cli_des,&msg,sizeof(msg));
	 	}
	 	
	 	 login_and_registration_of_user(cli_des,userCred,name_login_user);
	 	 char name_to_send[MAX_LEN];
	 	 log<<"[+Info] Sending the online name status to client.."<<endl;
	 	 strcpy(name_to_send,name_login_user.c_str());
	 	 write(cli_des,&name_to_send,sizeof(name_to_send));
	 	
	 	
	 	
	 	
	 	
	 	
	 	seed++;
		thread t(handle_client,cli_des,seed);//call to thread function once the client connection is established
		lock_guard<mutex> guard(clients_mtx);
		clients.push_back({seed, string("Client Without Name"),cli_des,(move(t))});
	 	
	}
	
       for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].th.joinable())
			clients[i].th.join();
	}
	
	 	
	//CLOSING the socket 1
	log<<"[+Info] closing the server descriptor"<<endl;
	if(close(sock_des) == -1)
	{
		perror("Client acceptance error ");
		log<<"[+error] error in closing the file descriptor"<<endl;
		exit(1);
	}
  
	return 0;
}



//for getting the current date 
string time_today()
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
  string str(buffer);
 
  string r = str.substr(0, 10);
 return r;
}


//for setting the colors in colors array
string color(int code)
{
	return colors[code%NUM_COLORS];
}



//Set name of client
void set_name(int id, char name[])
{
	for(int i=0; i<clients.size(); i++)
	{
			if(clients[i].id==id)	
			{
				clients[i].name=string(name);
			}
	}	
}



// For synchronisation of cout statements
void shared_print(string str, bool endLine=true)
{	
	lock_guard<mutex> guard(cout_mtx);
	cout<<str;
	if(endLine)
			cout<<endl;
}



//Broadcast message to all clients except the sender
int broadcast_message(string message, int sender_id)
{
	char temp[MAX_LEN];
	strcpy(temp,message.c_str());
	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].id!=sender_id)
		{
			send(clients[i].socket,temp,sizeof(temp),0);
		}
	}	
        return 0;			
}





// Broadcast a number to all clients except the sender
int broadcast_message(int num, int sender_id)
{
//cout<<"inside secoond looooop"<<endl;
	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].id!=sender_id)
		{
			send(clients[i].socket,&num,sizeof(num),0);
		}
	}
	return 0;		
}



//end the connection of the client who leaves
void end_connection(int id)
{
	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].id==id)	
		{
			lock_guard<mutex> guard(clients_mtx);
			clients[i].th.detach();
			clients.erase(clients.begin()+i);
			close(clients[i].socket);
			break;
		}
	}				
}






//Broadcast message to user that the client select///hi aman
int broadcast_message_online(string person_to_talk, int sender_id, string name)//aman 2 rahul
{
	char temp[MAX_LEN];	
	strcpy(temp,name.c_str());

	for(int i=0; i<clients.size(); i++)
	{
	
		if(clients[i].id!=sender_id && clients[i].name==string(person_to_talk))
		{
		
			send(clients[i].socket,temp,sizeof(temp),0);
		}
	}
	return 0;		
}




// Broadcast a number to the client except the sender
int broadcast_message_online(int num,int sender_id, string name)
{

	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].id!=sender_id && clients[i].name==string(name))
		{
			send(clients[i].socket,&num,sizeof(num),0);
		}
	}
	return 0;		
}



int broadcast(string person_to_talk,int sender_id,string message ){
//cout<<person_to_talk<<sender_id<<message<<endl;
//cout<<"inside send3"<<endl;
char temp[MAX_LEN];

        strcpy(temp,message.c_str());
	for(int i=0; i<clients.size(); i++)
	{
	
		if(clients[i].id!=sender_id && clients[i].name==string(person_to_talk))
		{
			send(clients[i].socket,temp,sizeof(temp),0);
		}
	}
	return 0;	
}


//thread handler function
void handle_client(int cli_des, int id)
{
	char name[MAX_LEN],str[MAX_LEN];
	
	string person_to_talk="";
        recv(cli_des,name,sizeof(name),0);
	set_name(id,name);
	
	cout<<"Total Online Users"<<endl;
	for(int i=0; i<clients.size(); i++)
	{
			if(i>0)	
				cout<<","<<clients[i].name;
			else
			{
			       cout<<clients[i].name;
			}
	}
	cout<<endl;
	
	// Display welcome message
	string welcome_message=string(name)+string(" has joined");
	broadcast_message("#NULL",id);	
	broadcast_message(id,id);								
	broadcast_message(welcome_message,id);	
	shared_print(color(id)+welcome_message+def_col);
	
	
	
	while(1)
	{	       
		int bytes_received=recv(cli_des,str,sizeof(str),0);
		cout<<str<<endl;
		l++;
		if(bytes_received<=0)
			return;
		if(strcmp(str,"bye")==0)
		{		
			// Display leaving message
			string message=string(name)+string(" has left");		
			broadcast_message("#NULL",id);			
			broadcast_message(id,id);						
			broadcast_message(message,id);
			shared_print(color(id)+message+def_col);
			end_connection(id);
			fout.close();
			fout2.close();
			l=0;										
			return;
		}
		
		//if the message starts with 1(denotes its the first message)
		if(str[0]=='1')
		{
		int cnt=0;
		for(int i=0;i<sizeof(str);i++)
		{
			if(str[i]==' ' || str[i]=='\0')
			cnt++;
			else if(cnt==1)
			person_to_talk+=str[i];		
		}
	

               if(l==1){	
                string current_date=time_today();   //getting the date from time_today()     
                file_name=string(name)+"_"+person_to_talk+"_"+current_date;//appending to make file name
                file_name2=person_to_talk+"_"+string(name)+"_"+current_date;
                fout.open(file_name,ios::app);//opening the file with appended name 
                fout2.open(file_name2,ios::app);
                }
                          
                char* substr = str + 1;//removing 1 from first message
                
                fout<<name<<" : "<<substr<<endl;
          	 fout2<<name<<" : "<<substr<<endl; 
                
                //sending message to the person to talk.   
                broadcast_message_online(string(person_to_talk),id,string(name));					
		broadcast_message_online(id,id,string(person_to_talk));		
		broadcast(string(person_to_talk),id,string(substr));
		shared_print(color(id)+name+" : "+def_col+substr);
		
		}
		
		else{	
		
               fout<<name<<" : "<<str<<endl;
		fout2<<name<<" : "<<str<<endl;
		broadcast_message_online(string(person_to_talk),id,string(name));					
		broadcast_message_online(id,id,string(person_to_talk));		
		broadcast(string(person_to_talk),id,string(str));	
		shared_print(color(id)+name+" : "+def_col+str);
		}
			
	}//while loop ends here	
}//thread handler ends here

