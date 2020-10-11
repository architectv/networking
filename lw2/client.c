#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "const.h"

int main(void) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        printf("Error [socket()]\n");
        return SOCKET_ERR;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    printf("Input decimal number: ");
    int number;
    char tmp;
    if (scanf("%d%c", &number, &tmp) != 2 || tmp != '\n') {
        printf("Wrong input! End of the program...\n");
        close(sockfd);
        return INPUT_ERR;
    }

    char msg[MSG_LEN];
    sprintf(msg, "%d", number);
    int sent_size = sendto(sockfd, (const char*)msg, strlen(msg),
        MSG_CONFIRM, (const struct sockaddr*)&server,
        sizeof(server));

    close(sockfd);
    if (sent_size != strlen(msg)) {
        printf("Error [sendto()]\n");
        return SEND_ERR;
    }
    else {
        printf("Decimal number %d was sent\n", number);
    }

    return 0;
}
