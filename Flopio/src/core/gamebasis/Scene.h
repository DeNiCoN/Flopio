#pragma once
#include <vector>
#include "viewports/Viewport.h"
#include "Actor.h"
#include <unordered_map>

namespace engine
{

	class Camera
	{
	private:
		mat44 view;
		vec3 position = { 0, 0, 0 };
		float angle = 0;
		float scale = 1;
		void updateView();
	public:
		vec3 getPosition() const { return position; }
		float getAngle() const { return angle; }
		float getScale() const { return scale; }
		mat44 getView() const { return view; }
		void set(vec3 position, float angle, float scale);
	};

	using RenderFuntion = void(*)(std::vector<SharedActor>&, Scene&, const double);
	class Scene
	{
	public:
		void render(const double ndelay);
		void addActor(SharedActor actor) { actors.push_back(actor); }
		static void registerRenderer(unsigned int componentId, RenderFuntion renderFunction);
		Viewport* getViewport() const { return viewport; }
		void setViewport(Viewport* viewport) { this->viewport = viewport; }
	private:
		Viewport* viewport;
		Camera camera;
		std::vector<SharedActor> actors;
		static std::unordered_map<unsigned int, std::vector<SharedActor>> tempActorsMap;
		static std::unordered_map<unsigned int, RenderFuntion> renderFunctionsMap;
	};
}