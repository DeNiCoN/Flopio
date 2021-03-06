#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		char* buffer;
		size_t size;
		size_t stackPointer;
	} StackAllocator;

	void* stackAlloc(StackAllocator* alloc, size_t size);
	void* stackAllocAligned(StackAllocator* alloc, size_t size, size_t align);
	int stackFree(StackAllocator* alloc);
	void stackReset(StackAllocator* alloc);
	void stackAllocInit(StackAllocator* alloc, char* buffer, size_t size);

#ifdef __cplusplus
}
#endif
