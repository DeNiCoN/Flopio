#include "TextureResourceLoader.h"
#include "../../Engine.h"
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace engine
{
	TextureExtraData::~TextureExtraData()
	{
		glDeleteTextures(1, &textureId);
	}

	bool TextureResourceLoader::VLoad(char * rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle)
	{
		char* extraData = resHandle->getBuffer();
		*reinterpret_cast<TextureExtraData*>(extraData) = TextureExtraData();
		std::shared_ptr<TextureExtraData> extra = std::shared_ptr<TextureExtraData>(reinterpret_cast<TextureExtraData*>(extraData));
		resHandle->setExtra(extra);

		glBindTexture(GL_TEXTURE_2D, extra->textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		bool ret = true;
		int width, height, fileChanels;
		unsigned char* data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(rawBuffer), rawBufSize, &width, &height, &fileChanels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			if (generateMipmaps)
			{
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
		}
		else
		{
			logger << "Failed to load texture " << resHandle->getResource() << "\n";
			ret = false;
		}
		stbi_image_free(data);
		return ret;
	};
}