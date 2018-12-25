#pragma once
#include "Actor.h"
#include <unordered_map>
#include <tuple>
#include "../resourse/ResourceCache.h"

namespace engine
{
	using ThreeResourceTuple = std::tuple<Resource, Resource, Resource>;
	class RenderComponent : public Component
	{
	private:
		unsigned int shaderProgramId;
		static std::unordered_map<ThreeResourceTuple, unsigned int> shaderPrograms;
	public:
		unsigned int getShaderProgramId() { return shaderProgramId; }
		static std::unordered_map<ThreeResourceTuple, unsigned int> getShaderProgramsMap() { return shaderPrograms; }
		bool shaderInit(Resource* vertex, Resource* geometry, Resource* fragment);
	};
}