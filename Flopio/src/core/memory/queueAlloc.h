#include <ctype.h>
#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		char* buffer;
		size_t size;
		char* head;
		char* tail;
	} QueueAllocator;

	void* queueAllocAligned(QueueAllocator* alloc, size_t size, size_t align);
	void* queueAlloc(QueueAllocator* alloc, size_t size);
	void queueAllocInit(QueueAllocator* alloc, char* buffer, size_t size);
	int queueFree(QueueAllocator* alloc);

#ifdef __cplusplus
}
#endif