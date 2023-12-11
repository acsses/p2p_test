#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "include/job.h"

int initJob(Job* work,int type,Node * self_node){
    work->self=self_node;
    int depth=0;

    if(type==WORK_REQ_NODE_LIST){
        snprintf(work->type,128,"ReqNodeList");
    }else if(type==WORK_EMPTY){
        snprintf(work->type,128,"empty");
    }
}


int initJobqueue(JobQueue * queue){

    queue->head=0;
    queue->tail=-1;
    return 0;

}

int enJobqueue(JobQueue * queue,Job *work){
    if(((queue->tail+2)%QUEUE_MAX_LEN)==queue->head){
        return -1;
    }

    queue->array[(queue->tail + 1) % QUEUE_MAX_LEN] = *work;
    queue->tail = (queue->tail + 1) % QUEUE_MAX_LEN;
    return 0;
}

Job * deJobqueue(JobQueue * queue){
    if((queue->tail + 1) % QUEUE_MAX_LEN == queue->head){
        Job* empty=(Job*)malloc(sizeof(Job));
        Node* self = (Node *)malloc(sizeof(Node));
        initJob(empty,-1,self);
        return empty;
    }

    Job* returns=&(queue->array[queue->head]);
    queue->head = (queue->head+1)%QUEUE_MAX_LEN;


    return returns;

}
