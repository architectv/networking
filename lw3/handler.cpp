#include "http_server.h"

void sigint_handler(int signum) {
    sigint_flag = 1;
    close(sockfd);
    printf("\nSocket is closed by signal (%d)\n", signum);
    save_statistics();
}

void* thread_function(void* argv) {
    while (true) {
        client_t* pclient = nullptr;

        pthread_mutex_lock(&mutex);
        if (q.empty()) {
            pthread_cond_wait(&condvar, &mutex);
            pclient = &q.front();
            q.pop();
        }
        pthread_mutex_unlock(&mutex);

        if (pclient != nullptr) {
            handle_function(*pclient);
        }
    }
}

void handle_function(client_t client) {
    char buff[MSG_LEN + 1];
    bzero(buff, sizeof(buff));
    read(client.socket, buff, sizeof(buff));

    std::string request = buff;
    auto response = get_response(client, request);

    write(client.socket, response.c_str(), response.length());
}
