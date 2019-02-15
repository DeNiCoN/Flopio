#include "Flopio.h"
#include "../core/Engine.h"
#include "../core/graphics/rendercomps/TextureRC.h"
#include "../core/resourse/files/DirectoryResourceFile.h"
#include "../core/resourse/loaders/FragmentShaderResourceLoader.h"
#include "../core/resourse/loaders/VertexShaderResourceLoader.h"
#include "../core/resourse/loaders/TextureResourceLoader.h"
#include "../core/graphics/viewports/ScreenViewport.h"
#include "../core/math/linearmath.h"
#include "../core/math/prng.h"

namespace game
{
	using namespace engine;

	ScreenViewport viewport;

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		((Flopio *)currentApp)->scrollCallback(window, xoffset, yoffset);
	}

	void Flopio::VOnResize(GLFWwindow* window, int width, int height) 
	{
		viewport.VResize(width, height);
	}

	void Flopio::VOnUpdate()
	{
		updateCam();
	}

	void Flopio::VOnRender(const double ndelay)
	{
		root.render(ndelay);
	}
	Actor back;
	TextureRC render;
	TextureRC render1;
	Resource texture("Resources:ship.png");
	Resource vertex("Resources:vertex.vs");
	Resource fragment("Resources:fragment.fs");
	Resource background("Resources:background.jpg");
	DirectoryResourceFile resFile("Resources");
	FragmentShaderResourceLoader fragmentLoader;
	VertexShaderResourceLoader vertexLoader;
	TextureResourceLoader textureLoader;

	

	void Flopio::VOnInit()
	{
		secondsPerUpdate = 1.0 / 60.0;
		root.setViewport(&viewport);
		int width, height;
		glfwGetWindowSize(glfwWindowHandle, &width, &height);
		glfwSetScrollCallback(glfwWindowHandle, scroll_callback);
		viewport.VResize(width, height);

		textureLoader.generateMipmaps = false;

		resourceCache.addFile(std::shared_ptr<DirectoryResourceFile>(&resFile, [](DirectoryResourceFile*) {}));
		resourceCache.addLoader(std::shared_ptr<FragmentShaderResourceLoader>(&fragmentLoader, [](FragmentShaderResourceLoader*) {}));
		resourceCache.addLoader(std::shared_ptr<VertexShaderResourceLoader>(&vertexLoader, [](VertexShaderResourceLoader*) {}));
		resourceCache.addLoader(std::shared_ptr<TextureResourceLoader>(&textureLoader, [](TextureResourceLoader*) {}));
		
		TextureRC::init();
		Scene::registerRenderer(render.getId(), &TextureRC::render);

		std::shared_ptr<RenderComponent> renderPtr { &render,[](RenderComponent*) {} };
		ship.setRenderer(renderPtr);
		std::shared_ptr<Actor> actorPtr { &ship ,[](Actor*) {} };

		std::shared_ptr<RenderComponent> renderPtr1{ &render1,[](RenderComponent*) {} };
		back.setRenderer(renderPtr1);
		std::shared_ptr<Actor> actorPtr1{ &back ,[](Actor*) {} };

		//render.shaderProgramId = RenderComponent::shaderInit(&vertex, nullptr, &fragment);
		render.texture = resourceCache.getHandle(texture);
		render.setDimensions(64.0f, 64.0f);

		//render1.shaderProgramId = RenderComponent::shaderInit(&vertex, nullptr, &fragment);
		render1.texture = resourceCache.getHandle(background);
		render1.setDimensions(128.0f, 128.0f);


		actorPtr1->setPosition({ 0, 0, 2 });
		actorPtr->setPosition({ 64, 45, 1 });

		root.addActor(actorPtr);
		root.addActor(actorPtr1);

		seedUsingRandXorshift128(glfwGetTime());
		Actor * tmp;
		TextureRC * r;
		for (int i = 0; i < 10; i++)
		{
			tmp = new Actor();
			r = new TextureRC();
			r->texture = resourceCache.getHandle(background);
			//r->shaderProgramId = RenderComponent::shaderInit(&vertex, nullptr, &fragment);
			std::shared_ptr<RenderComponent> renderPtr2{ r };
			std::shared_ptr<Actor> actorPtr2{ tmp };
			actorPtr2->setRenderer(renderPtr2);
			r->setDimensions(100.f, 100.f);

			actorPtr2->setPosition({ xorshift128Limit(width) - 200.f, xorshift128Limit(height) - 200.f, xorshift128f() + 1.f });
			root.addActor(actorPtr2);
		}
		//std::sort(root.actors.begin(), root.actors.end(), [](SharedActor f, SharedActor l) 
		//{
		//	return f->getPosition().z < l->getPosition().z;
		//});
		root.camera.set({ 200, 300 }, 0.0f, 1.0f);
	}

	void Flopio::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		root.camera.set(root.camera.getPosition(), root.camera.getAngle(), root.camera.getScale() + root.camera.getScale()*yoffset*0.1f);
	}

	void Flopio::updateCam()
	{
		const float speed = 400.f;
		Camera & cam = root.camera;
		float angle = cam.getAngle();
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_W) == GLFW_PRESS)
		{
			cam.set(vec2Add(cam.getPosition(), vec2Scale({ -sinf(angle), cosf(angle) }, speed * (float)secondsPerUpdate * (1 / cam.getScale()))), cam.getAngle(), cam.getScale());
		}
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_S) == GLFW_PRESS)
		{
			cam.set(vec2Add(cam.getPosition(), vec2Scale({ sinf(angle), -cosf(angle) }, speed * (float)secondsPerUpdate * (1 / cam.getScale()))), cam.getAngle(), cam.getScale());
		}
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_A) == GLFW_PRESS)
		{
			cam.set(vec2Add(cam.getPosition(), vec2Scale({ -cosf(angle), -sinf(angle) }, speed * (float)secondsPerUpdate * (1 / cam.getScale()))), cam.getAngle(), cam.getScale());
		}
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_D) == GLFW_PRESS)
		{
			cam.set(vec2Add(cam.getPosition(), vec2Scale({ cosf(angle), sinf(angle) }, speed * (float)secondsPerUpdate * (1 / cam.getScale()))), cam.getAngle(), cam.getScale());
		}
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_Q) == GLFW_PRESS)
		{
			cam.set(cam.getPosition(), cam.getAngle() + 1.f*secondsPerUpdate, cam.getScale());
		}
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_E) == GLFW_PRESS)
		{
			cam.set(cam.getPosition(), cam.getAngle() + -1.f*secondsPerUpdate, cam.getScale());
		}
	}
}
