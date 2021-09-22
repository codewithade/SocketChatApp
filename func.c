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

// Uses the getsockname() function to get the
// socket address family from the socket file descriptor (sockfd)
int sockfd_to_family(int sockfd) {
	struct sockaddr_storage ss;
	socklen_t len;
	
	len = sizeof ss;
	if (getsockname(sockfd, (struct sockaddr *)&ss, &len) < 0)
		return -1;
	return (ss.ss_family);
}



	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
