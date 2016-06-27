#include "pch.h"
#include "axl_xml_ExpatParser.h"
#include "axl_io_MappedFile.h"

namespace axl {
namespace xml {

//.............................................................................

bool
ExpatParserRoot::create (const char* encoding)
{
	close ();

	m_h = XML_ParserCreate (encoding);
	return err::completeWithSystemError (m_h != NULL, false, err::SystemErrorCode_InsufficientResources);
}

bool
ExpatParserRoot::createNs (
	const char* encoding,
	char separator
	)
{
	close ();

	m_h = XML_ParserCreateNS (encoding, separator);
	return err::completeWithSystemError (m_h != NULL, false, err::SystemErrorCode_InsufficientResources);
}

bool
ExpatParserRoot::parseFile (
	const char* fileName,
	size_t blockSize
	)
{
	bool result;

	io::SimpleMappedFile mappedFile;

	if (!blockSize || blockSize == -1)
	{
		return 
			mappedFile.open (fileName, io::FileFlag_ReadOnly) &&
			parse (mappedFile.p (), (size_t) mappedFile.getFileSize (), true);
	}

	// round-up block size

	size_t alignFactor = g::g_module.getSystemInfo ()->m_mappingAlignFactor;
	blockSize = (blockSize + alignFactor - 1) & ~(alignFactor - 1); 

	result = mappedFile.open (fileName, 0, 0, io::FileFlag_ReadOnly);
	if (!result)
		return false;

	void* p;
	uint64_t offset = 0;
	uint64_t size = mappedFile.getFileSize ();

	while (offset + blockSize < size)
	{
		result = 
			(p = mappedFile.view (offset, blockSize)) &&
			parse (p, blockSize, false);

		if (!result)
			return false;

		offset += blockSize;
	}

	// final block

	blockSize = (size_t) (size - offset);

	return
		(p = mappedFile.view (offset, blockSize)) &&
		parse (p, blockSize, true);
}

//.............................................................................

} // namespace xml
} // namespace axl
