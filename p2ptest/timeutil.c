#include <sys/time.h>

#include "include/timeutil.h"

long long  gettime(){
    struct timeval time;
    gettimeofday(&time, NULL);
    return (long long)time.tv_sec;
}