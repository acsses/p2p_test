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
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <net/if_arp.h> 
#include <net/if.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <errno.h>

#if defined(__APPLE__)

    #include <ifaddrs.h>
    #include <net/if_dl.h>
    #include <net/if_types.h>

#elif defined(__linux__)

    #define  MAX_IFS 20

#endif

#include "util.h"
/*--     include libs     --*/

    
    
    
/*--   prototype declaration    --*/
int getselfip(char ifname[]);

int getselfsubnet(char ifname[]);

int getifname(int idx,char ifname[]);

int getmacaddr(char name[],unsigned char addr[]);
/*--   prototype declaration    --*/


#endif