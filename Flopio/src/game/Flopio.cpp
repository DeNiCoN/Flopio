#include "Flopio.h"
#include "../core/gamebasis/RectangleRenderComponent.h"
#include "../core/resourse/files/DirectoryResourceFile.h"
#include "../core/resourse/loaders/FragmentShaderResourceLoader.h"
#include "../core/resourse/loaders/VertexShaderResourceLoader.h"
#include "../core/resourse/loaders/TextureResourceLoader.h"
#include "../core/gamebasis/viewports/ScreenViewport.h"
#include "../core/math/linearmath.h"
#include "../core/math/prng.h"

namespace game
{
	using namespace engine;

	ScreenViewport viewport;

	void Flopio::VOnResize(GLFWwindow* window, int width, int height) 
	{
		viewport.VResize(width, height);
	}

	void Flopio::VOnUpdate()
	{
		root.camera.set(root.camera.getPosition(), root.camera.getAngle() + 1.1f*secondsPerUpdate, 1.0f);
		ship.setAngle(ship.getAngle() + 1.0*secondsPerUpdate);
	}

	void Flopio::VOnRender(const double ndelay)
	{
		root.render(ndelay);
	}
	Actor back;
	RectangleRenderComponent render;
	RectangleRenderComponent render1;
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
		secondsPerUpdate = 1.0 / 20.0;
		root.setViewport(&viewport);
		int width, height;
		glfwGetWindowSize(glfwWindowHandle, &width, &height);
		viewport.VResize(width, height);

		Scene::registerRenderer(render.getId(), &RectangleRenderComponent::render);
		RectangleRenderComponent::init();

		textureLoader.generateMipmaps = false;

		resourceCache.addFile(std::shared_ptr<DirectoryResourceFile>(&resFile, [](DirectoryResourceFile*) {}));
		resourceCache.addLoader(std::shared_ptr<FragmentShaderResourceLoader>(&fragmentLoader, [](FragmentShaderResourceLoader*) {}));
		resourceCache.addLoader(std::shared_ptr<VertexShaderResourceLoader>(&vertexLoader, [](VertexShaderResourceLoader*) {}));
		resourceCache.addLoader(std::shared_ptr<TextureResourceLoader>(&textureLoader, [](TextureResourceLoader*) {}));
		
		std::shared_ptr<RenderComponent> renderPtr { &render,[](RenderComponent*) {} };
		ship.setRenderer(renderPtr);
		std::shared_ptr<Actor> actorPtr { &ship ,[](Actor*) {} };

		std::shared_ptr<RenderComponent> renderPtr1{ &render1,[](RenderComponent*) {} };
		back.setRenderer(renderPtr1);
		std::shared_ptr<Actor> actorPtr1{ &back ,[](Actor*) {} };

		render.shaderInit(&vertex, nullptr, &fragment);
		render.texture = &texture;
		render.setDimensions(64.0f, 64.0f);

		render1.shaderInit(&vertex, nullptr, &fragment);
		render1.texture = &background;
		render1.setDimensions(128.0f, 128.0f);


		actorPtr1->setPosition({ 0, 0, 2 });
		actorPtr->setPosition({ 64, 45, 1 });

		root.addActor(actorPtr);
		root.addActor(actorPtr1);

		seedUsingRandXorshift128(0);
		Actor * tmp;
		RectangleRenderComponent * r;
		for (int i = 0; i < 1000; i++)
		{
			tmp = new Actor();
			r = new RectangleRenderComponent(&background);
			r->shaderInit(&vertex, nullptr, &fragment);
			std::shared_ptr<RenderComponent> renderPtr2{ r };
			std::shared_ptr<Actor> actorPtr2{ tmp };
			actorPtr2->setRenderer(renderPtr2);
			r->setDimensions(10.f, 10.f);

			actorPtr2->setPosition({ xorshift128Limit(width) - 200.f, xorshift128Limit(height) - 200.f, xorshift128f() + 1.f });
			root.addActor(actorPtr2);
		}
		std::sort(root.actors.begin(), root.actors.end(), [](SharedActor f, SharedActor l) 
		{
			return f->getPosition().z < l->getPosition().z;
		});
		//root.camera.set({ 200, 300 }, 0.0f, 1.0f);
	}
}
