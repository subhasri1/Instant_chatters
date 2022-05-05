#include<iostream>//system call, data types
#include <bits/stdc++.h>
using namespace std;
#include<sys/socket.h>//socket
#include<unistd.h>//read & write
#include<arpa/inet.h>//ip
#include<stdio.h>//gets function
#include<string.h>
#include<signal.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <thread>
#include <mutex>

#define MAX_LEN 1024
#define NUM_COLORS 6


extern bool exit_flag;
extern thread t_send, t_recv;

extern int client_des;

extern string def_col;
extern string colors[];



class newUser
{
	string mobile,name,temp_User,userId,password,emailId,temp_Password;
	public:
	newUser();
	newUser(string mobile, string name, string temp_User, string userId, string password, string emailId, string temp_Password);
	void add_new_User();
	void existing_user(int);
	string newUserDataInOneString();
	string existingUserCredInOneString();
};



void sig_handler(int signum);

void catch_ctrl_c(int);
string color(int);
int eraseText(int);
void send_message(int);
void recv_message(int);


