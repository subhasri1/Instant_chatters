#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <termios.h>
#include <unistd.h>
using namespace std;

int user_id = 1001;
int no_of_users = 0;
int SERVER_PORT = 0;

/* For storing the user details */
struct user_details {
    char name[15];
	char email_id[20];
	char password[15];
	char mobile_no[11];
} temp_user;

void new_user_registration(int sock);
void validate_user(int sock);

int main(int argc, char **argv) {
    if(argv[1]) {
        SERVER_PORT = atoi(argv[1]);
    } else {
        cout << "[Usage Error] ./Server <Server-Port>" << endl;
        return 0;
    }
	struct sockaddr_in client, server;
    int address_len = sizeof(server);
	int sock, accept_sock, client_socket[30], max_clients = 30, max_sd, sd, activity;
	int option, read_status;
	int index = 0;
    fd_set readfds;

    for(int i = 0; i < max_clients; i++)
        client_socket[i] = 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error" << endl;
        return -1;
    }
	server.sin_family=AF_INET;
	server.sin_port = SERVER_PORT;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	cout << "Today Date is : " << tm.tm_mday << "/" << tm.tm_mon + 1 << endl;

    if (bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        cout << "Socket binding with port:" << SERVER_PORT << " failed" << endl;
        return -1;
    }
    cout << "Server is Running in LISTEN mode" << endl;
    if (listen(sock, 3) < 0) {
        cout << "Error while listening to incoming connection" << endl;
        return -1;
    }

    while(true) {
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        max_sd = sock;

        for (int i = 0 ; i < max_clients ; i++) {  
            sd = client_socket[i];  
            if(sd > 0)  
                FD_SET(sd , &readfds);  
            if(sd > max_sd)  
                max_sd = sd;
        }

        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
        if ((activity < 0) && (errno!=EINTR))  
            cout << "ERROR in select" << endl;

        if (FD_ISSET(sock, &readfds)) {  
            if ((accept_sock = accept(sock, (struct sockaddr *) &server, (socklen_t*) &address_len)) < 0) { 
                cout << "Error while accepting the incoming connection" << endl;
                exit(-1);
            }
            cout << "Received new connection to the Server" << endl;
            for (int i = 0; i < max_clients; i++) {  
                if( client_socket[i] == 0 ) {  
                    client_socket[i] = accept_sock;  
                    break;  
                }  
            }
        }

        for (int i = 0; i < max_clients; i++) {  
            sd = client_socket[i];    
            if (FD_ISSET(sd , &readfds)) { 
                if((read_status = read(sd, &option, sizeof(int))) != 0) {
                    switch(option) {
                        case 1: /* New User Registration */
                            cout << "Starting the execution of add_user_details method" << endl;
                            new_user_registration(sd);
                            cout << "Successfully executed add_user_details method" << endl;
                            break;

                        case 2: /* Validating the User */
                            cout << "Starting the execution of validate_user method" << endl;
                            validate_user(sd);
                            cout << "Successfully executed validate_user method" << endl;
                            break;
                    }
	            }  
            }
        } 
    }
	return 0;
}

/* New User Registration */
void new_user_registration(int sock) {
	int temp, return_status = 0;
	temp = read(sock, &temp_user, sizeof(struct user_details));
    ofstream user_file("user-details.txt", ios::app);
    user_file << temp_user.name << " " << temp_user.password << " " << temp_user.mobile_no << " " << temp_user.email_id << " " << endl;
	cout << "User Successfully Registered" << endl;	
	send(sock, &return_status, sizeof(int), 0);
	return;
}

/* Validate User Details */
void validate_user(int sock) {
	int temp, validation_status = -1;
    user_details user;
	temp = read(sock, &temp_user, sizeof(struct user_details));	
    ifstream user_file("user-details.txt");
    while(user_file >> user.name >> user.password >> user.mobile_no >> user.email_id)
	 {
        if(strcmp(user.name, temp_user.name) == 0) {
            if(strcmp(user.password, temp_user.password) == 0){
                validation_status = 0;
            break;
		}
	    else
		{
			validation_status =1;
		}
        }
    }
    send(sock, &validation_status, sizeof(int), 0);
}
