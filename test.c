#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <json-c/json.h>

#include "p2ptest/include/util.h"
#include "p2ptest/include/node.h"
#include "p2ptest/include/job.h"
#include "p2ptest/include/sha256.h"
#include "p2ptest/include/block.h"
struct block_stack{
    Block * array;
    int32_t len;
};

int verifyBlock(Block * block,char cid[]);

int initBlock(Block * block,char content[],char orient[]);

int JsonStr2Block(Block *block,char json_str[]);

int Block2JsonStr(Block *block,char json_str[]);

int initBlockQueue(BlockQueue * queue);

void* roop1(void * works){
    long long start_origin = gettime();
    long long now = gettime();

    while(1){
        Job * work = (Job*)malloc(sizeof(Job));
        work = deJobqueue((JobQueue *)works);
        if(strcmp(work->type,"empty")!=0){
            printf("--------------------\n");
            printf("type:%s\n",work->type);
            printf("self:id:%s\n",(work->self)->id);
            printf("self:addr:%s\n",(work->self)->addr);
            printf("self:status:%d\n",(work->self)->status);
            printf("depth:%d\n",work->depth);
            printf("--------------------\n");

        }

        now=gettime();

        
        
        if (now-start_origin>200){
            printf("finish 1!\n");
            break;
        }
    }

}

void* roop2(void * works){
    printf("test2\n");
    long long start = gettime();
    long long start_origin = gettime();
    long long now = gettime();

    int i =0;

    while(1){
        now=gettime();
        if (now-start>32){

            Job* work = (Job*)malloc(sizeof(Job));
            Node* self = (Node *)malloc(sizeof(Node));
            getselfNode(self,2,"test");

            initJob(work,0,self);

            enJobqueue((JobQueue *)works,work);

            
            start=now;
        }
        if (now-start_origin>250){
            printf("finish 2!\n");
            break;
        }
    }

}

pthread_t starter(JobQueue * work){
    pthread_t new_thread;
    pthread_create(&new_thread, NULL, roop1, work);
    printf("test1\n");

    return new_thread;
}

int killer(pthread_t thread){
    printf("test3\n");
    pthread_cancel(thread);
    printf("test4\n");

    return 0;

}

int main(){
    BlockQueue * queue = (Block *)malloc(sizeof(BlockQueue));

    initBlockQueue(queue);

    Block * block = (Block *)malloc(sizeof(Block));
    char jsonstr[2048];

    initBlock(block,"hello world!","\0");
    //printf("%s\n",block->cid);
    //printf("%s\n",block->content);
    //printf("%s\n",block->orient);
    Block2JsonStr(block,jsonstr);
    //printf("%s\n",jsonstr);

    enBlockQueue(queue,block);

    for (int i = queue->head;(i% MAX_LEN ) <= queue->tail;++i ){
        char re_jsonstr[2048];
        Block * re_block = &(queue->array[i]);
        Block * re_re_block = (Block *)malloc(sizeof(Block));
        Block2JsonStr(re_block,re_jsonstr);
        printf("%s\n",re_jsonstr);
        JsonStr2Block(re_re_block,re_jsonstr);

        printf("%s\n",re_re_block->cid);
        printf("%s\n",re_re_block->content);
        printf("%s\n",re_re_block->orient);

    }

    Block2JsonStr(block,jsonstr);

    return 0;



//    JobQueue * work = (JobQueue *)malloc(sizeof(JobQueue));
////
//    //Work* work = (Work*)malloc(sizeof(Work));
//    //Node* self = (Node *)malloc(sizeof(Node));
////
//    //getselfNode(self,2,"test")
//    //initWork(work,0,self);
//
//    initJobqueue(work);
//    pthread_t thread_1;
//    //pthread_t thread_2;
////
//////
//    //pthread_create(&thread_1, NULL, roop1, work);
//    //pthread_create(&thread_2, NULL, roop2, work);
//////
//    //pthread_join(thread_1, NULL);
//    //pthread_join(thread_2, NULL);
//
//    thread_1=starter(work);
//    printf("test2\n");
//    sleep(3);
//    Job* job = (Job*)malloc(sizeof(Job));
//    Node* self = (Node *)malloc(sizeof(Node));
//    getselfNode(self,2,"test");
//    initJob(job,0,self);
//    enJobqueue(work,job);
//    sleep(3);
//
//    killer(thread_1);
////
    
}


