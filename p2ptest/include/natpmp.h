#ifndef NATPMP_H
#define NATPMP_H
    
    
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <errno.h>
    
    
    
    
int requestNatpmp(unsigned char buf[],char addres[],short int ex_port, short int in_port,int ttl);


#endif