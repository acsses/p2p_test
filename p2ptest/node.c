#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <json-c/json.h>

#include "include/id.h"
#include "include/parser.h"

#include "include/node.h"

int getselfNode(Node *node,int status,char addr[]){
    char id[128];
    memset(id,0,128);
    getid(id);
    
    
    snprintf(node->id,128,"%s",id);
    snprintf(node->addr,128,"%s",addr);
    node->status=status;

    return 0;
}

int JsonStr2Node(Node *node,char json_str[]){
    char id[256];
    //memset(id,0,128);
    char addr[256];
    //memset(addr,0,128);
    int status;

    parseJson(json_str,"id",id);
    parseJson(json_str,"address",addr);
    parseJson(json_str,"status",&status);

    snprintf(node->id,128,"%s",id);
    snprintf(node->addr,128,"%s",addr);

    node->status=status;

    return 0;
}

int Node2JsonStr(Node *node,char json_str[]){

    json_object *root = json_object_new_object();

    json_object_object_add(root, "id", json_object_new_string(node->id));
    json_object_object_add(root, "address", json_object_new_string(node->addr));
    json_object_object_add(root, "status", json_object_new_int(node->status));

    snprintf(json_str,1024,"%s",json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
    json_object_put(root);
    return 0;
}

NodeStack * initNodeList(void){
    NodeStack *nodelist =  calloc(sizeof(NodeStack),1 );

    if (!nodelist) {
        return NULL;
    }

    nodelist->len=0;
    nodelist->array = calloc( sizeof(Node),nodelist->len+1);
    if (!nodelist->array) {
        free(nodelist);
        return NULL;
    }

    return nodelist;

}

NodeStack * appendNodeList(NodeStack * nodelist,Node* node){
    Node *tmp = realloc(nodelist->array, (nodelist->len + 1)*sizeof(struct node));

    if (!tmp) {
        return NULL;
    }

    nodelist->array = tmp; 

    int i=nodelist->len;
    
    nodelist->array[i]=*node;
    nodelist->len = nodelist->len+1; 

    return nodelist;
}

Node * getNodefromNodeList(NodeStack * nodelist,int idx){
    return &nodelist->array[idx];
}

NodeStack * deleteNodefromNodeList(NodeStack * nodelist,int idx){

    for(int i=idx;i<=nodelist->len-2;i++){
        if(i==nodelist->len-2){
            nodelist->array[i]=nodelist->array[i+1];
            Node * empty =(Node*)malloc(sizeof(Node));
            nodelist->array[i+1]=*empty;
        }else{
            printf("test\n");
            nodelist->array[i]=nodelist->array[i+1];
        }
    }

    Node *tmp = realloc(nodelist->array, (nodelist->len)*sizeof(struct node));

    if (!tmp) {
        return NULL;
    }

    nodelist->array = tmp; 
    nodelist->len = nodelist->len-1;

    return nodelist;
}