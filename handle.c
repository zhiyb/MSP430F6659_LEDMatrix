#include <stdio.h>
#include <string.h>
//#include <sys/socket.h>
#include <unistd.h>
#include "handle.h"

int handle(const int sockid, const int sockfd)
{
	char buffer[256];
	bzero(buffer, 256);
	if (read(sockfd, buffer, 255) < 0) {
		fputs("ERROR reading from socket", stderr);
		goto ret;
	}
	printf("[%d]: %s\n", sockid, buffer);
#if 0
	if (write(newsockfd, "I got your message", 18) < 0)
		error("ERROR writing to socket");
#endif
ret:
	close(sockfd);
	return 1;
}
