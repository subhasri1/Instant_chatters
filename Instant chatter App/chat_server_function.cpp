#include "chat_server_header.h"
#include<iostream>//system call, data types
using namespace std;
#include<sys/socket.h>//socket
#include<unistd.h>//read & write
#include<arpa/inet.h>//ip
#include<fstream>//filehandling
#include<vector>//map STL
#include<cstring>//strcpy
#include<stdlib.h>//atoi

#define size 256


void user_cred_file_to_vector(vector<user_details> &user_cred)//--->userid,uname,password
{
	ifstream fin;
	string data;
	fin.open("user_Credentials.txt",ios::in);
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
		 	char buffer[size];
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
	fout.open("user_Credentials.txt",ios::app);
	fout<<data;
	fout<<"\n";
	fout.close();
}

