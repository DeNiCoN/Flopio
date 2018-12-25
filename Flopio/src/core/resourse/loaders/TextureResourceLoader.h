#pragma once
#include "../ResourceLoader.h"

namespace engine
{
	class TextureExtraData : public ResourceExtraData 
	{
		friend class TextureResourceLoader;
		std::string getName() { return "textureExtraData"; }
	private:
		unsigned int textureId;
	public:
		unsigned int getTextureId() const { return textureId; }
		~TextureExtraData();
	};

	class TextureResourceLoader : public ResourceLoader
	{
		virtual bool VUseRawFile() override { return false; }
		virtual bool VDiscardRawBufferAfterLoad() override { return true; }
		virtual const std::string* VGetWildcardPattern(int* size) override
		{ std::string r[] = { "*.png", "*.jpg", "*.bmp", "*.pic", "*.psd","*.hdr", "*.tga", "*.pgm", "*.ppm", "*.gif" }; *size = sizeof(r) / sizeof(r[0]); return r; }

		virtual unsigned int VGetLoadedSize(char * rawBuffer, unsigned int rawBufSize) override { return 0; }
		virtual unsigned int VGetExtraDataSize() override { return sizeof(TextureExtraData); }
		virtual bool VLoad(char * rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle) override;

	public:
		bool generateMipmaps = false;
	};
}
