#pragma once
#include "../ResourceLoader.h"

namespace engine
{
	class DefaultResourceLoader : ResourceLoader
	{
		// Inherited via ResourceLoader
		virtual bool VUseRawFile() override { return true; }
		virtual bool VDiscardRawBufferAfterLoad() override { return false; }
		virtual std::string VGetWildcardPattern() override { return "*"; }
		virtual unsigned int VGetLoadedSize(char * rawBuffer, unsigned int rawBufSize) override { return rawBufSize; }
		virtual unsigned int VGetExtraDataSize() override { return 0; }
		virtual bool VLoad(char * rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle) override { return true; };
	};
}