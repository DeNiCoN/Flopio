#pragma once
#include <vector>
#include "../utils.h"
#include <memory>
#include <map>
#include <tuple>
#include <optional>
#include "../resourse/ResourceCache.h"
#include "../math/linearmath.h"
#include <algorithm>
#include <tinyxml2.h>

namespace engine
{
	class Actor;

	class Component
	{
		friend class Actor;
	private:
		static std::vector<unsigned int> renderIds;
		void setParent(Actor * parent) { this->parent = parent; }
	protected:
		virtual void VOnActorAngleSet(float radians) {}
		virtual void VOnActorPositionSet(vec3 pos) {}
		virtual void VUpdate(const double delta) {}
		virtual void VPostInit() {};
		Actor* parent;
		unsigned int id;
	public:
		virtual ~Component() {};
		bool hasParent() { return parent; }
		virtual const char* getName() const = 0;
		virtual bool VInit(const tinyxml2::XMLElement * pData) = 0;

		unsigned int getId() { return id; };
		static constexpr unsigned int getId(const char* name) { return hash(name, strlen(name)); }

		//keep renderIds sorted for binary search
		static void registerRender(unsigned int id) { renderIds.insert(std::lower_bound(renderIds.begin(), renderIds.end(), id), id); }
		static bool isRender(unsigned int id) { return std::binary_search(renderIds.begin(), renderIds.end(), id); };
	};

	using SharedComponent = std::shared_ptr<Component>;
	using ThreeResourceTuple = std::tuple<Resource, std::optional<Resource>, Resource>;

	class RenderComponent : public Component
	{
		friend class Actor;
	private:
		static std::map<ThreeResourceTuple, unsigned int> shaderPrograms;
	public:
		virtual ~RenderComponent() {};
		static std::map<ThreeResourceTuple, unsigned int> getShaderProgramsMap() { return shaderPrograms; }
		static unsigned int shaderInit(Resource* vertex, Resource* geometry, Resource* fragment);
		static unsigned int shaderInitFromXML(const tinyxml2::XMLElement * pData);
	};

	class Actor
	{
		friend class Scene;
	private:
		std::vector<SharedComponent> components;
		vec3 position;
		vec3 speed;
		float angle;
		unsigned int id;
	public:
		Actor(unsigned int id) : id(id) {}
		~Actor();
		void setPosition(vec3 pos);
		vec3 getPosition() const { return position; }
		float getAngle() const { return angle; }
		void setAngle(float radians);
		void addComponent(SharedComponent component);
		void removeComponent(SharedComponent component);

		bool init(const tinyxml2::XMLElement * root) { return true; }
		void postInit() { for (auto comp : components) comp->VPostInit(); }
		void update(const double delta) { for (auto comp : components) comp->VUpdate(delta); }
	};
	typedef std::shared_ptr<Actor> SharedActor;
}