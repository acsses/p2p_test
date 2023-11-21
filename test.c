#include "p2ptest/include/id.h"

int main(){
    char id[128];
    getid(id);
    printf("%s\n",id);
}