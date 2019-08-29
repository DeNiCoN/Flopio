#pragma once
#include "../core/gamebasis/App.h"
using namespace engine;
namespace game {
	class Flopio : public App
	{
	public:
		void VOnUpdate(const double delta) override;
		void VOnRender(const double ndelay) override;
		void VOnInit() override;
		void VOnExit() override;
		void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void dropCallback(GLFWwindow* window, int count, const char** paths);
		Flopio() : App(1, 1) {}
	private:
		void updateCam(const double delta);
	};
}
