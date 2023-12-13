#ifndef NODE_H
#define NODE_H
    
    
/*--     include libs     --*/
#include <string.h>
#include <json-c/json.h>

/*--     include libs     --*/

/*--     macro define     --*/
#define CONNECTION_PORTMAPPED 0
#define CONNECTION_PORTMAPPPED_LIMITTED 1
#define CONNECTION_NOT_PORTMAPPPED 2
/*--     macro define     --*/

/*--     type define     --*/
struct node
{
    int type;
    char id[128];
    int status;
    unsigned char gip[4];
    int ex_port;
    char addr[128];
};
typedef struct node Node;

struct node_stack
{
    Node *array;
    int32_t len;
};
typedef struct node_stack NodeStack;
/*--     type define     --*/


/*--   prototype declaration    --*/
int getselfNode(Node *node,int status,char addr[]);

int JsonStr2Node(Node *node,char json_str[]);

int Node2JsonStr(Node *node,char json_str[]);

NodeStack * initNodeList(void);

NodeStack * appendNodeList(NodeStack * nodelist,Node* node);

Node * getNodefromNodeList(NodeStack * nodelist,int idx);

NodeStack * deleteNodefromNodeList(NodeStack * nodelist,int idx);
/*--   prototype declaration    --*/


#endif