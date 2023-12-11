#ifndef JOB_H
#define JOB_H
    
    
/*--     include libs     --*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#include "node.h"
/*--     include libs     --*/

/*--     macro define     --*/
#define QUEUE_MAX_LEN 64

#define WORK_EMPTY -1
#define WORK_REQ_NODE_LIST 0
/*--     macro define     --*/

/*--     type define     --*/
struct job{
    Node* self;
    char type[128];
    int depth;
};

typedef struct job Job;

struct job_queue{
    int head;
    int tail;
    Job array[QUEUE_MAX_LEN];
};

typedef struct job_queue JobQueue;
/*--     type define     --*/
    
    
    
/*--   prototype declaration    --*/
int initJob(Job* work,int type,Node * self_node);

int initJobqueue(JobQueue * queue);

int enJobqueue(JobQueue * queue,Job *work);

Job * deJobqueue(JobQueue * queue);
/*--   prototype declaration    --*/


#endif