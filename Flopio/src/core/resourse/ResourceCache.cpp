#include "ResourceCache.h"

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

		char* poolBuf = new char[cacheBufferSize];
		if (!poolBuf)
			return false;
		if (!poolInit(&handlePool, poolBuf, sizeof(ResourceHandle), maxHandleCount))
			return false;


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
		ResourceHandlesMap::iterator i = handlesMap.find(resource->name);
		if (i == handlesMap.end())
			return std::shared_ptr<ResourceHandle>();
		return i->second;
	}

	void ResourceCache::update(std::shared_ptr<ResourceHandle> resHandle) 
	{
		LRUList.remove(resHandle);
		LRUList.push_front(resHandle);
	}

	std::shared_ptr<ResourceHandle> ResourceCache::load(Resource * resource) 
	{
		std::shared_ptr<ResourceHandle> handle;
	}
}