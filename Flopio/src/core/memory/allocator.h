#pragma once
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif


	typedef struct
	{
		void* startOfBlock;
		size_t buffSize;
	} alloc_resHeader;

	typedef struct alloc_blockHeader
	{
		struct alloc_blockHeader* prevBlockHeader;
		void* nextFreeSpaceStart;
	} alloc_blockHeader;


	void* allocate(unsigned int size);
	void deallocate(char* buffer);
	void alloc_init(char* buffer, size_t size);


#ifdef __cplusplus
}
#endif