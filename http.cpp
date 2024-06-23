#include "http.h"


int get_binary_file(char* &buff, const char *file_name) {
    std::ifstream file(file_name, std::ios::binary);
    if(!file) return -1;

    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);

    buff = new char[size + 1];
    file.read(buff, size);

    file.close();

    return size;
}

int get_char_file(char* &buff, const char *file_name) {
    std::ifstream file(file_name);
    if(!file) return -1;

    file.seekg(0, std::ios::end);
    int full_size = file.tellg();
    file.seekg(0, std::ios::beg);

    buff = new char[full_size + 1];
    char ch;
    int size = 0;
    while(file.get(ch)) {
        if(ch != '\t' && ch != '\r' && ch != '\n') {
            buff[size] = ch;
            size++;
        }
    }

    file.close();

    return size;
}

void get_method_route(char* &method, char* &route, char *buff) {
    char method_len = 0;
    for(; buff[method_len] != ' '; method_len++);
    method = new char[method_len + 1];
    for(char i = 0; i < method_len; i++) {
        method[i] = buff[i];
    }
    method[method_len] = 0;

    char route_len = method_len + 1;
    for(; buff[route_len] != ' '; route_len++);
    route = new char[route_len - method_len];
    for(char i = method_len + 1; i < route_len; i++) {
        route[i - method_len - 1] = buff[i];
    }
    route[route_len - method_len - 1] = 0;
}

HttpMethods parse_method(char *method) {
    if(method[0] == 'G') return HttpMethods::Get;
    else if(method[0] == 'P' && method[1] == 'O') return HttpMethods::Post;
    else if(method[0] == 'P' && method[1] == 'U') return HttpMethods::Put;
    else if(method[0] == 'D') return HttpMethods::Delete;

    return HttpMethods::Req;
}

bool types_compare(const char *file_name, const char *str) {
    short offset = 0;
    for(; file_name[offset] != '.'; offset++) {
        if(file_name[offset] == 0) return false;
    }
    short i = offset + 1;
    for(; file_name[i] != 0 && str[i - offset - 1] != 0; i++) {
        if(file_name[i] != str[i - offset - 1]) return false;
    }
    
    return (file_name[i] == 0 && str[i - offset - 1] == 0);
}

FileType get_type(const char *file_name) {
    if(types_compare(file_name, "html")) return FileType::Html;
    else if(types_compare(file_name, "css")) return FileType::Css;
    else if(types_compare(file_name, "js")) return FileType::Js;
    else if(types_compare(file_name, "ico")) return FileType::Ico;
    else if(types_compare(file_name, "jpg")) return FileType::Jpg;

    return FileType::Empty;
}

void handle_request(char *buff, const char* &buff_headers, int &headers_len, char* &buff_body, int &body_len, HashMap *get_map, HashMap *post_map, HashMap *put_map, HashMap *delete_map) {
    char *method = nullptr;
    char *route = nullptr;
    get_method_route(method, route, buff);

    switch(parse_method(method)) {
        case HttpMethods::Get:
            if(get_map -> is_key(route)) {
                const char *file_name = (*get_map)[route].get_value();
                switch(get_type(file_name)) {
                    case FileType::Html:
                        buff_headers = html_header;
                        headers_len = sizeof(html_header) - 1;
                        body_len = get_char_file(buff_body, file_name);
                        break;
                    case FileType::Css:
                        buff_headers = css_header;
                        headers_len = sizeof(css_header) - 1;
                        body_len = get_char_file(buff_body, file_name);
                        break;
                    case FileType::Js:
                        buff_headers = js_header;
                        headers_len = sizeof(js_header) - 1;
                        body_len = get_char_file(buff_body, file_name);
                        break;
                    case FileType::Ico:
                        buff_headers = favicon_header;
                        headers_len = sizeof(favicon_header) - 1;
                        body_len = get_binary_file(buff_body, file_name);
                        break;
                    case FileType::Jpg:
                        buff_headers = jpeg_header;
                        headers_len = sizeof(jpeg_header) - 1;
                        body_len = get_binary_file(buff_body, file_name);
                        break;
                }
            }
            break;
        default:
            break;
    }
    delete[] method;
    delete[] route;
}
