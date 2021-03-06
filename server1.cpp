#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main()
{


int serv_fd = socket(AF_INET,SOCK_STREAM,0);
if (serv_fd == -1)
{
	perror("socket creation error");
	exit(1);
}


struct sockaddr_in sock_addr_serv;
sock_addr_serv.sin_family = AF_INET;
sock_addr_serv.sin_port = 9988;
sock_addr_serv.sin_addr.s_addr = inet_addr("127.0.0.1");

if (bind(serv_fd,(struct sockaddr *)&sock_addr_serv,sizeof(sock_addr_serv))==-1)
{
	perror("bind error");
	exit(1);
}


if (listen(serv_fd,2)==-1)
{
	perror("listen error");
	exit(1);
}

while (1)
{
	cout << "server waiting for client to connect"  << endl;


	struct sockaddr_in sock_addr_cli;
	socklen_t cli_len = sizeof(sock_addr_cli);
	int client_fd=accept(serv_fd,(struct sockaddr *)&sock_addr_cli,&cli_len);
	cout << "in server serv fd" << serv_fd << endl;
	cout << "in server client fd" << client_fd << endl;
	cout << "in server port (50) " << sock_addr_cli.sin_port << endl;
	cout << "in server ip " << sock_addr_cli.sin_addr.s_addr << endl;

	if (client_fd==-1)
	{
		perror(" accept error");
		exit(1);
	}
	char ch;
	read(client_fd,&ch,1);
	cout << " from client : " << ch << endl;
	ch++;
	write(client_fd,&ch,1);

}


if (close(serv_fd)==-1)
{
	perror("socket close error");
	exit(1);
}
}
