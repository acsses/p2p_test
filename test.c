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

int main(){
    int sock;
    struct sockaddr_in addr;
    struct sockaddr_in local_addr;
    struct sockaddr_in from_addr;
    socklen_t sin_size;
    int status;
    int flags = 0;
    struct timeval tv;
    long long before;
    long long after;
    int d;
 

    sock = socket(AF_INET, SOCK_STREAM, 0);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(5008);
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
        char buff[2048];
        while(1){
            int recv_size;
            recv_size = recv(sock, buff, 1048, 0);
            if(recv_size==0){
                break;
            }
            printf("%s\n",buff);

        }
        
    }
    
    return 0;
}