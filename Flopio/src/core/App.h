#pragma once
#include "ProcessManager.h"
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine {

	class AppConfig 
	{
	public:
		AppConfig(int width = 800, int height = 600, bool fullscreen = false, std::string appTitle = "App", std::string logFileName = "lastestLog.txt")
			: width(width), height(height), fullscreen(fullscreen), appTitle(appTitle), logFileName(logFileName) {}
		std::string logFileName;
		std::string appTitle;
		unsigned int height;
		unsigned int width;
		bool fullscreen;
	};

	class App
	{
		friend void VOnResizeWrapper(GLFWwindow* window, int width, int height);
		friend bool appInit(App &app, AppConfig &config);
	public:
		virtual void VOnInit() {}
		virtual void VOnUpdate(double delta) = 0;
		ProcessManager processManager;
	protected:
		virtual void VOnResize(GLFWwindow* window, int width, int height) {}
		GLFWwindow *glfwWindowHandle;
	};

}