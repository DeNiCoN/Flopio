#include "RenderComponent.h"
#include "../Engine.h"
#include "../resourse/loaders/VertexShaderResourceLoader.h"
#include "../resourse/loaders/FragmentShaderResourceLoader.h"
#include <tuple>

namespace engine
{
	bool RenderComponent::shaderInit(Resource * vertex, Resource * geometry, Resource * fragment)
	{
		char tmplog[512];
		int success;

		auto tuple = std::tuple(*vertex, *geometry, *fragment);
		if (shaderPrograms.find(tuple) == shaderPrograms.end())
		{

			shaderProgramId = glCreateProgram();
			if (vertex != nullptr)
			{
				auto handle = currentApp->resourceCache.getHandle(vertex);
				glAttachShader(shaderProgramId, static_cast<VertexShaderData>(*handle->getExtra())->getId());
			}
			else {
				logger << "RenderComponent::shaderInit error: vertex is null\n";
				return false;
			}

			if (fragment != nullptr)
			{
				auto handle = currentApp->resourceCache.getHandle(fragment);
				glAttachShader(shaderProgramId, static_cast<FragmentShaderData>(*handle->getExtra())->getId());
			}
			else {
				logger << "RenderComponent::shaderInit error: fragment is null\n";
				return false;
			}

			if (geometry != nullptr)
			{

			}

			glLinkProgram(shaderProgramId);
			glGetShaderiv(shaderProgramId, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderProgramId, 512, NULL, tmplog);
				engine::logger << "SHADER PROGRAM LINK FAILED\n" << tmplog << "\n";
				return false;
			};
			shaderPrograms[tuple] = shaderProgramId;
		}
		else 
		{
			shaderProgramId = shaderPrograms[tuple];
		}
		return true;
	}
}