#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <json-c/json.h>

#include "include/block.h"

int initBlock(Block * block,char content[],char orient[]){
    memset(block->content,0,1024);
    snprintf(block->content,1024,"%s",content);
    snprintf(block->orient,1024,"%s",orient);

    sha256hex(block->content, 1024, block->cid);
    return 0;
}

int verifyBlock(Block * block,char cid[]){
    char *cid_tilda = (char *)malloc(128);
    sha256hex(block->content, 1024, cid_tilda);
    if(strcmp(cid,cid_tilda)==0){
        return 1;
    }else{
        return 0;
    }

}

int JsonStr2Block(Block *block,char json_str[]){
    char cid[128];
    char content[1024];
    char orient[128];


    parseJson(json_str,"cid",cid);
    parseJson(json_str,"content",content);
    parseJson(json_str,"orient",orient);

    snprintf(block->cid,128,"%s",cid);
    snprintf(block->content,1024,"%s",content);
    snprintf(block->orient,128,"%s",orient);


    return 0;
}

int Block2JsonStr(Block *block,char json_str[]){
    json_object *root = json_object_new_object();

    json_object_object_add(root, "cid", json_object_new_string(block->cid));
    json_object_object_add(root, "content", json_object_new_string(block->content));
    json_object_object_add(root, "orient", json_object_new_string(block->orient));
//
    snprintf(json_str,2048,"%s",json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
    json_object_put(root);
    return 0;
    
}

int initBlockQueue(BlockQueue * queue){

    queue->head=0;
    queue->tail=-1;
    return 0;

}

int enBlockQueue(BlockQueue * queue,Block *block){
    if(((queue->tail+2)%MAX_LEN)==queue->head){
        queue->head=(queue->head + 1) % MAX_LEN;
    }

    queue->array[(queue->tail + 1) % MAX_LEN] = *block;
    queue->tail = (queue->tail + 1) % MAX_LEN;
    return 0;
}