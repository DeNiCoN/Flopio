#pragma once
#include "Resource.h"
#include <memory>

namespace engine
{
    class ResourceCache;
    class ResourceHandle;
    class ResourceExtraData
    {
        friend class ResourceHandle;
    public:
        virtual std::string getName() = 0;
        virtual ~ResourceExtraData() = default;
    };

    class ResourceHandle
    {
        friend class ResourceCache;
    protected:
        Resource resource;
        char* buffer;
        unsigned int size;
        ResourceCache *resCache;
        std::shared_ptr<ResourceExtraData> extra;
    public:
        ResourceHandle(Resource & res, char* buffer, unsigned int size, ResourceCache * resCache)
            : resource(res), buffer{ buffer }, size(size), resCache { resCache }, extra(nullptr) {}
        ~ResourceHandle();
        char* getBuffer() const { return buffer; }
        unsigned int getSize() const { return size; }
        Resource getResource() const { return resource; }
        std::shared_ptr<ResourceExtraData> getExtra() const { return extra; }
        void setExtra(std::shared_ptr<ResourceExtraData> extra) { this->extra = extra; }
    };
}
