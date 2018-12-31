#include "RectangleRenderComponent.h"
#include "../Engine.h"
#include "../resourse/loaders/TextureResourceLoader.h"

namespace engine
{
	bool RectangleRenderComponent::initialized;
	unsigned int RectangleRenderComponent::VBO, 
		RectangleRenderComponent::VAO, 
		RectangleRenderComponent::EBO;

	void RectangleRenderComponent::init()
	{
		if (!initialized)
		{
			initialized = true;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(rect[0]), (void*)0);
			glEnableVertexAttribArray(0);
		}
	}

	void RectangleRenderComponent::render(std::vector<SharedActor>& actors, const double ndelay)
	{
		RectangleRenderComponent component;
		glBindVertexArray(VAO);
		for (auto actor : actors)
		{
			component = *static_cast<RectangleRenderComponent*>(&*actor->renderer);
			auto textureExtra = std::static_pointer_cast<TextureExtraData>(currentApp->resourceCache.getHandle(component.texture)->getExtra());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureExtra->getTextureId());
			glUseProgram(component.getShaderProgramId());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
	}
}
