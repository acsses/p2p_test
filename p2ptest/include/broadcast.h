#ifndef BROADCAST_H
#define BROADCAST_H
    
    
/*--     include libs     --*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#include "message.h"
#include "broadcast.h"
#include "node.h"
/*--     include libs     --*/
    
    
    
    
/*--   prototype declaration    --*/
int brocasReqNodeList(char path[],int status,NodeStack * nodelist,char address[]);
/*--   prototype declaration    --*/


#endif