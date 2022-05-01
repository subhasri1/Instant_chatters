all:client server
client:chat_client_main.cpp chat_client_function.o
	g++ -o client chat_client_main.cpp -lpthread chat_client_function.o	
chat_client_function.o:chat_client_function.cpp chat_client_header.h
	g++ -c chat_client_function.cpp
server:chat_server_main.cpp chat_server_function.o
	g++ -o server chat_server_main.cpp -lpthread chat_server_function.o	
chat_server_function.o:chat_server_function.cpp chat_server_header.h
	g++ -c chat_server_function.cpp

