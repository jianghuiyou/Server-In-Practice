#include "MySocket.h"

int Socket(int domain, int type, int protocol)
{
	int socketfd;
	if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("create socket failure");
		exit(1);
	}
	return socketfd;
}

void Bind(int sockfd, const struct sockaddr * addr, socklen_t len)
{
	if (bind(sockfd, addr, len) < 0)
	{
		perror("bind server failure");
		exit(1);
	}
}

void Listen(int listenfd, int backlog)
{
	if (listen(listenfd, backlog) < 0)
	{
		perror("listen failure");
		exit(1);
	}
}

void Connect(int sockfd, const struct sockaddr* addr, socklen_t len)
{
	if (connect(sockfd, addr, len) < 0)
	{
		perror("connect failure");
		exit(1);
	}
}

int Accept(int listenfd, struct sockaddr *addr, socklen_t *len)
{
	int connfd;
	if ((connfd = accept(listenfd, addr, len)) < 0)
	{
		perror("server accept failure");
		exit(1);
	}
	return connfd;
}

void Close(int sockfd)
{
	if (close(sockfd) < 0)
	{
		perror("close sockfd failure");
		exit(1);
	}
}

void Inet_pton(int domain, const char *ip, void *addr)
{
	if (inet_pton(domain, ip, addr) < 0)
	{
		perror("inet_pton failure");
		exit(1);
	}
}

ssize_t writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0)
	{
		if ( (nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return (-1);
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return (n);
}

void Writen(int fd, void *ptr, size_t nbytes)
{
	if (writen(fd, ptr, nbytes) != nbytes)
	{
		perror("writen error");
		exit(1);
	}
}


static ssize_t my_read(int fd, char *ptr)
{
	int	read_cnt;
	char	*read_ptr;
	char	read_buf[MAXLINE];
	if (read_cnt <= 0)
	{
again:
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0)
		{
			if (errno == EINTR)
				goto again;
			return (-1);
		}
		else if (read_cnt == 0)
			return (0);
		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return (1);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t	n, rc;
	char	c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++)
	{
		if ( (rc = my_read(fd, &c)) == 1)
		{
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		}
		else if (rc == 0)
		{
			*ptr = 0;
			return (n - 1);	/* EOF, n - 1 bytes were read */
		}
		else
			return (-1);		/* error, errno set by read() */
	}

	*ptr = 0;	/* null terminate like fgets() */
	return (n);
}

ssize_t Readline(int fd, void *ptr, size_t maxlen)
{
	ssize_t n;

	if ( (n = readline(fd, ptr, maxlen)) < 0)
	{
		perror("readline error");
		exit(1);
	}
	return (n);
}