#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "include/node.h"
#include "include/broadcast.h"

#include "include/cache.h"

int loadSaveNodeList(char path[],NodeStack * nodelist){
    char *buf;
    FILE* fp;
 
    buf=(char*)malloc(2048);
 
    fp = fopen(path, "r"); 
    fread(buf, 1, 2048, fp);
    fclose(fp);
    
    /* define nodes list*/
    char **ns = (char **)malloc(40 * sizeof(char *));
    for(int i = 0; i<40; ++i){
       ns[i] = (char *)malloc(1024 * sizeof(char));
    }
    /* parsing json */
    parseJson(buf,"node",ns);   

    for(int i=0;ns[i][0]!=NULL;++i){
        Node * node = (Node*)malloc(sizeof(Node));

        JsonStr2Node(node,ns[i]);
        nodelist=appendNodeList(nodelist,node);

        free(node);
    }

    for(int i = 0; i<40; ++i){
        free(ns[i]);
    }
    free(ns);
}

int saveNodeStack(char path[],NodeStack * nodelist){
    FILE* fp;
    json_object *nodes_json = json_object_new_array();
    struct json_object *obj;
    char* buf;
    buf=(char*)malloc(2048);

    fp = fopen(path, "r"); 
    fread(buf, 1, 2048, fp);
    fclose(fp);

    obj = parseJson(buf,"",NULL);

    free(buf);

    for(int i=0; i<nodelist->len; ++i){
        Node * node = (Node*)malloc(sizeof(Node));
        char json_str[1024];
        node=getNodefromNodeList(nodelist,i);
        Node2JsonStr(node,json_str);

        json_object *node_obj =parseJson(json_str,"",NULL);
        json_object_array_add(nodes_json, node_obj);
    }

    fp = fopen(path, "w"); 
    json_object_object_add(obj, "node", nodes_json);
    fprintf(fp, "%s", json_object_to_json_string_ext(obj, JSON_C_TO_STRING_PRETTY));
    fclose(fp);

}

int loadDiscoveryServer(char path[],NodeStack * nodelist){
    char *buf;
    FILE* fp;
 
    buf=(char*)malloc(2048);
 
    fp = fopen(path, "r"); 
    fread(buf, 1, 2048, fp);
    fclose(fp);
    
    /* define nodes list*/
    char **ds = (char **)malloc(40 * sizeof(char *));
    for(int i = 0; i<40; ++i){
       ds[i] = (char *)malloc(1024 * sizeof(char));
    }
    /* parsing json */
    parseJson(buf,"node",ds);   

    for(int i=0;ds[i][0]!=NULL;++i){
        Node * node = (Node*)malloc(sizeof(Node));
        node->type=1;
        node->status=2;
        snprintf(node->addr,128,"%s",ds[i]);

        JsonStr2Node(node,ds[i]);
        nodelist=appendNodeList(nodelist,node);

        free(node);
    }

    for(int i = 0; i<40; ++i){
        free(ds[i]);
    }
    free(ds);

}