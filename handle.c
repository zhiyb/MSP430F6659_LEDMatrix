// Author: Yubo Zhi (normanzyb@gmail.com)

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "handle.h"
#include "socket.h"

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

int handle(const int id, const int sockfd)
{
	char buffer[256];
	uint32_t cnt = 0;
	ssize_t size;
	struct tm *t;
loop:
	bzero(buffer, 256);
	size = socket_readline(sockfd, buffer, sizeof(buffer));
	if (size == 0)
		goto ret;
	else if (size < 0) {
		fputs("ERROR reading from socket\n", stderr);
		goto ret;
	}
	printf("[%d/%d]: %s\n", id, cnt, buffer);
	if (strcmp(buffer, "Time") == 0) {
		time_t tm = time((time_t *)NULL);
		t = localtime(&tm);
		printf("[%d/%d]# Time: %04d-%02d-%02d %01d %02d:%02d:%02d\n", \
			id, cnt, 1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday,\
			t->tm_wday, t->tm_hour, t->tm_min, t->tm_sec);
	} else if (strcmp(buffer, "TIME") == 0) {
		time_t tm = time((time_t *)NULL);
		t = localtime(&tm);
		char tdata[8];
		tdata[0] = (1900 + t->tm_year) >> 8;
		tdata[1] = (1900 + t->tm_year) & 0xFF;
		tdata[2] = 1 + t->tm_mon;
		tdata[3] = t->tm_mday;
		tdata[4] = t->tm_wday;
		tdata[5] = t->tm_hour;
		tdata[6] = t->tm_min;
		tdata[7] = t->tm_sec;
		socket_write(sockfd, tdata, sizeof(tdata));
	}
	cnt++;
	goto loop;
ret:
	close(sockfd);
	return 0;
}
