#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#include "include/message.h"
#include "include/parser.h"
#include "include/node.h"
#include "include/cache.h"

#include "include/broadcast.h"

#define CONNECTION_PORTMAPPED 0
#define CONNECTION_PORTMAPPPED_LIMITTED 1
#define CONNECTION_NOT_PORTMAPPPED 2


int brocasReqNodeList(char path[],int status,NodeStack * nodelist,char address[]){
   char *buf;
   FILE* fp;
   struct json_object *obj;

   buf=(char*)malloc(2048);

   fp = fopen(path, "r"); 
   fread(buf, 1, 2048, fp);
   fclose(fp);

   if(nodelist->len==0){
      loadSaveNodeList(path,nodelist);
      /* define discovery server list*/
      char **ds = (char **)malloc(10 * sizeof(char *));
      for(int j = 0; j<10; ++j){
         ds[j] = (char *)malloc(256 * sizeof(char));
      }
      /* parsing json */
      parseJson(buf,"discvoery_server",ds);

      /* circuring discovery server list */
      for(int i=0;ds[i][0]!=NULL;++i){
         printf("discovery-server : %d : %s\n",i,ds[i]);
   
         char *body;
         body=(char *)malloc(2048);
   
         if(messageReqNodeList(ds[i],address,status,body)==0){
            char **nodes = (char **)malloc(10 * sizeof(char *));
   
            for(int j = 0; j<10; ++j){
               nodes[j] = (char *)malloc(1024 * sizeof(char));
            }
   
            parseJson(body,"result",nodes);

            inStackNodeStack(nodes,nodelist);
            
            free(body);
   
            for(int j = 0; j<10; ++j){
               free(nodes[j]);
            }
            free(nodes);
   
   
         };
   
         
      }
      /* freeing discovery server list*/
      for(int i = 0; i<10; ++i){
         free(ds[i]);
      }
      free(ds);

      saveNodeStack(path,nodelist);


   }

   free(buf);


   for(int i=0;i<nodelist->len;i++){
      char buf[1024];
      Node *node = (Node *)malloc(sizeof(Node));
      node=getNodefromNodeList(nodelist,i);
      if(node->status!=CONNECTION_NOT_PORTMAPPPED && node->type!=1){
         char *body;
         body=(char *)malloc(2048);

         if(messageReqNodeList(node->addr,address,status,body)==0){
            char **nodes = (char **)malloc(10 * sizeof(char *));
   
            for(int j = 0; j<10; ++j){
               nodes[j] = (char *)malloc(1024 * sizeof(char));
            }
   
            parseJson(body,"result",nodes);

            inStackNodeStack(nodes,nodelist);
            
            free(body);

   
            for(int j = 0; j<10; ++j){
               free(nodes[j]);
            }
            free(nodes);
   
   
         };

      }
   }
   return 0;
}

int inStackNodeStack(char **node_strs,NodeStack * nodelist){

   for(int i=0;node_strs[i][0]!=NULL;++i){
      //printf("%s\n",node_strs[i]);
      Node * node_obj = (Node*)malloc(sizeof(Node));
      JsonStr2Node(node_obj,node_strs[i]);

      if(node_obj->status!=CONNECTION_NOT_PORTMAPPPED){
         for(int i=0;i<nodelist->len;i++){
            Node * sample =(Node*)malloc(sizeof(Node));
            sample=getNodefromNodeList(nodelist,i);

            if(strcmp(sample->id,node_obj->id)==0){
               nodelist=deleteNodefromNodeList(nodelist,i);
            }
         }
         nodelist=appendNodeList(nodelist,node_obj);
      }
      free(node_obj);
   }
   return 0;
}

