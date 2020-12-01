#pragma once

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <csignal>
#include <pthread.h>
#include <queue>
#include <string>
#include <cstring>
#include <sys/ioctl.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <map>
#include <tuple>
#include "const.h"

#define THREAD_POOL_SIZE 20
#define LISTEN_COUNT 5

extern int sockfd;
extern int sigint_flag;

struct client_t {
    std::string ip;
    int socket;
};
extern std::queue<client_t> q;

extern pthread_t thread_pool[THREAD_POOL_SIZE];
extern pthread_mutex_t content_mutex;
extern pthread_mutex_t mutex;
extern pthread_cond_t condvar;

extern std::map<std::string, std::map<std::string, int>> content_map;

const std::map<int, std::string> statuses{
    std::pair<int, std::string>{200, "OK"},
    std::pair<int, std::string>{403, "Forbidden"},
    std::pair<int, std::string>{404, "Not Found"},
    std::pair<int, std::string>{405, "Method Not Allowed"},
};

const std::map<std::string, std::string> content_types{
    std::pair<std::string, std::string>{".html", "text/html"},
    std::pair<std::string, std::string>{".css", "text/css"},
    std::pair<std::string, std::string>{".js", "application/javascript"},
    std::pair<std::string, std::string>{".jpg", "image/jpeg"},
    std::pair<std::string, std::string>{".jpeg", "image/jpeg"},
    std::pair<std::string, std::string>{".png", "image/png"},
    std::pair<std::string, std::string>{".gif", "image/gif"},
};

void* thread_function(void* argv);
void handle_function(client_t client);
void sigint_handler(int signum);

std::map<std::string, std::string> get_headers(std::string header_text);
std::string get_start_response_line(
    std::string protocol, int status_code, std::string status_string);

void save_statistics(void);
void write_statistics(std::string ip, std::string ext);

std::string response_format(
    std::string protocol, int status_code, std::string status_string,
    std::map<std::string, std::string> headers, std::string body);

std::tuple<std::string, std::string, std::string>
split_start_request_line(std::string line);

std::string get_content_type(std::string ext);
int check_host(std::map<std::string, std::string> headers);
std::string get_extension(std::string path);
std::string get_response(client_t client, std::string request);
std::string string_to_hex(const std::string& input);
