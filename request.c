#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <stdbool.h>

#include "p2ptest/include/portmapping.h"
#include "p2ptest/include/parser.h"
#include "p2ptest/include/http.h"
#include "p2ptest/include/id.h"

#define CONNECTION_PORTMAPPED 0
#define CONNECTION_NOT_PORTMAPPPED 1

#define DEV_MODE 0


int messageReqNodeList(char disc_server[],int status,char address[],char returns[]);

int messageBroCasAddr(char disc_server_list[][256],char node_list[][256],char addr[],int status);

int main(){
    short int ex_port=3600;
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
            
            struct json_object *obj;

            char **ds = (char **)malloc(10 * sizeof(char *));
            for(int j = 0; j<10; ++j){
               ds[j] = (char *)malloc(256 * sizeof(char));
            }
            obj = parseJson(buf,"discvoery_server",ds);
            for(int i=0;ds[i][0]!=NULL;++i){
                printf("discovery-server:%s\n",ds[i]);
                char *body;
                body=(char *)malloc(2048);
                if(body==NULL){
                    printf("error\n");
                }
                messageReqNodeList(ds[i],statuscode,NULL,body);

                char **nodes = (char **)malloc(10 * sizeof(char *));
                for(int j = 0; j<10; ++j){
                   nodes[j] = (char *)malloc(1024 * sizeof(char));
                }

                parseJson(body,"result",nodes);
                fp = fopen("table.json", "w"); 
                int j=0;
                int k=0;

                json_object *nodes_json = json_object_new_array();
                for(int j=0;nodes[j][0]!=NULL;++j){
                    int status;
                    json_object *node =parseJson(nodes[j],"status",&status);
                    if(status==CONNECTION_PORTMAPPED){
                        json_object_array_add(nodes_json, node);
                    }

                }
                json_object_object_add(obj, "node", nodes_json);
                fprintf(fp, "%s", json_object_get_string(obj));
                fclose(fp);
                free(body);
                printf("hi\n");
            }
            for(int i = 0; i<10; ++i){
               free(ds[i]);
            }
            free(ds);

            char **ns = (char **)malloc(40 * sizeof(char *));
            for(int i = 0; i<40; ++i){
               ns[i] = (char *)malloc(1024 * sizeof(char));
            }
            parseJson(buf,"node",ns);
            for(int i=0;ns[i][0]!=NULL;++i){
                printf("%s\n",ns[i]);
            }
            for(int i = 0; i<40; ++i){
               free(ns[i]);
            }
            free(ns);

            
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
            char ns[40][256]={};
            char ds[10][256]={};
            struct json_object *obj;
            //obj = parseJson(buf,"node",&ns);
            //parseJson(buf,"discvoery_server",&ds);
            //int c=0;
            //while(1){
            //    if(ns[c][0]=='\0'){
            //        break;
            //    }
            //    ++c;
            //}
            //if(c==0){
            //    messageReqNodeList(ds,statuscode,addr,obj,fp);
            //}
            //messageBroCasAddr(ds,ns,addr,statuscode);
            //printf("broadcast my adderss\n");
            //printf("set ttl timer\n");
        }
    }else{

    }
}

int messageReqNodeList(char disc_server[],int status,char address[],char returns[]){
    int d=0;
    
    char *buf;
    buf = (char *)malloc(2048);

    char url[256];
    char header[1024];
    char content[2048];
    char id[128];

    memset(id,0,sizeof(id));
    getid(id);

    Http *res;
    res = (Http*)malloc(sizeof(Http));

    if(address!=NULL){
        snprintf(content,2048,
            "{"
            "\"id\":\"%s\","
            "\"address\":\"%s\","
            "\"type\":\"reqlist\","
            "\"status\":\"%d\""
            "}",
            id,address,status
        );
    }else{
        snprintf(content,2048,
            "{"
            "\"id\":\"%s\","
            "\"type\":\"reqlist\","
            "\"status\":\"%d\""
            "}",
            id,status
        );
    }

    if(DEV_MODE){
        snprintf(url,256,"http://%s/api/nodes",disc_server);
        snprintf(header,1024,
            "Accept: */*\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %lu\r\n",
            strlen(content)
        );
        requestHttpPOST(buf,2048,url,1,header,content);
        parseHttp(buf,res);
        free(buf);
    }else{
        snprintf(url,256,"https://%s/api/nodes",disc_server);
        snprintf(header,1024,
            "Accept: */*\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %lu\r\n",
            strlen(content)
        );
        requestHttpsPOST(buf,2048,url,header,content);
        parseHttp(buf,res);
        free(buf);
    }

    if(strlen(res->Body)==0){
        return -1;
    }

    snprintf(returns,2048,"%s",res->Body);
    
    return 0;
};

int messageBroCasAddr(char disc_server_list[][256],char node_list[][256],char addr[],int status){
    printf("%s\n",addr);

    int c=0;
    while(1){
        if(disc_server_list[c][0]=='\0'){
            break;
        }
        printf("%s\n",disc_server_list[c]);
        ++c;
    }
    c=0;
    while(1){
        if(node_list[c][0]=='\0'){
            break;
        }
        printf("%s\n",node_list[c]);
        ++c;
    }

}