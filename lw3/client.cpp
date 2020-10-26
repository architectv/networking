#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include "const.h"

void get_request(int sockfd, std::string file) {
    char buff[MSG_LEN + 1];
    sprintf(buff, "GET /%s HTTP/1.1\nHost: 127.0.0.1:%d\nConnection: close\n",
        file.c_str(), PORT);
    printf("Request:\n%s\n", buff);
    write(sockfd, buff, sizeof(buff));
}

void get_response(int sockfd) {
    char buff[MSG_LEN + 1];
    bzero(buff, sizeof(buff));
    int n = 0;
    printf("Response From Server:\n");
    do {
        n = read(sockfd, buff, sizeof(buff) - 1);
        printf("%s", buff);
        bzero(buff, sizeof(buff));
        fflush(stdout);
    } while (n == sizeof(buff) - 1);
    printf("\n");
}

int main(int argc, char** argv) {
    int client_port = 8989;
    std::string file = "index.html";
    if (argc > 1) {
        client_port = atoi(argv[1]);
        if (client_port < 1024 || client_port > 65535 || client_port == PORT) {
            return -1;
        }
        if (argc == 3) {
            file = argv[2];
        }
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        printf("Error [socket()]\n");
        return SOCKET_ERR;
    }

    SA_IN client;
    memset(&client, 0, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_port = htons(client_port);

    int number = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &number, sizeof(int));

    int ret = bind(sockfd, (const SA*)&client, sizeof(client));
    if (ret < 0) {
        close(sockfd);
        printf("\nError [bind()]\n");
        return BIND_ERR;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (const struct sockaddr*)&server, sizeof(server)) != 0) {
        close(sockfd);
        printf("\nError [connect()]\n");
        return CONNECT_ERR;
    }

    get_request(sockfd, file);
    get_response(sockfd);

    close(sockfd);

    return 0;
}
