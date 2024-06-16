#include "socket.h"
#include <iostream>
#define IP "192.168.254.174"
#define PORT "1488"


int main() {
    WSA_Socket wsa_socket = {};
    if(create_all_sockets(&wsa_socket, IP, PORT) != 0) {
        std::cout << "Socket Errors\n";

        return 1;
    }
    std::cout << "Server is live\n";
    while(get_data(&wsa_socket) == 0) {

    }

    return 1;
}