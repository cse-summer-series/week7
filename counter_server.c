#include "http_server.c"
#include <stdlib.h>
#include <string.h>

int counter = 0;

int example_handler(char* path, char* buffer, size_t size) {
    if(strncmp(path, "/inc", 4) == 0) {
        counter += 1;
        int amount = sprintf(buffer, "Incremented to: %d\n", counter);
        return amount;
    }
    else if(strncmp(path, "/show", 5) == 0) {
        int amount = sprintf(buffer, "Current value: %d\n", counter);
        return amount;
    }
    else {
        int amount = snprintf(buffer, size, "Use /inc or /show, invalid path: %s\n", path);
        return amount;
    }
}

int main(int argc, char **argv) {
    start_server(&example_handler, 8082);
}