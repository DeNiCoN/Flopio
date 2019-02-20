#include "ActorFactory.h"
#include "../Engine.h"
#include "../resourse/loaders/XmlResourceLoader.h"
#include "../graphics/rendercomps/CustomShaderTextureRC.h"
#include "../graphics/rendercomps/TextureRC.h"

namespace engine
{

	ActorFactory::ActorFactory() 
	{
		registerComponent<CustomShaderTextureRC>(Component::getId(CustomShaderTextureRC::name));
		registerComponent<TextureRC>(Component::getId(TextureRC::name));
	}

	SharedActor ActorFactory::createActor(Resource xmlFile, const vec3 * initialPosition, const float * initialAngle)
	{
		createActor(std::static_pointer_cast<XmlExtraData>(currentApp->resourceCache.getHandle(xmlFile)->getExtra())->getRoot(), initialPosition, initialAngle);
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
			SharedComponent pComponent(createComponent(pNode));
			if (pComponent)
			{
				pActor->addComponent(pComponent);
			}
			else
			{
				return SharedActor();
			}
		}

		if (!initialPosition)
		{
			pActor->setPosition(*initialPosition);
		}

		if (!initialAngle)
		{
			pActor->setAngle(*initialAngle);
		}

		pActor->postInit();

		return pActor;
	}

}
