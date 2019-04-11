#pragma once
#include "../../gamebasis/Actor.h"
#include "../../resourse/Resource.h"
#include "../Scene.h"
#include "TextureRC.h"

namespace engine
{
	class CustomShaderTextureRC : public TextureRC
	{
	private:
		static bool initialized;
		static unsigned int VBO, VAO, EBO;
		static constexpr float rect[] = {
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f };

		static constexpr unsigned int indices[] = {
		3, 1, 0,
		2, 3, 0 };
	public:
		Resource * texture;
		unsigned int shaderProgramId;
		static void render(std::vector<std::shared_ptr<RenderComponent>>& renderers, Scene& scene, const double ndelay);
		static void init();
		static const char* name;
		CustomShaderTextureRC() { id = Component::getId(name); }
		CustomShaderTextureRC(Resource * texture) : texture{ texture } { CustomShaderTextureRC(); }
		
		virtual const char* getName() const override { return name; }
		virtual bool VInit(const tinyxml2::XMLElement * pData) override;
	};
}