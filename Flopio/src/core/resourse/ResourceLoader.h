#pragma once
#include<string>
#include "ResourceHandle.h"

namespace engine
{
	class ResourceLoader
	{
	public:
		virtual std::string VGetWildcardPattern() = 0;
		virtual unsigned int VGetLoadedSize(char* rawBuffer, unsigned int rawBufSize) = 0;
		virtual void VLoad(char* rawBuffer, unsigned int rawBufSize, ResourceHandle * resHandle) = 0;
	};
}