#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  
#include <errno.h>

#include "include/util.h"
#include "include/network.h"

#include "include/natpmp.h"

int requestNatpmp(unsigned char buf[],char addres[],short int ex_port, short int in_port,int ttl){
    int sock;
    struct sockaddr_in addr;
    struct sockaddr_in local_addr;
    char *message;
    int size;
    long long before;
    long long after;
    int flags = 0;
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
    addr.sin_port = htons(5351);
    addr.sin_addr.s_addr = inet_addr(addres);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(5350);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    
    if(ex_port>=0 && in_port>=0 && ttl>=0){
        message = (char*)malloc(12);
    
        message[0]=0;
        message[1]=2;

        message[4]=(in_port >> 8) & 0xFF;
        message[5]=(in_port >> 0) & 0xFF;

        message[6]=(ex_port >> 8) & 0xFF;
        message[7]=(ex_port >> 0) & 0xFF;

        message[8]=(ttl >> 24) & 0xFF;
        message[9]=(ttl >> 16) & 0xFF;
        message[10]=(ttl >> 8) & 0xFF;
        message[11]=(ttl >> 0) & 0xFF;

        size = 12;
    }else{
        message = (char*)malloc(2);

        message[0]=0;
        message[1]=0;

        size = 2;

    }


    if(bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr))==-1){
        printf("socket bind error:%s\r\n",strerror(errno));
    }

    u_long val=1;
    ioctl(sock, FIONBIO, &val);

    if(sendto(sock, message, size, 0, (struct sockaddr *)&addr, sizeof(addr))==-1){
        printf("send error:%s\r\n",strerror(errno));
    }

    before = gettime();
    while (flags==0){
        if((d=recv(sock, buf, 2048, 0))<1){
            after = gettime();
            if(after-before>5){
                printf("timeout\n");
                flags=-1;
            }
        }else{
            flags=1;
        }
    }

    if(flags==-1){
        close(sock);
        return -1;
    }

    close(sock);
    
    return 0;
};
