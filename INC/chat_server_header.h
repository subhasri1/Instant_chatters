#include<iostream>//system call, data types
using namespace std;
#include<sys/socket.h>//socket
#include<unistd.h>//read & write
#include<arpa/inet.h>//ip
#include<fstream>//filehandling
#include<vector>//map STL
#include<stdlib.h>//atoi

#include<fcntl.h>
#include<sys/stat.h>

#include <bits/stdc++.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <thread>
#include <mutex>

#define NUM_COLORS 6
#define MAX_LEN 1024


//static int l;//static variable

extern string name_login_user;
extern string file_name;
extern string file_name2;

extern ofstream fout;//object of ofstream class to write on the file
extern ofstream fout2;



extern string def_col;

extern string colors[NUM_COLORS];
extern int seed;
extern mutex cout_mtx,clients_mtx;


//structure to store clients details along with its thread
struct terminal
{
	int id;
	string name;
	int socket;
	thread th;
};

extern vector<terminal> clients;

struct user_details{
string user_id;
string user_name;
string password;
};

void user_cred_file_to_vector(vector<user_details> &user_cred);//key,value--->userid,password

void login_and_registration_of_user(int cli_des,vector<user_details> userCred,string&);

char user_cred_authentication(vector<user_details> user_cred, char data[],int n,string&);

void user_cred_addition(vector<user_details> &user_cred,char new_user_info[],int k);



string color(int code);
void set_name(int id, char name[]);//for givin the client name
void shared_print(string str, bool endLine);//for proper sync of cout statements
int broadcast_message(string message, int sender_id);//sending name and message to all the user except client
int broadcast_message(int num, int sender_id);//sending id to all user except to ownself
void end_connection(int id);//for endiing the thread

void handle_client(int cli_des, int id);//thread handler for handling client
int broadcast_message_online(string person_to_talk, int sender_id, string name);//sending name to the person who the user choose to talk
int broadcast_message_online(int num, int sender_id, string name);//sending ID to person who the user choose to talk with
int broadcast(string  person_to_talk,int id,string message);//sending message to person who the user choose to talk to
