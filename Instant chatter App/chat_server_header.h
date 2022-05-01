#include<iostream>//system call, data types
using namespace std;
#include<sys/socket.h>//socket
#include<unistd.h>//read & write
#include<arpa/inet.h>//ip
#include<fstream>//filehandling
#include<vector>//map STL
#include<cstring>//strcpy
#include<stdlib.h>//atoi



struct user_details{
string user_id;
string user_name;
string password;
};

void user_cred_file_to_vector(vector<user_details> &user_cred);//key,value--->userid,password

void login_and_registration_of_user(int cli_des,vector<user_details> userCred,string&);

char user_cred_authentication(vector<user_details> user_cred, char data[],int n,string&);

void user_cred_addition(vector<user_details> &user_cred,char new_user_info[],int k);
