#include "ResourceCache.h"
#include "../memory/poolAlloc.h"

namespace engine
{
	ResourceHandle::~ResourceHandle() 
	{
		resCache->deallocate(buffer); 
		poolFree(&resCache->handlePool, this);
	}

}
