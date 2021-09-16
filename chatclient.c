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

#define BYE "bye"
#define PORT "5555"
#define MAXDATASIZE 256 // buffer max number of bytes
#define EXIT "exit"
#define SERVER "[+] server: "
#define CLIENT "[+] client: "

char *get_line(){
	char line[MAXDATASIZE];
	return fgets(line, MAXDATASIZE, stdin);
}

int sendall(int s, char *buf, int *len) {
	int total = 0;
	// how many bytes we've sent
	int bytesleft = *len; // how many we have left to send
	int n;
	while(total < *len) {
		n = send(s, buf+total, bytesleft, 0);
		if (n == -1) { break; }
		total += n;
		bytesleft -= n;
	}
	*len = total; // return number actually sent here
	return n==-1?-1:0; // return -1 on failure, 0 on success
}

void show(char *data) {
	printf("[+] %s: ", data);
}

void start_chat (int sockfd, char *name) {
	int len, nbytes;
	char sndbuf[MAXDATASIZE];
	char rcvbuf[MAXDATASIZE];
	
	printf("\n<<<<<<<< Chat Commenced :) >>>>>>>>\n\n");
	
	for(;;){
		if ((nbytes = recv(sockfd, rcvbuf, MAXDATASIZE-1, 0)) <= 0) {
			if (nbytes == 0) {
				printf("[!] Server has hung up!\n");
			}
			else perror("recv");
			exit(1);
		}
		rcvbuf[nbytes] = '\0';
		printf("%s%s", SERVER, rcvbuf);
		
		if (strncmp(rcvbuf, EXIT, strlen(EXIT)) == 0) break;
		
		printf(CLIENT);
		fgets(sndbuf, MAXDATASIZE, stdin);
		len = strlen(sndbuf);
		
		if (sendall(sockfd, sndbuf, &len) == -1) {
			perror("sendall");
			printf("we only sent %d byte(s) because of the error.\n", len);
			exit(1);
		}
	}
}

// get sockaddr, IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	
	if (argc != 3) {
		fprintf(stderr, "usage: chatclient hostname chatname\n");
		exit(1);
	}
	
	char *chatname = argv[2];
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	// loop through all the results and connect to the first we can
	for (p=servinfo; p!=NULL; p=p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}
	
	if (p==NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}
	
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
	printf("%sconnecting to %s\n", CLIENT, s);
	
	freeaddrinfo(servinfo);
	
	start_chat(sockfd, chatname);	
	close(sockfd);
	
	printf("\n<<<<<<<< Chat Ended :( >>>>>>>>\n\n");
	
	return 0;
	
	
	
	
	
	
	
	
	
	
	
	
	
}
