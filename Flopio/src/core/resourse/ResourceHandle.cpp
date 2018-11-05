#include "ResourceCache.h"

namespace engine
{
	ResourceHandle::~ResourceHandle() { resCache->deallocate(buffer); }

}
