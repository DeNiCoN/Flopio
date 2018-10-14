#pragma once
#include "Engine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Logger.h"
#include <fstream>

namespace engine {

	void glfwErrorCallback(int error, const char* description) 
	{
		log << description << std::endl;
	}

	bool AppInit(App &app, AppConfig &config) 
	{
		std::ofstream out(config.logFileName);
		log.addStream(out);
		
		glfwSetErrorCallback(glfwErrorCallback);

		if (!glfwInit())
			return false;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow *window = glfwCreateWindow(config.width, config.height, config.appTitle.c_str, config.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (window == NULL)
			return false;
		app.glfwWindowHandle = window;

		app.VOnEngineInit();
	}

}