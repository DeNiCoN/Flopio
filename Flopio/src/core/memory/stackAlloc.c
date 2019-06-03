#include "stackAlloc.h"
#include "memutils.h"
#include <ctype.h>
#define STACK_HEADER_SIZE_BYTES sizeof(size_t)


void stackAllocInit(StackAllocator* alloc, char* buffer, size_t size) 
{
	alloc->buffer = buffer;
	alloc->size = size;
	alloc->stackPointer = 0;
}

//Returns null if overflow or given size is 0
void* stackAlloc(StackAllocator* alloc, size_t size) {
	if (size == 0 || (alloc->stackPointer + size + STACK_HEADER_SIZE_BYTES > alloc->size))
	{
		return NULL;
	}
	size_t prevSP = alloc->stackPointer;
	alloc->stackPointer += size + STACK_HEADER_SIZE_BYTES;
	void* headerPtr = alloc->buffer + (prevSP + size);
	*(unsigned __int32*) headerPtr = (unsigned __int32)size;
	return (void*) (alloc->buffer + prevSP);
}

//Returns null if overflow or given size is 0 or align is not power of 2 
void* stackAllocAligned(StackAllocator* alloc, size_t size, size_t align) 
{
	if ((align & (align - 1)) || (size == 0)) 
	{
		return NULL;
	}
	uintptr_t mask = align - 1;
	size_t actualSize = size + STACK_HEADER_SIZE_BYTES + mask;
	if (alloc->stackPointer + actualSize > alloc->size) 
	{
		return NULL;
	}

	size_t prevSP = alloc->stackPointer;
	alloc->stackPointer += actualSize;
	void* headerPtr = alloc->buffer + (prevSP + size + mask);
	*(unsigned int*)headerPtr = actualSize;
	return (void*)((uintptr_t) (alloc->buffer + prevSP) & ~mask);
}

//returns 0 if underflow, 1 if ok
int stackFree(StackAllocator* alloc) 
{
	if (alloc->stackPointer == 0)
	{
		return 0;
	}
	size_t size = ((size_t*)(alloc->buffer + alloc->stackPointer))[-1];
	alloc->stackPointer -= size;
	return 1;
}

void stackReset(StackAllocator* alloc) 
{
	alloc->stackPointer = 0;
}