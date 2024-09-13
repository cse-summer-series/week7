#include "http_server.c"
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 10000

char words[HISTORY_SIZE];
int current_index = 0;

int min(int n, int m) {
    return n > m ? m : n;
}

int words_handler(char* path, char* buffer, size_t size) {
    if(strncmp(path, "/add?", 5) == 0) {
        char* after_q_mark = path + 5;
        if(current_index < HISTORY_SIZE) {
            strncpy(words + current_index, after_q_mark, HISTORY_SIZE - current_index);
            words[current_index + strlen(after_q_mark)] = '\n';
            words[current_index + strlen(after_q_mark) + 1] = '\0';
            current_index += strlen(after_q_mark) + 1;
        }

        snprintf(buffer, size - 2, "%s\n", words);
        return min(strlen(words) + 1, size - 2);
    }
    else if(strlen(path) == 1 && strncmp(path, "/", 1) == 0) {
        snprintf(buffer, size - 2, "%s\n", words);
        return min(strlen(words) + 1, size - 2);
    }
    else {
        sprintf(buffer, "Got a different path: %s\n", path);
        return strlen("Got a different path: ") + 1 + strlen(path);
    }
}

int main(int argc, char **argv) {
    start_server(&words_handler, atoi(argv[1]));
}


/*
 Implement a web server that takes paths that look like

 /add?<somestring>

 and adds them to a running string joined by newlines, and returns the running string.
 
 So if you load these paths you get these corresponding responses:

 /add?hello

 hello


 /add?hi
 
 hello
 hi


 /add?words

 hello
 hi
 words

 You will have to keep track of a big char* array instead of a counter.
*/