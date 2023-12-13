#ifndef LISTENER_H
#define LISTENER_H


/*--     include libs     --*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/param.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>

#include "util.h"
#include "node.h"
#include "message.h"
#include "portmapping.h"
/*--     include libs     --*/




/*--   prototype declaration    --*/
int listener(int sock,Node * self,NodeStack *nodelist);
/*--   prototype declaration    --*/


#endif