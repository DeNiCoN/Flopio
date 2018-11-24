#pragma once
#include "../ResourceLoader.h"

namespace engine
{
	class DefaultResourceLoader : public ResourceLoader
	{
		virtual bool VUseRawFile() override { return true; }
		virtual bool VDiscardRawBufferAfterLoad() override { return false; }
		virtual const std::string* VGetWildcardPattern(int* size) override 
		{ std::string r[] = { "*" };*size = sizeof(r) / sizeof(r[0]); return r;}

		virtual unsigned int VGetLoadedSize(char * rawBuffer, unsigned int rawBufSize) override { return rawBufSize; }
		virtual unsigned int VGetExtraDataSize() override { return 0; }
		virtual bool VLoad(char * rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle) override { return true; };
	};
}