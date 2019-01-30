#include "TextureRC.h"
#include "../../Engine.h"
#include "../../resourse/loaders/TextureResourceLoader.h"

namespace engine
{
	bool TextureRC::initialized;
	unsigned int TextureRC::VBO, 
		TextureRC::VAO, 
		TextureRC::EBO,
		TextureRC::transformBO,
		TextureRC::textureHandleBO;

	void TextureRC::init()
	{
		if (!initialized)
		{
			initialized = true;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &transformBO);
			glGenBuffers(1, &textureHandleBO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16 * 5, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, textureHandleBO);
			glVertexAttribPointer(1, 2, GL_UNSIGNED_INT, GL_FALSE, 16 * 5, (void*)8);

			glBindBuffer(GL_ARRAY_BUFFER, transformBO);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 16 * 5, (void*)16);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * 5, (void*)32);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * 5, (void*)48);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * 5, (void*)64);

			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);
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

	void TextureRC::render(std::vector<SharedActor>& actors, Scene& scene, const double ndelay)
	{
		TextureRC * component;
		glBindVertexArray(VAO);
		unsigned int pspid = 0;
		for (auto actor : actors)
		{
			component = static_cast<TextureRC*>(&*actor->getRenderer());
			auto textureExtra = std::static_pointer_cast<TextureExtraData>(currentApp->resourceCache.getHandle(*component->texture)->getExtra());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureExtra->getTextureId());
			unsigned int id = component->getShaderProgramId();
			if (pspid != id)
			{
				pspid = id;
				glUseProgram(id);
				glUniformMatrix4fv(glGetUniformLocation(id, "projectionView"), 1, GL_FALSE, (GLfloat*) &scene.getProjectionView());
			}
			glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, (GLfloat*)&component->model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
	}
}
