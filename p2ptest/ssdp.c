#include <stdio.h> //printf,snprintf,
#include <string.h> //strerror
#include <stdlib.h> //malloc
#include <sys/types.h> 
#include <sys/socket.h> //socket,connect
#include <sys/ioctl.h> //ioctl
#include <netinet/in.h> //struct sockaddr_in
#include <arpa/inet.h>
#include <unistd.h>//close
#include <errno.h> //errno

#include "include/timeutil.h"
#include "include/parser.h"
#include "include/network.h"

#include  "include/ssdp.h"

int ssdpMsearch(char buf[],int buf_size){
    int sock;
    struct sockaddr_in addr;
    struct sockaddr_in local_addr;
    int flags = 0;
    long long before;
    long long after;
    int d;
 

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    char *ifname;
    ifname = (char*)malloc(128);
    getifname(0,ifname);
    int idx = if_nametoindex(ifname);


    if(setsockopt(sock, IPPROTO_IP, IP_BOUND_IF, &idx, sizeof(idx))!=0){
        printf("set sock opt error\n");
    };
    free(ifname);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1900);
    addr.sin_addr.s_addr = inet_addr("239.255.255.250");

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(1900);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    char *msearch;
    msearch=(char*)malloc(1024);
    snprintf(msearch, 1024,
        "%s"
        "HOST:%s:%d\r\n"
        "MAN:\"ssdp:discover\"\r\n"
        "MX:5\r\n"
        "ST:%s\r\n"
        "USER-AGENT:OS/version product/version\r\n"
        "\r\n",
        "M-SEARCH * HTTP/1.1\r\n",              // HEADER
        "239.255.255.250", 1900, // HOST
        "upnp:rootdevice"         // ST (Search Target)
    );

    if(bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr))==-1){
        printf("socket bind error:%s\r\n",strerror(errno));
    }

    u_long val=1;
    ioctl(sock, FIONBIO, &val);

    if(sendto(sock, msearch, 1024, 0, (struct sockaddr *)&addr, sizeof(addr))==-1){
        printf("send msearch error:%s\r\n",strerror(errno));
    }
    free(msearch);

    before = gettime();
    while (flags==0){
        if((d=recv(sock, buf, buf_size, 0))<1){
            after = gettime();
            if(after-before>5){
                printf("timeout\n");
                flags=-1;
            }
        }else{
            flags=1;
        }
    }

    close(sock);
    if(flags==-1){
        return -1;
    }

    
    return 0;
};