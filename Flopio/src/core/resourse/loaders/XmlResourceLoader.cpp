#include "XmlResourceLoader.h"
#include "../../Engine.h"

namespace engine
{

	bool XmlExtraData::parseXml(char* pRawBuffer)
	{
		document.Parse(pRawBuffer);
		if (document.Error()) {
			logger << document.ErrorStr() << "\n";
			return false;
		}
		return true;
	}

	bool XmlResourceLoader::VLoad(char * rawBuffer, unsigned int rawBufSize, std::shared_ptr<ResourceHandle> resHandle)
	{
		if (rawBufSize <= 0)
			return false;

		std::shared_ptr<XmlExtraData> pExtraData = std::shared_ptr<XmlExtraData>(new XmlExtraData());
		if (!pExtraData->parseXml(rawBuffer))
			return false;

		resHandle->setExtra(std::shared_ptr<XmlExtraData>(pExtraData));

		return true;
	}
}