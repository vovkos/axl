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
#include "axl_dox_Block.h"
#include "axl_dox_Module.h"
#include "axl_dox_Host.h"

namespace axl {
namespace dox {

//..............................................................................

BlockData::BlockData() {
	m_blockKind = BlockKind_Normal;
	m_currentDescription = &m_detailedDescription;
	m_group = NULL;
}

//..............................................................................

Block::Block(Host* host) {
	m_host = host;
	m_item = NULL;
}

const sl::String&
Block::getRefId() {
	if (!m_refId.isEmpty())
		return m_refId;

	ASSERT(m_item);
	m_refId = m_host->createItemRefId(m_item);
	return m_refId;
}

sl::String
Block::getDescriptionString() {
	sl::String string;

	m_briefDescription.trim();
	m_detailedDescription.trim();
	m_returnDescription.trim();
	m_seeAlsoDescription.trim();

	if (!m_briefDescription.isEmpty()) {
		string.append("<briefdescription><para>");
		appendXmlElementContents(&string, m_briefDescription);
		string.append("</para></briefdescription>\n");
	}

	if (m_detailedDescription.isEmpty() &&
		m_paramList.isEmpty() &&
		m_returnDescription.isEmpty() &&
		m_seeAlsoDescription.isEmpty() &&
		m_internalDescription.isEmpty()
	)
		return string;

	string.append("<detaileddescription>\n");

	if (!m_detailedDescription.isEmpty()) {
		string.append("<para>");
		appendXmlElementContents(&string, m_detailedDescription);
		string.append("</para>\n");
	}

	if (!m_paramList.isEmpty()) {
		string.append("<para><parameterlist kind=\"param\">\n");

		sl::Iterator<Param> it = m_paramList.getHead();
		for (; it; it++) {
			string.append("<parameteritem><parameternamelist><parametername>");
			string.append(it->m_name);
			string.append("</parametername></parameternamelist>\n<parameterdescription><para>");
			appendXmlElementContents(&string, it->m_description);
			string.append("</para></parameterdescription></parameteritem>\n");
		}

		string.append("</parameterlist></para>\n");
	}

	if (!m_returnDescription.isEmpty()) {
		string.append("<para><simplesect kind='return'><para>");
		appendXmlElementContents(&string, m_returnDescription);
		string.append("</para></simplesect></para>\n");
	}

	if (!m_seeAlsoDescription.isEmpty()) {
		string.append("<para><simplesect kind='see'><para>");
		appendXmlElementContents(&string, m_seeAlsoDescription);
		string.append("</para></simplesect></para>\n");
	}

	if (!m_internalDescription.isEmpty()) {
		string.append("<internal><para>");
		appendXmlElementContents(&string, m_internalDescription);
		string.append("</para></internal>\n");
	}

	string.append("</detaileddescription>\n");
	return string;
}

sl::String
Block::getFootnoteString() {
	sl::String string;

	size_t count = m_footnoteArray.getCount();
	for (size_t i = 0; i < count; i++) {
		Block* footnote = m_footnoteArray[i];

		string.append("<memberdef kind='footnote'>\n");
		string.appendFormat("<name>%s</name>\n", footnote->getRefId().sz());
		string.append(footnote->getDescriptionString());
		string.append("</memberdef>\n");
	}

	return string;
}

sl::String
Block::getImportString() {
	sl::String string;

	sl::BoxIterator<sl::String> it = m_importList.getHead();
	for (; it; it++)
		string.appendFormat("<includes>%s</includes>\n", it->sz());

	return string;
}

//..............................................................................

void
appendXmlElementContents(
	sl::String* string,
	const sl::StringRef& contents
) {
	if (contents.findOneOf("<>&") == -1) {
		string->append(contents);
	} else {
		string->append("<![CDATA[");
		string->append(contents);
		string->append("]]>");
	}
}

//..............................................................................

} // namespace dox
} // namespace axl
