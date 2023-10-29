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

#include "include/network.h"
#include "include/sha256.h"

#include "include/id.h"

int getid(char buf[]){
    struct ifaddrs *ifa_list, *ifa; 
    struct sockaddr_dl *dl; 
    unsigned char *addr;
    char name[12];
    unsigned char src_[256];
    if (getifaddrs(&ifa_list) < 0) {
        return 1;
    }
    long int j=0;
    for (ifa = ifa_list; ifa != NULL; ifa = ifa->ifa_next) { 
        dl = (struct sockaddr_dl*)ifa->ifa_addr; 
        if (dl->sdl_family == AF_LINK && dl->sdl_type == IFT_ETHER) {
            memcpy(name, dl->sdl_data, dl->sdl_nlen);
            name[dl->sdl_nlen] = '\0';
            unsigned char *buf;
            buf = (unsigned char *)malloc(6);
            getmacaddr(name,buf);
            for(int i=0;i<6;++i){
                src_[j*6+1+i]=buf[i];
            }
            free(buf);
            ++j;
        }
    } 
    freeifaddrs(ifa_list); 
    unsigned char src[j*6];
    for(int l=0;l<32;++l){
        src[l]=src_[l];
    }
    sha256hex(src,sizeof(src),buf);
    return 0;
}