#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <stdbool.h>

#include "include/parser.h"
#include "include/http.h"
#include "include/id.h"

#include "include/message.h"

int messageReqNodeList(char disc_server[],char address[],int status,char returns[]){
    
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

    snprintf(url,256,"https://%s/api/nodes",disc_server);
    snprintf(header,1024,
        "Accept: */*\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %lu\r\n",
        strlen(content)
    );
    if(requestHttpsPOST(buf,2048,url,header,content)!=0){
        return -1;

    };
    parseHttp(buf,res);
    free(buf);

    if(strlen(res->Body)==0){
        return -1;
    }

    snprintf(returns,2048,"%s",res->Body);
    
    return 0;
};

int messageBroCasAddr(char addr[],char self_addr[],int status,char returns[]){
    printf("%s\n",addr);
    
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

    snprintf(content,2048,
        "{"
        "\"id\":\"%s\","
        "\"address\":\"%s\","
        "\"type\":\"decaddr\","
        "\"status\":\"%d\""
        "}",
        id,self_addr,status
    );

    snprintf(url,256,"http://%s",addr);
    snprintf(header,1024,
        "Accept: */*\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %lu\r\n",
        strlen(content)
    );
    requestHttpPOST(buf,2048,url,1,header,content);

    parseHttp(buf,res);
    free(buf);

    snprintf(returns,2048,"%s",res->Body);
    return 1;

}

int messageReqBlock(char addr[],int status,char returns[]){

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

    snprintf(content,2048,
        "{"
        "\"type\":\"reqblocks\","
        "\"status\":\"%d\""
        "}",
        status
    );

    snprintf(url,256,"https://%s/api/block",addr);
    snprintf(header,1024,
        "Accept: */*\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %lu\r\n",
        strlen(content)
    );
    if(requestHttpsPOST(buf,2048,url,header,content)!=0){
        return -1;
    }

    parseHttp(buf,res);

    snprintf(returns,2048,"%s",res->Body);
    free(buf);
    return 0;

}