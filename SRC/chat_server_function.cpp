#include "../INC/chat_server_header.h"
static  int l=0;//static variable
void user_cred_file_to_vector(vector<user_details> &user_cred)//--->userid,uname,password
{
	ifstream fin;
	string data;
	fin.open("../SRC/user_Credentials.txt",ios::in);
	while(getline(fin,data))
	{
		string name="", user="", password="";
		int cnt =0;
		for(int i=0;i<data.length();i++)
		{
			if(cnt==3)
			break;
			else if(data[i]==',')
			cnt++;
			else if(cnt==0)
			user+=data[i];
			else if(cnt==1)
			password+=data[i];
			else if(cnt==2)
			name+=data[i];
		}
		user_details details{
		user,name,password
		};
		user_cred.push_back(details);
	}
	
	fin.close();
	
}

void login_and_registration_of_user(int cli_des,vector<user_details> userCred,string &name_login_user)
{

	while(1)
	 	{
	 		int k=0;
		 	char buffer[MAX_LEN];
		 	bzero(buffer,sizeof(buffer));	 	
		 	int n=read(cli_des,&buffer,sizeof(buffer));
		 	
		 	if(buffer[0]=='1')
		 	{
			 	char ret=user_cred_authentication(userCred,buffer,n,name_login_user);
			 	write(cli_des,&ret,sizeof(ret));
			 	//cout<<"Online User : "<<name_login_user<<endl;		 	
			 	if(ret == '1')
			 		k=1;
		 	}
		 	else if(buffer[0]=='2')
		 	{
			 	user_cred_addition(userCred,buffer,n);//to write user in file and map
			 	char msg='3';
			 	write(cli_des,&msg,sizeof(msg));
			 	//k=1;
		 	}
		 	
			 if(buffer[0]=='e' || k==1)
			 break;
		 
	 	}
}


char user_cred_authentication(vector<user_details> user_cred, char data[],int n,string &name_login_user)
{

	int cnt=0;//user,password
	string user="",password="";
	for(int i=1;i<n;i++)
	{
		if(data[i]==',')
		  cnt++;
		else if(cnt==0)
		  user+=data[i];
		else if(cnt==1)
		  password+=data[i];
	}
	
	for(auto i:user_cred)
	{
		if(i.user_id==user && i.password==password)
		{
		name_login_user=i.user_name;
		return '1';
		}
		else if(i.user_id==user)
		return '2';
	}
	return '0';
}



void user_cred_addition(vector<user_details> &user_cred,char new_user_info[],int k)
{
	int cnt=0;//user,password
	string user="",password="",data="",name="";
	for(int i=1;i<k;i++)
	{
		if(new_user_info[i]==',')
		  cnt++;
		else if(cnt==0)
		  user+=new_user_info[i];
		else if(cnt==1)
		  password+=new_user_info[i];
		else if(cnt==2)
		name+=new_user_info[i];
		  
		data+=new_user_info[i];
	}
	        user_details details{
		user,name,password
		};
		user_cred.push_back(details);
	
	ofstream fout;
	fout.open("../SRC/user_Credentials.txt",ios::app);
	fout<<data;
	fout<<"\n";
	fout.close();
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
                fout.open("../SRC/"+file_name,ios::app);//opening the file with appended name 
                fout2.open("../SRC/"+file_name2,ios::app);
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
