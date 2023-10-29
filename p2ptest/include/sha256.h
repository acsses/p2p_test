#ifndef SHA256_H
#define SHA256_H
    
    
/*--     include libs     --*/
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>
/*--     include libs     --*/
    
    
    
    
/*--   prototype declaration    --*/
int sha256hex(unsigned char * src, unsigned int src_len, char dst[]);

int sha256oct(unsigned char * src, unsigned int src_len, char dst[]);

int sha256dec(unsigned char * src, unsigned int src_len, char dst[]);
/*--   prototype declaration    --*/


#endif