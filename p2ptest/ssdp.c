#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <stdbool.h>
#include <errno.h>
#include <sys/time.h> 

#include "include/timeutil.h"
#include "include/parser.h"

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

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1900);
    addr.sin_addr.s_addr = inet_addr("239.255.255.250");

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(5008);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    char msearch[2048] = {};
    snprintf(msearch, sizeof(msearch),
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

    if(sendto(sock, msearch, sizeof(msearch), 0, (struct sockaddr *)&addr, sizeof(addr))==-1){
        printf("send msearch error:%s\r\n",strerror(errno));
    }

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