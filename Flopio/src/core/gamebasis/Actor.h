#pragma once
#include <vector>
#include "../utils.h"
#include <memory>
#include <map>
#include <tuple>
#include <optional>
#include "../resourse/ResourceCache.h"
#include "../math/linearmath.h"

namespace engine
{
	class Actor;

	class Component
	{
	protected:
		Actor* parent;
	public:
		void setParent(Actor * parent) { this->parent = parent; }
		virtual void update(double delta) {}
		virtual const char* getName() const = 0;
		constexpr unsigned int getId() const { return hash(getName(), strlen(getName())); }

	};

	using SharedComponent = std::shared_ptr<Component>;
	using ThreeResourceTuple = std::tuple<Resource, std::optional<Resource>, Resource>;

	class RenderComponent : public Component
	{
		friend class Actor;
	private:
		static std::map<ThreeResourceTuple, unsigned int> shaderPrograms;
	protected:
		virtual void VOnActorAngleSet(float radians) {}
		virtual void VOnActorPositionSet(vec3 pos) {}
	public:
		static std::map<ThreeResourceTuple, unsigned int> getShaderProgramsMap() { return shaderPrograms; }
		static unsigned int shaderInit(Resource* vertex, Resource* geometry, Resource* fragment);
	};

	class Actor
	{
	private:
		std::vector<SharedComponent> components;
		float angle;
		vec3 position;
		vec3 speed;
		std::shared_ptr<RenderComponent> renderer;
	public:
		void setPosition(vec3 pos);
		vec3 getPosition() const { return position; }
		float getAngle() const { return angle; }
		void setAngle(float radians);
		void setRenderer(std::shared_ptr<RenderComponent> renderer);
		void addComponent(SharedComponent component);
		void removeComponent(SharedComponent component);
		std::shared_ptr<RenderComponent> getRenderer() const { return renderer; }
	};
	typedef std::shared_ptr<Actor> SharedActor;
}