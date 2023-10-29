#ifndef NETWORK_H
#define NETWORK_H
    
    
/*--     include libs     --*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
/*--     include libs     --*/

    
    
    
/*--   prototype declaration    --*/
int getselfip();

int getselfsubnet();

int getmacaddr(char name[],unsigned char addr[]);
/*--   prototype declaration    --*/


#endif