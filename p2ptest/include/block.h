#ifndef BLOCK_H
#define BLOCK_H


/*--     include libs     --*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <json-c/json.h>

#include "sha256.h"
/*--     include libs     --*/

#define MAX_LEN 65

/*--     type define     --*/

/*--     structure define     --*/
struct block{
    char cid[128];
    char content[1024];
    char orient[128];
};
/*--     structure define     --*/
/*--     type define     --*/
typedef struct block Block;
/*--     type define     --*/

/*--     structure define     --*/
struct block_queue{
    int tail;
    int head;
    Block array[MAX_LEN];
};
/*--     structure define     --*/
/*--     type define     --*/
typedef struct block_queue BlockQueue;
/*--     type define     --*/

/*--     type define     --*/



/*--   prototype declaration    --*/
int verifyBlock(Block * block,char cid[]);

int initBlock(Block * block,char content[],char orient[]);

int JsonStr2Block(Block *block,char json_str[]);

int Block2JsonStr(Block *block,char json_str[]);

int initBlockQueue(BlockQueue * queue);
/*--   prototype declaration    --*/


#endif