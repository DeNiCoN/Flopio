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
		0, 1, 3,
		0, 3, 2 };
	public:
		Resource * texture;
		unsigned int shaderProgramId;
		static void render(std::vector<SharedActor>& actors, Scene& scene, const double ndelay);
		static void init();
		CustomShaderTextureRC() {}
		CustomShaderTextureRC(Resource * texture) : texture{ texture } { CustomShaderTextureRC(); }
		
		static const char* name;
		virtual const char* getName() const override { return name; }
		virtual bool VInit(const tinyxml2::XMLElement * pData) override;
	};
}