#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //malloc
#include <sys/types.h>
#include <sys/socket.h> //socket
#include <sys/ioctl.h> //ioctl
#include <netinet/in.h>
#include <netinet/if_ether.h> //IFT_ETHER
#include <netinet/ip.h>
#include <net/if_arp.h> 
#include <net/if.h>
#include <unistd.h> //close
#include <arpa/inet.h>
#include <errno.h>

#if defined(__APPLE__)

    #include <ifaddrs.h>
    #include <net/if_dl.h>
    #include <net/if_types.h>

#elif defined(__linux__)

    #define  MAX_IFS 20

#endif

#include "include/util.h"


#include "include/network.h"




int getselfip(char ifname[]){
   int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* IPv4のIPアドレスを取得したい */
    ifr.ifr_addr.sa_family = AF_INET;

    /* en0のIPアドレスを取得したい */
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);
    struct in_addr addr =  ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;

    return (int)addr.s_addr;
};

int getselfsubnet(char ifname[]){
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* IPv4のIPアドレスを取得したい */
    ifr.ifr_addr.sa_family = AF_INET;

    /* en0のIPアドレスを取得したい */
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);

    ioctl(fd, SIOCGIFNETMASK, &ifr);

    close(fd);
    struct in_addr addr =  ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;

    return (int)addr.s_addr;
};

#if defined(__APPLE__)

int getmacaddr(char ifname[],unsigned char buf[]){
  struct ifaddrs *ifa_list, *ifa; 
  struct sockaddr_dl *dl; 
  unsigned char *addr;


  if (getifaddrs(&ifa_list) < 0) {
      return 1;
  }
  for (ifa = ifa_list; ifa != NULL; ifa = ifa->ifa_next) { 
      dl = (struct sockaddr_dl*)ifa->ifa_addr; 
      if (dl->sdl_family == AF_LINK && dl->sdl_type == IFT_ETHER) {
          char *name;
          name=(char*)malloc(12);
          memcpy(name, dl->sdl_data, dl->sdl_nlen);
          name[dl->sdl_nlen] = '\0';
          if(strcmp(name,ifname)==0){
            addr = (unsigned char *)LLADDR(dl);
            buf[0] = addr[0];
            buf[1] = addr[1];
            buf[2] = addr[2];
            buf[3] = addr[3];
            buf[4] = addr[4];
            buf[5] = addr[5];
          }
          free(name);
      }
  } 
  freeifaddrs(ifa_list); 
  return 0;
}

int getifname(int idx,char ifname[]){
    struct ifaddrs *ifa_list, *ifa; 
    struct sockaddr_dl *dl; 
    unsigned char *addr;

    int i=0;


    if (getifaddrs(&ifa_list) < 0) {
        return 1;
    }
    for (ifa = ifa_list; ifa != NULL; ifa = ifa->ifa_next) { 
        dl = (struct sockaddr_dl*)ifa->ifa_addr; 
        if (dl->sdl_family == AF_LINK && dl->sdl_type == IFT_ETHER) {
            char *name;
            name=(char*)malloc(12);
            memcpy(name, dl->sdl_data, dl->sdl_nlen);
            name[dl->sdl_nlen] = '\0';
            
            if(i==idx){
                snprintf(ifname,strlen(name)+1,"%s",name);
            }
            free(name);
            ++i;
        }
    } 
    freeifaddrs(ifa_list); 
    return 0;
}

#elif defined(__linux__)

int getmacaddr(char ifname[],unsigned char buf[]){
    struct ifreq *ifr, *ifend;
    struct ifreq ifreq;
    struct ifconf ifc;
    struct ifreq ifs[MAX_IFS];
    int sock;
    unsigned char *addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    ifc.ifc_len = sizeof(ifs);
    ifc.ifc_req = ifs;
    if (ioctl(sock, SIOCGIFCONF, &ifc) < 0) {
	    return 1;
    }

    ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
    for (ifr = ifc.ifc_req; ifr < ifend; ifr++) {
	    if (ifr->ifr_addr.sa_family == AF_INET) {
	        strncpy(ifreq.ifr_name, ifr->ifr_name, sizeof(ifreq.ifr_name));
	        if (ioctl (sock, SIOCGIFHWADDR, &ifreq) < 0) {
	    	    return 1;
	        }
            addr = ifreq.ifr_hwaddr.sa_data;
            if(strcmp(ifreq.ifr_name,ifname)==0){
                buf[0] = addr[0];
                buf[1] = addr[1];
                buf[2] = addr[2];
                buf[3] = addr[3];
                buf[4] = addr[4];
                buf[5] = addr[5]; 
            }
	    }
    }
    return 0;

}

int getifname(int idx,char ifname[]){
    struct ifreq *ifr, *ifend;
    struct ifreq ifreq;
    struct ifconf ifc;
    struct ifreq ifs[MAX_IFS];
    int sock;
    unsigned char *addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    ifc.ifc_len = sizeof(ifs);
    ifc.ifc_req = ifs;
    if (ioctl(sock, SIOCGIFCONF, &ifc) < 0) {
	    return 1;
    }

    int i=0;

    ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
    for (ifr = ifc.ifc_req; ifr < ifend; ifr++) {
	    if (ifr->ifr_addr.sa_family == AF_INET) {
	        strncpy(ifreq.ifr_name, ifr->ifr_name, sizeof(ifreq.ifr_name));
	        if (i==idx) {
	    	    snprintf(ifname,strlen(ifr->ifr_name)+1,"%s",ifr->ifr_name);
	        }
            ++i;
	    }
    }
    return 0;

}

#endif