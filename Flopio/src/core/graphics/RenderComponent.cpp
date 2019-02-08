#include "../gamebasis/Actor.h"
#include "../Engine.h"
#include "../resourse/loaders/VertexShaderResourceLoader.h"
#include "../resourse/loaders/FragmentShaderResourceLoader.h"
#include <tuple>

namespace engine
{
	std::map<ThreeResourceTuple, unsigned int> RenderComponent::shaderPrograms;

	unsigned int RenderComponent::shaderInit(Resource * vertex, Resource * geometry, Resource * fragment)
	{
		unsigned int shaderProgramId;
		char tmplog[1024];
		int success;

		ThreeResourceTuple tuple;

		if (geometry == nullptr)
			tuple = std::make_tuple(*vertex, std::nullopt, *fragment);
		else
			tuple = std::make_tuple(*vertex, *geometry, *fragment);

		if (shaderPrograms.find(tuple) == shaderPrograms.end())
		{

			std::shared_ptr<ResourceHandle> vhandle;
			if (vertex != nullptr)
			{
				vhandle = currentApp->resourceCache.getHandle(*vertex);
				if (!vhandle)
					return 0;
			}
			else {
				logger << "RenderComponent::shaderInit error: vertex is null\n";
				return 0;
			}

			std::shared_ptr<ResourceHandle> fhandle;
			if (fragment != nullptr)
			{
				fhandle = currentApp->resourceCache.getHandle(*fragment);
				if (!fhandle)
					return 0;
			}
			else {
				logger << "RenderComponent::shaderInit error: fragment is null\n";
				return 0;
			}

			shaderProgramId = glCreateProgram();

			glAttachShader(shaderProgramId, static_cast<VertexShaderData*>(&*vhandle->getExtra())->getId());
			glAttachShader(shaderProgramId, static_cast<FragmentShaderData*>(&*fhandle->getExtra())->getId());

			glLinkProgram(shaderProgramId);
			glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shaderProgramId, 1024, NULL, tmplog);
				engine::logger << "SHADER PROGRAM LINK FAILED\n" << tmplog << "\n";
				glDeleteProgram(shaderProgramId);
				return 0;
			};

			shaderPrograms[tuple] = shaderProgramId;
		}
		else 
		{
			shaderProgramId = shaderPrograms[tuple];
		}
		return shaderProgramId;
	}
}