#include "server/core/connection.h"


int open_clientfd(char* hostname, int port) {
    //first create value for using(e,g file descriptor and IP address)
    int clientfd;

    //DNS host entry structure
    /*
    struct hostent{
    char* h_name; // official domain name of list
    char** h_aliases; //null-terminated array of domain names
    int h_addrtype; //host address type (AF_INET)
    int h_length; //length of an address , in bytes
    char** h_addr_list; //null-terminated array of in_addr structs
    }
    And
    struct in_addr{
    unsigned int s_addr;//network byte order (big-endian)
    }

    */
    struct hostent *hp;

    //note: network byte order is big-endian

    /*
    struct sockaddr_in{
    unsigned short sin_family;//address family (always AF_INET)
    unsigned short sin_port; //port number in network byte order
    struct in_addr sin_addr; //IP address in network byte order
    unsigned char sin_zero[8]; //for padding
    }
    */

    struct sockaddr_in serveraddr;
    //clear all in sockaddr_in
    memset((char*)&serveraddr, 0, sizeof(serveraddr));

    //get file descriptor of socket(default is client)
    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd < 0) {
        return -1;
    }

    /*
    fill in the server's IP address and port

    */

    //first get DNS entry according to hostname
    hp = gethostbyname(hostname);

    //check hp if it is NULL
    if (hp == NULL) {
        return -2;
    }

    serveraddr.sin_family = AF_INET;

    //htons convert host port to network (big-endian)
    serveraddr.sin_port = htons(port);

    //set IP address
    //get the first address in the list
    serveraddr.sin_addr.s_addr = *(unsigned int*)hp->h_addr_list[0];

    // establish a connection with the server
    //client fd connects to server by IP
    //must convert SA type
    //return 0 when success otherwise return -1
    if (connect(clientfd, (SA*)&serveraddr, sizeof(serveraddr)) < 0) {
        return -1;
    }

    //return client file descriptor
    return clientfd;
}


/*
server listen some port that is binded

*/
int open_listenfd(int port) {

    /*
    create a socket (due to it is client socket by default,
    so convert it to server socket.
    */
    int serverfd;

    struct sockaddr_in serveraddr;

    //now it is just client by default,then call listen
    //function convert it
    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    //return -1 when creating socket failed
    if (serverfd < 0) {
        return -1;
    }

    //create serveraddr,clear all and set value like before

    memset((char*)&serveraddr, 0, sizeof(serveraddr));

    serveraddr.sin_port = htons(port);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htons(INADDR_ANY);

    //bind server IP address and port
    //return -1 when failed
    if (bind(serverfd, (SA*)&serveraddr, sizeof(serveraddr)) < 0) {
        return -1;
    }

    //convert client to server and be ready
    //to accept connection requests
    //can compare it with connect when fd is client socket
    if (listen(serverfd, LISTENQ) < 0) {
        return -1;
    }


    return serverfd;

}

void error(const char *s) {

    fprintf(stderr, "%s\n", s);
    exit(0);
}

/*robust IO function*/

// no buffer(read data from file to memorry directly)

//note ssize_t means for signed int while size_t means for
//unsigned int

//wraped read function
ssize_t rio_readn(int fd, char *usrbuf, size_t n) {

    //left bytes to read
    size_t nleft = n;

    //the number of bytes read at one time
    ssize_t nread;

    char *bufp = usrbuf;

    //restart read function when be interrupted
    //by sig handler return
    while (nleft > 0) {

        if ((nread = read(fd, bufp, nleft)) < 0) {
            //if it is interrupted
            if (errno == EINTR) {
                //reset 0 and read again later
                nread = 0;
            } else {
                //error
                return -1;
            }
        } else if (nread == 0) {
            break;//break the loop when EOF
        }
        //less the nleft
        nleft -= nread;

        //foward the bufp
        bufp += nread;

    }
    return (n - nleft);
}

//wraped write function

ssize_t rio_writen(int fd, char *usrbuf, size_t n) {

    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nwritten == write(fd, bufp, nleft)) < 0) {
            if (errno == EINTR) {
                nwritten = 0;
            } else {
                return -1;
            }
        }
        nleft -= nwritten;
        bufp += nwritten;
    }

    return n;

}

/*
rio_t is a struction for reading data from buffer

*/



//initiate rio_t by using fd
void rio_readinitb(rio_t *rp, int fd) {

    rp->rio_fd = fd;

    rp->rio_cnt = 0;

    //reset bufptr
    rp->rio_bufptr = rp->rio_buf;

}



//with buffer
/*
the rio_read can just read the number of RIOBUFSIZE bytes
And it can read at many times
functions of rio_read() is the same as those of read()
*/
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
    int cnt;

    //when rp->rio_cnt = 0 ,refill the internal buffer by
    //getting data from fd

    //1.when none for reading from internal buffer
    //2.when interrupted by signal handler rio_cnt = -1
    while (rp->rio_cnt <= 0) {

        /*
        read from fd to internal buffer
        */
        //return the number of bytes read successfully
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, RIOBUFSIZE);

        if (rp->rio_cnt < 0) {
            if (errno != EINTR) {
                //error and cannot recover
                return -1;
            }
        } else if (rp->rio_cnt == 0) {
            //EOF
            return 0;
        } else {
            //reset bufptr and read buffer from the start position
            rp->rio_bufptr = rp->rio_buf;
        }
    }
    /*
    read(copy) from buffer to usrbuf
    */

    //min(n,rp->rio_cnt)
    cnt = n;
    if (cnt > rp->rio_cnt) {
        cnt = rp->rio_cnt;
    }

    memcpy(usrbuf, rp->rio_bufptr, cnt);

    //inc the bufptr
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;

}

/*
rio_readnb is like as rio_readn other than using rio_read
*/

ssize_t rio_readnb(rio_t *rp, char *usrbuf, size_t n) {
    //left bytes to read
    size_t nleft = n;

    //the number of bytes read at one time
    ssize_t nread;

    char *bufp = usrbuf;

    //restart read function when be interrupted
    //by sig handler return
    while (nleft > 0) {

        if ((nread = rio_read(rp, bufp, nleft)) < 0) {
            //if it is interrupted
            if (errno == EINTR) {
                //reset 0 and read again later
                nread = 0;
            } else {
                //error
                return -1;
            }
        } else if (nread == 0) {
            break;//break the loop when EOF
        }
        //less the nleft
        nleft -= nread;

        //foward the bufp
        bufp += nread;

    }
    return (n - nleft);
}


//forbiding using %s format to read or write in socket programming

ssize_t PrintToScreen(const char *buf, size_t n) {
    //STDOUT_FILENO is stdin's file descriptor
    ssize_t num_write = write(STDOUT_FILENO, buf, n);
    if (num_write <= 0) {
        error("print to screen error");
    }
    return num_write;
}

//str should be a string
int StringSizeByBytes(const char *str) {
    int string_size_bybytes = strlen(str) + 1;
    return  string_size_bybytes;
}


