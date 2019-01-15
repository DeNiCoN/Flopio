#include "ResourceCache.h"
#include "../utils.h"
#include "../Engine.h"
#include <iostream>

namespace engine {

	ResourceCache::~ResourceCache() 
	{
		if (cacheBuffer)
		{
			LRUList.clear();
			handlesMap.clear();
			delete[] cacheBuffer;
		}
	}

	bool ResourceCache::init()
	{
		cacheBuffer = new char[cacheBufferSize];

		if (!cacheBuffer)
			return false;

		char* poolBuf = new char[maxHandleCount*sizeof(ResourceHandle)];
		if (!poolBuf)
			return false;
		if (!poolInit(&handlePool, poolBuf, sizeof(ResourceHandle), maxHandleCount))
			return false;

		//first data of cacheBuffer this is a pointer that points to first free location
		ResCacheBlockHeader* firstHeader = reinterpret_cast<ResCacheBlockHeader*>(cacheBuffer);
		firstHeader->prevBlockHeader = nullptr;
		firstHeader->nextFreeSpaceStart = cacheBuffer + sizeof(ResCacheBlockHeader);
		*static_cast<uintptr_t*>(firstHeader->nextFreeSpaceStart) = cacheBufferSize - sizeof(ResCacheBlockHeader); // each free block contains it size

		return true;
	}

	std::shared_ptr<ResourceHandle> ResourceCache::getHandle(Resource & resource)
	{
		std::shared_ptr<ResourceHandle> handle = find(resource);
		if (handle == nullptr)
		{
			handle = load(resource);
		}
		else
		{
			//update(handle);
		}
		return handle;
	}

	std::shared_ptr<ResourceHandle> ResourceCache::find(Resource & resource)
	{
		ResourceHandlesMap::iterator i = handlesMap.find(resource.getHashed());
		if (i == handlesMap.end())
			return std::shared_ptr<ResourceHandle>();
		return i->second;
	}

	void ResourceCache::update(std::shared_ptr<ResourceHandle> resHandle) 
	{
		LRUList.remove(resHandle);
		LRUList.push_front(resHandle);
	}

	char* ResourceCache::allocate(unsigned int size) 
	{ 
		if (size == 0)
			return nullptr;

		ResCacheBlockHeader* blockHeader;
		ResCacheResourceHeader * resHeader;
		unsigned int nextFreeSize;
		unsigned int diff;
		char* ptr;
		unsigned int fullSize = size + sizeof(ResCacheResourceHeader);
		blockHeader = reinterpret_cast<ResCacheBlockHeader*>(cacheBuffer);
		uintptr_t* nextPtr = reinterpret_cast<uintptr_t*>(blockHeader->nextFreeSpaceStart);
		for (int i = 0; i < 2; i++) // do two times
		{
			while (true)
			{
				nextFreeSize = *nextPtr;
				if (fullSize <= nextFreeSize)
				{
					//enough space
					resHeader = reinterpret_cast<ResCacheResourceHeader*>(nextPtr);
					ptr = reinterpret_cast<char*>(nextPtr) + sizeof(ResCacheResourceHeader);
					diff = nextFreeSize - fullSize;
					resHeader->startOfBlock = blockHeader;
					resHeader->handle = nullptr;
					if (diff <= sizeof(ResCacheBlockHeader))
					{
						resHeader->buffSize = size + diff;
						ResCacheBlockHeader * nextBlock = reinterpret_cast<ResCacheBlockHeader*>(reinterpret_cast<char*>(nextPtr) + nextFreeSize - sizeof(ResCacheBlockHeader));
						blockHeader->nextFreeSpaceStart = nextBlock->nextFreeSpaceStart;
						reinterpret_cast<ResCacheBlockHeader*>(reinterpret_cast<char*>(blockHeader->nextFreeSpaceStart) + *reinterpret_cast<uintptr_t*>(blockHeader->nextFreeSpaceStart) - sizeof(ResCacheBlockHeader))->prevBlockHeader = blockHeader;
						ResCacheResourceHeader* nextHeader = reinterpret_cast<ResCacheResourceHeader*>(reinterpret_cast<char*>(resHeader + 1) + resHeader->buffSize);
						while (nextHeader != blockHeader->nextFreeSpaceStart)
						{
							nextHeader->startOfBlock = blockHeader;

							nextHeader = reinterpret_cast<ResCacheResourceHeader*>(reinterpret_cast<char*>(nextHeader + 1) + nextHeader->buffSize);
						}
					}
					else
					{
						resHeader->buffSize = size;
						nextPtr = reinterpret_cast<uintptr_t*>(reinterpret_cast<char*>(nextPtr) + fullSize);
						blockHeader->nextFreeSpaceStart = nextPtr;
						*nextPtr = diff;
					}
					return ptr;
				}
				blockHeader = reinterpret_cast<ResCacheBlockHeader*>(reinterpret_cast<char*>(nextPtr) + nextFreeSize - sizeof(ResCacheBlockHeader));
				if (reinterpret_cast<char*>(blockHeader) >= (cacheBuffer + cacheBufferSize - sizeof(ResCacheBlockHeader) * 2)) // out of buffer;
				{
					logger << "Resource cache out of memory, trying to do something\n";
					makeRoom(size);
					break;
				}
				nextPtr = reinterpret_cast<uintptr_t*>(blockHeader->nextFreeSpaceStart);
			}
		}
		return nullptr; 
	}


	void ResourceCache::deallocate(char* buffer) 
	{
		if (buffer == nullptr)
			return;

		ResCacheResourceHeader* header;
		ResCacheBlockHeader* block;
		ResCacheBlockHeader* previous; 
		ResCacheBlockHeader* next;

		header = reinterpret_cast<ResCacheResourceHeader*>(buffer - sizeof(ResCacheResourceHeader));
		block = reinterpret_cast<ResCacheBlockHeader*>(header->startOfBlock);
		previous = block->prevBlockHeader;
		void* pNextFreeSpaceStart = previous == nullptr ? cacheBuffer + sizeof(ResCacheBlockHeader) : previous->nextFreeSpaceStart;
		next = reinterpret_cast<ResCacheBlockHeader*>(reinterpret_cast<char*>(block->nextFreeSpaceStart) + *reinterpret_cast<uintptr_t*>(block->nextFreeSpaceStart) - sizeof(ResCacheBlockHeader));

		//Check: does length between start of block and header equals to size of block header
		bool first = reinterpret_cast<char*>(header) - reinterpret_cast<char*>(header->startOfBlock) <= sizeof(ResCacheBlockHeader);
		//Check: does address of next free area equals to end of header buffer 
		bool last = block->nextFreeSpaceStart == buffer + header->buffSize;

		if (first && last)
		{
			//Resource is alone in block
			
			//set size of previous free space 
			*reinterpret_cast<uintptr_t*>(pNextFreeSpaceStart) = reinterpret_cast<uintptr_t>(block->nextFreeSpaceStart) - reinterpret_cast<uintptr_t>(pNextFreeSpaceStart) + *reinterpret_cast<uintptr_t*>(block->nextFreeSpaceStart);
			next->prevBlockHeader = previous;
		}
		else if(first)
		{
			//Resource lay first after block header
			//get new previous next free space start size and set it
			uintptr_t nextFSize = reinterpret_cast<uintptr_t>(header) - reinterpret_cast<uintptr_t>(pNextFreeSpaceStart) + header->buffSize + sizeof(ResCacheResourceHeader);
			*reinterpret_cast<uintptr_t*>(pNextFreeSpaceStart) = nextFSize; // Note: when resource header lay after first block header, this will corrupt it startOfBlock data. No. Yes.

			//create new block 
			ResCacheBlockHeader* newBlock = reinterpret_cast<ResCacheBlockHeader*>(reinterpret_cast<char*>(pNextFreeSpaceStart) + nextFSize - sizeof(ResCacheBlockHeader));
			*newBlock = *block;
			if (reinterpret_cast<char*>(block) == this->cacheBuffer)
			{
				block->nextFreeSpaceStart = reinterpret_cast<char*>(block + 1);
				newBlock->prevBlockHeader = block;
			}
			next->prevBlockHeader = newBlock;

			//update startOfBlock of each resource header in block;
			ResCacheResourceHeader* nextHeader = reinterpret_cast<ResCacheResourceHeader*>(newBlock + 1);
			while (nextHeader != newBlock->nextFreeSpaceStart)
			{
				nextHeader->startOfBlock = newBlock;

				nextHeader = reinterpret_cast<ResCacheResourceHeader*>(reinterpret_cast<char*>(nextHeader + 1) + nextHeader->buffSize);
			}

		}
		else if (last)
		{
			//Resource lay last in block
			block->nextFreeSpaceStart = header;
			*reinterpret_cast<uintptr_t*>(block->nextFreeSpaceStart) = reinterpret_cast<uintptr_t>(next) - reinterpret_cast<uintptr_t>(header) + sizeof(ResCacheBlockHeader);
		} 
		else
		{
			//Resource lay in the middle of block
			ResCacheBlockHeader* nextBlock = reinterpret_cast<ResCacheBlockHeader*>(reinterpret_cast<char*>(header + 1) + header->buffSize - sizeof(ResCacheBlockHeader));
			void* oldNextFSS = block->nextFreeSpaceStart;
			block->nextFreeSpaceStart = header;
			char* endOfResBuff = reinterpret_cast<char*>(nextBlock) + sizeof(ResCacheBlockHeader);
			*reinterpret_cast<uintptr_t*>(block->nextFreeSpaceStart) = reinterpret_cast<uintptr_t>(endOfResBuff) - reinterpret_cast<uintptr_t>(header);
			nextBlock->nextFreeSpaceStart = oldNextFSS;
			nextBlock->prevBlockHeader = block;
			ResCacheResourceHeader* nextHeader = reinterpret_cast<ResCacheResourceHeader*>(nextBlock + 1);
			while (nextHeader != nextBlock->nextFreeSpaceStart)
			{
				nextHeader->startOfBlock = nextBlock;

				nextHeader = reinterpret_cast<ResCacheResourceHeader*>(reinterpret_cast<char*>(nextHeader + 1) + nextHeader->buffSize);
			}
		}
	};

	void ResourceCache::makeRoom(unsigned int size)
	{
		//lol
	}

	std::shared_ptr<ResourceHandle> ResourceCache::load(Resource & resource) 
	{
		std::shared_ptr<ResourceHandle> handle;
		std::shared_ptr<ResourceLoader> loader;
		std::shared_ptr<ResourceFile> file;
		//std::cout << "load " << resource->getName() << "\n";
		for (std::shared_ptr<ResourceLoader> l : loaders) 
		{
			int size;
			const std::string* wilds = l->VGetWildcardPattern(&size);
			for (int i = 0; i < size; i++)
			{
				if (wildcardMath(wilds[i].c_str(), resource.getName().substr(resource.getColonPos() + 1).c_str()))
				{
					loader = l;
					break;
				}
			}
			if (loader)
				break;
		}

		if (!loader)
		{
			logger << "Resource loader for " << resource.getName() << " not found\n";
			return handle;
		}

		for (std::shared_ptr<ResourceFile> f : files)
		{
			if (!resource.getName().compare(0, resource.getColonPos(), f->getName(), 0, resource.getColonPos()))
			{
				file = f;
				break;
			}
		}

		if (!file)
		{
			logger << "ResourceFile for " << resource.getName() << " not found\n";
			return handle;
		}

		int rawSize = file->VGetRawResourceSize(resource);
		if (rawSize == 0)
		{
			logger << "Resource size returned 0 - Resource " << resource.getName() << " not found\n";
			return std::shared_ptr<ResourceHandle>();
		}
		char *rawBuffer = allocate(rawSize);

		if (rawBuffer == NULL)
		{
			logger << "ResourceCache out of memory while loading " << resource.getName() << "\n";
			return std::shared_ptr<ResourceHandle>();
		}

		if (file->VGetRawResource(resource, rawBuffer) == -1)
		{
			logger << "Something went wrong when getting " << resource.getName() << "\n";
			return std::shared_ptr<ResourceHandle>();
		}

		char *buffer = NULL;
		unsigned int size = 0;

		if (loader->VUseRawFile())
		{
			buffer = rawBuffer;
			ResourceHandle * handl = reinterpret_cast<ResourceHandle *>(poolAlloc(&handlePool));
			if (!handl)
			{
				logger << "ResourceCache out of ResourceHandles\n";
			}
			new (handl) ResourceHandle(resource, buffer, rawSize, this);
			handle = std::shared_ptr<ResourceHandle>(handl,
			[&](ResourceHandle * ptr)
			{
				this->deallocate(buffer);
				poolFree(&this->handlePool, this);
			});
		}
		else
		{
			size = loader->VGetLoadedSize(rawBuffer, rawSize) + loader->VGetExtraDataSize();
			buffer = allocate(size);
			if ((rawBuffer == NULL || buffer == NULL) && size != 0)
			{
				logger << "ResourceCache out of memory while loading " << resource.getName() << "\n";
				return std::shared_ptr<ResourceHandle>();
			}
			handle = std::shared_ptr<ResourceHandle>(new (reinterpret_cast<ResourceHandle *>(poolAlloc(&handlePool))) ResourceHandle(resource, buffer, size, this),
			[&](ResourceHandle * ptr)
			{
				this->deallocate(ptr->getBuffer());
				poolFree(&this->handlePool, ptr);
			});
			bool success = loader->VLoad(rawBuffer, rawSize, handle);

			if (loader->VDiscardRawBufferAfterLoad())
			{
				deallocate(rawBuffer);
			}

			if (!success)
			{
				logger << "Something went wrong when loading " << resource.getName() << "\n";
				return std::shared_ptr<ResourceHandle>();
			}
		}

		if (handle)
		{
			LRUList.push_front(handle);
			handlesMap[resource.getHashed()] = handle;
		}

		return handle;

	}

	void ResourceCache::addFile(std::shared_ptr<ResourceFile> file) 
	{
		if (file->VOpen())
			files.push_back(file);
		else
			logger << "failed to open file " << file->getName() << "\n";
	}
}