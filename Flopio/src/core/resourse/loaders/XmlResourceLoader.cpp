#include "XmlResourceLoader.h"

namespace engine
{

	void XmlExtraData::parseXml(char* pRawBuffer)
	{
		document.Parse(pRawBuffer);
	}

	bool XmlResourceLoader::VLoad(char * rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle)
	{
		if (rawBufSize <= 0)
			return false;

		std::shared_ptr<XmlExtraData> pExtraData = std::shared_ptr<XmlExtraData>(new XmlExtraData());
		pExtraData->parseXml(rawBuffer);

		resHandle->setExtra(std::shared_ptr<XmlExtraData>(pExtraData));

		return true;
	}
}