#pragma once
#include "../ResourceLoader.h"

namespace engine
{

	class VertexShaderData : public ResourceExtraData
	{
		friend class VertexShaderResourceLoader;
		virtual std::string getName() override { return "vertexShaderData"; }
	public:
		~VertexShaderData();
	private:
		unsigned int vertexShaderID;
	};

	class VertexShaderResourceLoader : public ResourceLoader
	{
		virtual bool VUseRawFile() override { return false; }
		virtual bool VDiscardRawBufferAfterLoad() override { return true; };
		virtual const std::string * VGetWildcardPattern(int * size) override
		{ std::string r[] = { "*.vs" }; *size = sizeof(r) / sizeof(r[0]); return r;}
		virtual unsigned int VGetLoadedSize(char * rawBuffer, unsigned int rawBufSize) override { return 0; }
		virtual unsigned int VGetExtraDataSize() override { return sizeof(VertexShaderData); }
		virtual bool VLoad(char * rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle) override;
	public:

	};
}

