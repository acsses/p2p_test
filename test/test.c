#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/param.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>

#include "../p2ptest/include/portmapping.h"

int main(){
    unsigned int ex_port=5008;
    unsigned char gip[4];

    NatpmpPortmapping(ex_port,gip);
    int sock;
    struct sockaddr_in local_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(ex_port);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr))==-1){
        printf("socket bind error:%s\r\n",strerror(errno));
    }

    if (listen(sock, 3) == -1) {
        printf("listen error\n");
        close(sock);
        return -1;
    }

    int c_sock;

    while(1){
        printf("Waiting connect...\n");
        c_sock = accept(sock, NULL, NULL);
        if (c_sock == -1) {
            printf("accept error\n");
            close(sock);
            return -1;
        }
        while(1){
            char *buff;
            buff=(char *)malloc(2048);

            int recv_size;
            recv_size = recv(c_sock, buff, 1048, 0);
            if(recv_size==0){
                break;
            }
            if(recv_size==-1){
                printf("%s\n",strerror(errno));
                break;
            }
            printf("%s\n",buff);
            free(buff);

        }
        
    }
    
    return 0;
}