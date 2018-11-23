#pragma once
#include<string>
#include "ResourceHandle.h"

namespace engine
{
	class ResourceLoader
	{
	public:
		virtual bool VUseRawFile() = 0;
		virtual bool VDiscardRawBufferAfterLoad() = 0;
		virtual const std::string* VGetWildcardPattern(int* size) = 0;
		virtual unsigned int VGetLoadedSize(char* rawBuffer, unsigned int rawBufSize) = 0;
		virtual unsigned int VGetExtraDataSize() = 0; //Resource loader allocates additional memory for extra data
		virtual bool VLoad(char* rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle) = 0;
	};
}