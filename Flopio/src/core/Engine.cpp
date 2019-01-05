#pragma once
#include "Engine.h"
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace engine {

	App *currentApp;
	Logger logger;

	void glfwErrorCallback(int error, const char* description) 
	{
		logger << description << "\n";
	}

	void VOnResizeWrapper(GLFWwindow* window, int width, int height) 
	{ 
		currentApp->VOnResize(window, width, height);
	}

	bool appInit(App &app, const AppConfig &config) 
	{
		currentApp = &app;
		std::ofstream out(config.logFileName);
		logger.addStream(&out);
		logger.addStream(&std::cout);

		if (!currentApp->resourceCache.init())
		{
			logger << "Failed to initialize resource cache\n";
			return false;
		}

		stbi_set_flip_vertically_on_load(true);
		
		glfwSetErrorCallback(glfwErrorCallback);

		if (!glfwInit())
			return false;

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		GLFWwindow *window = glfwCreateWindow(config.width, config.height, config.appTitle.c_str(), config.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (window == NULL)
			return false;
		glfwMakeContextCurrent(window);
		app.glfwWindowHandle = window;

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			logger << "Failed to initialize GLAD" << "\n";
			return false;
		}

		glEnable(GL_DEPTH_TEST);

		glfwSetWindowSizeCallback(window, VOnResizeWrapper);
		
		app.VOnInit();

		double currentTime;
		double delta;
		double delay = 0.0;
		double lastTime = glfwGetTime();

		while (!glfwWindowShouldClose(window))
		{
			currentTime = glfwGetTime();
			delta = currentTime - lastTime;
			delay += delta;
			while (delay >= app.secondsPerUpdate)
			{
				delay -= app.secondsPerUpdate;
				//std::cout << delay << ":" << app.secondsPerUpdate << ":" << delta << "\n";
				app.VOnUpdate();
			}

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			app.VOnRender(delay / app.secondsPerUpdate);

			lastTime = currentTime;
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;
	}

}