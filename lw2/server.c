#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include "const.h"
#include "convert.h"

int sockfd = 0;
int sigint_flag = 0;

void sigint_handler(int signum) {
    close(sockfd);
    printf("\nSocket is closed\nSignal : [%d]\n", signum);
    sigint_flag = 1;
}

int main(void) {
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        printf("Error [socket()]\n");
        return SOCKET_ERR;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr*)&server,
        sizeof(server)) < 0) {
        close(sockfd);
        printf("\nError [bind()]\n");
        return BIND_ERR;
    }

    struct sockaddr_in client;
    int client_size = sizeof(client);
    memset(&client, 0, client_size);

    signal(SIGINT, sigint_handler);

    printf("Server is working on port %d\n", ntohs(server.sin_port));
    char msg[MSG_LEN];
    for (int i = 0; ; i++) {
        int msg_end = recvfrom(sockfd, msg, MSG_LEN,
            MSG_WAITALL, (struct sockaddr*)&client,
            &client_size);
        if (sigint_flag) {
            return 0;
        }
        if (msg_end < 0) {
            close(sockfd);
            return RECV_ERR;
        }
        msg[msg_end] = '\0';

        printf("\nCLIENT #%d [%s:%d]: \"%s\"\n",
            i + 1, inet_ntoa(client.sin_addr), ntohs(client.sin_port), msg);
        int number = atoi(msg);
        printf("%d --> 0b%s --> 0o%s --> 0x%s\n",
            number,
            convert(number, 2),
            convert(number, 8),
            convert(number, 16));
    }

    close(sockfd);

    return 0;
}
