#include "server/web_server/web_server.h"
#include "server/core/connection.h"
#include "server/core/config_parser.h"

int main(int argc, char **argv) {
	int listenfd , connfd, port, clientlen;
	struct sockaddr_in clientaddr;

	//check args
	if (argc < 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}
    char config_file[MAX_STRING_SIZE] = "../config/default.yml";
    if(argc>=3){
        strcpy(config_file, argv[2]);
    }

    // init config
    ServerConfig* server_config = GetGlobalConfig();
    ParseConfig(config_file, server_config);

	port = atoi(argv[1]);

	listenfd = open_listenfd(port);
	while (1) {
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (SA*)&clientaddr, (socklen_t*)&clientlen);
        if(connfd==-1){
            printf("accept error due to fd: %d\n", connfd);
            continue;
        }
		HandleRequest(connfd);
		close(connfd);
	}
	close(listenfd);
}
