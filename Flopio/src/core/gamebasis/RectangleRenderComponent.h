#pragma once
#include "RenderComponent.h"
#include "../resourse/Resource.h"

namespace engine
{
	class RectangleRenderComponent : RenderComponent
	{

	public:
		RectangleRenderComponent();
		RectangleRenderComponent(Resource texture);
		virtual void VRender(const double ndelay) override;
	};
}