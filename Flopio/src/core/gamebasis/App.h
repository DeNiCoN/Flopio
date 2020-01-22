#pragma once
#include "ProcessManager.h"
#include "../resourse/ResourceCache.h"
#include <string>
#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../graphics/Scene.h"
#include "EventManager.h"
#include "../graphics/viewports/Viewport.h"
#include "../graphics/viewports/ScreenViewport.h"

namespace engine {
	class AppConfig 
	{
	public:
		AppConfig(int width = 800, int height = 600,
			bool fullscreen = false,
			std::string appTitle = "App", 
			std::string logFileName = "lastestLog.txt")
			: width(width), height(height),
			fullscreen(fullscreen), 
			appTitle(std::move(appTitle)), logFileName(std::move(logFileName)) {}
    unsigned int width;
    unsigned int height;
    bool fullscreen;
    std::string appTitle;
    std::string logFileName;
	};

	class App
	{
		friend void resize_callback(GLFWwindow* window, int width, int height);
		friend bool appInit(App &app, const AppConfig &config);
		friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); 
		friend void drop_callback(GLFWwindow* window, int count, const char** paths); 
		friend void mouse_click_callback(GLFWwindow*, int, int, int);
	public:
		ProcessManager processManager;
		ResourceCache resourceCache;
		EventManager eventManager;
		App(unsigned int RCSizeInMB, unsigned int EMSizeInMB)
      : resourceCache(RCSizeInMB), eventManager(EMSizeInMB), viewport(sv) {}
    virtual ~App() = default;
	protected:
		ScreenViewport sv;
		Viewport& viewport;
		GLFWwindow *glfwWindowHandle;
		Scene root;
		virtual void VOnInit() {}
		virtual void VOnExit() {}
		virtual void VOnUpdate(const double delta) = 0;
		virtual void VOnRender(const double ndelta) = 0;
		double secondsPerUpdate = 1.0 / 60.0;
	};
}
