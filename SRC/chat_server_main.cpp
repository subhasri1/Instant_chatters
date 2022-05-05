#include "../INC/chat_server_header.h"



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




//initialize the default port
int port=8080;
int main(int argc,char *argv[])
{

ofstream log;
log.open("../BIN/server_logs",ios::app);

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





