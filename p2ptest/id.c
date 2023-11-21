#include <stdio.h> //printf,sprintf
#include <stdlib.h> //malloc
#include <string.h> //memcpy
#include <sys/socket.h>
#include <net/if.h>

#if defined(__APPLE__)

    #include <ifaddrs.h>
    #include <net/if_dl.h>
    #include <net/if_types.h>

#elif defined(linux)

    #include <sys/ioctl.h>
    #include <netinet/in.h>

    #define  MAX_IFS 20

#endif

#include "include/network.h"
#include "include/sha256.h"

#include "include/id.h"
#include "include/util.h"

#if defined(__APPLE__)

int getid(char buf[]){
    struct ifaddrs *ifa_list, *ifa; 
    struct sockaddr_dl *dl;
    unsigned char *addr;
    unsigned char *src_;
    char ifname_list[10][256];

    src_=(unsigned char*)malloc(128);


    if (getifaddrs(&ifa_list) < 0) {
        return 1;
    }
    long int j=0;

    for (ifa = ifa_list; ifa != NULL; ifa = ifa->ifa_next) { 
        dl = (struct sockaddr_dl*)ifa->ifa_addr; 

        if (dl->sdl_family == AF_LINK && dl->sdl_type == IFT_ETHER) {
            char *name;
            name=(char *)malloc(12);
            memcpy(name, dl->sdl_data, dl->sdl_nlen);
            name[dl->sdl_nlen] = '\0';
            snprintf(ifname_list[j],256,"%s",name);
            free(name);
            ++j;
        }
    } 
    sortchar(ifname_list, j-1);
    for(int i=0;i<j;++i){
        unsigned char *buf;
        buf = (unsigned char *)malloc(6);
        getmacaddr(ifname_list[i],buf);
        for(int k=0;k<6;++k){
            src_[i*6+1+k]=buf[k];
        }
        free(buf);
    }
    freeifaddrs(ifa_list); 
    unsigned char src[j*6];
    for(int l=0;l<j*6+1;++l){
        src[l]=src_[l];
    }
    sha256hex(src,sizeof(src),buf);
    free(src_);
    return 0;
}

#elif defined(linux)

int getid(char buf[]){
    struct ifreq *ifr, *ifend;
    struct ifreq ifreq;
    struct ifconf ifc;
    struct ifreq ifs[MAX_IFS];
    int sock;
    unsigned char *addr;
    unsigned char src_[6*MAX_IFS];
    char ifname_list[10][256];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    ifc.ifc_len = sizeof(ifs);
    ifc.ifc_req = ifs;
    if (ioctl(sock, SIOCGIFCONF, &ifc) < 0) {
	    return 1;
    }
    long int j=0;
    ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
    for (ifr = ifc.ifc_req; ifr < ifend; ifr++) {
	    if (ifr->ifr_addr.sa_family == AF_INET) {
	        strncpy(ifreq.ifr_name, ifr->ifr_name, sizeof(ifreq.ifr_name));
	        if (ioctl (sock, SIOCGIFHWADDR, &ifreq) < 0) {
	    	    return 1;
	        }
            addr = ifreq.ifr_hwaddr.sa_data;
            snprintf(ifname_list[j],256,"%s",ifreq.ifr_name);
            ++j;
	    }
    }
    sortchar(ifname_list, j-1);
    for(int i=0;i<j;++i){
        unsigned char *buf;
        buf = (unsigned char *)malloc(6);
        getmacaddr(ifname_list[i],buf);
        for(int k=0;k<6;++k){
            src_[i*6+1+k]=buf[k];
        }
        free(buf);
    }
    unsigned char src[j*6];
    for(int l=0;l<j*6+1;++l){
        src[l]=src_[l];
    }
    sha256hex(src,sizeof(src),buf);
    return 0;
}

#endif