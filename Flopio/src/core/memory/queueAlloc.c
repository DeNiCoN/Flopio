#include "queueAlloc.h"
#define QUEUE_HEADER_SIZE_BYTES sizeof(QueueHeader)

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
	QueueHeader* header = (QueueHeader*)alloc->tail;
	size_t actualSize = size + mask + QUEUE_HEADER_SIZE_BYTES;

	if (alloc->tail >= alloc->head)
	{
		if ((alloc->tail + actualSize + sizeof(size_t)) > (alloc->buffer + alloc->size))
		{
			if ((alloc->buffer + actualSize + sizeof(size_t)) > alloc->head)
			{
				//out of memory
				return NULL;
			}
			else
			{
				//allocate from buffer start

				//size of 0 mean that allocated memory starts from buffer
				header->size = 0;
				header = (QueueHeader*)alloc->buffer;
				*((size_t*)alloc->buffer) = actualSize;
				alloc->tail = alloc->buffer + actualSize;
			}
		}
		else
		{
			alloc->tail += actualSize;
		}
	}
	else if ((alloc->tail + actualSize + sizeof(size_t)) > alloc->head)
	{
		//out of memory
		return NULL;
	}
	else
	{
		alloc->tail += actualSize;
	}

	char* rawAddress = (char*)header + QUEUE_HEADER_SIZE_BYTES;
	ptrdiff_t adjustment = align - ((uintptr_t)rawAddress & mask);
	header->size = actualSize;
	char* actualAddress = rawAddress + adjustment;
	header->adjustment = adjustment;
	return (void*)actualAddress;
}

void* queueAlloc(QueueAllocator* alloc, size_t size)
{
	return queueAllocAligned(alloc, size, 1);
}

void* queueAllocGetFirst(QueueAllocator* alloc)
{
	if (alloc->head == alloc->tail)
	{
		return NULL;
	}
	QueueHeader* header = (QueueHeader*)alloc->head;
	return (void*)((char*)(header + 1) + header->adjustment);
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