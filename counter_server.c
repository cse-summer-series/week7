#include "http_server.c"
#include <stdlib.h>

int counter = 0;

int example_handler(char* path, char* buffer, size_t size) {
    int amount = sprintf(buffer, "Current: %d", counter);
    counter += 1;
    return amount;
}

int main(int argc, char **argv) {
    start_server(&example_handler);
}