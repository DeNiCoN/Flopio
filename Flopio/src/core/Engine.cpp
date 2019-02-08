#pragma once
#include "Engine.h"
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace engine {

	App *currentApp;
	Logger logger;
	LinearAllocator oneFrame;
	char* oneFrameBuffer;

	void GLAPIENTRY GLErrorCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		logger << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **\n" : "\n") <<
			"type = " << type << ", severity = " << severity << "\n" << message << "\n";
	}

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

		oneFrameBuffer = (char*) malloc(1024 * 1024 * 1);
		linAllocInit(&oneFrame, oneFrameBuffer, 1024 * 1024 * 1);

		stbi_set_flip_vertically_on_load(true);
		
		glfwSetErrorCallback(glfwErrorCallback);

		if (!glfwInit())
			return false;

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		glfwWindowHint(GLFW_SAMPLES, 4);

		GLFWwindow *window = glfwCreateWindow(config.width, config.height, config.appTitle.c_str(), config.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (window == NULL)
			return false;
		glfwMakeContextCurrent(window);
		glfwSetWindowSizeCallback(window, VOnResizeWrapper);
		glfwSwapInterval(0);

		app.glfwWindowHandle = window;

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			logger << "Failed to initialize GLAD\n";
			return false;
		}
		//glEnable(GL_DEBUG_OUTPUT);
		//glDebugMessageCallback(GLErrorCallback, 0);

		if (!GLAD_GL_ARB_bindless_texture)
		{
			logger << "Bindless textures not supported\n";
			return false;
		}
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		
		app.VOnInit();
		int fps = 0;
		double second = 0.0;
		double currentTime;
		double delta;
		double delay = 0.0;
		double lastTime = glfwGetTime();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		while (!glfwWindowShouldClose(window))
		{
			currentTime = glfwGetTime();
			delta = currentTime - lastTime;
			lastTime = currentTime;
			delay += delta;
			second += delta;
			while (delay >= app.secondsPerUpdate)
			{
				delay -= app.secondsPerUpdate;
				app.VOnUpdate();
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			app.VOnRender(delay / app.secondsPerUpdate);
			glfwSwapBuffers(window);
			
			fps++;
			if (second >= 1.0)
			{
				std::cout << fps << "\n";
				fps = 0;
				second = 0.0;
			}
			linReset(&oneFrame);
			glfwPollEvents();
		}

		free(oneFrameBuffer);

		glfwTerminate();
		return 0;
	}

	void* oneFrameAlloc(size_t size)
	{
		return linAlloc(&oneFrame, size);
	}

}