#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/param.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>


int requestHttpGET(char buff[], int buf_size,char * URL,int is_local);

int requestHttpsGET(char buff[], int buf_size,char * URL);

int requestHttpPOST(char buff[], int buf_size,char * URL,int is_local,char header[],char body[]);

int requestHttpsPOST(char buff[], int buf_size,char * URL,char header[],char body[]);

#endif