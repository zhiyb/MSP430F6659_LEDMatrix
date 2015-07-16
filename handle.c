#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "handle.h"

static void sigchldHandler(int signo)
{
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

void initSIGCHLDHandler()
{
	struct sigaction newact;
	newact.sa_handler = sigchldHandler;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGCHLD, &newact, NULL);
}

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
	return 0;
}
