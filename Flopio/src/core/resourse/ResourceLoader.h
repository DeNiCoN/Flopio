#pragma once
#include<string>
#include "ResourceHandle.h"

namespace engine
{
	class ResourceLoader
	{
	public:
		virtual std::string getWildcardPattern() = 0;
		virtual unsigned int getLoadedSize(char* rawBuffer, unsigned int rawBufSize) = 0;
		virtual void load(char* rawBuffer, unsigned int rawBufSize, ResourceHandle * resHandle) = 0;
	};
}