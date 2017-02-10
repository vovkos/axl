//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_xml_ExpatParser.h"

namespace axl {
namespace xml {

//..............................................................................

bool
ExpatParserRoot::create (const sl::StringRef& encoding)
{
	close ();

	m_h = XML_ParserCreate (encoding.szn ());
	return err::completeWithSystemError (m_h != NULL, false, err::SystemErrorCode_InsufficientResources);
}

bool
ExpatParserRoot::createNs (
	const sl::StringRef& encoding,
	char separator
	)
{
	close ();

	m_h = XML_ParserCreateNS (encoding.szn (), separator);
	return err::completeWithSystemError (m_h != NULL, false, err::SystemErrorCode_InsufficientResources);
}

bool
ExpatParserRoot::parseFile (
	const sl::StringRef& fileName,
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

	size_t alignFactor = g::getModule ()->getSystemInfo ()->m_mappingAlignFactor;
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

//..............................................................................

} // namespace xml
} // namespace axl
