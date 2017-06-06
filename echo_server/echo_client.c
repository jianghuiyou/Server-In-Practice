#include "../lib/MySocket.h"

void str_echo(int sockfd)
{
	char sendBuffer[MAXLINE], recvBUFFER[MAXLINE];
	while (fgets(sendBuffer, MAXLINE, stdin) != NULL)
	{
		Writen(sockfd, sendBuffer, strlen(sendBuffer));
		if (Readline(sockfd, recvBUFFER, MAXLINE) == 0)
		{
			printf("server terminated prematurely\n");
			exit(1);
		}
		fputs(recvBUFFER, stdout);
	}
}

int main(int argc, char ** argv)
{
	if (argc != 2)
	{
		printf("usage: ./echo_client <IP Address>\n");
		exit(1);
	}
	int sockfd;
	struct sockaddr_in server;
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	server.sin_family = AF_INET;
	server.sin_port = htons(ECHO_SERVER_PORT);
	Inet_pton(AF_INET, argv[1], &server.sin_addr);

	Connect(sockfd, (const struct sockaddr *)&server, sizeof(server));
	str_echo(sockfd);
	exit(0);
}