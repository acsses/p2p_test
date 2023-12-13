#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include "include/portmapping.h"
#include "include/broadcast.h"
#include "include/node.h"

#include "include/init.h"

#define CONNECTION_PORTMAPPED 0
#define CONNECTION_PORTMAPPPED_LIMITTED 1
#define CONNECTION_NOT_PORTMAPPPED 2

int initConnection(short int ex_port,Node * self,NodeStack * nodelist){
    char buf[2048];
    unsigned char gip[4];
    int statuscode;

    int status;
    FILE* fp; 

    fp = fopen("table.json", "r"); 
    fread(buf, 1, 2048, fp);
    fclose(fp);

    if(fp == NULL){
        fprintf(stderr, "Error: file not opened.\n");
        return 1;
    }

    printf("starting port mapping by UPnP...\n");
    if((status=UpnpPortmapping(ex_port,gip))<0){
        printf("port mapping by UPnP failed\n\n");
        printf("starting port mapping by NAT-PMP...\n");
        if ((status=NatpmpPortmapping(ex_port,gip))!=0){
            printf("port mapping by NAT-PMP failed\n\n");
            statuscode = CONNECTION_NOT_PORTMAPPPED;
            getselfNode(self,statuscode,gip);



            brocasReqNodeList("table.json",statuscode,nodelist,NULL);
            
            return -1;
        }else{
            statuscode = CONNECTION_PORTMAPPED;

            char addr[32];
            snprintf(addr,sizeof(addr),
                "%d.%d.%d.%d:%d",
                gip[0],
                gip[1],
                gip[2],
                gip[3],
                ex_port
            );

            getselfNode(self,statuscode,addr);
            
            brocasReqNodeList("table.json",statuscode,nodelist,addr);
            printf("test for fxxkin seg fau");

            return initSock(ex_port);
        }

    }else{
        char addr[32];
        snprintf(addr,sizeof(addr),
            "%d.%d.%d.%d:%d",
            gip[0],
            gip[1],
            gip[2],
            gip[3],
            ex_port
        );

        getselfNode(self,statuscode,addr);
        
        brocasReqNodeList("table.json",statuscode,nodelist,addr);

        return initSock(ex_port);
    }
}

int initSock(int ex_port){
    long long start = gettime();
    long long now = gettime();

    int sock;
    struct sockaddr_in local_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(ex_port);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr))==-1){
        printf("socket bind error:%s\r\n",strerror(errno));
    }

    int val;

    val = 1;
    ioctl(sock, FIONBIO, &val);

    return sock;

};
