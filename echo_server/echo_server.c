#include "../lib/MySocket.h"

void str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE];
again:
	while ((n = read(sockfd, buf, MAXLINE)) > 0)
		Writen(sockfd, buf, n);
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
	{
		perror("read error");
		exit(1);
	}
}

int main(int argc, char **argv)
{
	int listenfd, connfd;
	struct sockaddr_in server;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	server.sin_family = AF_INET;
	server.sin_port = htons(ECHO_SERVER_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(listenfd, (const struct sockaddr *)&server, sizeof(server));

	Listen(listenfd, LISTENQ);

	for (;;)
	{
		connfd = Accept(listenfd, NULL, NULL);
		if (fork() == 0) //enter child process
		{
			Close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		Close(connfd);
	}
}
