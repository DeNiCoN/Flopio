#pragma once
#include "../ResourceLoader.h"

namespace engine
{

	class FragmentShaderData : public ResourceExtraData
	{
		friend class FragmentShaderResourceLoader;
		virtual std::string getName() override { return "vertexShaderData"; }
	public:
		~FragmentShaderData();
		unsigned int getId() const { return fragmentShaderID; }
	private:
		unsigned int fragmentShaderID;
	};

	class FragmentShaderResourceLoader : public ResourceLoader
	{
	private:
		std::string r[1] = { "*.fs" };
	public:

		virtual bool VUseRawFile() override { return false; }
		virtual bool VDiscardRawBufferAfterLoad() override { return true; };
		virtual const std::string * VGetWildcardPattern(int * size) override
		{ *size = sizeof(r) / sizeof(r[0]); return r; }
		virtual unsigned int VGetLoadedSize(char * rawBuffer, unsigned int rawBufSize) override { return 0; }
		virtual unsigned int VGetExtraDataSize() override { return sizeof(FragmentShaderData); }
		virtual bool VLoad(char * rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle) override;
	};
}