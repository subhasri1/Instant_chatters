#include "chat_client_header.h"
#include<iostream>//system call, data types
using namespace std;
#include<sys/socket.h>//socket
#include<unistd.h>//read & write
#include<arpa/inet.h>//ip
#include<stdio.h>//gets function
#include<cstring>//strcpy
//#include<string.h>

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




