#ifndef SOCKET_H
#define SOCKET_H
#include "winsock2.h"
#include "ws2tcpip.h"
#pragma comment(lib, "Ws2_32.lib")
#include "http.h"
#define BUFF_LEN 1024


typedef struct WSA_Socket {
    WSADATA wsa_data;
    addrinfo *operation_result = NULL;
    addrinfo settings;
    SOCKET listen_socket = INVALID_SOCKET;
    SOCKET client_socket = INVALID_SOCKET;
    char buffer[BUFF_LEN];
} WSA_Socket;

int create_wsa(WSA_Socket* obj);
void setup_ipv4(WSA_Socket* obj);
int open_ip_port(WSA_Socket* obj, const char* ip, const char* port);
SOCKET create_socket(WSA_Socket* obj);
int create_tcp_socket(WSA_Socket* obj);
int listen_connections(WSA_Socket* obj);
SOCKET accept_connections(WSA_Socket* obj);
int close_connection(WSA_Socket* obj);
int get_data(WSA_Socket* obj, HashMap *get_map, HashMap *post_map, HashMap *put_map, HashMap *delete_map);
int create_all_sockets(WSA_Socket* obj, const char* ip, const char* port);


#endif