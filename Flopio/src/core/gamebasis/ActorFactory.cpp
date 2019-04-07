#include "ActorFactory.h"
#include "../Engine.h"
#include "../resourse/loaders/XmlResourceLoader.h"
#include "../graphics/rendercomps/CustomShaderTextureRC.h"
#include "../graphics/rendercomps/TextureRC.h"
#include <algorithm>

namespace engine
{

	ActorFactory::ActorFactory() 
	{
		registerComponent<CustomShaderTextureRC>(Component::getId(CustomShaderTextureRC::name));
		registerComponent<TextureRC>(Component::getId(TextureRC::name));
	}

	SharedActor ActorFactory::createActor(Resource xmlFile, const vec3 * initialPosition, const float * initialAngle)
	{
		if (auto handle = currentApp->resourceCache.getHandle(xmlFile))
			return createActor(std::static_pointer_cast<XmlExtraData>(handle->getExtra())->getRoot(), initialPosition, initialAngle);
		else
			return SharedActor();
	}

	SharedComponent ActorFactory::createComponent(const tinyxml2::XMLElement * pData)
	{
		const char* name = pData->Value();

		auto it = genericComponentFactory.find(Component::getId(name));


		if (it != genericComponentFactory.end())
		{
			SharedComponent pComponent(it->second());
			if (!pComponent->VInit(pData))
			{
				logger << "Component failed to initialize: " << name << "\n";
				return SharedComponent();
			}
			return pComponent;
		}
		else
		{
			logger << "Couldn't find the ActorComponent named " << std::string(name) << "\n";
			return SharedComponent();
		}
	}

	SharedActor ActorFactory::createActor(const tinyxml2::XMLElement* pRoot, const vec3 * initialPosition, const float * initialAngle)
	{
		if (!pRoot)
		{
			logger << "Failed to create actor from resource\n";
			return SharedActor();
		}

		unsigned int nextActorId = ActorFactory::nextActorId();
		SharedActor pActor(new Actor(nextActorId));
		if (!pActor->init(pRoot))
		{
			logger << "Failed to initialize actor\n";
			return SharedActor();
		}

		for (const tinyxml2::XMLElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			SharedComponent pComponent = createComponent(pNode);
			if (pComponent)
			{
				pActor->addComponent(pComponent);
			}
			else
			{
				return SharedActor();
			}
		}

		if (initialPosition)
		{
			pActor->setPosition(*initialPosition);
		}

		if (initialAngle)
		{
			pActor->setAngle(*initialAngle);
		}

		pActor->postInit();

		return pActor;
	}

}
