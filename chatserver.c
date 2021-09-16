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

#define BACKLOG 10
#define PORT "5555"
#define EXIT "exit"
#define MAXLINE 256 // buffer max number of bytes
#define SERVER "[+] server: "
#define CLIENT "[+] client: "

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

void start_chat (int sockfd) {
	int len, nbytes;
	char sndbuf[MAXLINE];
	char rcvbuf[MAXLINE];	
	
	printf("\n<<<<<<<< Chat Commenced :) >>>>>>>>\n\n");
	
	for(;;) {
		printf(SERVER);
		fgets(sndbuf, MAXLINE, stdin);
		len = strlen(sndbuf);
		
		if (sendall(sockfd, sndbuf, &len) == -1) {
			perror("sendall");
			printf("we only sent %d byte(s) because of the error.", len);
			exit(1);
		}
		
		if (strncmp(sndbuf, EXIT, strlen(EXIT)) == 0) break;
	
		if ((nbytes = recv(sockfd, rcvbuf, MAXLINE-1, 0)) <= 0) {
			if (nbytes == 0) {
				printf("Remote socket hung up!");
			}
			else perror("recv");
			exit(1);
		}
		rcvbuf[nbytes] = '\0';
		
		if (strncmp(rcvbuf, EXIT, strlen(EXIT)) == 0) break;
		
		printf("%s%s\n", CLIENT, rcvbuf);
	}
		
}

// get sockaddr, IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa){
	if(sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void sigchild_handler(int s){
	// waitpid() might overwrite errno, so we save and restore it
	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
}

int main(int argc, char**argv) {
	int listenfd, connfd;
	pid_t childpid;
	struct sockaddr_storage cliaddr;
	socklen_t cliaddrlen;
	struct addrinfo hints, *servinfo, *p;
	struct sigaction sa;
	int yes = 1;
	int rv;
	char s[INET6_ADDRSTRLEN];
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	
	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	// loop through all the results and bind to the first we can
	for(p=servinfo; p!=NULL; p=p->ai_next){
		if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}
		
		if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
		
		if(bind(listenfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(listenfd);
			perror("listenfd");
			continue;
		}
		
		break;
	}
	
	freeaddrinfo(servinfo);
	
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}
	
	if (listen(listenfd, BACKLOG) != 0) {
		perror("server: listen");
		exit(1);
	}
	
	sa.sa_handler= sigchild_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == 1) {
		perror("sigaction");
		exit(1);
	}
	
	printf("%swaiting for connection...\n", SERVER);
	
	for(;;) {
		cliaddrlen = sizeof cliaddr;
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddrlen); 
		
		if(connfd == -1) {
			perror("server: accept");
			continue;
		}
		
		inet_ntop(cliaddr.ss_family, get_in_addr((struct sockaddr *)&cliaddr), s, sizeof s);
		printf("%s got connection from %s\n", SERVER, s);
		
		if((childpid = fork()) == 0) { // this is the child process
			close(listenfd); // child closes listening socket
			start_chat(connfd); // start chatting
			close(connfd); // done with this client
			printf("[!] Client has hung up!\n\n");
			printf("%swaiting for new connection...\n", SERVER);
			exit(0); // child terminates
		}
		
		close(connfd); // parent closes connected host
	}	 
}











