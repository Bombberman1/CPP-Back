#include <fstream>
#include "hashmap.h"


const char html_header[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=iso-8859-1\r\n"
    "Connection: Closed\r\n"
    "\r\n";

const char js_header[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: application/javascript\r\n"
    "Connection: Closed\r\n"
    "\r\n";

const char css_header[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/css\r\n"
    "Connection: Closed\r\n"
    "\r\n";

const char favicon_header[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: image/x-icon\r\n"
    "Connection: Closed\r\n"
    "\r\n";

const char jpeg_header[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: image/jpeg\r\n"
    "Connection: Closed\r\n"
    "\r\n";

enum HttpMethods {
    Get,
    Post,
    Put,
    Delete,
    Req
};

enum FileType {
    Html,
    Js,
    Css,
    Ico,
    Jpg,
    Empty
};

int get_binary_file(char* &buff, const char *file_name);
int get_char_file(char* &buff, const char *file_name);
void get_method_route(char* &method, char* &route, char *buff);
HttpMethods parse_method(char *method);
bool types_compare(const char *file_name, const char *str);
FileType get_type(const char *file_name);
void handle_request(char *buff, const char* &buff_headers, int &headers_len, char* &buff_body, int &body_len, HashMap *get_map, HashMap *post_map, HashMap *put_map, HashMap *delete_map);
