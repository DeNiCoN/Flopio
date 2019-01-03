#pragma once
#include "Actor.h"
#include "../resourse/Resource.h"

namespace engine
{
	class RectangleRenderComponent : public RenderComponent
	{
	private:
		static bool initialized;
		static unsigned int VBO,VAO,EBO;
		static constexpr float rect[] = { 
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f };

		static constexpr unsigned int indices[] = {
		0, 1, 3,
		0, 3, 2 };
	protected:
		float width = 1, height = 1;
		mat44 model;
		virtual void VOnActorAngleSet(float radians) override;
		virtual void VOnActorPositionSet(vec3 pos) override;
		void updateModel();
	public:
		Resource * texture;
		std::pair<float, float> getDimensions();
		void setDimensions(float width, float height);
		static void render(std::vector<SharedActor>& actors, Scene& scene, const double ndelay);
		static void init();
		RectangleRenderComponent() {}
		RectangleRenderComponent(Resource * texture) : texture{ texture } { RectangleRenderComponent(); }
		virtual const char* getName() const override { return "RectangleRenderComponent"; }
	};
}