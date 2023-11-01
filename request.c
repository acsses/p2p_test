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


int messageReqNodeList(char disc_server_list[][256],int status,struct json_object *obj,FILE* f);

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

    if((status=UpnpPortmapping(ex_port,gip))<0){

        if ((status=NatpmpPortmapping(ex_port,gip))!=0){
            statuscode = CONNECTION_NOT_PORTMAPPPED;
            char ns[40][256]={};
            struct json_object *obj;
            obj = parseJson(buf,"node",&ns);
            int c=0;
            while(1){
                if(ns[c][0]=='\0'){
                    break;
                }
                ++c;
            }
            if(c==0){
                char ds[10][256]={};
                parseJson(buf,"discvoery_server",&ds);
                messageReqNodeList(ds,statuscode,obj,fp);
            }
        }else{
            printf("broadcast my adderss\n");
            printf("set ttl timer\n");
        }
    }else{

    }
}

int messageReqNodeList(char disc_server_list[][256],int status,struct json_object *obj,FILE* f){
    int d=0;
    int i=0;
    while(d==0){
        d=(disc_server_list[i][0]=='\0');
        if(d==1){
            break;
        }


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
            "\"type\":\"reqlist\","
            "\"status\":\"%d\""
            "}",
            id,status
        );

        if(DEV_MODE){
            snprintf(url,256,"http://%s/api/nodes",disc_server_list[i]);
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
            snprintf(url,256,"https://%s/api/nodes",disc_server_list[i]);
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

        char nodes[10][1024]={};
        if(strlen(res->Body)==0){
            return -1;
        }
        f = fopen("table.json", "w"); 
        parseJson(res->Body,"result",&nodes);
        int j=0;
        int k=0;
        json_object *nodes_json = json_object_new_array();
        while(k==0){
            k=(nodes[j][0]=='\0');
            if(k==1){
                break;
            }
            int status;
            json_object *node =parseJson(nodes[j],"status",&status);
            if(status==CONNECTION_PORTMAPPED){
                json_object_array_add(nodes_json, node);
            }
            ++j;
        }
        ++i;
        json_object_object_add(obj, "node", nodes_json);
        fprintf(f, "%s", json_object_get_string(obj));
        fclose(f);
    }
    
    return 0;
};