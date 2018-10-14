#pragma once
#include "ProcessManager.h"
#include <string>
#include <GLFW/glfw3.h>

namespace engine {

	class AppConfig 
	{
	public:
		std::string logFileName;
		std::string appTitle;
		unsigned int height;
		unsigned int width;
		bool fullscreen;
	};

	class App
	{
		friend bool AppInit(App &app, AppConfig &config);
	public:
		virtual void VOnEngineInit() {}
		ProcessManager processManager;
	protected:
		GLFWwindow *glfwWindowHandle;
	};

}