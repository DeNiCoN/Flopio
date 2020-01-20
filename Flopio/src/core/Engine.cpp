#include "Engine.h"
#include "utils/Benchmark.h"
#include "gamebasis/events/WindowResizeEventData.h"
#include "gamebasis/events/MouseEventData.h"
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


namespace engine {

    double frame;
    double render;
    double update;

    App *currentApp;
    Logger logger;
    LinearAllocator oneFrame;
    char oneFrameBuffer[1024 * 1024 * 1];
	
//*    void GLAPIENTRY GLErrorCallback(GLenum source,
//*                                    GLenum type,
//*                                    GLuint id,
//*                                    GLenum severity,
//*                                    GLsizei length,
//*                                    const GLchar* message,
//*                                    const void* userParam)
//*    {
//*        logger << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **\n" : "\n") <<
//*            "type = " << type << ", severity = " << severity << "\n" << message << "\n";
//*    }

    void glfwErrorCallback(int error, const char* description) 
    {
        logger << description << "\n";
    }

    void mouse_click_callback(GLFWwindow* window, int button, int action, int mode)
    {
        int width, height;
        double xpos, ypos;
        glfwGetWindowSize(window, &width, &height);
        glfwGetCursorPos(window, &xpos, &ypos);
        vec4 pos = vec4CreateSse(xpos - width / 2, height / 2 - ypos, 1, 1);
        pos.ssevalue = sseVecMat44Multiply(pos.ssevalue, mat44TransformInverse(currentApp->root.camera.getView()));
        vec2 pos2 = { {pos.x, pos.y} };
        currentApp->eventManager.newEvent<MouseClickEventData>("MouseClick", { { {static_cast<float>(xpos), static_cast<float>(ypos)} }, pos2, button, action, mode });
    }

    void resize_callback(GLFWwindow* window, int width, int height) 
    { 
        currentApp->viewport.VResize(width, height);
        currentApp->eventManager.newEvent<WindowResizeEventData>("WindowResize", { width, height });
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        currentApp->eventManager.newEvent<MouseScrollEventData>("MouseScroll", { xoffset, yoffset });
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

        linAllocInit(&oneFrame, oneFrameBuffer, 1024 * 1024 * 1);

        stbi_set_flip_vertically_on_load(true);
		
        glfwSetErrorCallback(glfwErrorCallback);
		
        if (!glfwInit())
            return false;
        logger << "Glfw initialized\n";
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //*glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        //*glfwWindowHint(GLFW_SAMPLES, 16);
		
        GLFWwindow *window = glfwCreateWindow(config.width, config.height, config.appTitle.c_str(), config.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
        if (window == NULL)
            return false;
        glfwMakeContextCurrent(window);
        glfwSetWindowSizeCallback(window, resize_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetMouseButtonCallback(window, mouse_click_callback);
        glfwSwapInterval(0);

        app.glfwWindowHandle = window;

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            logger << "Failed to initialize GLAD\n";
            return false;
        }
        logger << "Glad initialized\n";
        //glEnable(GL_DEBUG_OUTPUT);
        //glDebugMessageCallback(GLErrorCallback, 0);

        //*if (!GLAD_GL_ARB_bindless_texture)
        //*{
        //*logger << "Bindless textures not supported\n";
        //*return false;
        //*}
		
        //*glEnable(GL_DEPTH_TEST);
        //*glEnable(GL_MULTISAMPLE);
        //glEnable(GL_SAMPLE_SHADING);
        //glMinSampleShading(1.f);
        app.VOnInit();
        double second = 0.0;
        double currentTime;
        double delta;
        double delay = 0.0;
        double lastTime = glfwGetTime();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        while (!glfwWindowShouldClose(window))
        {
            static double endOfUpdate;
            currentTime = glfwGetTime();
            delta = currentTime - lastTime;

            lastTime = currentTime;
            delay += delta;
            second += delta;
            while (delay >= app.secondsPerUpdate)
            {
                delay -= app.secondsPerUpdate;
                app.VOnUpdate(app.secondsPerUpdate);
            }
            endOfUpdate = glfwGetTime();
            update = endOfUpdate - currentTime;

            app.VOnRender(delay / app.secondsPerUpdate);
            render = glfwGetTime() - endOfUpdate;

            glfwSwapBuffers(window);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            frame = delta;

            linReset(&oneFrame);
            glfwPollEvents();
        }
        app.VOnExit();

        glfwTerminate();
        return 0;
    }

    void* oneFrameAlloc(size_t size)
    {
        return linAlloc(&oneFrame, size);
    }

}
