#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <stdlib.h>

#define BUF_LEN 256

struct ssdp{
    char Date[BUF_LEN];
    char Location[BUF_LEN];
    char Server[BUF_LEN];
    char ST[BUF_LEN];
};

struct URL{
    char host[BUF_LEN];
    char path[BUF_LEN];
    char query[BUF_LEN];
    char fragment[BUF_LEN];
    unsigned short port;
};

struct http{
    char Content_type[BUF_LEN];
    char Content_Length[BUF_LEN];
    char Body[1024];
};

struct natpmp{
    unsigned int Epochs;
    unsigned int ResultCode;
};

typedef struct ssdp Ssdp; 
typedef struct URL Url; 
typedef struct http Http;
typedef struct natpmp Natpmp;


int parseSsdp(char target[],Ssdp *ssdp);

int parseNatpmp(unsigned char buf[],Natpmp * natpmp);

int parseHttp(char response[],Http *http);

void parseURL(const char *urlStr,Url *url);

int parseXML(Http *http,char returns[],char search[]);

struct json_object * parseJson(char buf[],char search[],void * returns);


#endif