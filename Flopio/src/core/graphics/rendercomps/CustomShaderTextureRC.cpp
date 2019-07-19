#include "CustomShaderTextureRC.h"
#include "../../Engine.h"
#include "../../resourse/loaders/TextureResourceLoader.h"

namespace engine
{
	bool CustomShaderTextureRC::initialized;
	const char* CustomShaderTextureRC::name = "CustomShaderTextureRC";
	unsigned int CustomShaderTextureRC::VBO,
		CustomShaderTextureRC::VAO,
		CustomShaderTextureRC::EBO;

	void CustomShaderTextureRC::init()
	{
		if (!initialized)
		{
			initialized = true;
			Component::registerRender(Component::getId(name));

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, (void*)0);

			glEnableVertexAttribArray(0);
		}
	}

	bool CustomShaderTextureRC::VInit(const tinyxml2::XMLElement * pData)
	{
		bool ret = TextureRC::VInit(pData);
		if (!ret)
			return ret;

		shaderProgramId = shaderInitFromXML(pData->FirstChildElement("shader"));
		if (!shaderProgramId)
			return false;
		return true;
	}

	void CustomShaderTextureRC::render(std::vector<std::shared_ptr<RenderComponent>>& renderers, Scene& scene, const double ndelay)
	{
		glBindVertexArray(VAO);
		unsigned int pspid = 0;
		for (auto r : renderers)
		{
			auto component = std::static_pointer_cast<CustomShaderTextureRC>(r);
			auto textureExtra = std::static_pointer_cast<TextureExtraData>(currentApp->resourceCache.getHandle(*component->texture)->getExtra());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureExtra->getTextureId());
			unsigned int id = component->shaderProgramId;
			if (pspid != id)
			{
				pspid = id;
				glUseProgram(id);
				glUniformMatrix4fv(glGetUniformLocation(id, "projectionView"), 1, GL_FALSE, (GLfloat*)scene.getProjectionView());
			}
			glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, (GLfloat*)&component->model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
	}
}
