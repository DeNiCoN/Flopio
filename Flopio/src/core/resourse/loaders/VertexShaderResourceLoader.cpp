#include "VertexShaderResourceLoader.h"
#include "../../Engine.h"
#include "../../utils.h"


namespace engine
{
	bool VertexShaderResourceLoader::VLoad(char * rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle)
	{
		char* extraData = resHandle->getBuffer();
		*reinterpret_cast<VertexShaderData*>(extraData) = VertexShaderData();
		std::shared_ptr<VertexShaderData> extra = std::shared_ptr<VertexShaderData>(reinterpret_cast<VertexShaderData*>(extraData));
		resHandle->setExtra(extra);

		return shaderLoadHelperFunc(GL_VERTEX_SHADER, rawBuffer, rawBufSize, &extra->vertexShaderID);
	}

	VertexShaderData::~VertexShaderData()
	{
		glDeleteShader(vertexShaderID);
	}
}