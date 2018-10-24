#pragma once
#include "Engine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Logger.h"
#include <iostream>
#include <fstream>

namespace engine {

	App *currentApp;
	Logger log;

	void glfwErrorCallback(int error, const char* description) 
	{
		log << description << "\n";
	}

	void VOnResizeWrapper(GLFWwindow* window, int width, int height) 
	{ 
		glViewport(0, 0, width, height);
		currentApp->VOnResize(window, width, height);
	}

	bool appInit(App &app, AppConfig &config) 
	{
		currentApp = &app;
		std::ofstream out(config.logFileName);
		log.addStream(&out);
		
		glfwSetErrorCallback(glfwErrorCallback);

		if (!glfwInit())
			return false;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow *window = glfwCreateWindow(config.width, config.height, config.appTitle.c_str(), config.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (window == NULL)
			return false;
		glfwMakeContextCurrent(window);
		app.glfwWindowHandle = window;

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			log << "Failed to initialize GLAD" << "\n";
			return false;
		}
		glViewport(0, 0, config.width, config.height);
		glfwSetWindowSizeCallback(window, VOnResizeWrapper);
		
		app.VOnInit();

		double currentTime;
		double delta;
		double lastTime = glfwGetTime();

		while (!glfwWindowShouldClose(window))
		{
			currentTime = glfwGetTime();
			delta = currentTime - lastTime;


			app.VOnUpdate(delta);

			lastTime = currentTime;
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;
	}

}