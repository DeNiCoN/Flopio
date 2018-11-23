#include "ResourceCache.h"
#include "../utils.h"
#include "../Engine.h"
#include <iostream>

namespace engine {

	ResourceCache::~ResourceCache() 
	{
		if (cacheBuffer)
			delete cacheBuffer;
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

	std::shared_ptr<ResourceHandle> ResourceCache::getHandle(Resource * resource)
	{
		std::shared_ptr<ResourceHandle> handle = find(resource);
		if (handle == nullptr)
			handle = load(resource);
		else
			update(handle);
		return handle;
	}

	std::shared_ptr<ResourceHandle> ResourceCache::find(Resource * resource)
	{
		ResourceHandlesMap::iterator i = handlesMap.find(resource->getName());
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
				resHeader->buffSize = size;
				resHeader->handle = nullptr;
				if (diff <= sizeof(ResCacheBlockHeader) + 1)
				{
					//TODO merge two blocks
					log << "ResourceCache data corrupting\n";
				}
				nextPtr = reinterpret_cast<uintptr_t*>(reinterpret_cast<char*>(nextPtr) + fullSize);
				blockHeader->nextFreeSpaceStart = nextPtr;
				*nextPtr = diff;
				return ptr;
			}
			blockHeader = reinterpret_cast<ResCacheBlockHeader*>(reinterpret_cast<char*>(nextPtr) + nextFreeSize - sizeof(ResCacheBlockHeader));
			if (reinterpret_cast<char*>(blockHeader) == (cacheBuffer + cacheBufferSize - sizeof(ResCacheBlockHeader) * 2)) // out of buffer;
				break;
			uintptr_t* nextPtr = reinterpret_cast<uintptr_t*>(blockHeader->nextFreeSpaceStart);
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

		header = reinterpret_cast<ResCacheResourceHeader*>(buffer[-sizeof(ResCacheResourceHeader)]);
		block = reinterpret_cast<ResCacheBlockHeader*>(header->startOfBlock);
		previous = block->prevBlockHeader;
		void* pNextFreeSpaceStart = previous == nullptr ? cacheBuffer + sizeof(ResCacheBlockHeader) : previous->nextFreeSpaceStart;
		next = reinterpret_cast<ResCacheBlockHeader*>(reinterpret_cast<char*>(block->nextFreeSpaceStart) + *reinterpret_cast<uintptr_t*>(block->nextFreeSpaceStart) - sizeof(ResCacheBlockHeader));

		bool first = reinterpret_cast<char*>(header) - reinterpret_cast<char*>(header->startOfBlock) <= sizeof(ResCacheResourceHeader);
		bool last = reinterpret_cast<ResCacheBlockHeader*>(header->startOfBlock)->nextFreeSpaceStart == reinterpret_cast<char*>(header + 1) + header->buffSize;

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

			uintptr_t nextFSize = reinterpret_cast<uintptr_t>(header) - reinterpret_cast<uintptr_t>(pNextFreeSpaceStart) + header->buffSize + sizeof(ResCacheResourceHeader);
			*reinterpret_cast<uintptr_t*>(pNextFreeSpaceStart) = nextFSize;
			ResCacheBlockHeader* newBlock = reinterpret_cast<ResCacheBlockHeader*>(nextFSize - sizeof(ResCacheBlockHeader));
			*newBlock = *block;
			next->prevBlockHeader = newBlock;

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
			ResCacheBlockHeader* nextBlock = reinterpret_cast<ResCacheBlockHeader*>(reinterpret_cast<char*>(header) + header->buffSize);
			void* oldNextFSS = block->nextFreeSpaceStart;
			block->nextFreeSpaceStart = header;
			char* endOfResBuff = reinterpret_cast<char*>(nextBlock) + sizeof(ResCacheResourceHeader);
			*reinterpret_cast<uintptr_t*>(block->nextFreeSpaceStart) = reinterpret_cast<uintptr_t>(endOfResBuff) - reinterpret_cast<uintptr_t>(header);
			nextBlock->nextFreeSpaceStart = oldNextFSS;
			nextBlock->prevBlockHeader = block;
		}
	};

	std::shared_ptr<ResourceHandle> ResourceCache::load(Resource * resource) 
	{
		std::shared_ptr<ResourceHandle> handle;
		std::shared_ptr<ResourceLoader> loader;
		std::shared_ptr<ResourceFile> file;
		
		for (std::shared_ptr<ResourceLoader> l : loaders) 
		{
			if (wildcardMath(l->VGetWildcardPattern().c_str(), resource->getName().substr(resource->getColonPos() + 1).c_str()))
			{
				loader = l;
				break;
			}
		}

		if (!loader)
		{
			log << "Resource loader for " << resource->getName() << " not found\n";
			return handle;
		}

		for (std::shared_ptr<ResourceFile> f : files)
		{
			if (!resource->getName().compare(0, resource->getColonPos(), f->getName(), 0, resource->getColonPos()))
			{
				file = f;
				break;
			}
		}

		if (!file)
		{
			log << "ResourceFile for " << resource->getName() << " not found\n";
			return handle;
		}

		int rawSize = file->VGetRawResourceSize(*resource);
		if (rawSize == 0)
		{
			log << "Resource size returned 0 - Resource " << resource->getName() << " not found";
			return std::shared_ptr<ResourceHandle>();
		}
		char *rawBuffer = allocate(rawSize);

		if (rawBuffer == NULL || file->VGetRawResource(*resource, rawBuffer) == 0)
		{
			log << "ResourceCache out of memory while loading " << resource->getName() << "\n";
			return std::shared_ptr<ResourceHandle>();
		}

		char *buffer = NULL;
		unsigned int size = 0;

		if (loader->VUseRawFile())
		{
			buffer = rawBuffer;
			ResourceHandle * handl = reinterpret_cast<ResourceHandle *>(poolAlloc(&handlePool));
			*handl = ResourceHandle(*resource, buffer, rawSize, this);
			handle = std::shared_ptr<ResourceHandle>(handl);
		}
		else
		{
			size = loader->VGetLoadedSize(rawBuffer, rawSize) + loader->VGetExtraDataSize();
			buffer = allocate(size);
			if ((rawBuffer == NULL || buffer == NULL) && size != 0)
			{
				log << "ResourceCache out of memory while loading " << resource->getName() << "\n";
				return std::shared_ptr<ResourceHandle>();
			}
			ResourceHandle * handl = reinterpret_cast<ResourceHandle *>(poolAlloc(&handlePool));
			*handl = ResourceHandle(*resource, buffer, rawSize, this);
			handle = std::shared_ptr<ResourceHandle>(handl);
			bool success = loader->VLoad(rawBuffer, rawSize, handle);

			if (loader->VDiscardRawBufferAfterLoad())
			{
				deallocate(rawBuffer);
			}

			if (!success)
			{
				log << "Something went wrong when loading " << resource->getName() << "\n";
				return std::shared_ptr<ResourceHandle>();
			}
		}

		if (handle)
		{
			LRUList.push_front(handle);
			handlesMap[resource->getName()] = handle;
		}

		return handle;

	}
}