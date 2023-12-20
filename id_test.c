#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <uuid/uuid.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/file.h>

int idgen(char path[]);

int main(){
    idgen(".id");
    idread(".id");
}

int idgen(char path[]){
    uuid_t value;
    uuid_string_t string;
    FILE* fp;

    struct stat stat_buf;

    if(stat(".id", &stat_buf)==0){
        if(stat_buf.st_flags==SF_IMMUTABLE){
            return -1;
        }
    }

    fp = fopen(".id", "w"); 

    uuid_generate(value);
    uuid_unparse_upper(value, string);

    fprintf(fp, "%s", string);
    fclose(fp);
    chflags(".id", SF_IMMUTABLE);

}

int idread(char path[]){
    char buf[2048];
    FILE* fp;
    struct stat stat_buf;

    if(stat(".id", &stat_buf)==0){
        if(stat_buf.st_flags==SF_IMMUTABLE){
            fp = fopen(".id", "r"); 
            fread(buf,1,2048,fp);
            printf("%s\n",buf);
            return 0;

        }else{
            printf("error: id file was changed invalidly.\n"); 
            return -1;

        }
    }else{
        printf("error: id file was changed invalidly.\n"); 
        return -1;

    }

}