#ifndef SSDP_H
#define SSDP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <stdbool.h>
#include <errno.h>

int ssdpMsearch(char buf[],int buf_size);

#endif