#include "CustomShaderTextureRC.h"
#include "../../Engine.h"
#include "../../resourse/loaders/TextureResourceLoader.h"

namespace engine
{
	bool CustomShaderTextureRC::initialized;
	unsigned int CustomShaderTextureRC::VBO,
		CustomShaderTextureRC::VAO,
		CustomShaderTextureRC::EBO;

	void CustomShaderTextureRC::init()
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

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16 * 5, (void*)0);

			glEnableVertexAttribArray(0);
		}
	}

	void CustomShaderTextureRC::VOnActorAngleSet(float radians)
	{
		updateModel();
	}

	void CustomShaderTextureRC::VOnActorPositionSet(vec3 pos)
	{
		updateModel();
	}

	void CustomShaderTextureRC::updateModel()
	{
		mat44 rotate, translate;
		rotate = mat44RotateByZ(parent->getAngle());
		translate = mat44Translate(parent->getPosition());
		model = mat44Multiply(translate, mat44Multiply(rotate, mat44Scale(width, height, 1)));
	}

	void CustomShaderTextureRC::setDimensions(float width, float height)
	{
		this->width = width;
		this->height = height;
		updateModel();
	}

	std::pair<float, float> CustomShaderTextureRC::getDimensions()
	{
		return std::make_pair(width, height);
	}

	void CustomShaderTextureRC::render(std::vector<SharedActor>& actors, Scene& scene, const double ndelay)
	{
		CustomShaderTextureRC * component;
		glBindVertexArray(VAO);
		unsigned int pspid = 0;
		for (auto actor : actors)
		{
			component = static_cast<CustomShaderTextureRC*>(&*actor->getRenderer());
			auto textureExtra = std::static_pointer_cast<TextureExtraData>(currentApp->resourceCache.getHandle(*component->texture)->getExtra());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureExtra->getTextureId());
			unsigned int id = component->shaderProgramId;
			if (pspid != id)
			{
				pspid = id;
				glUseProgram(id);
				glUniformMatrix4fv(glGetUniformLocation(id, "projectionView"), 1, GL_FALSE, (GLfloat*)&scene.getProjectionView());
			}
			glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, (GLfloat*)&component->model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
	}
}
