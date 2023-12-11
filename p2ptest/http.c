#include <stdio.h> //printf,sprintf
#include <string.h> //memset,strcat
#include <stdlib.h> //malloc
#include <sys/types.h> //struct addrinfo
#include <sys/socket.h> //socket,connect
#include <sys/ioctl.h> //ioctl
#include <netdb.h> //getaddrinfo,
#include <netinet/in.h> //struct sockaddr_in
#include <arpa/inet.h> //<------------------------------------------
#include <unistd.h> //close
#include <errno.h> //errno
#include <net/if.h> //<------------------------------------------
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "include/parser.h"
#include "include/util.h"
#include "include/network.h"

#include "include/http.h"



int requestHttpGET(char buff[], int buf_size,char * URL,int is_local) {

    // ソケットのためのファイルディスクリプタ
    int sock;

    // IPアドレスの解決
    struct addrinfo hints, *res;
    int err;
    long long before;
    long long after;

    // サーバに送るHTTPプロトコル用バッファ
    char *send_buf;
    send_buf=(char *)malloc(BUF_LEN);

    Url url = {};

    parseURL(URL,&url);

    if(is_local==1){
        struct sockaddr_in addrs;
        memset(&addrs, 0, sizeof(struct sockaddr_in));
        addrs.sin_family = AF_INET;
        addrs.sin_port = htons(url.port);
        addrs.sin_addr.s_addr = inet_addr(url.host);

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("socket create error\n");
            return 1;
        }
        char *ifname;
        ifname = (char*)malloc(128);
        getifname(0,ifname);
        int idx = if_nametoindex(ifname);

        #if defined(__APPLE__)
            if(setsockopt(sock, IPPROTO_IP, IP_BOUND_IF, &idx, sizeof(idx))!=0){
                printf("set sock opt error\n");
            };
            free(ifname);

        #elif defined(__linux__)
            if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname))!=0){
                printf("set sock opt error\n");
            };
            free(ifname);
        #endif


        // サーバに接続
        if (connect(sock, (struct sockaddr*)&addrs, sizeof(struct sockaddr_in)) != 0) {
            printf("connection error\n");
            return 1;
        }

    }else{
        // 0クリア
        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_family   = AF_INET;

        char *serviceType = "http";

        if ((err = getaddrinfo(url.host, serviceType, &hints, &res)) != 0) {
            printf("error %d\n", err);
            return 1;
        }

        // ソケット生成
        if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
            printf("socket create error\n");
            return 1;
        }

        char *ifname;
        ifname = (char*)malloc(128);
        getifname(0,ifname);
        int idx = if_nametoindex(ifname);


        #if defined(__APPLE__)
            if(setsockopt(sock, IPPROTO_IP, IP_BOUND_IF, &idx, sizeof(idx))!=0){
                printf("set sock opt error\n");
            };
            free(ifname);

        #elif defined(__linux__)
            if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname))!=0){
                printf("set sock opt error\n");
            };
            free(ifname);
        #endif

        // サーバに接続
        if (connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
            printf("socket create error\n");
            return 1;
        }

    }

    // HTTPプロトコルの開始 ＆サーバに送信
    sprintf(send_buf, "GET %s HTTP/1.0\r\n", url.path);
    write(sock, send_buf, strlen(send_buf));

    sprintf(send_buf, "Host: %s:%d\r\n", url.host, url.port);
    write(sock, send_buf, strlen(send_buf));

    sprintf(send_buf, "\r\n");
    write(sock, send_buf, strlen(send_buf));
    free(send_buf);

    // 受信が終わるまでループ
    int i=0;
    before =gettime();
    while(1) {
        char *buf;
        buf = (char*)malloc(1024);
        int read_size;
        read_size = read(sock, buf, 1024);

        if (read_size > 0) {
            strcat(buff, buf);
            free(buf);
            buf = NULL;
            ++i;
        }
        else {
            if(i==0){
                after=gettime();
                if(after-before>5){
                    break;
                }
            }else{
                break;
            }
        }
    }

    // ソケットを閉じる
    close(sock);

    return 0;
}

int requestHttpsGET(char buff[], int buf_size,char * URL) {

    // ソケットのためのファイルディスクリプタ
    int sock;

    struct addrinfo hints, *res;
    int err;

    SSL *ssl;
    SSL_CTX *ctx;

    long long before;
    long long after;

    Url url = {};

    parseURL(URL, &url);

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family   = AF_INET;

    char *serviceType = "https";

    if ((err = getaddrinfo(url.host, serviceType, &hints, &res)) != 0) {
        printf("error %d\n", err);
        return 1;
    }

    // サーバに送るHTTPプロトコル用バッファ
    char *send_buf;
    send_buf = (char*)malloc(BUF_LEN);



    if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0){
        printf("socket set error\n");
        return -1;
    }

    char *ifname;
    ifname = (char*)malloc(128);
    getifname(0,ifname);
    int idx = if_nametoindex(ifname);


    #if defined(__APPLE__)
        if(setsockopt(sock, IPPROTO_IP, IP_BOUND_IF, &idx, sizeof(idx))!=0){
            printf("set sock opt error\n");
        };
        free(ifname);

    #elif defined(__linux__)
        if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname))!=0){
            printf("set sock opt error\n");
        };
        free(ifname);
    #endif
    

    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0){
        printf("connection error\n");
        return -1;
    }
    SSL_load_error_strings();
    SSL_library_init();

    ctx = SSL_CTX_new(SSLv23_client_method());
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);
    SSL_connect(ssl);

    sprintf(send_buf, "GET %s HTTP/1.0\r\n", url.path);
    int d;
    if((d=SSL_write(ssl, send_buf, strlen(send_buf)))<0){
        printf("sending error\n");
        return -1;
    };
    sprintf(send_buf, "Host: %s:%d\r\n", url.host, url.port);
    SSL_write(ssl, send_buf, strlen(send_buf));
    free(send_buf);

    before = gettime();
    int i=0;
    while(1) {
        char *buf;
        buf = (char*)malloc(1024);
        memset(buf,'\0',1024);
        int read_size;
        read_size = SSL_read(ssl, buf, 1024);

        if (read_size > 0) {
            strcat(buff, buf);
            free(buf);
            buf = NULL;
            ++i;
        }
        else {
            free(buf);
            if (i==1){
                ++i;
            }else if(i>=2){
                break;
            }else {
                //printf("%d\n",i);
                after = gettime();
                if(after-before>5){
                    printf("timeout\n");
                    return -1;
                }
            }
        }
    }

    SSL_shutdown(ssl); 

    SSL_free(ssl); 
    SSL_CTX_free(ctx);
    ERR_free_strings();

    close(sock);    
    return 0;
}

int requestHttpPOST(char buff[], int buf_size,char * URL,int is_local,char header[],char body[]) {

    // ソケットのためのファイルディスクリプタ
    int sock;

    struct addrinfo hints, *res;
    int err;

    long long before;
    long long after;

    char *send_buf;
    send_buf=(char *)malloc(BUF_LEN);

    Url url = {};

    parseURL(URL, &url);

    if(is_local==1){
        struct sockaddr_in addrs;
        memset(&addrs, 0, sizeof(struct sockaddr_in));
        addrs.sin_family = AF_INET;
        addrs.sin_port = htons(url.port);
        addrs.sin_addr.s_addr = inet_addr(url.host);

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("socket create error:%s\n",strerror(errno));
            return 1;
        }

        char *ifname;
        ifname = (char*)malloc(128);
        getifname(0,ifname);
        int idx = if_nametoindex(ifname);


        #if defined(__APPLE__)
            if(setsockopt(sock, IPPROTO_IP, IP_BOUND_IF, &idx, sizeof(idx))!=0){
                printf("set sock opt error\n");
            };
            free(ifname);

        #elif defined(__linux__)
            if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname))!=0){
                printf("set sock opt error\n");
            };
            free(ifname);
        #endif

        // サーバに接続
        if (connect(sock, (struct sockaddr*)&addrs, sizeof(struct sockaddr_in)) != 0) {
            printf("connection error:%s\n",strerror(errno));
            return 1;
        }

    }else{
        // 0クリア
        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_family   = AF_INET;

        char *serviceType = "http";

        if ((err = getaddrinfo(url.host, serviceType, &hints, &res)) != 0) {
            printf("error %d\n", err);
            printf("%s\n",gai_strerror(err));
            return 1;
        }

        // ソケット生成
        if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
            printf("socket create error:%s\n",strerror(errno));
            return 1;
        }

        char *ifname;
        ifname = (char*)malloc(128);
        getifname(0,ifname);
        int idx = if_nametoindex(ifname);


        #if defined(__APPLE__)
            if(setsockopt(sock, IPPROTO_IP, IP_BOUND_IF, &idx, sizeof(idx))!=0){
                printf("set sock opt error\n");
            };
            free(ifname);

        #elif defined(__linux__)
            if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname))!=0){
                printf("set sock opt error\n");
            };
            free(ifname);
        #endif

        // サーバに接続
        if (connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
            printf("connection error:%s\n",strerror(errno));
            return 1;
        }

    }

    sprintf(send_buf, "POST %s HTTP/1.0\r\n", url.path);
    if(write(sock, send_buf, strlen(send_buf))<0){
        printf("sending error\n");
        return -1;
    };
    sprintf(send_buf, "Host: %s:%d\r\n", url.host, url.port);
    write(sock, send_buf, strlen(send_buf));

    if(&header[0]!=NULL){
        sprintf(send_buf, "%s\r\n", header);
        write(sock, send_buf, strlen(send_buf));
    }
    if(&body[0]!=NULL){
        sprintf(send_buf, "%s", body);
        write(sock, send_buf, strlen(send_buf));
    }

    free(send_buf);

    before = gettime();

    int i=0;

    while(1) {
        char *buf;
        buf = (char*)malloc(1024);
        int read_size;
        read_size = read(sock, buf, 1024);

        if (read_size > 0) {
            strcat(buff, buf);
            free(buf);
            buf = NULL;
        }
        else {
            free(buf);
            if(i==0){
                after=gettime();
                if(after-before>5){
                    break;
                }
            }else{
                break;
            }
        }
    }

    close(sock);    
    return 0;
}

int requestHttpsPOST(char buff[], int buf_size,char * URL,char header[],char body[]) {

    // ソケットのためのファイルディスクリプタ
    int sock;

    struct addrinfo hints, *res;
    int err;

    SSL *ssl;
    SSL_CTX *ctx;

    long long before;
    long long after;

    Url url = {};

    parseURL(URL, &url);

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family   = AF_INET;

    char *serviceType = "https";

    if ((err = getaddrinfo(url.host, serviceType, &hints, &res)) != 0) {
        printf("error %d\n", err);
        printf("%s\n",gai_strerror(err));
        return 1;
    }

    // サーバに送るHTTPプロトコル用バッファ
    char *send_buf;
    send_buf = (char*)malloc(BUF_LEN);



    if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0){
        printf("socket set error\n");
        return -1;
    }

    char *ifname;
    ifname = (char*)malloc(128);
    getifname(0,ifname);
    int idx = if_nametoindex(ifname);


    #if defined(__APPLE__)
        if(setsockopt(sock, IPPROTO_IP, IP_BOUND_IF, &idx, sizeof(idx))!=0){
            printf("set sock opt error\n");
        };
        free(ifname);

    #elif defined(__linux__)
        if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname))!=0){
            printf("set sock opt error\n");
        };
        free(ifname);
    #endif
    

    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0){
        printf("connection error\n");
        return -1;
    }
    SSL_load_error_strings();
    SSL_library_init();

    ctx = SSL_CTX_new(SSLv23_client_method());
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);
    SSL_connect(ssl);

    sprintf(send_buf, "POST %s HTTP/1.0\r\n", url.path);
    int d;
    if((d=SSL_write(ssl, send_buf, strlen(send_buf)))<0){
        printf("sending error\n");
        return -1;
    };
    sprintf(send_buf, "Host: %s:%d\r\n", url.host, url.port);
    SSL_write(ssl, send_buf, strlen(send_buf));

    if(&header[0]!=NULL){
        sprintf(send_buf, "%s\r\n", header);
        SSL_write(ssl, send_buf, strlen(send_buf));
    }
    if(&body[0]!=NULL){
        sprintf(send_buf, "%s", body);
        SSL_write(ssl, send_buf, strlen(send_buf));
    }

    free(send_buf);

    before = gettime();
    int i=0;
    while(1) {
        char *buf;
        buf = (char*)malloc(1024);
        memset(buf,'\0',1024);
        int read_size;
        read_size = SSL_read(ssl, buf, 1024);

        if (read_size > 0) {
            strcat(buff, buf);
            free(buf);
            buf = NULL;
            ++i;
        }else {
            free(buf);
            if(i==0){
                after=gettime();
                if(after-before>5){
                    return -1;
                }
            }else{
                break;
            }
        }
    }

    SSL_shutdown(ssl); 

    SSL_free(ssl); 
    SSL_CTX_free(ctx);
    ERR_free_strings();

    close(sock);    
    return 0;
}