#include <stdint.h>

void heapInit(uint32_t start, uint32_t size);
void* heapAlloc(uint32_t size);
void heapFree(void* ptr);