#ifndef CACHE_H
#define CACHE_H
    
    
/*--     include libs     --*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#include "node.h"
/*--     include libs     --*/
    
    
    
    
/*--   prototype declaration    --*/
int loadSaveNodeList(char path[],NodeStack * nodelist);

int saveNodeStack(char path[],NodeStack * nodelist);
/*--   prototype declaration    --*/


#endif