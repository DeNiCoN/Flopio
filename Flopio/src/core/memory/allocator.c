#include "allocator.h"
#include <stdint.h>

char* buf;
size_t bufSize;

void alloc_init(char* buffer, size_t size)
{
	buf = buffer;
	bufSize = size;

	//first data of cacheBuffer this is a pointer that points to first free location
	alloc_blockHeader* firstHeader = (alloc_blockHeader*)(buf);
	firstHeader->prevBlockHeader = 0;
	firstHeader->nextFreeSpaceStart = buf + sizeof(alloc_blockHeader);
	*(uintptr_t*)(firstHeader->nextFreeSpaceStart) = bufSize - sizeof(alloc_blockHeader); // each free block contains it size
}

void* allocate(unsigned int size)
{
	if (size == 0)
		return 0;

	alloc_blockHeader* blockHeader;
	alloc_resHeader * resHeader;
	unsigned int nextFreeSize;
	unsigned int diff;
	char* ptr;
	unsigned int fullSize = size + sizeof(alloc_resHeader);
	blockHeader = (alloc_blockHeader*)(buf);
	uintptr_t* nextPtr = (uintptr_t*)(blockHeader->nextFreeSpaceStart);
	//for (int i = 0; i < 2; i++) // do two times
	//{
		while (1)
		{
			nextFreeSize = *nextPtr;
			if (fullSize <= nextFreeSize)
			{
				//enough space
				resHeader = (alloc_resHeader*)(nextPtr);
				ptr = (char*)(nextPtr) + sizeof(alloc_resHeader);
				diff = nextFreeSize - fullSize;
				resHeader->startOfBlock = blockHeader;
				if (diff <= sizeof(alloc_blockHeader))
				{
					resHeader->buffSize = size + diff;
					alloc_blockHeader * nextBlock = (alloc_blockHeader*)((char*)(nextPtr) + nextFreeSize - sizeof(alloc_blockHeader));
					blockHeader->nextFreeSpaceStart = nextBlock->nextFreeSpaceStart;
					((alloc_blockHeader*)((char*)(blockHeader->nextFreeSpaceStart) + *(uintptr_t*)(blockHeader->nextFreeSpaceStart) - sizeof(alloc_blockHeader)))->prevBlockHeader = blockHeader;
					alloc_resHeader* nextHeader = (alloc_resHeader*)((char*)(resHeader + 1) + resHeader->buffSize);
					while (nextHeader != blockHeader->nextFreeSpaceStart)
					{
						nextHeader->startOfBlock = blockHeader;

						nextHeader = (alloc_resHeader*)((char*)(nextHeader + 1) + nextHeader->buffSize);
					}
				}
				else
				{
					resHeader->buffSize = size;
					nextPtr = (uintptr_t*)((char*)(nextPtr) + fullSize);
					blockHeader->nextFreeSpaceStart = nextPtr;
					*nextPtr = diff;
				}
				return ptr;
			}
			blockHeader = (alloc_blockHeader*)((char*)(nextPtr) + nextFreeSize - sizeof(alloc_blockHeader));
			if ((char*)(blockHeader) >= (buf + bufSize - sizeof(alloc_blockHeader) * 2)) // out of buffer;
			{
				//logger << "Resource cache out of memory, trying to do something\n";
				//makeRoom(size);
				break;
			}
			nextPtr = (uintptr_t*)(blockHeader->nextFreeSpaceStart);
		}
	//}
	return 0;
}


void deallocate(char* buffer)
{
	if (buffer == 0 || buffer < buf || buffer > (buf + bufSize))
		return;

	alloc_resHeader* header;
	alloc_blockHeader* block;
	alloc_blockHeader* previous;
	alloc_blockHeader* next;

	header = (alloc_resHeader*)(buffer - sizeof(alloc_resHeader));
	block = (alloc_blockHeader*)(header->startOfBlock);
	previous = block->prevBlockHeader;
	void* pNextFreeSpaceStart = previous == 0 ? buf + sizeof(alloc_blockHeader) : previous->nextFreeSpaceStart;
	next = (alloc_blockHeader*)((char*)(block->nextFreeSpaceStart) + *(uintptr_t*)(block->nextFreeSpaceStart) - sizeof(alloc_blockHeader));

	//Check: does length between start of block and header equals to size of block header
	char first = (char*)(header) - (char*)(header->startOfBlock) <= sizeof(alloc_blockHeader);
	//Check: does address of next free area equals to end of header buffer 
	char last = block->nextFreeSpaceStart == buffer + header->buffSize;

	if (first && last)
	{
		//Resource is alone in block

		//set size of previous free space 
		*(uintptr_t*)(pNextFreeSpaceStart) = (uintptr_t)(block->nextFreeSpaceStart) - (uintptr_t)(pNextFreeSpaceStart) + *(uintptr_t*)(block->nextFreeSpaceStart);
		next->prevBlockHeader = previous;
	}
	else if (first)
	{
		//Resource lay first after block header
		//get new previous next free space start size and set it
		uintptr_t nextFSize = (uintptr_t)(header) - (uintptr_t)(pNextFreeSpaceStart) + header->buffSize + sizeof(alloc_resHeader);
		*(uintptr_t*)(pNextFreeSpaceStart) = nextFSize; // Note: when resource header lay after first block header, this will corrupt it startOfBlock data. No. Yes.

		//create new block 
		alloc_blockHeader* newBlock = (alloc_blockHeader*)((char*)(pNextFreeSpaceStart) + nextFSize - sizeof(alloc_blockHeader));
		*newBlock = *block;
		if ((char*)(block) == buf)
		{
			block->nextFreeSpaceStart = (char*)(block + 1);
			newBlock->prevBlockHeader = block;
		}
		next->prevBlockHeader = newBlock;

		//update startOfBlock of each resource header in block;
		alloc_resHeader* nextHeader = (alloc_resHeader*)(newBlock + 1);
		while (nextHeader != newBlock->nextFreeSpaceStart)
		{
			nextHeader->startOfBlock = newBlock;

			nextHeader = (alloc_resHeader*)((char*)(nextHeader + 1) + nextHeader->buffSize);
		}

	}
	else if (last)
	{
		//Resource lay last in block
		block->nextFreeSpaceStart = header;
		*(uintptr_t*)(block->nextFreeSpaceStart) = (uintptr_t)(next) - (uintptr_t)(header) + sizeof(alloc_blockHeader);
	}
	else
	{
		//Resource lay in the middle of block
		alloc_blockHeader* nextBlock = (alloc_blockHeader*)((char*)(header + 1) + header->buffSize - sizeof(alloc_blockHeader));
		void* oldNextFSS = block->nextFreeSpaceStart;
		block->nextFreeSpaceStart = header;
		char* endOfResBuff = (char*)(nextBlock) + sizeof(alloc_blockHeader);
		*(uintptr_t*)(block->nextFreeSpaceStart) = (uintptr_t)(endOfResBuff) - (uintptr_t)(header);
		nextBlock->nextFreeSpaceStart = oldNextFSS;
		nextBlock->prevBlockHeader = block;
		alloc_resHeader* nextHeader = (alloc_resHeader*)(nextBlock + 1);
		while (nextHeader != nextBlock->nextFreeSpaceStart)
		{
			nextHeader->startOfBlock = nextBlock;

			nextHeader = (alloc_resHeader*)((char*)(nextHeader + 1) + nextHeader->buffSize);
		}
	}
};
