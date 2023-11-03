#ifndef HTTP_H
#define HTTP_H

#include <stdio.h> //printf,sprintf
#include <string.h> //memset,strcat
#include <stdlib.h> //malloc
#include <sys/types.h> //struct addrinfo
#include <sys/socket.h> //socket,connect
#include <sys/ioctl.h> //ioctl
#include <netdb.h> //getaddrinfo,
#include <netinet/in.h> //struct sockaddr_in
#include <arpa/inet.h> //<------------------------------------------
#include <unistd.h> //close
#include <errno.h> //errno
#include <net/if.h> //<------------------------------------------
#include <openssl/ssl.h>
#include <openssl/err.h>


int requestHttpGET(char buff[], int buf_size,char * URL,int is_local);

int requestHttpsGET(char buff[], int buf_size,char * URL);

int requestHttpPOST(char buff[], int buf_size,char * URL,int is_local,char header[],char body[]);

int requestHttpsPOST(char buff[], int buf_size,char * URL,char header[],char body[]);

#endif