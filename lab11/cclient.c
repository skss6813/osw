#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define DEFAULT_PROTOCOL 0
#define MAXLINE 100

int readLine(int, char*);

int main() {
	int clientfd, result;
	char inmsg[MAXLINE], outmsg[MAXLINE];
	struct sockaddr_un serverAddr;

	clientfd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
	serverAddr.sun_family = AF_UNIX;
	strcpy(serverAddr.sun_path, "convert");

	do {
		result = connect(clientfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
		if (result == -1) sleep(1);
	} while (result == -1);

	printf("변환할 문자열 입력 : ");
	fgets(inmsg, MAXLINE, stdin);
	write(clientfd, inmsg, strlen(inmsg)+1);

	readLine(clientfd, outmsg);
	printf("%s --> \n%s", inmsg, outmsg);
	close(clientfd);
	exit(0);
}

int readLine(int fd, char* str) {
	int n;
	do {
		n = read(fd, str, 1);
	} while ( n>0 && *str++ != '\0');
	return ( n>0);
}
