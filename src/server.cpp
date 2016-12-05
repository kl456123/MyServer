#include "connection.h"
#include "echo.h"

int main(int argc, char **argv) {
	/*
	argv[1]:port
	*/

	//first check input argv
	if (argc != 2) {
		fprintf(stderr, "uasge: %s <port>\n", argv[0]);
		exit(0);
	}

	// char s[]="hello world";
	char s[MAXSIZE];
	//get client message
	struct sockaddr_in clientaddr;

	// struct

	//run all the time as long as the server socket is not closed
	int listenfd;

	//for one connection and close when client socket is closed
	int connfd;

	int iMode = 0;


	//port to bind
	int port;
	port = atoi(argv[1]);

	//create listenfd
	listenfd = open_listenfd(port);
	if (listenfd < 0) {
		error("open_listenfd error");
	}

	//len of clientaddr
	int clientlen;

	while (1) {

		clientlen = sizeof(clientaddr);

		//must convert to communication format
		//the last arg must be a pointer for reduce coping(I think so)
		// and it returns temp socket for connecting with client
		connfd = accept(listenfd, (SA*)&clientaddr, (socklen_t*)&clientlen);
		ioctl(connfd, FIONBIO, &iMode);
		if (connfd < 0) {
			error("accept error");
		}
		// if(connfd == -1){
		// 	fprintf(stderr,"accept error\n");
		// 	exit(-1);
		// }

		/*		if(connfd < 0){
					continue;
				}*/


		/*determinate client message including its IP address and hostname */

		printf("server connected to client\n");

		/*success*/
		// int len = read(connfd,s,MAXSIZE);
		// strcat(s,"asfaf");
		// 	write(connfd,s,len);

		echo(connfd);

		int rc = close(connfd);

		//if error
		if (rc < 0) {
			error("close error");
		}
	}

	close(listenfd);

	exit(0);


}
