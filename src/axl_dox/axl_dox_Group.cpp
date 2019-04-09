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
#include "axl_dox_Group.h"
#include "axl_dox_Module.h"
#include "axl_dox_Host.h"

namespace axl {
namespace dox {

//..............................................................................

bool
Group::generateDocumentation(
	const sl::StringRef& outputDir,
	sl::String* itemXml,
	sl::String* indexXml
	)
{
	indexXml->appendFormat(
		"<compound kind='group' refid='%s'><name>%s</name></compound>\n",
		m_refId.sz(),
		m_name.sz()
		);

	itemXml->format(
		"<compounddef kind='group' id='%s' language='Jancy'>\n"
		"<compoundname>%s</compoundname>\n"
		"<title>",
		m_refId.sz(),
		m_name.sz()
		);

	appendXmlElementContents(itemXml, m_title);
	itemXml->append("</title>\n");

	sl::String sectionDef;

	size_t count = m_itemArray.getCount();
	for (size_t i = 0; i < count; i++)
	{
		handle_t item = m_itemArray[i];
		Block* doxyBlock = m_host->getItemBlock(item);
		sl::String refId = doxyBlock->getRefId();

		sl::StringRef compoundElementName = m_host->getItemCompoundElementName(item);
		if (compoundElementName.isEmpty())
		{
			sectionDef.appendFormat("<memberdef id='%s'/>", refId.sz());
			sectionDef.append('\n');
		}
		else
		{
			itemXml->appendFormat("<%s refid='%s'/>", compoundElementName.sz(), refId.sz());
			itemXml->append('\n');
		}
	}

	sectionDef += getFootnoteString();

	if (!sectionDef.isEmpty())
	{
		itemXml->append("<sectiondef>\n");
		itemXml->append(sectionDef);
		itemXml->append("</sectiondef>\n");
	}

	sl::BoxIterator<Group*> groupIt = m_groupList.getHead();
	for (; groupIt; groupIt++)
	{
		Group* group = *groupIt;
		itemXml->appendFormat("<innergroup refid='%s'/>", group->m_refId.sz());
		itemXml->append('\n');
	}

	itemXml->append(getImportString());
	itemXml->append(getDescriptionString());
	itemXml->append("</compounddef>\n");

	return true;
}

//..............................................................................

} // namespace dox
} // namespace axl
