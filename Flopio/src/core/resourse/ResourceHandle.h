#pragma once
#include "ResourceCache.h"
#include "Resource.h"

namespace engine
{

	class ResourceExtraData;

	class ResourceHandle
	{
		friend class ResourceCache;
	protected:
		Resource resource;
		char* buffer;
		unsigned int size;
		std::shared_ptr<ResourceExtraData> extra;
		ResourceCache* resCache;
	public:
		ResourceHandle(Resource & res, char* buffer, unsigned int size, ResourceCache * resCache)
			: resource(res), buffer(buffer), size(size), resCache(resCache), extra(NULL) {}
		~ResourceHandle();
		char* getBuffer() const { return buffer; }
		unsigned int getSize() const { return size; }
		std::shared_ptr<ResourceExtraData> getExtra() const { return extra; }
		void setExtra(std::shared_ptr<ResourceExtraData> extra) { this->extra = extra; }
	};
}