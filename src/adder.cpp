#include "connection.h"

int main() {


	char *buf, *p;
	char arg1[MAXSIZE], arg2[MAXSIZE], content[MAXSIZE];
	int n1 = 0;
	int n2 = 0;

	/*extract two number*/
	if ((buf = getenv("QUERY_STRING")) != NULL) {

		//return the pointer to the first occurrence of the character c
		p = strchr(buf, '&');
		*p = '\0';
		strcpy(arg1, buf);
		strcpy(arg2, p + 1);
		//convert string to int
		n1 = atoi(arg1);
		n2 = atoi(arg2);
	}

	/*response body*/
	sprintf(content, "Welcome to add.com ");
	sprintf(content, "%sTHE Internet addtion portal.\r\n<p>", content);
	sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>",
	        content, n1, n2, n1 + n2);
	sprintf(content, "%sThanks for visiting!\r\n", content);


	/*genernate HTTP response(other than some response header)*/
	printf("Content-length: %d\r\n", strlen(content));

	//response header ends here
	printf("Content-type: text/html\r\n\r\n");

	//response body
	printf("%s", content);

	fflush(stdout);
	exit(0);

}

