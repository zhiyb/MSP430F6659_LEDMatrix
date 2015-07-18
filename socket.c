// Author: Yubo Zhi (normanzyb@gmail.com)

#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"

ssize_t socket_readline(const int sockfd, void *buf, size_t len)
{
	ssize_t size = 0;
	char *ptr = (char *)buf;
start:
	while (len != size) {
		char c;
		ssize_t sz = recv(sockfd, &c, 1, 0);
		if (sz == 0)
			continue;
		else if (sz < 0)
			return sz;
		else if (sz > 1)
			return -1;
		switch (c) {
		case '\0':
		case '\r':
		case '\n':
			if (size == 0)
				goto start;
			*ptr = '\0';
			return size;
		default:
			*ptr = c;
		}
		ptr++;
		size++;
	}
	return size;
}

ssize_t socket_write(const int sockfd, const void *buf, size_t len)
{
	ssize_t size = len;
	const unsigned char *ptr = (const unsigned char *)buf;
	while (size != 0) {
		ssize_t sz = send(sockfd, ptr, size, 0);
		if (sz == 0)
			continue;
		else if (sz < 0)
			return sz;
		ptr += sz;
		size -= sz;
	}
	return len;
}
