#pragma once
#include "../../gamebasis/Actor.h"
#include "../../resourse/Resource.h"
#include "../Scene.h"

namespace engine
{
	class TextureRC : public RenderComponent
	{
	private:
		static bool initialized;
		static unsigned int shaderProgramId;
		static unsigned int VBO, VAO, EBO, transformBO, textureHandleBO;
		static constexpr float rect[] = { 
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f };

		static constexpr unsigned int indices[] = {
		0, 1, 3,
		0, 3, 2 };
	protected:
		float width = 64, height = 64;
		mat44 model;
		virtual void VOnActorAngleSet(float radians) override;
		virtual void VOnActorPositionSet(vec3 pos) override;
		void updateModel();
	public:
		~TextureRC() { texture = nullptr; }
		std::shared_ptr<ResourceHandle> texture;
		std::pair<float, float> getDimensions();
		void setDimensions(float width, float height);
		static void render(std::vector<std::shared_ptr<RenderComponent>>& renderers, Scene& scene, const double ndelay);
		static void init();

		static const char* name;
		TextureRC() { id = Component::getId(name); }
		virtual const char* getName() const override { return name; }

		virtual bool VInit(const tinyxml2::XMLElement * pData) override;
		virtual void VPostInit() override;
	};
}