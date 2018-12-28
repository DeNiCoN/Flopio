#pragma once
#include "ProcessManager.h"
#include "../resourse/ResourceCache.h"
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Scene.h"

namespace engine {

	class AppConfig 
	{
	public:
		AppConfig(int width = 800, int height = 600,
			double secondsPerUpdate = 1.0/60.0,
			bool fullscreen = false,
			std::string appTitle = "App", 
			std::string logFileName = "lastestLog.txt")
			: width(width), height(height), 
			secondsPerUpdate(secondsPerUpdate),
			fullscreen(fullscreen), 
			appTitle(appTitle), logFileName(logFileName) {}
		std::string logFileName;
		std::string appTitle;
		unsigned int height;
		unsigned int width;
		bool fullscreen;
		double secondsPerUpdate;
	};

	class App
	{
		friend void VOnResizeWrapper(GLFWwindow* window, int width, int height);
		friend bool appInit(App &app, const AppConfig &config);
	public:
		App(unsigned int resourceCacheSizeInMB) : resourceCache(resourceCacheSizeInMB) {}
		ProcessManager processManager;
		ResourceCache resourceCache;
	protected:
		virtual void VOnInit() {}
		virtual void VOnUpdate(const double delta) = 0;
		virtual void VOnRender(const double ndelta) = 0;
		virtual void VOnResize(GLFWwindow* window, int width, int height) {}
		GLFWwindow *glfwWindowHandle;
		Scene root;
		double secondsPerUpdate;
	};

}