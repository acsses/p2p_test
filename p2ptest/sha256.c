#include <string.h> //strcat
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>


void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int sha256hex(unsigned char * src, unsigned int src_len, char dst[]){
    EVP_MD_CTX *mdctx;
    unsigned char buffer[SHA256_DIGEST_LENGTH];

    if((mdctx = EVP_MD_CTX_new()) == NULL)
        handleErrors();

    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
        handleErrors();

    if(1 != EVP_DigestUpdate(mdctx, src, sizeof(src)))
        handleErrors();

    if(1 != EVP_DigestFinal_ex(mdctx, buffer, &src_len))
        handleErrors();

    EVP_MD_CTX_free(mdctx);


    for(int k=0;k<32;++k){
        char *set;
        set=(char *)malloc(4);
        if(buffer[k]==0){
            snprintf(set,4,"00");
        }else if(buffer[k]<16){
            snprintf(set,4,"0%x",buffer[k]);
        }else{
            snprintf(set,4,"%x",buffer[k]);
        }
        strcat(dst,set);
        free(set);
    }
    return 0;
}

int sha256oct(unsigned char * src, unsigned int src_len, char dst[]){
    EVP_MD_CTX *mdctx;
    unsigned char buffer[SHA256_DIGEST_LENGTH];

    if((mdctx = EVP_MD_CTX_new()) == NULL)
        handleErrors();

    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
        handleErrors();

    if(1 != EVP_DigestUpdate(mdctx, src, sizeof(src)))
        handleErrors();

    if(1 != EVP_DigestFinal_ex(mdctx, buffer, &src_len))
        handleErrors();

    EVP_MD_CTX_free(mdctx);


    for(int k=0;k<32;++k){
        char *set;
        set=(char *)malloc(6);
        if(buffer[k]==0){
            snprintf(set,6,"000");
        }else if(buffer[k]<8){
            snprintf(set,6,"00%o",buffer[k]);
        }else if(buffer[k]<64){
            snprintf(set,6,"0%o",buffer[k]);
        }else{
            snprintf(set,6,"%o",buffer[k]);
        }
        strcat(dst,set);
        free(set);
    }
    return 0;
}

int sha256dec(unsigned char * src, unsigned int src_len, char dst[]){
    EVP_MD_CTX *mdctx;
    unsigned char buffer[SHA256_DIGEST_LENGTH];

    if((mdctx = EVP_MD_CTX_new()) == NULL)
        handleErrors();

    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
        handleErrors();

    if(1 != EVP_DigestUpdate(mdctx, src, sizeof(src)))
        handleErrors();

    if(1 != EVP_DigestFinal_ex(mdctx, buffer, &src_len))
        handleErrors();

    EVP_MD_CTX_free(mdctx);


    for(int k=0;k<32;++k){
        char *set;
        set=(char *)malloc(6);
        if(buffer[k]==0){
            snprintf(set,6,"000");
        }else if(buffer[k]<10){
            snprintf(set,6,"00%d",buffer[k]);
        }else if(buffer[k]<100){
            snprintf(set,6,"0%d",buffer[k]);
        }else{
            snprintf(set,6,"%d",buffer[k]);
        }
        strcat(dst,set);
        free(set);
    }
    return 0;
}