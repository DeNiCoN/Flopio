#pragma once
#include "Actor.h"
#include <map>
#include <tuple>
#include <optional>
#include "../resourse/ResourceCache.h"

namespace engine
{
	using ThreeResourceTuple = std::tuple<Resource, std::optional<Resource>, Resource>;
	class RenderComponent : public Component
	{
	private:
		unsigned int shaderProgramId;
		static std::map<ThreeResourceTuple, unsigned int> shaderPrograms;
	public:
		unsigned int getShaderProgramId() { return shaderProgramId; }
		static std::map<ThreeResourceTuple, unsigned int> getShaderProgramsMap() { return shaderPrograms; }
		bool shaderInit(Resource* vertex, Resource* geometry, Resource* fragment);
	};
}