#include <stdlib.h>

#include "include/natpmp.h"
#include "include/ssdp.h"
#include "include/parser.h"
#include "include/http.h"
#include "include/network.h"

#include "include/portmapping.h"

int UpnpPortmapping(unsigned int ex_port){
    char *buf;
    char *servicetype;
    char *selfip;
    Ssdp *ssdp;
    Http *res;

    buf=(char*)malloc(2048);
    servicetype=(char*)malloc(1024);
    selfip=(char*)malloc(256);
    ssdp = (Ssdp*)malloc(sizeof(Ssdp));
    res = (Http*)malloc(sizeof(Http));

    if(ssdpMsearch(buf,2048)==-1){
        printf("not found root device\n");
        return -1;
    };
    printf("found root device\n\n");

    parseSsdp(buf,ssdp);
    if(strlen(ssdp->Location)==0){
        printf("not found doc url\n");
        return -2;
    }
    printf("found doc url\n");
    printf("location -> %s\n\n",ssdp->Location);

    memset(buf, '\0', 2048);
    requestHttpGET(buf,2048,ssdp->Location,1);
    parseHttp(buf,res);

    if(strlen(res->Body)==0){
        printf("failed to get document\r\n");
        return -3;
    }

    printf("got document\n\n");

    parseXML(res,servicetype,"serviceType");
    if(strstr(servicetype,"urn:schemas-upnp-org:service:WANIPConnection:1")){
        char controlurl[256];
        Url url;
        char *error = NULL;
        char sendurl[256];
        Http *res_2;
        res_2=(Http*)malloc(sizeof(Http));
        char succsess[1024];
        memset(buf, '\0', 2048);
        parseURL(ssdp->Location,&url);
        parseXML(res,controlurl,"controlURL");

        if (controlurl[0]=='/'){
            snprintf(sendurl, sizeof(sendurl),
                "http://%s:%d%s",url.host,url.port,controlurl
            );
        }else {
            snprintf(sendurl, sizeof(sendurl),
                "http://%s:%d/%s",url.host,url.port,controlurl
            );
        }

        char body[2048];
        snprintf(body, sizeof(body),
            "<?xml version='1.0'?>\r\n"
            "<s:Envelope xmlns:s='http://schemas.xmlsoap.org/soap/envelope/' s:encodingStyle='http://schemas.xmlsoap.org/soap/encoding/'>\r\n"
            "<s:Body>\r\n"   
            "<u:AddPortMapping xmlns:u='urn:schemas-upnp-org:service:WANIPConnection:1'>\r\n"  
            "<NewRemoteHost></NewRemoteHost>\r\n"            
            "<NewExternalPort>%u</NewExternalPort>\r\n"    
            "<NewProtocol>%s</NewProtocol>\r\n"   
            "<NewInternalClient>%s</NewInternalClient>\r\n"   
            "<NewEnabled>1</NewEnabled>\r\n"     
            "<NewPortMappingDescription>test</NewPortMappingDescription>\r\n"
            "<NewLeaseDuration>0</NewLeaseDuration>\r\n"
            "</u:AddPortMapping>\r\n"
            "</s:Body>\r\n"
            "</s:Envelope>\r\n",
            ex_port, "tcp",selfip
        );
        free(selfip);
        char header[1024];
        snprintf(header, sizeof(header),
            "Content-Type: text/xml\r\n"
            "Content-Length: %lu\r\n"
            "SOAPACTION: urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping\r\n",              // HEADER
            strlen(body)
        );

        requestHttpPOST(buf,2048,sendurl,1,header,body);
        parseHttp(buf,res_2);
        parseXML(res_2,succsess,"serviceType");
        if(strlen(succsess)==0){
            printf("failed to port open with WANIPConfig");
            return -1;
        }
        free(servicetype);
    }else if (strstr(servicetype,"urn:schemas-upnp-org:service:WANPPPConnection:1")){
        char controlurl[256];
        Url url;
        char *error = NULL;
        char sendurl[256];
        Http *res_2;
        res_2=(Http*)malloc(sizeof(Http));
        char succsess[1024];
        memset(buf, '\0', 2048);
        parseURL(ssdp->Location,&url);
        parseXML(res,controlurl,"controlURL");

        if (controlurl[0]=='/'){
            snprintf(sendurl, sizeof(sendurl),
                "http://%s:%d%s",url.host,url.port,controlurl
            );
        }else {
            snprintf(sendurl, sizeof(sendurl),
                "http://%s:%d/%s",url.host,url.port,controlurl
            );
        }

        char body[2048];
        snprintf(body, sizeof(body),
            "<?xml version='1.0'?>\r\n"
            "<s:Envelope xmlns:s='http://schemas.xmlsoap.org/soap/envelope/' s:encodingStyle='http://schemas.xmlsoap.org/soap/encoding/'>\r\n"
            "<s:Body>\r\n"   
            "<u:AddPortMapping xmlns:u='urn:schemas-upnp-org:service:WANPPPConnection:1'>\r\n"  
            "<NewRemoteHost></NewRemoteHost>\r\n"            
            "<NewExternalPort>%u</NewExternalPort>\r\n"    
            "<NewProtocol>%s</NewProtocol>\r\n"   
            "<NewInternalClient>%s</NewInternalClient>\r\n"   
            "<NewEnabled>1</NewEnabled>\r\n"     
            "<NewPortMappingDescription>test</NewPortMappingDescription>\r\n"
            "<NewLeaseDuration>0</NewLeaseDuration>\r\n"
            "</u:AddPortMapping>\r\n"
            "</s:Body>\r\n"
            "</s:Envelope>\r\n",
            ex_port, "tcp",selfip
        );
        free(selfip);
        char header[1024];
        snprintf(header, sizeof(header),
            "Content-Type: text/xml\r\n"
            "Content-Length: %lu\r\n"
            "SOAPACTION: urn:schemas-upnp-org:service:WANPPPConnection:1#AddPortMapping\r\n",
            strlen(body)
        );

        requestHttpPOST(buf,2048,sendurl,1,header,body);
        parseHttp(buf,res_2);
        parseXML(res_2,succsess,"serviceType");
        if(strlen(succsess)==0){
            printf("failed to port open with WANPPPConfig");
            return -1;
        }
    free(servicetype);
    }else{
        free(buf);
        free(servicetype);
        free(selfip);
        printf("serviceType is not Compatible\n");
        return -1;
    }
    free(buf);
    
    return 1;
    
}

int NatpmpPortmapping(unsigned int ex_port){
    int ip;
    int subnet;
    int target_int;
    int shift;
    int status;
    char target[256];
    unsigned char *buf;
    Natpmp *natpmp;

    buf = (unsigned char*)malloc(2048);
    natpmp = (Natpmp*)malloc(sizeof(Natpmp));
    
    ip = getselfip();
    subnet = getselfsubnet();
    target_int = ip & subnet;

    target_int = target_int + (1<<24);

    snprintf(target, sizeof(target),
        "%d.%d.%d.%d",
        (target_int>>0)&0xFF,
        (target_int>>8)&0xFF,
        (target_int>>16)&0xFF,
        (target_int>>24)&0xFF
    );

    if((status=requestNatpmp(buf,target,-1,-1,-1))>=0){
        if(buf[8]==192){
            free(buf);
            free(natpmp);
            printf("I'm in double nat situation\n");
            return -1;
        }
    }else{
        free(buf);
        free(natpmp);
        return -1;
    }
    if((status=requestNatpmp(buf,target,ex_port ,ex_port,60))>=0){
        parseNatpmp(buf,natpmp);
        if(natpmp->ResultCode==0){
            free(buf);
            free(natpmp);
            printf("successed port mapping!");
            return 0;
        }
    }
    return -1;
}
