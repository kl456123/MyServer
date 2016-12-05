#include "echo.h"

//call echo function when client requests to server
void echo(int connfd) {

	int n;
	char buf[MAXSIZE];
	char hi[] = "hello ";
	char ret[MAXSIZE] = "";
	char test[MAXSIZE] = "This line is for test";
	int hi_bytes_length = StringSizeByBytes(hi);
	int ret_bytes_length = 0;
	// rio_t rio;
	while ((n = read(connfd, buf, MAXSIZE)) != 0) {
		memset(ret, '\0', sizeof(ret));


		// printf("server received %d bytes\n", n);

		memcpy(ret, hi, hi_bytes_length);
		memcpy(ret + hi_bytes_length, buf, n);
		ret_bytes_length = hi_bytes_length + n;

		//no suggest %s
		PrintToScreen(buf, n);
		PrintToScreen("\n", StringSizeByBytes("\n"));

		printf("ret len: %d\n", strlen(ret));
		PrintToScreen("ret str: ", StringSizeByBytes("ret str: "));

		PrintToScreen(ret, ret_bytes_length);

		PrintToScreen("\n", StringSizeByBytes("\n"));

		//warn '\0'
		write(connfd, ret, ret_bytes_length);
		write(connfd, "\n", StringSizeByBytes("\n"));
		write(connfd, test, StringSizeByBytes(test));
	}
}