#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#define MAX_LEN 200
#define NUM_COLORS 6

using namespace std;

//structure to store clients details along with its thread
struct terminal
{
	int id;
	string name;
	int socket;
	thread th;
};


static  int l=0;//static variable


string file_name="";
ofstream fout;//object of ofstream class to write on the file


vector<terminal> clients;

string def_col="\033[0m";//for heading

string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m","\033[36m"};
int seed=0;
mutex cout_mtx,clients_mtx;


string color(int code);
void set_name(int id, char name[]);//for givin the client name
void shared_print(string str, bool endLine);
int broadcast_message(string message, int sender_id);
int broadcast_message(int num, int sender_id);
void end_connection(int id);
void handle_client(int client_socket, int id);
int broadcast_message_online(string person_to_talk, int sender_id, string name);
int broadcast_message_online(int num, int sender_id, string name);
int broadcast(string  person_to_talk,int id,string message);



//main function for server starts 
int port=8080;
int main(int argc ,char * argv[])
{


if(argc==2)
port=atoi(argv[1]);

	int server_socket;
	if((server_socket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(port);
	server.sin_addr.s_addr=INADDR_ANY;
	bzero(&server.sin_zero,0);

	if((bind(server_socket,(struct sockaddr *)&server,sizeof(struct sockaddr_in)))==-1)
	{
		perror("bind error: ");
		exit(-1);
	}

	if((listen(server_socket,8))==-1)
	{
		perror("listen error: ");
		exit(-1);
	}

	struct sockaddr_in client;
	int client_socket;
	unsigned int len=sizeof(sockaddr_in);

	cout<<colors[NUM_COLORS-1]<<"\n\t  ====== All the Online users ======   "<<endl<<def_col;

	while(1)
	{
		if((client_socket=accept(server_socket,(struct sockaddr *)&client,&len))==-1)
		{
			perror("accept error: ");
			exit(-1);
		}
		seed++;
		thread t(handle_client,client_socket,seed);//call to thread function once the client connection is established
		lock_guard<mutex> guard(clients_mtx);
		clients.push_back({seed, string("Client Without Name"),client_socket,(move(t))});
	}

	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].th.joinable())
			clients[i].th.join();
	}

	close(server_socket);
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
void handle_client(int client_socket, int id)
{
	char name[MAX_LEN],str[MAX_LEN];
	
	string person_to_talk="";
	recv(client_socket,name,sizeof(name),0);
	set_name(id,name);
	cout<<"Name is :: "<<name<<"ID is :: "<<id<<endl;
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
		int bytes_received=recv(client_socket,str,sizeof(str),0);
		l++;
		if(bytes_received<=0)
			return;
		if(strcmp(str,"bye")==0)
		{
		        //fout<<name<<" : "<<str<<endl; 
		
			// Display leaving message
			string message=string(name)+string(" has left");		
			broadcast_message("#NULL",id);			
			broadcast_message(id,id);						
			broadcast_message(message,id);
			shared_print(color(id)+message+def_col);
			end_connection(id);
			fout.close();
			l=0;										
			return;
		}
		
		
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
                string current_date=time_today();        
                file_name=string(name)+"_"+person_to_talk+"_"+current_date;
                fout.open(file_name,ios::app);
                }
                          
                char* substr = str + 1;
                
                fout<<name<<" : "<<substr<<endl;
          
                   
                broadcast_message_online(string(person_to_talk),id,string(name));					
		broadcast_message_online(id,id,string(person_to_talk));		
		broadcast(string(person_to_talk),id,string(substr));
		shared_print(color(id)+name+" : "+def_col+substr);
		
		}
		
		else{	
		
               fout<<name<<" : "<<str<<endl;
		
		broadcast_message_online(string(person_to_talk),id,string(name));					
		broadcast_message_online(id,id,string(person_to_talk));		
		broadcast(string(person_to_talk),id,string(str));	
		shared_print(color(id)+name+" : "+def_col+str);
		}
			
	}//while loop ends here	
}//thread handler ends here



