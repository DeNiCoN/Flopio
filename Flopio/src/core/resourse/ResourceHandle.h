#pragma once
#include "ResourceCache.h"
#include "Resource.h"

namespace engine
{
	class ResourceHandle
	{
		friend class ResourceCache;
	protected:
		Resource resource;
		char* buffer;
		unsigned int size;
		ResourceCache* resCache;
	public:
		ResourceHandle(Resource & res, char* buffer, unsigned int size, ResourceCache * resCache)
			: resource(res), buffer(buffer), size(size), resCache(resCache) {}
		~ResourceHandle() { resCache->deallocate(buffer); }
		char* getBuffer() const { return buffer; }
		unsigned int getSize() const { return size; }

	};
}