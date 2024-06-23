#include "socket.h"
#define IP "192.168.254.174"
#define PORT "1488"


int main() {
    WSA_Socket wsa_socket = {};
    if(create_all_sockets(&wsa_socket, IP, PORT) != 0) {
        std::cout << "Socket Errors\n";

        return 1;
    }
    std::cout << "Server is live\n";

    HashMap get_map;
    get_map["/"] = "resources/index.html";
    get_map["/favicon.ico"] = "resources/c_icon_132529.ico";
    get_map["/script.js"] = "resources/script.js";
    get_map["/style.css"] = "resources/style.css";
    get_map["/gachi1.jpg"] = "resources/gachi1.jpg";
    get_map["/gachi2.jpg"] = "resources/gachi2.jpg";
    get_map["/gachi3.jpg"] = "resources/gachi3.jpg";
    get_map["/gachi4.jpg"] = "resources/gachi4.jpg";
    HashMap post_map;
    HashMap put_map;
    HashMap delete_map;

    while(true) {
        if(get_data(&wsa_socket, &get_map, &post_map, &put_map, &delete_map) != 0) break;
    }

    return 1;
}