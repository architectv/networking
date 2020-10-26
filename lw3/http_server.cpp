#include "http_server.h"

int sockfd = 0;
int sigint_flag = 0;
std::queue<client_t> q;
pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t content_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condvar = PTHREAD_COND_INITIALIZER;
std::map<std::string, std::map<std::string, int>> content_map;

int main(void) {
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&thread_pool[i], nullptr, thread_function, nullptr);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error [socket()]\n");
        return SOCKET_ERR;
    }

    SA_IN server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    int number = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &number, sizeof(int));

    int ret = bind(sockfd, (const SA*)&server, sizeof(server));
    if (ret < 0) {
        close(sockfd);
        printf("\nError [bind()]\n");
        return BIND_ERR;
    }

    if (listen(sockfd, LISTEN_COUNT) != 0) {
        close(sockfd);
        printf("\nError [listen()]\n");
        return LISTEN_ERR;
    }

    signal(SIGINT, sigint_handler);
    SA_IN client;
    int client_size = sizeof(client);
    memset(&client, 0, client_size);

    printf("Server is working...\n");
    while (true) {
        int connfd = accept(sockfd, (SA*)&client, (socklen_t*)&client_size);
        if (connfd < 0) {
            close(sockfd);
            printf("\nError [accept()]\n");
            return ACCEPT_ERR;
        }

        client_t new_client;
        new_client.ip = std::string(inet_ntoa(client.sin_addr))
            + ":" + std::to_string(ntohs(client.sin_port));
        new_client.socket = connfd;

        pthread_mutex_lock(&mutex);
        q.push(new_client);
        pthread_cond_signal(&condvar);
        pthread_mutex_unlock(&mutex);
    }

    close(sockfd);

    return 0;
}
