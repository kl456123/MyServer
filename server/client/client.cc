#include "server/core/connection.h"

//note: every strings must be followed '\0'
int main(int argc, char **argv) {
	int clientfd;
	int port;
	char buf[50];
	int connfd;
	char *host;
	rio_t rio;

	// check legal input
	// exit when argc is not three
	if (argc != 3) {
		fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
		exit(0);
	}

	/*
	get all value from argv
	*/

	//get IP address
	host = argv[1];

	//convert intgrate to string
	port = atoi(argv[2]);

	//create client socket for requesting
	clientfd = open_clientfd(host, port);
	int iMode = 0;
	ioctl(clientfd, FIONBIO, &iMode);

	if (clientfd < 0) {
		error("open_clientfd error");
	}

	// rio_readinitb(&rio,clientfd);
	/*
		read buf from stdin and send it to server ,
		get data back to client and display it
		exit when types ctrl-d
	*/
	int n;
	char recbuf[MAXSIZE] = {0};
	while (~scanf("%s", buf)) {

		//bug fixed pass '\0'
		//problem here
		write(clientfd, buf, strlen(buf) + 1);
		write(clientfd, "client", 7);

		n = read(clientfd, recbuf, MAXSIZE);
		if (n <= 0) {
			printf("error\n");
			return -1;
		}

		printf("recbuf size: %d\n", n);

		//no %s
		write(STDOUT_FILENO, recbuf, n);

		fputs("\n", stdout);
	}



	// while(fgets(buf,MAXSIZE,stdin) != NULL){
	// 	write(clientfd,buf,sizeof(buf));
	// 	fflush((FILE*)&clientfd);
	// 	read(clientfd,buf,MAXSIZE);
	// 	fputs(buf,stdout);
	// 	rio_writen(clientfd,buf,sizeof(buf));
	// 	rio_readnb(&rio,buf,MAXSIZE);
	// 	fputs(buf,stdout);
	// }

	// read(clientfd,buf,sizeof(buf)-1);
	// printf("message %s\n",buf);

	// close flle descriptor of client
	int rc = close(clientfd);

	if (rc < 0) {
		error("close error");
	}
	exit(0);
}
