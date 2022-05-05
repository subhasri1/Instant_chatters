#include "../INC/chat_client_header.h"
bool exit_flag=false;
thread t_send, t_recv;

int client_des;

string def_col="\033[0m";
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};


newUser::newUser()
{
}

newUser::newUser(string mobile,string name,string temp_User,string userId,string password,string emailId,string temp_Password){
this->mobile=mobile;
this->name=name;
this->userId=userId;
this->password=password;
this->emailId=emailId;
this->temp_User=temp_User;
this->temp_Password=temp_Password;
}

void newUser :: add_new_User()
{
	string re_password;
	cout<<"\n\nNew User Details "<<endl;
	cout<<"-------------------------------"<<endl;
	
	cout<<"\nEnter your Name :"<<endl<<"=> ";
	//fgets(name,sizeof(name)-1,stdin);
	getline(cin>>ws,name);
	cout<<"Enter User ID :"<<endl<<"=> ";
	cin>>userId;
	cout<<"Enter your Email ID :"<<endl<<"=> ";
	cin>>emailId;
	cout<<"Enter your Mobile Number :"<<endl<<"=> ";
	cin>>mobile;
	while(1)
	{
		cout<<"Enter Password :"<<endl<<"=> ";
		cin>>password;
		cout<<"Enter Re-Password :"<<endl<<"=> ";
		cin>>re_password;
		if(password == re_password)
		{
			break;
		}
		else
		{
			cout<<"\n\nPassword and Re-Password are not same ! \nPlease Enter again\n"<<endl;
		}
	
	}
}
void newUser :: existing_user(int cnt)
{
	if(cnt==3)
	{
	cout<<"\n\nExisting User"<<endl;
	cout<<"------------------------------------"<<endl;
	//cout<<"User Id : "<<user<<endl;
	//cout<<"Only "<<cnt<<" Attempts Left !"<<endl;
	cout<<"\nEnter Your User ID :"<<endl;
	cin>>temp_User;
	cout<<"Enter Your Password :"<<endl<<"=> ";
	cin>>temp_Password;
	}
	else
	{
	cout<<"\nLogin Credentials are Invalid !"<<endl;
	cout<<"Only "<<cnt<<" Attempts Left !"<<endl;
	cout<<"\nEnter Your User ID :"<<endl;
	cin>>temp_User;
	cout<<"Please enter your Correct Password :"<<endl<<"=> ";
	cin>>temp_Password;
	}
}
string newUser :: newUserDataInOneString()
{
	string sum="2"+userId+","+password+","+name+","+mobile+","+emailId;
	return sum;
}

string newUser :: existingUserCredInOneString()
{
	string sum="1"+temp_User+","+temp_Password;
	return sum;
}

void sig_handler(int signum)
{
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



