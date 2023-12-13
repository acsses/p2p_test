#ifndef INIT_H
#define INIT_H
    
    
/*--     include libs     --*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include "portmapping.h"
#include "broadcast.h"
#include "node.h"
/*--     include libs     --*/




/*--   prototype declaration    --*/
int initConnection(short int ex_port,Node * self,NodeStack * nodelist);

int initSock(int ex_port);
/*--   prototype declaration    --*/


#endif