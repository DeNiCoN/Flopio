#pragma once
#include <vector>
#include "viewports/Viewport.h"
#include "../gamebasis/Actor.h"
#include <unordered_map>
#include <Box2D/Box2D.h>

namespace engine
{

	class Camera
	{
	private:
		mat44 view;
		vec2 position = { 0, 0 };
		float angle = 0;
		float scale = 1;
		void updateView();
	public:
		Camera();
		vec2 getPosition() const { return position; }
		float getAngle() const { return angle; }
		float getScale() const { return scale; }
		const mat44 * getView() const { return &view; }
		void set(vec2 position, float angle, float scale);
	};

	class Scene;
	using RenderFuntion = void(*)(std::vector<std::shared_ptr<RenderComponent>>&, Scene&, const double);
	class Scene
	{
	public:
		~Scene();
		Scene() : world({ 0.f, 9.8f }) {}
		Camera camera;
		void render(const double ndelay);
		void update(const double delta);
		void addActor(SharedActor actor);
		void removeActor(SharedActor actor);
		static void registerRenderer(unsigned int componentId, RenderFuntion renderFunction);
		Viewport* getViewport() const { return viewport; }
		const mat44* getProjectionView() const { return &projectionView; }
		b2World& getWorld() { return world; }
		void setViewport(Viewport* viewport) { this->viewport = viewport; }
		std::vector<SharedActor> actors;
	private:
		void rendererRemoved(std::shared_ptr<RenderComponent> renderer);
		Viewport* viewport;
		b2World world;
		mat44 projectionView;
		std::vector<std::shared_ptr<RenderComponent>> renderers;
		std::unordered_map<unsigned int, std::vector<std::shared_ptr<RenderComponent>>> tempRenderersMap;
		static std::unordered_map<unsigned int, RenderFuntion> renderFunctionsMap;
	};
}