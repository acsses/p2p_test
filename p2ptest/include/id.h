#ifndef ID_H
#define ID_H
    
    
/*--     include libs     --*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>  
#include <sys/ioctl.h>
#include <ifaddrs.h>

#include "network.h"
#include "sha256.h"
/*--     include libs     --*/
    
    
    
    
/*--   prototype declaration    --*/
int getid(char buf[]);
/*--   prototype declaration    --*/


#endif