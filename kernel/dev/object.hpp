#ifndef _dev_object_hpp
#define _dev_object_hpp

#include <inttypes.h>

typedef enum NodeType {
    NODE_TYPE_ROOT = 0,
    NODE_TYPE_CLASS = 1,
    NODE_TYPE_DEVICE = 2,
    NODE_TYPE_FUNCTION = 3
} NodeType;

#define NODE_CHILDREN_MAX 10

typedef struct Node {
    char *name;
    NodeType type;
    struct Node *children[NODE_CHILDREN_MAX];

    void *functionData;
    void (*functionPointer)(void *data, void *argument);
} Node;

uint16_t nodeQuery(char *name);
void nodeCall(uint16_t nodeId, void *arguments, void *result);
void nodeRegister(char *name, void *functionData, void (*functionPointer)(void *data, void *argument));

#endif