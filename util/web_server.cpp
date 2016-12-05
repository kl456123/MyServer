#include "web_server.h"



//pass
int ParseUri(char *uri, char *filename, char *cgi_args) {

	char *ptr;

	if (!strstr(uri, "?")) {
		//static content

		//clear all in cgi_args
		strcpy(cgi_args, "");
		strcpy(filename, ".");
		strcat(filename, uri);

		/*
		default main page: "index.html"
		*/
		if (uri[strlen(uri) - 1] == '/')
			strcat(filename, "index.html");
		return 1;

	} else {
		//dynamic content
		ptr = index(uri, '?');

		//slice the string at the character '?'.
		if (ptr) {
			strcpy(cgi_args, ptr + 1);
			*ptr = '\0';
		} else {

			//if ptr = -1(It means for no '?')
			strcpy(cgi_args, "");
		}

		//filename of cgi
		strcpy(filename, ".");
		strcat(filename, uri);
		return 0;
	}
}

void ServeStatic(int file_descriptor, char *filename, int filesize) {
	int src_file_descriptor;
	char *src_pointer, file_type[MAXSIZE], buf[MAXSIZE];
	char file_buf[FILESIZE];

	/*response header*/
	//buf
	GetFileType(filename, file_type);
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
	sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
	sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, file_type);


	/*response body*/
	//file_buf
	src_file_descriptor = open(filename, O_RDONLY);
	if (read(src_file_descriptor, file_buf, filesize) < 0) {
		fprintf(stderr, "filesize is too big (more than FILESIZE)\n");
		return ;
	}
	//concat file_buf with buf
	sprintf(buf, "%s%s", buf, file_buf);

	//send it to client
	//buf
	write(file_descriptor, buf, strlen(buf));

	//close after open file
	close(src_file_descriptor);

}




//determinate file type
void GetFileType(char *filename, char *file_type) {
	if (strstr(filename, ".html")) {
		strcpy(file_type, "text/html");
	} else if (strstr(filename, ".gif")) {
		strcpy(file_type, "image/gif");
	} else if (strstr(filename, ".jpg")) {
		strcpy(file_type, "image/jpeg");
	} else {
		strcpy(file_type, "text/plain");
	}
}


void ServeDynamic(int file_descriptor, char *filename, char *cgi_args) {
	char buf[MAXSIZE], *empty_list[] = {NULL};

	/*first part of HTTP response*/
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
	write(file_descriptor, buf, strlen(buf));

	//fork a child process and exec cgi
	if (fork() == 0) {
		//set all CGI vars here

		setenv("QUERY_STRING", cgi_args, 1);
		//send msg to client directly
		dup2(file_descriptor, STDOUT_FILENO);
		//run CGI program
		execve(filename, empty_list, environ);
	}
	// parent process waits for and reaps child
	wait(NULL);
	return ;
}

//can handle it by redirecting to a static web page
void HandleClientError(int file_descriptor, char *cause, char *err_num,
                       char *short_msg, char *long_msg) {
	char buf[MAXSIZE], body[MAXSIZE];

	/*response body*/
	//body
	sprintf(body, "<html><title>Tiny Error</title>");
	sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
	sprintf(body, "%s%s: %s\r\n", body, err_num, short_msg);
	sprintf(body, "%s<p>%s: %s\r\n", body, long_msg, cause);
	sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

	//for test
	// printf("The line is for test\n");
	/*send response to client*/
	//all are put in buf
	sprintf(buf, "HTTP/1.0 %s %s\r\n", err_num, short_msg);
	sprintf(buf, "%sContent-type: text/html\r\n", buf);
	sprintf(buf, "%sContent-length: %d\r\n\r\n", buf, strlen(body));
	sprintf(buf, "%s%s", buf, body);

	write(file_descriptor, buf, strlen(buf));
	// write(file_descriptor, body, strlen(body));
	return ;
}

//ignore request header
void ReadRequest(int file_descriptor) {
	char buf[MAXSIZE];
	FILE *file_pointer = fdopen(file_descriptor, "rw");
	fgets(buf, MAXSIZE, file_pointer);
	// printf("header: %s\n", buf);
	while (strcmp(buf, "\r\n")) {

		// printf("%s", buf);
		fgets(buf, MAXSIZE, file_pointer);
	}
	return;
}


//main event handler
void HandleRequest(int file_descriptor) {

	//get or post(no support at present)
	char method[MAXSIZE];

	char uri[MAXSIZE];

	//HTTP version(1.0 or 1.1)
	char version[MAXSIZE];

	//store filename
	char filename[MAXSIZE];

	//store cgi args
	char cgi_args[MAXSIZE];

	//store infomation of file
	struct stat sbuf;

	int is_static;
	char buf[MAXSIZE];
	FILE *file_pointer = fdopen(file_descriptor, "rw");

	fgets(buf, MAXSIZE, file_pointer);

	/*read request line and header*/

	//read strings of method like this "GET / HTTP/1.1"
	sscanf(buf, "%s %s %s", method, uri, version);
	if (strcasecmp(method, "GET")) {
		HandleClientError(file_descriptor, method, "501", "Not Implemented",
		                  "Tiny does not implement this method");
		return ;
	}
	//for test
	// printf("method: %s\n", method);
	// read(file_descriptor,MAXSI)
	ReadRequest(file_descriptor);
	//choose how to deal with it
	is_static = ParseUri(uri, filename, cgi_args);

	//for test
	// printf("is_static: %d\n", is_static);
	// //get information of file
	// //return 0 if success otherwise return -1
	if (stat(filename, &sbuf) < 0) {
		HandleClientError(file_descriptor, filename, "404", "Not found",
		                  "Tiny couldn't find this file");
		return ;
	}
	if (is_static) {
		//is regular file or not
		if (!S_ISREG(sbuf.st_mode) || !(S_IRUSR & sbuf.st_mode)) {
			HandleClientError(file_descriptor, filename, "403", "Forbidden",
			                  "Tiny couldn't read the file");
		}
		ServeStatic(file_descriptor, filename, sbuf.st_size);
	}
	else {
		if (!S_ISREG(sbuf.st_mode) || !(S_IXUSR & sbuf.st_mode ) ) {
			HandleClientError(file_descriptor, filename , "403", "Forbidden",
			                  "Tiny couldn't run the CGI program");
			return;
		}

		ServeDynamic(file_descriptor, filename, cgi_args);
	}
}
