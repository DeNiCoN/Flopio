#include "Flopio.h"
#include "../core/gamebasis/RectangleRenderComponent.h"
#include "../core/resourse/files/DirectoryResourceFile.h"
#include "../core/resourse/loaders/FragmentShaderResourceLoader.h"
#include "../core/resourse/loaders/VertexShaderResourceLoader.h"
#include "../core/resourse/loaders/TextureResourceLoader.h"
#include "../core/gamebasis/viewports/ScreenViewport.h"


namespace game
{
	using namespace engine;

	void Flopio::VOnResize(GLFWwindow* window, int width, int height) 
	{
		viewport.VResize(width, height);
	}

	void Flopio::VOnUpdate(const double delta)
	{

	}

	void Flopio::VOnRender(const double ndelay)
	{
		root.render(ndelay);
	}

	ScreenViewport viewport;
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

		render.shaderInit(&vertex, nullptr, &fragment);
		render.texture = &texture;

		render1.shaderInit(&vertex, nullptr, &fragment);
		render1.texture = &background;
		render1.setDimensions(128.0f, 128.0f);

		std::shared_ptr<RenderComponent> renderPtr { &render,[](RenderComponent*) {} };
		ship.setRenderer(renderPtr);
		std::shared_ptr<Actor> actorPtr { &ship ,[](Actor*) {} };

		std::shared_ptr<RenderComponent> renderPtr1{ &render1,[](RenderComponent*) {} };
		ship.setRenderer(renderPtr);
		std::shared_ptr<Actor> actorPtr1{ &back ,[](Actor*) {} };

		root.addActor(actorPtr);
		root.addActor(actorPtr1);
	}
}
