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
		auto extra = std::shared_ptr<TextureExtraData>(new (extraData) TextureExtraData());
		resHandle->setExtra(extra);
		glGenTextures(1, &extra->textureId);

		glBindTexture(GL_TEXTURE_2D, extra->textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		bool ret = true;
		int width, height, fileChanels;
		unsigned char* data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(rawBuffer), rawBufSize, &width, &height, &fileChanels, 0);

		if (data)
		{
			unsigned int pixelFormat = GL_RGB;
			std::cout << fileChanels << "\n";
			switch (fileChanels)
			{
			case(3):
				pixelFormat = GL_RGB;
				break;
			case(4):
				pixelFormat = GL_RGBA;
				break;
			default:
				logger << "Channels in file less than 3\n";
				break;
			}
			glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
			if (generateMipmaps)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			
		}
		else
		{
			logger << "Failed to load texture " << resHandle->getResource().getName() << "\n";
			ret = false;
		}
		stbi_image_free(data);
		return ret;
	};
}