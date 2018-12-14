#include "RenderComponent.h"
#include "../Engine.h"
#include "../resourse/loaders/VertexShaderResourceLoader.h"
#include "../resourse/loaders/FragmentShaderResourceLoader.h"
#include <tuple>

namespace engine
{
	bool RenderComponent::shaderInit(Resource * vertex, Resource * geometry, Resource * fragment)
	{
		auto tuple = std::tuple(*vertex, *geometry, *fragment);
		if (shaderPrograms.find(tuple) == shaderPrograms.end())
		{

			shaderProgramId = glCreateProgram();
			if (vertex != nullptr)
			{
				auto handle = currentApp->resourceCache.getHandle(vertex);
				glAttachShader(shaderProgramId, static_cast<std::shared_ptr<VertexShaderData>>(handle->getExtra())->getId());
			}
			else {
				logger << "RenderComponent::shaderInit error: vertex is null\n";
				return false;
			}

			if (fragment != nullptr)
			{
				auto handle = currentApp->resourceCache.getHandle(fragment);
				glAttachShader(shaderProgramId, static_cast<std::shared_ptr<FragmentShaderData>>(handle->getExtra())->getId());
			}
			else {
				logger << "RenderComponent::shaderInit error: fragment is null\n";
				return false;
			}

			if (geometry != nullptr)
			{

			}

			glLinkProgram(shaderProgramId);

		}
		else 
		{
			shaderProgramId = shaderPrograms[tuple];
		}
		return true;
	}
}