#include "DefaultResourceLoader.h"

bool engine::DefaultResourceLoader::VUseRawFile()
{
	return false;
}

bool engine::DefaultResourceLoader::VDiscardRawBufferAfterLoad()
{
	return false;
}

std::string engine::DefaultResourceLoader::VGetWildcardPattern()
{
	return std::string();
}

unsigned int engine::DefaultResourceLoader::VGetLoadedSize(char * rawBuffer, unsigned int rawBufSize)
{
	return 0;
}

unsigned int engine::DefaultResourceLoader::VGetExtraDataSize()
{
	return 0;
}

bool engine::DefaultResourceLoader::VLoad(char * rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle)
{
	return false;
}
