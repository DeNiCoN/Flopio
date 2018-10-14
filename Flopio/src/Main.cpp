#include "core/Engine.h"
#include <iostream>
#include <list>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Logger.h"
#include <fstream>
#include "core/ProcessManager.h"
#include "core/DelayProcess.h"

using namespace engine;
int main() {
	ProcessManager processManger;
	Logger logger(&std::cout);
	logger << "Initializing\n";
	if (!glfwInit()) {
		logger << "glfw failed to init\n";
	}
	GLFWwindow *window = glfwCreateWindow(800, 600, "test", NULL, NULL);
	if (window == NULL) {
		logger << "glfw window create failed\n";
	}
	glfwMakeContextCurrent(window);
	SharedProcessPtr proc(new DelayProcess(3.0));
	processManger.attachProcess(proc);
	double delta;
	double currentTime;
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		processManger.updateProcesses(delta);
		glfwPollEvents();

		lastTime = currentTime;
	}
}