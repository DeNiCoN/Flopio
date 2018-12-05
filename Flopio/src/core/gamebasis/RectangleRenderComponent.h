#pragma once
#include "RenderComponent.h"
#include "../resourse/Resource.h"

namespace engine
{
	class RectangleRenderComponent : public RenderComponent
	{

	public:
		RectangleRenderComponent();
		RectangleRenderComponent(Resource texture);
		virtual void VRender(const double ndelay) override;
		virtual const char* getName() const override { return "RectangleRenderComponent"; }
	};
}