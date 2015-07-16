#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "handle.h"

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int sockid = 0;
	pid_t pid;
	if (argc < 2) {
		fputs("ERROR, no port provided\n", stderr);
		exit(1);
	}
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error("ERROR opening socket");
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(argv[1]));
	const int optVal = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&optVal, sizeof(int));
	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	initSIGCHLDHandler();
	listen(sockfd, 8);
loop:
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	if (newsockfd < 0) {
		//fputs("ERROR on accept\n", stderr);
		goto loop;
	}
	if ((pid = fork()) != 0) {	// Parent
		sockid++;
		close(newsockfd);
		goto loop;
	} else {			// Child
		close(sockfd);
		return handle(sockid, newsockfd);
	}
	close(sockfd);
	return 0;
}
