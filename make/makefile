OBJ=../OBJ
BIN=../BIN
INC=../INC
SRC=../SRC
CC=g++

all:$(BIN)/client $(BIN)/server

$(BIN)/client:$(SRC)/chat_client_main.cpp $(OBJ)/chat_client_function.o
	$(CC) -o $(BIN)/client $(SRC)/chat_client_main.cpp -lpthread $(OBJ)/chat_client_function.o	
	
$(OBJ)/chat_client_function.o:$(SRC)/chat_client_function.cpp $(INC)/chat_client_header.h
	$(CC) -c $(SRC)/chat_client_function.cpp
	mv -f *.o $(OBJ)/	
	
$(BIN)/server:$(SRC)/chat_server_main.cpp $(OBJ)/chat_server_function.o
	$(CC) -o $(BIN)/server $(SRC)/chat_server_main.cpp -lpthread $(OBJ)/chat_server_function.o	
	
$(OBJ)/chat_server_function.o:$(SRC)/chat_server_function.cpp $(INC)/chat_server_header.h
	$(CC) -c $(SRC)/chat_server_function.cpp
	mv -f *.o $(OBJ)/
	
