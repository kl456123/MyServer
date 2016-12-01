#ifndef CONNECTION_H_INCLUDED
#define CONNECTION_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define LISTENQ 1024
#define MAXSIZE 100
#define RIOBUFSIZE 1024
typedef struct sockaddr SA;

int open_clientfd(char* hostname,int port);

int open_listenfd(int port);

void error(char *msg);

struct rio_t{
    //fd of rio version
    int rio_fd;

    //unread bytes in internal buffer
    int rio_cnt;

    //next unread byte
    char *rio_bufptr;

    //internal buffer
    char rio_buf[RIOBUFSIZE];

};



void rio_readinitb(rio_t *rp,int fd);

static ssize_t rio_read(rio_t *rp,char *usrbuf,size_t n);

ssize_t rio_readnb(rio_t *rp,char *usrbuf,size_t n);

ssize_t rio_writen(int fd,char *usrbuf,size_t n);

ssize_t rio_readn(int fd,char *usrbuf,size_t n);

#endif // CONNECTION_H_INCLUDED
