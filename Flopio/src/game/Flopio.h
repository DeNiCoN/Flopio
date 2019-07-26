#pragma once
#include "../core/gamebasis/App.h"
using namespace engine;
namespace game {
	class Flopio : public App
	{
	public:
		virtual void VOnResize(GLFWwindow* window, int width, int height) override;
		virtual void VOnUpdate(const double delta) override;
		virtual void VOnRender(const double ndelay) override;
		virtual void VOnInit() override;
		virtual void VOnExit() override;
		void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void dropCallback(GLFWwindow* window, int count, const char** paths);
		Flopio() : App(1, 1) {}
	private:
		void updateCam(const double delta);
	};
}