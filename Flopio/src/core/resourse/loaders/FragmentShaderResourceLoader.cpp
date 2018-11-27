#include "FragmentShaderResourceLoader.h"
#include "../../Engine.h"
#include "../../utils.h"

namespace engine
{
	bool FragmentShaderResourceLoader::VLoad(char * rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle)
	{
		char* extraData = resHandle->getBuffer();
		*reinterpret_cast<FragmentShaderData*>(extraData) = FragmentShaderData();
		std::shared_ptr<FragmentShaderData> extra = std::shared_ptr<FragmentShaderData>(reinterpret_cast<FragmentShaderData*>(extraData);
		resHandle->setExtra(extra);

		return shaderLoadHelperFunc(GL_VERTEX_SHADER, rawBuffer, rawBufSize, &extra->fragmentShaderID);
	}

	FragmentShaderData::~FragmentShaderData()
	{
		glDeleteShader(fragmentShaderID);
	}
}