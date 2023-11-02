#include <dev/object.hpp>
#include <mem/heap.hpp>
#include <lib/string.hpp>
#include <debug.hpp>

Node rootNode = {
    .name = "Root",
    .type = NODE_TYPE_ROOT,
    .children = {}
};

Node nodeList[128];
uint8_t currentNode = 0;

/*
Node *findNode(char *path)
{
    Node *currentNode = &rootNode;

    if (path[0] == '/' && path[1] == '\0') {
        return currentNode;
    }

    uint32_t nextSlashPosition = 0;
    char *currentSection;
    Node *currentChild;

    while (true) {
        path++; // Remove leading slash

        nextSlashPosition = strsplit(path, '/');
        currentSection = (char *) heapAlloc((nextSlashPosition + 1) * sizeof(char));
        memcpy(currentSection, path, nextSlashPosition * sizeof(char));
        currentSection[nextSlashPosition] = '\0';

        path+=nextSlashPosition;

        if (path[0] == 0) {
            break;
        }

        // Find child nodes
        for (uint16_t i=0; i < NODE_CHILDREN_MAX; i++) {
            currentChild = currentNode->children[i];

            if (strcmp(currentChild->name, currentSection) == 0) {
                currentNode = currentChild;
                break;
            }
        }
    }

    return currentNode;
}

void nodeCall(char *path, void *arguments, void *result)
{
    Node *node = findNode(path);

    (*node->functionPointer)(node->functionData, arguments);
}

void nodeRegister(char *path, char *name, void *functionData, void (*functionPointer)(void *data, void *argument))
{
    debugPrint("Registering node under %s as %s\n", path, name);

    Node *node = findNode(path);

    for (uint8_t i=0; i < NODE_CHILDREN_MAX; i++) {
        if (!node->children[i]) {
            node->children[i] = (Node *) heapAlloc(sizeof(Node));
            Node *childNode = node->children[i];

            childNode->name = (char *) heapAlloc(strlen(name) * sizeof(char) + 1);
            memcpy(childNode->name, name, strlen(name) * sizeof(char));
            childNode->name[strlen(name)] = '\0';

            childNode->functionData = functionData;
            childNode->functionPointer = functionPointer;

            return;
        }
    }
}
*/

uint16_t nodeQuery(char *name)
{
    for (uint16_t i=0; i < currentNode; i++) {
        Node *node = &nodeList[i];

        if (strcmp(name, node->name) == 0)  {
            return i;
        }
    }
}

void nodeCall(uint16_t nodeId, void *arguments, void *result)
{
    Node *node = &nodeList[nodeId];

    (*node->functionPointer)(node->functionData, arguments);
}

void nodeRegister(char *name, void *functionData, void (*functionPointer)(void *data, void *argument))
{
    debugPrint("Register %s (%d)\n", name, currentNode);

    Node *node = &nodeList[currentNode++];

    uint16_t nameLength = strlen(name);
    node->name = (char *) heapAlloc(nameLength + 1);
    memcpy(node->name, name, nameLength);
    node->name[nameLength] = '\0';

    node->functionData = functionData;
    node->functionPointer = functionPointer;
}