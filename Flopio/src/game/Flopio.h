#pragma once
#include "../core/gamebasis/App.h"
using namespace engine;
namespace game {
	class Flopio : public App
	{
	public:
		virtual void VOnResize(GLFWwindow* window, int width, int height) override;
		virtual void VOnUpdate() override;
		virtual void VOnRender(const double ndelay) override;
		virtual void VOnInit() override;
		Flopio() : App(1) {}
	private:
		Actor ship;
	};
}