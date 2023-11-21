#include <sys/time.h>
#include <stdio.h>
#include <string.h>

#include "include/util.h"

long long  gettime(){
    struct timeval time;
    gettimeofday(&time, NULL);
    return (long long)time.tv_sec;
}

void sortchar(char data[][256], int LIST_LEN) {
    int start;
    int i;
    char min[256];
    int i_min;
    char tmp[256];

    /* データ数が１の場合はソート済みなのでソート終了 */    
    if (LIST_LEN==0) {
        return;
    }

    /* ソート範囲（開始点）の初期化 */
    start = 0;

    /* ソート範囲を狭めながらソート処理 */
    for (start = 0; start < LIST_LEN; start++) {

        /* ひとまずソート範囲の先頭を最小値とみなす */
        i_min = start;
        strcpy(min, data[i_min]);

        /* ソート範囲の中から最小値を探索 */
        for (i = start; i <= LIST_LEN; i++) {
            if (strcmp(min, data[i]) > 0) {
                /* 最小値とその値を持つインデックスを更新 */
                strcpy(min, data[i]);
                i_min = i;
            }
        }

        if (start != i_min) {

            /* ソート範囲の先頭と最小値を交換 */
            strcpy(tmp, data[start]);
            strcpy(data[start], data[i_min]);
            strcpy(data[i_min], tmp);
        }
    }
}
