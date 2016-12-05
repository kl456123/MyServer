#include "web_server.h"
#include "connection.h"

int main(int argc, char **argv) {
	int listenfd , connfd, port, clientlen;
	struct sockaddr_in clientaddr;

	//check args
	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}

	port = atoi(argv[1]);

	listenfd = open_listenfd(port);
	while (1) {
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (SA*)&clientaddr, (socklen_t*)&clientlen);
		HandleRequest(connfd);
		close(connfd);
	}
	close(listenfd);
}