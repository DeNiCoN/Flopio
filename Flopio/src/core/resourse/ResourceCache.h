#pragma once
#include <list>
#include <memory>
#include "Resource.h"
#include "ResourceHandle.h"
#include "ResourceLoader.h"
#include "ResourceFile.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "../memory/poolAlloc.h"

namespace engine 
{
    using ResourceHandlesList = std::list<std::shared_ptr<ResourceHandle> >;
    typedef std::unordered_map<int, std::shared_ptr<ResourceHandle>> ResourceHandlesMap;
    using ResourceLoadersVector = std::vector<std::shared_ptr<ResourceLoader> >;
    using ResourceFileVector = std::vector<std::shared_ptr<ResourceFile> >;

	

    class ResourceCache 
    {
        friend class ResourceHandle;

    protected:
        PoolAllocator handlePool;
        unsigned int allocatedSize;
        unsigned int maxHandleCount;

        ResourceHandlesList LRUList;
        ResourceHandlesMap handlesMap;
        ResourceLoadersVector loaders;
        ResourceFileVector files;

        void makeRoom(unsigned int size);
        std::shared_ptr<ResourceHandle> load(Resource & resource);
        void update(std::shared_ptr<ResourceHandle> resHandle);
        void free(std::shared_ptr<ResourceHandle> resHandle);
        void freeOneResource();
        std::shared_ptr<ResourceHandle> find(Resource & resource);

    public:
        //setting default maxHandleCount to sizeInMB * 256 means that we 'have' 4 KB of buffer for each handle
        ResourceCache(unsigned int sizeInMB) : allocatedSize(0), maxHandleCount(sizeInMB * 256) {}
        virtual ~ResourceCache();
        bool init();
        bool init(unsigned int maxHandleCount) { this->maxHandleCount = maxHandleCount; return init(); }
		
        std::shared_ptr<ResourceHandle> getHandle(Resource & resource);
        inline std::shared_ptr<ResourceHandle> getHandle(Resource&& resource)
        {
            return getHandle(resource);
        }
        inline std::shared_ptr<ResourceHandle> getHandle(const char* resource)
        {
            return getHandle(Resource(resource));
        }
        inline std::shared_ptr<ResourceHandle> getHandle(std::string& resource)
        {
            return getHandle(Resource(resource));
        }
        void clearResources();
        void addLoader(std::shared_ptr<ResourceLoader> loader) { loaders.push_back(loader); }
        void addFile(std::shared_ptr<ResourceFile> file);
    };
}
