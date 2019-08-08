#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

	typedef struct 
	{
		char* buffer;
		size_t size;
		char* head;
		char* tail;
	} QueueAllocator;

	typedef struct
	{
		size_t size;
		size_t adjustment;
	} QueueHeader;

	void* queueAllocAligned(QueueAllocator* alloc, size_t size, size_t align);
	void* queueAlloc(QueueAllocator* alloc, size_t size);
	void* queueAllocGetFirst(QueueAllocator* alloc);
	void queueAllocInit(QueueAllocator* alloc, char* buffer, size_t size);
	int queueFree(QueueAllocator* alloc);

#ifdef __cplusplus
}
#endif
