#include "TextureRC.h"
#include "../../Engine.h"
#include <glad/glad.h>
#include "../../resourse/loaders/TextureResourceLoader.h"

namespace engine
{
	bool TextureRC::initialized;

	const char* TextureRC::name = "TextureRC";

	unsigned int 
		TextureRC::VBO, 
		TextureRC::VAO, 
		TextureRC::EBO,
		TextureRC::transformBO,
		TextureRC::textureHandleBO,
		TextureRC::shaderProgramId;

	Resource 
		vertex("EngineResources|TextureRC.vs"), 
		fragment("EngineResources|TextureRC.fs");

	void TextureRC::init()
	{
		if (!initialized)
		{
			initialized = true;
			Component::registerRender(Component::getId(name));

			//*glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &transformBO);
			glGenBuffers(1, &textureHandleBO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			
			//*glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, textureHandleBO);
			//*glVertexAttribLPointer(1, 1, GL_UNSIGNED_INT64_ARB, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, transformBO);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 16 * 4, (void*)0);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * 4, (void*)16);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * 4, (void*)32);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * 4, (void*)48);

			//*glVertexAttribDivisor(1, 1);
			//*glVertexAttribDivisor(2, 1);
			//*glVertexAttribDivisor(3, 1);
			//*glVertexAttribDivisor(4, 1);
			//*glVertexAttribDivisor(5, 1);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			//*glBindVertexArray(0);

			if (!(shaderProgramId = shaderInit(&vertex, nullptr, &fragment)))
			{
				logger << "Failed to initialize shader for TextureRC\n";
			}
		}
	}

	void TextureRC::VOnActorAngleSet(float radians)
	{
		updateModel();
	}

	void TextureRC::VOnActorPositionSet(vec3 pos)
	{
		updateModel();
	}

	void TextureRC::updateModel()
	{
		mat44 rotate, translate;
		rotate = mat44RotateByZ(parent->getAngle());
		translate = mat44Translate(parent->getPosition());
		model = mat44Multiply(translate, mat44Multiply(rotate, mat44Scale(width, height, 1)));
	}

	void TextureRC::setDimensions(float width, float height)
	{
		this->width = width;
		this->height = height;
		updateModel();
	}

	std::pair<float, float> TextureRC::getDimensions()
	{
		return std::make_pair(width, height);
	}

	void TextureRC::render(std::vector<std::shared_ptr<RenderComponent>>& renderers, Scene& scene, const double ndelay)
	{
		size_t count = renderers.size();

		glBindBuffer(GL_ARRAY_BUFFER, transformBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(mat44)*count, NULL, GL_DYNAMIC_DRAW);
		auto transformBufferPtr = reinterpret_cast<mat44*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		glBindBuffer(GL_ARRAY_BUFFER, textureHandleBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLint64)*count, NULL, GL_DYNAMIC_DRAW);
		auto textureHandleBufferPtr = reinterpret_cast<GLint64*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		size_t i = 0;
		for (auto r : renderers)
		{
			auto component = std::static_pointer_cast<TextureRC>(r);
			auto textureExtra = std::static_pointer_cast<TextureExtraData>(component->texture->getExtra());
			memcpy(transformBufferPtr + i, &component->model, sizeof(mat44));
			*(textureHandleBufferPtr + i) = textureExtra->getTextureHandle();
			i++;
		}
		
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, transformBO);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		
		//*glBindVertexArray(VAO);
		glUseProgram(shaderProgramId);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, "projectionView"), 1, GL_FALSE, (GLfloat*)&scene.getProjectionView());
		//*glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
		//*glBindVertexArray(0);
	}

	bool TextureRC::VInit(const tinyxml2::XMLElement * pData)
	{
		this->width = pData->FloatAttribute("width", 64.f);
		this->height = pData->FloatAttribute("height", 64.f);
		const char* textureName = pData->Attribute("texture");
		Resource textureResource(textureName);
		std::cout << textureResource.getName();

		texture = currentApp->resourceCache.getHandle(textureResource);

		if (!texture)
		{
			logger << "failed to get texture handle for TextureRenderComponent: " << textureName << "\n";
			return false;
		}

		return true;
	}

	void TextureRC::VPostInit()
	{
		updateModel();
	}
}
