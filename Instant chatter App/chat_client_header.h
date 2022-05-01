#include<iostream>//system call, data types
using namespace std;
#include<sys/socket.h>//socket
#include<unistd.h>//read & write
#include<arpa/inet.h>//ip
#include<stdio.h>//gets function
#include<cstring>//strcpy
//#include<string.h>
class newUser
{
	string mobile,name,temp_User,userId,password,emailId,temp_Password;
	public:
	void add_new_User();
	void existing_user(int);
	string newUserDataInOneString();
	string existingUserCredInOneString();
};



void sig_handler(int signum);


