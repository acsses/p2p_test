#ifndef ID_H
#define ID_H
    
    
/*--     include libs     --*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>  
#include <sys/ioctl.h>

#if defined(__APPLE__)

    #include <ifaddrs.h>
    #include <net/if_dl.h>
    #include <net/if_types.h>

#elif defined(__linux__)

    #define  MAX_IFS 20

#endif

#include "network.h"
#include "sha256.h"
/*--     include libs     --*/
    
    
    
    
/*--   prototype declaration    --*/
int getid(char buf[]);
/*--   prototype declaration    --*/


#endif