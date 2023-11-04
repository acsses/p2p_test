#ifndef PORTMAPPING_H
#define PORTMAPPING_H
    
    
/*--     include libs     --*/
#include <stdlib.h>
/*--     include libs     --*/
    
    
    
    
/*--   prototype declaration    --*/
int UpnpPortmapping(unsigned int ex_port,unsigned char gip[]);

int NatpmpPortmapping(unsigned int ex_port,unsigned char buf[]);
/*--   prototype declaration    --*/


#endif