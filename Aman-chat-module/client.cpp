#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <signal.h>
#include <mutex>

#define MAX_LEN 200
#define NUM_COLORS 6

using namespace std;


bool exit_flag=false;
thread t_send, t_recv;

int client_socket;

string def_col="\033[0m";
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};



void catch_ctrl_c(int signal);
string color(int code);
int eraseText(int cnt);
void send_message(int client_socket);
void recv_message(int client_socket);


int port=8080;
int main(int argc ,char * argv[])
{

if(argc==2)
port=atoi(argv[1]);
	if((client_socket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_port=htons(port); // Port no. of server
	client.sin_addr.s_addr=INADDR_ANY;
	//client.sin_addr.s_addr=inet_addr("127.0.0.1"); // Provide IP address of server
	bzero(&client.sin_zero,0);

	if((connect(client_socket,(struct sockaddr *)&client,sizeof(struct sockaddr_in)))==-1)
	{
		perror("connect: ");
		exit(-1);
	}
	
	signal(SIGINT, catch_ctrl_c);
	
	char name[MAX_LEN];

	cout<<"Enter your name : ";
	cin.getline(name,MAX_LEN);
	send(client_socket,name,sizeof(name),0);
	
	

	cout<<colors[NUM_COLORS-1]<<"\n\t         ================== Welcome to the chat "<<name<<"  =================="<<endl<<def_col;

	thread t1(send_message, client_socket);
	thread t2(recv_message, client_socket);

	t_send=move(t1);
	t_recv=move(t2);

	if(t_send.joinable())
		t_send.join();
	if(t_recv.joinable())
		t_recv.join();
			
	return 0;
}//end main






// Handler for "Ctrl + C"
void catch_ctrl_c(int signal) 
{
	char str[MAX_LEN]="bye";
	send(client_socket,str,sizeof(str),0);
	exit_flag=true;
	t_send.detach();
	t_recv.detach();
	close(client_socket);
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

void send_message(int client_socket)//client descriptor
{
cout<<colors[NUM_COLORS-2]<<"\n\t - Enter the first message as 'hi' 'user-name' to who you want to chat with -"<<endl<<def_col;
int count=0;
	while(1)
	{
		cout<<colors[1]<<"You : "<<def_col;
		char str[MAX_LEN],temp[MAX_LEN];
		string s,s1;
		cin.getline(str,MAX_LEN);
		count++;
		if(count==1){
		s=to_string(count);
		s1=(char*)str;
		s=s+s1;
		
		strcpy(temp,s.c_str());
		send(client_socket,temp,sizeof(temp),0);
		}
		else
		send(client_socket,str,sizeof(str),0);
		
		if(strcmp(str,"bye")==0)
		{
			exit_flag=true;
			t_recv.detach();	
			close(client_socket);
			return;
		}	
	}		
}

// Receive message
void recv_message(int client_socket)
{
	while(1)
	{
		if(exit_flag)
			return;
		char name[MAX_LEN], str[MAX_LEN];
		int color_code;
				
		int bytes_received=recv(client_socket,name,sizeof(name),0);
		cout<<"NAME is :: "<<name<<endl;		
		if(bytes_received<=0)
			continue;
						
		recv(client_socket,&color_code,sizeof(color_code),0);
		cout<<"color code is :: "<<color_code<<endl;
		
		recv(client_socket,str,sizeof(str),0);
		cout<<"message is :: "<<str<<endl;			
		eraseText(6);
		if(strcmp(name,"#NULL")!=0)
			cout<<color(color_code)<<name<<" : "<<def_col<<str<<endl;
		else
			cout<<color(color_code)<<str<<endl;
		cout<<colors[1]<<"You : "<<def_col;
		fflush(stdout);
	}	
}



                
