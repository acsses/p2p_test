#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <json-c/json.h>
#include <stdbool.h>

#include "include/parser.h"


#define BUF_LEN 256

char word[128];

static void XMLCALL
elementStart(void *user_data, const XML_Char *el, const XML_Char *attr[]) 
{
    if(strcmp(el,word)==0){
        strcpy(user_data,el);
    }
}

static void XMLCALL
elementData(void *user_data, const XML_Char *data, int data_size) 
{
  if (data_size > 0) {
    if(strcmp(user_data,word)==0){
        strcpy(user_data,data);
    }
  }
}

int parseSsdp(char target[],struct ssdp *ssdp){
    int i=0;
    int start=0;

    for(i=0;i<strlen(target);++i){
        if(target[i]=='\r'){
            char l[i-start];
            snprintf(l,i-start+1,"%s",target+start);
            int j=0;
            int colon=0;
            for(j=0;j<strlen(l);++j){
                if(l[j]==':'){
                    colon=j+2;
                    if(strstr(l,"LOCATION")){
                        snprintf(ssdp->Location,256,"%s",l+colon);
                    }
                    if(strstr(l,"ST")){
                        snprintf(ssdp->ST,256,"%s",l+colon);
                    }
                    if(strstr(l,"SERVER")){
                        snprintf(ssdp->Server,256,"%s",l+colon);
                    }
                    if(strstr(l,"DATE")){
                        snprintf(ssdp->Date,256,"%s",l+colon);
                    }
                    if(strstr(l,"Location")){
                        snprintf(ssdp->Location,256,"%s",l+colon);
                    }
                    if(strstr(l,"Server")){
                        snprintf(ssdp->Server,256,"%s",l+colon);
                    }
                    if(strstr(l,"Date")){
                        snprintf(ssdp->Date,256,"%s",l+colon);
                    }
                    break;
                }
            }
            start=i+2;
        }

    }

    return 0;
}

int parseHttp(char response[],Http *http){
    int n=0;
    int start=0;
    int i=0;
    
    for (n=0;n<strlen(response);++n){
        if(response[n]=='\r'){
            char l[n-start];
            snprintf(l,n-start+1,"%s",response+start);
            int j=0;
            int colon=0;
            for(j=0;j<strlen(l);++j){
                if(l[j]==':'){
                    colon=j+2;
                    if(strstr(l,"Content-Type")){
                        snprintf(http->Content_type,256,"%s",l+colon);
                    }
                    if(strstr(l,"Content-Length")){
                        snprintf(http->Content_Length,256,"%s",l+colon);
                    }
                }
            }
            if(n-start==1){
                if(i==0){
                    snprintf(http->Body,strlen(response)-n,"%s",response+n+2);
                }
                ++i;
            }
            start=n+1;
        }
    }
    

    return 1;
}

int parseNatpmp(unsigned char buf[],struct natpmp * natpmp){
    unsigned char rc[2];
    unsigned char eps[4];
    /*littile endian*/
    rc[0]=buf[3];
    rc[1]=buf[2];

    eps[0]=buf[7];
    eps[1]=buf[6];
    eps[2]=buf[5];
    eps[3]=buf[4];


    natpmp->Epochs = *(unsigned int *)&eps;
    natpmp->ResultCode = *(unsigned short int *)&rc;

    return 0;
};

void parseURL(const char *urlStr,Url *url) {
    char *host_path;
    host_path = (char*)malloc(BUF_LEN);

    if (strlen(urlStr) > BUF_LEN - 1) {
        printf("too long url");
        return;
    }

    // http://から始まる文字列で
    // sscanfが成功して
    // http://の後に何か文字列が存在するなら
    if ((strstr(urlStr, "http://")              &&
        sscanf(urlStr, "http://%s", host_path) &&
        strcmp(urlStr, "http://"))||
        (strstr(urlStr, "https://")              &&
        sscanf(urlStr, "https://%s", host_path) &&
        strcmp(urlStr, "https://"))) {

        char *p = NULL;

        p = strchr(host_path, '#');
        if (p != NULL) {
            strcpy(url->fragment, p);
            *p = '\0';
        }

        p = strchr(host_path, '?');
        if (p != NULL) {
            strcpy(url->query, p);
            *p = '\0';
        }

        p = strchr(host_path, '/');
        if (p != NULL) {
            strcpy(url->path, p);
            *p = '\0';
        }

        strcpy(url->host, host_path);

        // ホスト名の部分に":"が含まれていたら
        p = strchr(url->host, ':');
        if (p != NULL) {
            // ポート番号を取得
            url->port = atoi(p + 1);
    
            // 数字ではない（atoiが失敗）か、0だったら
            // ポート番号は80に決め打ち
            if (url->port <= 0) {
                url->port = 80;
            }

            
            // 終端文字で空にする
            *p = '\0';
        }
        else {
            url->port = 80;
        }
    }
    else {
        printf("this url format is not comatible\n");
        return;
    }
    free(host_path);
}

int parseXML(Http *http,char returns[],char search[]){

    XML_Parser parser;
    int done = 0;
    char searched[1024];
    char *target;

    strcpy(word,search);

    if ((parser = XML_ParserCreate(NULL)) == NULL) {
      fprintf(stderr, "Parser Creation Error.\n");
      return -1;
    }
    XML_SetElementHandler(parser, elementStart,NULL);
    XML_SetCharacterDataHandler(parser, elementData);
    XML_SetUserData( parser, &searched );

    if(strlen(http->Body)==0){
        printf("container error\n");
        XML_ParserFree(parser);
        return -1;
    }
    while(!done) {
        done = strlen(http->Body) < sizeof(http->Body);
        if (XML_Parse(parser, http->Body, strlen(http->Body), done) == XML_STATUS_ERROR) {
          XML_ParserFree(parser); 
          break ;
        }
    } ;
    if(strlen(searched)==0){
        printf("not found\n");
        return -1;
    }
    target=strtok(searched, "<");
    strcpy(returns,target);
    return 1;
}

struct json_object * parseJson(char buf[],char search[],void * returns){
    struct json_object *table = json_tokener_parse(buf);
    
    json_object_object_foreach(table, key, val) {
        if(strcmp(key,search)==0){
            if (json_object_is_type(val, json_type_boolean)){
                bool value = json_object_get_boolean(val)? true:false;
                bool *ptr = (bool *)returns;

                *ptr = value;
            }else if (json_object_is_type(val, json_type_int)){
                int value = json_object_get_int(val);
                int *ptr = (int *)returns;

                *ptr = value;
            }else if (json_object_is_type(val, json_type_double)) {
                double value = json_object_get_double(val);
                double *ptr = (double *)returns;
                
                *ptr = value;
            }else if (json_object_is_type(val, json_type_string)){
                char *ptr = (char *)returns;
                snprintf(ptr,256,"%s",json_object_get_string(val));
            }else if (json_object_is_type(val, json_type_array)){
                if (json_object_is_type(json_object_array_get_idx(val, 0), json_type_string)){
                    char (*ptr)[256]=(char (*)[256])returns;

                    for (int i = 0; i < json_object_array_length(val); ++i) {
                        char *p = ptr[i];
                        struct json_object *a = json_object_array_get_idx(val, i);
                        snprintf(p,256,"%s",json_object_get_string(a));
                    }
                }else if (json_object_is_type(json_object_array_get_idx(val, 0), json_type_int)){
                    int *ptr=(int *)returns;

                    for (int i = 0; i < json_object_array_length(val); ++i) {
                        struct json_object *a = json_object_array_get_idx(val, i);
                        ptr[i]=json_object_get_int(a);
                    }
                }else if (json_object_is_type(json_object_array_get_idx(val, 0), json_type_double)){
                    double *ptr=(double *)returns;

                    for (int i = 0; i < json_object_array_length(val); ++i) {
                        struct json_object *a = json_object_array_get_idx(val, i);
                        ptr[i]=json_object_get_double(a);
                    }
                }else if (json_object_is_type(json_object_array_get_idx(val, 0), json_type_boolean)){
                    bool *ptr=(bool *)returns;

                    for (int i = 0; i < json_object_array_length(val); ++i) {
                        struct json_object *a = json_object_array_get_idx(val, i);
                        ptr[i]=json_object_get_boolean(a)? true:false;
                    }

                }else{
                    char (*ptr)[1024]=(char (*)[1024])returns;

                    for (int i = 0; i < json_object_array_length(val); ++i) {
                        char *p = ptr[i];
                        struct json_object *a = json_object_array_get_idx(val, i);
                        snprintf(p,1024,"%s",json_object_get_string(a));
                    }
                }
            }else{
                char *ptr = (char *)returns;
                snprintf(ptr,1024,"%s",json_object_get_string(val));
            }
            
        }

    }
    return table;
}
