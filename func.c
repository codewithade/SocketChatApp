#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE 100

int sockfd_to_family(int sockfd) {
	struct sockaddr_storage ss;
	socklen_t len;
	
	len = sizeof ss;
	if (getsockname(sockfd, (struct sockaddr *)&ss, &len) < 0)
		return -1;
	return (ss.ss_family);
}

// reads a max of 19 chars from command line
char *get_line(){
	int numbytes = 80;
	char line[numbytes];
	return fgets(line, numbytes, stdin);
}

int get_char(char *buf, int size) {
	int i = 0;
	char c;
	char sentence[size];
	while((i<size-1) && (c = getchar()) != '\n') {
		sentence[i++] = c;
	}
	sentence[i] = '\0';
	buf = sentence;
	return 0;
}

int main(int argc, char *argv[]) {
	char data[20];
	fgets(data, 20, stdin);
	printf("Data: %s", data);
	fgets(data, 20, stdin);
	printf("Data: %s", data);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
