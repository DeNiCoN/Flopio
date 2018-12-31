#include "Flopio.h"
#include "../core/gamebasis/RectangleRenderComponent.h"
#include "../core/resourse/files/DirectoryResourceFile.h"
#include "../core/resourse/loaders/FragmentShaderResourceLoader.h"
#include "../core/resourse/loaders/VertexShaderResourceLoader.h"
#include "../core/resourse/loaders/TextureResourceLoader.h"


namespace game
{
	using namespace engine;

	void Flopio::VOnUpdate(const double delta)
	{

	}

	void Flopio::VOnRender(const double ndelay)
	{
		//std::cout << "render\n";
		root.render(ndelay);
	}

	RectangleRenderComponent render;
	Resource texture("Resources:ship.png");
	Resource vertex("Resources:vertex.vs");
	Resource fragment("Resources:fragment.fs");
	DirectoryResourceFile resFile("Resources");
	FragmentShaderResourceLoader fragmentLoader;
	VertexShaderResourceLoader vertexLoader;
	TextureResourceLoader textureLoader;

	void Flopio::VOnInit()
	{

		Scene::registerRenderer(render.getId(), &RectangleRenderComponent::render);
		RectangleRenderComponent::init();

		textureLoader.generateMipmaps = false;

		resourceCache.addFile(std::shared_ptr<DirectoryResourceFile>(&resFile));
		resourceCache.addLoader(std::shared_ptr<FragmentShaderResourceLoader>(&fragmentLoader));
		resourceCache.addLoader(std::shared_ptr<VertexShaderResourceLoader>(&vertexLoader));
		resourceCache.addLoader(std::shared_ptr<TextureResourceLoader>(&textureLoader));

		render.shaderInit(&vertex, nullptr, &fragment);
		render.texture = &texture;

		std::shared_ptr<RenderComponent> renderPtr { &render };
		ship.renderer = renderPtr;
		std::shared_ptr<Actor> actorPtr { &ship };
		root.addActor(actorPtr);
	}
}
