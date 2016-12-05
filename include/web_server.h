#ifndef WEB_SERVER_H_INCLUDED
#define WEB_SERVER_H_INCLUDED

#include "connection.h"
#define FILESIZE 102400

//utils function for common

/*
web server parse uri to static content or cgi file.
return 0 when it is cgi,otherwise return 1
*/
int ParseUri(char *uri, char *filename, char *cgi_args);



// static serve
void ServeStatic(int file_descriptor, char *filename, int filesize);


//dynamic serve
void ServeDynamic(int file_descriptor, char *filename, char *cgi_args);

//error handler
void HandleClientError(int file_descriptor, char *cause, char *err_num,
                       char *short_msg, char *long_msg);


//request handler
void HandleRequest(int file_descriptor);

void ReadRequest(int file_descriptor);

void GetFileType(char *filename, char *file_type);

#endif
