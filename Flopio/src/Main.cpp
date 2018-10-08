#include"core/Engine.h"
#include <iostream>
#include <list>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Logger.h"
#include <fstream>

int main() {
	Logger logger(&std::cout);
	logger << "Initializing";
	if (!glfwInit()) {
		logger << "glfw failed to init";
	}

	return 0;
}
