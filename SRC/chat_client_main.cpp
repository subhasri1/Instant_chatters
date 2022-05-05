#include "../INC/chat_client_header.h"

/*bool exit_flag=false;
thread t_send, t_recv;

int client_des;

string def_col="\033[0m";
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};
*/





//initializing default port number
int port =8080;
char logged_name[MAX_LEN];
int main(int argc,char *argv[])
{
ofstream log;
log.open("../BIN/client_logs",ios::app);
       
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
				  fin.open("../SRC/"+combine,ios::in);
				  
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







































