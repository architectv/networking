all: client server test

client: client.c const.h
	gcc client.c -o client.out

server: server.c convert.c const.h convert.h
	gcc server.c convert.c -o server.out

test: convert_test.c convert.c convert.h
	gcc convert_test.c convert.c -o convert_test.out

clean:
	rm *.out