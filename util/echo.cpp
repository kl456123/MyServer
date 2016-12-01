#include "echo.h"

//call echo function when client requests to server 
void echo(int connfd){

	int n;
	char buf[MAXSIZE];
	char hi[]="hello ";
	char ret[MAXSIZE]="";
	// rio_t rio;
	while((n = read(connfd,buf,MAXSIZE)) !=0){
		memset(ret,'\0',MAXSIZE);
		printf("server received %d bytes\n",n);
		strcat(ret,hi);
		strcat(ret,buf);
		printf("buf: %s\n",buf);
		printf("hi: %s\n",hi);
		printf("ret len: %d\n",strlen(ret));
		printf("ret str: %s\n",ret);
		// strcat(ret,"\n");
		// int len = strlen(ret);
		// write(connfd,hi,sizeof(hi));
		write(connfd,ret,n + strlen(hi));
	}
}