#pragma once

#define PORT 8080
#define MSG_LEN 1024
#define SOCKET_ERR (-1)
#define BIND_ERR (-2)
#define RECV_ERR (-3)
#define INPUT_ERR (-4)
#define SEND_ERR (-5)
#define LISTEN_ERR (-6)
#define ACCEPT_ERR (-7)
#define CONNECT_ERR (-8)

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;
