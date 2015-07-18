// Author: Yubo Zhi (normanzyb@gmail.com)

#ifndef _SOCKET_H
#define _SOCKET_H

ssize_t socket_readline(const int sockfd, void *buf, size_t len);
ssize_t socket_write(const int sockfd, const void *buf, size_t len);

#endif
