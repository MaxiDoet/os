#include <mem/heap.hpp>
#include <debug.hpp>

#define HEAP_MAGIC 0xABCD

typedef struct heapChunk {
	uint16_t magic;
	struct heapChunk *nextChunk;
	struct heapChunk *previousChunk;
	bool allocated;
	bool dirty;
	uint32_t size;
} heapChunk;

static heapChunk *firstChunk;
uint32_t maxHeapSize = 0;
uint32_t heapSize = 0;

void heapInit(uint32_t start, uint32_t size)
{
	firstChunk = (heapChunk *) start;

	firstChunk->nextChunk = 0;
	firstChunk->previousChunk = 0;
	firstChunk->allocated = false;
	firstChunk->dirty = false;
	firstChunk->size = 0;

	maxHeapSize = size;
}

void* heapAlloc(uint32_t size)
{
	heapChunk *current = firstChunk;
	heapChunk *last = 0;
	heapChunk *result = 0;

	while (!result) {
		if (current->allocated) {
			current->nextChunk = (heapChunk *) ((uint32_t) current + sizeof(heapChunk) + current->size);
			current = current->nextChunk;
			continue;
		}

		// Last link in the chain
		if (!current->dirty) {
			current->magic = HEAP_MAGIC;
			current->size = size;
			current->dirty = true;
			current->nextChunk = (heapChunk *) ((uint32_t) current + sizeof(heapChunk) + current->size);
		}

		if (current->size < size) {
			current = current->nextChunk;
			continue;
		}

		if ((heapSize + size) >= maxHeapSize) {
			debugPrint("Out of heap\n");
		}

		current->allocated = true;
		result = current;
	}

	return (void *) ((uint32_t) result + sizeof(heapChunk));
}

void heapFree(void* ptr)
{
	heapChunk *chunk = (heapChunk *) ((uint32_t) ptr - sizeof(heapChunk));

	// Check if this points to a valid chunk
	if (chunk->magic == HEAP_MAGIC) {
		chunk->allocated = false;
	} else {
		debugPrint("[heap] Tried to free invalid chunk\r\n");
	}
}