#pragma once
#include "../ResourceLoader.h"
#include <tinyxml2.h>

namespace engine
{
	class XmlExtraData : public ResourceExtraData
	{
	private:
		tinyxml2::XMLDocument document;
	public:
		const tinyxml2::XMLElement * getRoot() const { return document.RootElement(); }
		bool parseXml(char* pRawBuffer);
		virtual std::string getName() { return "XmlExtraData"; }
	};

	class XmlResourceLoader : public ResourceLoader
	{
		virtual bool VUseRawFile() override { return false; }
		virtual bool VDiscardRawBufferAfterLoad() override { return true; }
		virtual unsigned int VGetLoadedSize(char *rawBuffer, unsigned int rawSize) override { return 0; }
		virtual unsigned int VGetExtraDataSize() override { return sizeof(XmlExtraData); }
		virtual bool VLoad(char* rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle) override;
		virtual std::string* VGetWildcardPattern(int* size) override 
		{ { static std::string r[1] = { "*.xml" }; *size = sizeof(r) / sizeof(r[0]); return r; }; }
	};

}