#include "socket.h"
#include "iostream"


int create_wsa(WSA_Socket* obj) {
    int res = WSAStartup(MAKEWORD(2, 2), &obj -> wsa_data);

    return res;
}

void setup_ipv4(WSA_Socket* obj) {
    ZeroMemory(&obj -> settings, sizeof(obj -> settings));
    obj -> settings.ai_family = AF_INET;
    obj -> settings.ai_socktype = SOCK_STREAM;
    obj -> settings.ai_protocol = IPPROTO_TCP;
    obj -> settings.ai_flags = AI_PASSIVE;
}

int open_ip_port(WSA_Socket* obj, const char* ip, const char* port) {
    int res = getaddrinfo(ip, port, &obj -> settings, &obj -> operation_result);
    
    return res;
}

SOCKET create_socket(WSA_Socket* obj) {
    obj -> listen_socket = socket(obj -> operation_result -> ai_family, 
                                obj -> operation_result -> ai_socktype, 
                                obj -> operation_result -> ai_protocol);

    return obj -> listen_socket;
}

int create_tcp_socket(WSA_Socket* obj) {
    int res = bind(obj -> listen_socket, 
                    obj -> operation_result -> ai_addr, 
                    (int)(obj -> operation_result -> ai_addrlen));

    return res;
}

int listen_connections(WSA_Socket* obj) {
    int res = listen(obj -> listen_socket, SOMAXCONN);

    return res;
}

SOCKET accept_connections(WSA_Socket* obj) {
    obj -> client_socket = accept(obj -> listen_socket, NULL, NULL);

    return obj -> client_socket;
}

int close_connection(WSA_Socket* obj) {
    int res = shutdown(obj -> client_socket, SD_SEND);
    if(res == SOCKET_ERROR) {
        std::cout << "Close connection Error\n";
        closesocket(obj -> listen_socket);
        closesocket(obj -> client_socket);
        WSACleanup();

        return 1;
    }
    closesocket(obj -> listen_socket);
    closesocket(obj -> client_socket);
    WSACleanup();

    return 0;
}

int get_data(WSA_Socket* obj) {
    if(accept_connections(obj) == INVALID_SOCKET) {
        std::cout << "Accept Error\n";
        closesocket(obj -> listen_socket);
        WSACleanup();

        return 1;
    }

    int data_length = recv(obj -> client_socket, obj -> buffer, BUFF_LEN, 0);
    if(data_length > 0) {
        std::cout << "Received " << data_length << " bytes\n";
        std::cout << "Data: ...\n";
        for(short i = 0; i < data_length; i++) {
            std::cout << obj -> buffer[i];
        }
        std::cout << "\n";

        const char httpData[] = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=iso-8859-1\r\n"
            "Connection: Closed\r\n"
            "\r\n"
            "<!DOCTYPE html>"
            "<html>"
            "<head>"
            "<title>SEEEEMEN</title>"
            "</head>"
            "<body>"
            "<h1>Swallow my cum</h1>"
            "</body>"
            "</html>";

        int res = send(obj -> client_socket, httpData, sizeof(httpData) - 1, 0);
        if(res == SOCKET_ERROR) {
            std::cout << "Send Error\n";
            closesocket(obj -> listen_socket);
            closesocket(obj -> client_socket);
            WSACleanup();

            return 1;
        }
        std::cout << "Sended: " << res << " bytes\n";

        closesocket(obj -> client_socket);

        return 0;
    } else if(data_length == 0) {
        std::cout << "Closing connection\n";
        
        return close_connection(obj);
    } else {
        std::cout << "Receive Error\n";
        closesocket(obj -> listen_socket);
        closesocket(obj -> client_socket);
        WSACleanup();

        return 1;
    }
}

int create_all_sockets(WSA_Socket* obj, const char* ip, const char* port) {
    if(create_wsa(obj) != 0) {
        std::cout << "Create WSA Error\n";

        return 1;
    }
    setup_ipv4(obj);
    if(open_ip_port(obj, ip, port) != 0) {
        std::cout << "Open IP Port Error\n";
        WSACleanup();

        return 1;
    }
    if(create_socket(obj) == INVALID_SOCKET) {
        std::cout << "Create Socket Error\n";
        freeaddrinfo(obj -> operation_result);
        WSACleanup();

        return 1;
    }
    if(create_tcp_socket(obj) == SOCKET_ERROR) {
        std::cout << "Create TCP Socket Error\n";
        std::cout << WSAGetLastError() << "\n";
        freeaddrinfo(obj -> operation_result);
        closesocket(obj -> listen_socket);
        WSACleanup();

        return 1;
    }
    freeaddrinfo(obj -> operation_result);
    if(listen_connections(obj) == SOCKET_ERROR) {
        std::cout << "Listen Error\n";
        closesocket(obj -> listen_socket);
        WSACleanup();

        return 1;
    }
    //closesocket(obj -> listen_socket);

    ZeroMemory(obj -> buffer, sizeof(obj -> buffer));

    return 0;
}
