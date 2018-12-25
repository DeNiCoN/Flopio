#pragma once
#include "RenderComponent.h"
#include "../resourse/Resource.h"

namespace engine
{
	class RectangleRenderComponent : public RenderComponent
	{
	private:
		static bool initialized;
		static unsigned int VBO,VAO,EBO;
		static constexpr float rect[] = { 
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f };

		static constexpr unsigned int indices[] = {
		0, 1, 3,
		0, 3, 2 };

		Resource * texture;

	public:
		static void render(std::vector<SharedActor>& actors, const double ndelay);
		static void init();
		RectangleRenderComponent() {}
		RectangleRenderComponent(Resource * texture) : texture{ texture } { RectangleRenderComponent(); }
		virtual const char* getName() const override { return "RectangleRenderComponent"; }
	};
}