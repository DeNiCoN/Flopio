#include "queueAlloc.h"
#define QUEUE_HEADER_SIZE_BYTES sizeof(size_t)

//returns pointer to new allocated space aligned by align 
//align must be power of 2
void* queueAllocAligned(QueueAllocator* alloc, size_t size, size_t align)
{
	//does align is power of 2
	if ((align & (align - 1)) || (size == 0))
	{
		return NULL;
	}
	uintptr_t mask = align - 1;
	uintptr_t prevTail = alloc->tail;
	size_t actualSize = size + mask + QUEUE_HEADER_SIZE_BYTES;

	if (alloc->tail > alloc->head)
	{
		if ((alloc->tail + actualSize) > (alloc->buffer + alloc->size))
		{
			if ((alloc->buffer + actualSize) > alloc->head)
			{
				//out of memory
				return NULL;
			}
			else
			{
				//allocate from buffer start

				//size of 0 mean that allocated memory starts from buffer
				*((size_t*)alloc->tail) = 0;
				*((size_t*)alloc->buffer) = actualSize;
				alloc->tail = alloc->buffer + actualSize;
				return (void*)((uintptr_t)(alloc->buffer + QUEUE_HEADER_SIZE_BYTES) & ~mask);
			}
		}
	}
	else if ((alloc->tail + actualSize) > alloc->head)
	{
		//out of memory
		return NULL;
	}

	*((size_t*)alloc->tail) = actualSize;
	alloc->tail += actualSize;
	return (void*)((uintptr_t)(alloc->buffer + prevTail + QUEUE_HEADER_SIZE_BYTES) & ~mask);
}

void* queueAlloc(QueueAllocator* alloc, size_t size)
{
	return queueAllocAligned(alloc, size, 1);
}

void queueAllocInit(QueueAllocator* alloc, char* buffer, size_t size)
{
	alloc->buffer = buffer;
	alloc->size = size;
	alloc->head = buffer;
	alloc->tail = buffer;
}
//Returns 0 if nothing to free, othervise returns 1
int queueFree(QueueAllocator* alloc)
{
	if (alloc->head == alloc->tail)
	{
		return 0;
	}

	if (*((size_t*)alloc->head) == 0)
	{
		alloc->head = alloc->buffer;
	}

	alloc->head += *((size_t*)alloc->head);
}