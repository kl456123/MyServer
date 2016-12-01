#include "connection.h"

int main(int argc,char **argv){
	int clientfd;
	int port;
	char buf[50];
	int connfd;
	char *host;
	rio_t rio;

	// check legal input
	// exit when argc is not three
	if(argc != 3){
		fprintf(stderr,"usage: %s <host> <port>\n",argv[0]);
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
	clientfd = open_clientfd(host,port);

	if(clientfd < 0){
		error("open_clientfd error");
	}

	// rio_readinitb(&rio,clientfd);
/*
	read buf from stdin and send it to server ,
	get data back to client and display it
	exit when types ctrl-d
*/
	char recbuf[MAXSIZE]={0};

	while(~scanf("%s",buf)){
		// scanf("%s",buf);

		//bug fixed pass '\0'
		write(clientfd,buf,strlen(buf)+1);
		
		read(clientfd,recbuf,MAXSIZE);
		fputs(recbuf,stdout);
		fputs("\n",stdout);
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

	if(rc < 0){
		error("close error");
	}
	exit(0);
}