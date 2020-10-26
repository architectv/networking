CC=g++
SERVER=http_server.cpp convert.cpp statistics.cpp handler.cpp http_parser.cpp
CLIENT=client.cpp
FLAGS=-std=c++17
LIBS=-pthread

all: client server

client:
	$(CC) $(FLAGS) $(CLIENT) -o client.out

server:
	$(CC) $(FLAGS) $(LIBS) $(SERVER) -o server.out

clean:
	rm *.out