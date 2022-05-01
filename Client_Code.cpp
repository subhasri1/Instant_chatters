#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int SERVER_PORT;

void user_menu(int sock, struct sockaddr_in server);
void user_registration(int sock, struct sockaddr_in server);

/* For storing the user details */
struct user_details {
    char name[15];
	char email_id[20];
	char password[15];
	char mobile_no[11];
} user;

void refresh_object() {
    strcpy(user.name, "");
    strcpy(user.email_id, "");
    strcpy(user.password, "");
    strcpy(user.mobile_no, "");
}

/* For Password Hiding while typing in Terminal */
string GetPassword() {
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	string s;
	string c;
	cin >> c;
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);	
	return c;
}

int main(int argc, char **argv) {
    if(argv[1] ) {
        SERVER_PORT = atoi(argv[1]);
    } else {
        cout << "[Usage Error] ./Client [Server-Port]" << endl;
        return 0;
    }

	int choice;
	struct sockaddr_in client, server;
	int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error" << endl;
        return -1;
    }
	server.sin_family = AF_INET;
    server.sin_port = SERVER_PORT;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) { 
        cout << "Connection to Server failed" << endl;
        return -1;
    }

	while(choice != 3) {
		/* MAIN MENU for the CHAT APPLICATION */
		cout << "---- MENU ----" << endl;
		cout << "1. User Login" << endl;
		cout << "2. New User Registration" << endl;
		cout << "3. Exit" << endl;
		cout << "Enter your choice : ";
		cin >> choice;
		switch(choice) {
			case 1 : /* User Menu */
				user_menu(sock, server);
				break;
			case 2 : /* New User Registration */
				user_registration(sock, server);
				break;
			case 3 : /* Exiting the Chat Application */
				 break;
			default : 
                cout << "Enter the correct option between 1 and 3" << endl;
		}
	}
	return 0;
}

/* New User Registration */
void user_registration(int sock, struct sockaddr_in server)
{
	int choice = 1, temp;
	string password_temp;
	string retype_password;
    int return_status = -1;

	cout << "NEW USER REGISTRATION MENU" << endl;
	cout << "Name : ";
	cin >> user.name;
	cout << "Email ID : ";
	cin >> user.email_id;
    	cout << "Mobile Number : ";
	cin >> user.mobile_no;

	begin:
	cout << "Password : ";
	password_temp = GetPassword();
	cout << endl << "Retype Password : ";
	retype_password = GetPassword();
	if(password_temp != retype_password){
		cout << endl << "Password entered didn't match, Kindly try it again " << endl;
		goto begin;
	}
	strcpy(user.password,password_temp.c_str());

	send(sock, &choice, sizeof(int), 0);
	send(sock, &user, sizeof(struct user_details), 0);
    temp = read(sock, &return_status, sizeof(int));
    if(return_status == 0)
	    cout << "User is registered Successfully" << endl;
    else
        cout << "User registration failed" << endl;

}

/* User Menu */
void user_menu(int sock, struct sockaddr_in server) {
	int choice = 2, temp = -1, validation_status;
        refresh_object();
	char user_name[15];
	string password_temp;
again:
	cout << "Enter the Name : ";
	cin >> user.name;
	cout << "Enter the Password : ";
	password_temp = GetPassword();
	strcpy(user.password,password_temp.c_str());

	send(sock, &choice, sizeof(int), 0);
	send(sock, &user, sizeof(struct user_details), 0);
    	temp = read(sock, &validation_status, sizeof(int));

	if(validation_status == 0)
	{ 
        cout << "\n \tLogin Successful" << endl;
	cout<<endl;
	bool is_valid = true;
	while(is_valid == true)
	{
		int ch;
		cout<<"Enter the given option"<<endl;
		cout<<" 1.Chat "<<endl;
		cout<<" 2.Search chat history "<<endl;
		cout<<" 3.Logout"<<endl;
		cin>>ch;
		switch(ch)
		{
			case 1:
				cout<<"Welcome to chat"<<endl;
				break;
			case 2:
				cout<<"Enter the word to search"<<endl;
				break;
			case 3:
				cout<<"Logged out successfully"<<endl;
				is_valid = false;
				break;
		}
	}
	}
	else if(validation_status == 1)
                                                {
                                                   int count=3;
						   if(count>1)
                                                    {
                                                     count--;
						     cout<<count<<" Attempts left!"<<endl;
                                                     cout<<"\nInvalid Credential!! \nPlease Enter"<<endl;
						    }goto again;
							
						}
	else
	{
		cout << "Username / Password Wrong" << endl;
}
}
