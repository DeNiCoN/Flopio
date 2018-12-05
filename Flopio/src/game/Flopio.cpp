#include "Flopio.h"
#include "../core/gamebasis/RectangleRenderComponent.h"
namespace game
{
	using namespace engine;

	void Flopio::VOnUpdate(const double delta)
	{

	}

	void Flopio::VOnRender(const double ndelay)
	{
		root.render(ndelay);
	}

	void Flopio::VOnInit()
	{
		RectangleRenderComponent render;
		std::shared_ptr<RenderComponent> renderPtr { &render };
		ship.renderer = renderPtr;
		std::shared_ptr<Actor> actorPtr { &ship };
		root.addActor(actorPtr);
	}
}
