#ifndef MESSAGE_H
#define MESSAGE_H
    
    
/*--     include libs     --*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <stdbool.h>

#include "./parser.h"
#include "./http.h"
#include "./id.h"
/*--     include libs     --*/
    
    
    
    
/*--   prototype declaration    --*/
int messageReqNodeList(char disc_server[],char address[],int status,char returns[]);

int messageBroCasAddr(char addr[],char self_addr[],int status,char returns[]);

int messageReqBlock(char addr[],int status,char returns[]);
/*--   prototype declaration    --*/


#endif