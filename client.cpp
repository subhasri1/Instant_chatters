#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{


<<<<<<< HEAD
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


	socklen_t len = sizeof(sock_addr_serv);
	if (connect(serv_fd,(struct sockaddr *)&sock_addr_serv,len)==-1)
	{
			perror("connect error");
				exit(1);
	}
	cout << "in client client fd" << serv_fd << endl;
	cout << "in client port (50) " << sock_addr_serv.sin_port << endl;
	char ch = 'a';

	write(serv_fd,&ch,1);
	read(serv_fd,&ch,1);
	cout << "from server " << ch << endl;

	close(serv_fd);

}

=======
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


socklen_t len = sizeof(sock_addr_serv);
if (connect(serv_fd,(struct sockaddr *)&sock_addr_serv,len)==-1)
{
	perror("connect error");
	exit(1);
}
cout << "in client client fd" << serv_fd << endl;
cout << "in client port (50) " << sock_addr_serv.sin_port << endl;
char ch = 'a';

write(serv_fd,&ch,1);
read(serv_fd,&ch,1);
cout << "from server " << ch << endl;

close(serv_fd);

}
>>>>>>> 1179a53d093945c0b74fe1b9358d1e574571ea97










