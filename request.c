#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <stdbool.h>

#include <signal.h>
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

#include "p2ptest/include/portmapping.h"
#include "p2ptest/include/parser.h"
#include "p2ptest/include/http.h"
#include "p2ptest/include/id.h"
#include "p2ptest/include/message.h"
#include "p2ptest/include/util.h"
#include "p2ptest/include/node.h"
#include "p2ptest/include/broadcast.h"
#include "p2ptest/include/listener.h"

volatile sig_atomic_t e_flag = 0;

#define DEV_MODE 1

int main(){
    NodeStack * nodelist = initNodeList();
    Node * self = (Node*)malloc(sizeof(Node));
    int sock;

    sock = initConnection(3600,self,nodelist);
    printf("%d\n",sock);

    for(int i=0;i<nodelist->len;i++){
        char buf[1024];
        Node *return_node;
        return_node=getNodefromNodeList(nodelist,i);
        Node2JsonStr(return_node,buf);
        printf("%s\n",buf);
    }

    listener(sock,self,nodelist);

}

void abrt_handler(int sig);



int startListen(int status,int ex_port,unsigned char gip[]){
    long long start = gettime();
    long long now = gettime();

    FILE* fp; 

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
        printf("\n\nlisten error\n");
        close(sock);
        return -1;
    }

    int c_sock;
 
    if ( signal(SIGINT, abrt_handler) == SIG_ERR ) {
      close(sock);
      exit(1);
    }
    printf("start Listen!\n");

    int val;

    val = 1;
    ioctl(sock, FIONBIO, &val);

    while(!e_flag){

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
            close(c_sock);

        };

        now=gettime();

        if(status==CONNECTION_PORTMAPPPED_LIMITTED){
            if (now-start>55){
                printf("\n");
                printf("starting port mapping by NAT-PMP...\n");
                if ((status=NatpmpPortmapping(ex_port,gip))!=0){
                    printf("port mapping by NAT-PMP failed\n\n");
                    printf("NAT-PMP reset error\n");
                    printf("restart init...\n\n");
                    return -2;
                }
                start=now;
            }
        }else if(status==CONNECTION_NOT_PORTMAPPPED){
            if (now-start>5){
                printf("\n");
                char *buf;
                buf = (char*)malloc(2048);
                memset(buf,0,2048);

                struct json_object *config;
                fp = fopen("table.json", "r"); 

                fread(buf, 1, 2048, fp);
                fclose(fp);

                char **ds = (char **)malloc(10 * sizeof(char *));
                for(int j = 0; j<10; ++j){
                   ds[j] = (char *)malloc(256 * sizeof(char));
                }

                config = parseJson(buf,"discvoery_server",ds);

                for(int i=0;ds[i][0]!=NULL;++i){
                    char* b_list;
                    b_list = (char*)malloc(2048);
                    if(b_list==NULL){
                        printf("error\n");
                    }
                    messageReqBlock(ds[i],status,b_list);
                    printf("%s\n",b_list);
                    free(b_list);
                }

                for(int i = 0; i<10; ++i){
                   free(ds[i]);
                }
                free(ds);


                char **ns = (char **)malloc(40 * sizeof(char *));
                for(int j = 0; j<10; ++j){
                   ns[j] = (char *)malloc(256 * sizeof(char));
                }
                parseJson(buf,"node",ns);

                for(int i=0;ns[i][0]!=NULL;++i){
                    printf("%s\n",ns[i]);
                }


                free(buf);

                start=now;
            }

        }

    }
    return 0;

}

void abrt_handler(int sig) {
  e_flag = 1;
}