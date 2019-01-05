#include "Actor.h"
#include "../Engine.h"
#include "../resourse/loaders/VertexShaderResourceLoader.h"
#include "../resourse/loaders/FragmentShaderResourceLoader.h"
#include <tuple>

namespace engine
{
	std::map<ThreeResourceTuple, unsigned int> RenderComponent::shaderPrograms;

	bool RenderComponent::shaderInit(Resource * vertex, Resource * geometry, Resource * fragment)
	{
		char tmplog[512];
		int success;

		ThreeResourceTuple tuple;

		if (geometry == nullptr)
			tuple = std::make_tuple(*vertex, std::nullopt, *fragment);
		else
			tuple = std::make_tuple(*vertex, *geometry, *fragment);

		if (shaderPrograms.find(tuple) == shaderPrograms.end())
		{

			shaderProgramId = glCreateProgram();
			if (vertex != nullptr)
			{
				auto handle = currentApp->resourceCache.getHandle(vertex);
				if (!handle)
					return false;
				glAttachShader(shaderProgramId, static_cast<VertexShaderData*>(&*handle->getExtra())->getId());
			}
			else {
				logger << "RenderComponent::shaderInit error: vertex is null\n";
				return false;
			}

			if (fragment != nullptr)
			{
				auto handle = currentApp->resourceCache.getHandle(fragment);
				if (!handle)
					return false;
				glAttachShader(shaderProgramId, static_cast<FragmentShaderData*>(&*handle->getExtra())->getId());
			}
			else {
				logger << "RenderComponent::shaderInit error: fragment is null\n";
				return false;
			}

			glLinkProgram(shaderProgramId);
			glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
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