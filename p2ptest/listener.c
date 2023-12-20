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
#include <json-c/json.h>

#include "include/util.h"
#include "include/node.h"
#include "include/message.h"
#include "include/portmapping.h"
#include "include/broadcast.h"

#include "include/listener.h"

int listener(int sock,Node * self,NodeStack *nodelist){
    long long start = gettime();
    long long now = gettime();
    int status;

    struct sockaddr_in local_addr;
    unsigned char gip[4];

    int c_sock;

    if(sock>=0){

        if (listen(sock, 3) == -1) {
            printf("\n\nlisten error\n");
            close(sock);
            return -1;
        }

    }

    while(1){

        if(sock>=0){

            if((c_sock = accept(sock, NULL, NULL))==-1){
                if(errno!=EWOULDBLOCK){
                    printf("accept error:%s\n",strerror(errno));
                    return -1;
                }
            }else{
                while(1){
                    char *buff;
                    buff=(char *)malloc(2048);

                    int recv_size;
                    recv_size = recv(c_sock, buff, 2048, 0);
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
                close(c_sock);
            };
        }

        now=gettime();

        if(self->status==CONNECTION_PORTMAPPPED_LIMITTED){
            if (now-start>55){
                printf("\n");
                printf("starting port mapping by NAT-PMP...\n");
                if ((status=NatpmpPortmapping(self->ex_port,gip))!=0){
                    printf("port mapping by NAT-PMP failed\n\n");
                    printf("NAT-PMP reset error\n");
                    printf("restart init...\n\n");
                    return -2;
                }
                start=now;
            }
        }else if(self->status==CONNECTION_NOT_PORTMAPPPED){
            if (now-start>5){
                int flg=0;
                for(int i=0;i<nodelist->len;i++){
                    Node *node = (Node*)malloc(sizeof(Node));
                    char* blocks;
                    blocks = (char*)malloc(2048);

                    node=getNodefromNodeList(nodelist,i);
                    if(node->status!=CONNECTION_NOT_PORTMAPPPED){
                        if(messageReqBlock(node->addr,self->status,blocks)==0){
                            flg=1;
                            printf("%s\n",blocks);
                        }else{
                            deleteNodefromNodeList(nodelist,i);
                            node->status=CONNECTION_NOT_PORTMAPPPED;
                            appendNodeList(nodelist,node);
                        }

                        free(blocks);
                    }
                }

                if(flg==0){
                    char *buf;
                    FILE* fp;
                    struct json_object *obj;

                    buf=(char*)malloc(2048);

                    fp = fopen("table.json", "r"); 
                    fread(buf, 1, 2048, fp);
                    fclose(fp);
                    char **ds = (char **)malloc(10 * sizeof(char *));
                    for(int j = 0; j<10; ++j){
                        ds[j] = (char *)malloc(256 * sizeof(char));
                    }
                    /* parsing json */
                    parseJson(buf,"discvoery_server",ds);
                    for(int i=0;ds[i][0]!=NULL;++i){
                        char* blocks;
                        blocks = (char*)malloc(2048);

                        printf("discovery-server : %d : %s\n",i,ds[i]);
                        if(messageReqBlock(ds[i],self->status,blocks)==0){
                            printf("blocks:%s\n",blocks);
                        }
                        free(blocks);
                    }
                    for(int i = 0; i<10; ++i){
                       free(ds[i]);
                    }
                    free(ds);

                }
                start=now;
            }

        }

    }


}