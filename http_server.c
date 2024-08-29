#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFSIZE 65536
#define VERSION 25

void error(char* err, int code) {
	fprintf(stderr, "%s\n", err);
	exit(code);
}

void handle_connection(int fd, int(*handler)(char*, char*, size_t)) {
   	static char buffer[BUFSIZE+1];
   	static char headerbuffer[1000];  // could be more specifically smaller
   	static char bodybuffer[BUFSIZE+1];

   	long i, ret;

	ret = read(fd,buffer,BUFSIZE);
	if(ret == 0 || ret == -1) {
		error("Read did not return 0", 105);
	}
	if( strncmp(buffer,"GET ",4) && strncmp(buffer,"get ",4) ) {
		error("Request did not start with GET or get", 106);
	}
	for(i=4;i<BUFSIZE;i++) { /* null terminate after the second space to isolate the path */
		if(buffer[i] == ' ') { /* string is "GET URL " +lots of other stuff */
			buffer[i] = 0;
			break;
		}
	}

    char* path = buffer + 4; // Start past the offset of the 'GET ', now null terminated

	// Call into the handler, which will return the size to use
	// (Could also have the protocol be that bodybuffer should be null terminated, depends on learning goals!)
    int amount = (*handler)(path, bodybuffer, sizeof(bodybuffer));

	if(amount > BUFSIZE) {
		error("Amount too large", 107);
	}
	(void)sprintf(headerbuffer,"HTTP/1.1 200 OK\nServer: cse29-server\nContent-Length: %d\nConnection: close\nContent-Type: %s\n\n", amount, "text/plain"); /* Header + a blank line */
	// Write the header bits
	(void)write(fd,headerbuffer,strlen(headerbuffer));
	(void)write(fd,bodybuffer,amount);
	printf("Sent: %s %s", headerbuffer, bodybuffer);
	close(fd);
	return;
}

void start_server(int(*handler)(char*, char*, size_t)) {
    int listenfd, socketfd;
    socklen_t length;
    static struct sockaddr_in cli_addr;
    static struct sockaddr_in serv_addr;
    
   	if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0) {
		error("socket() failed", 101);
	}
    
   	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(8080);
	if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0) {
		error("Bind failed", 102);
	}
	if( listen(listenfd,64) < 0) {
		error("listen failed", 103);
	}

	while(1) {
	    length = sizeof(cli_addr);
		if((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0) {
			error("Accept failed", 104);
		}
		printf("Listening and accepted! %d\n", socketfd);
		fflush(stdout);
		handle_connection(socketfd, handler);
		close(socketfd);
	}

}

