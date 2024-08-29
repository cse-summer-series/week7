#include "http_server.c"
#include <stdlib.h>

char LIST[10000];
int cur_index = 0;

int list_handler(char* path, char* buffer, size_t size) {
    int added = strlen(path) - 5; // everything after the ?
    if(strncmp(path, "/add?", 5) == 0) {
        strncpy(LIST + cur_index, path + 5, added);
        LIST[cur_index + added] = '\n';
        cur_index += added + 1;
        memcpy(buffer, LIST, cur_index);
        return cur_index;
    }
    else {
        char message[] = "Path must begin with '/path?'";
        memcpy(buffer, message, strlen(message));
        return strlen(message);
    }

}

int main(int argc, char **argv) {
    start_server(&list_handler);
}