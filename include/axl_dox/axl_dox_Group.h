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

#pragma once

#include "axl_dox_Block.h"

namespace axl {
namespace dox {

class Module;

//..............................................................................

class Group: public Block
{
	friend class Module;
	friend class Parser;

protected:
	sl::String m_name;
	sl::Array<handle_t> m_itemArray;
	sl::BoxList<Group*> m_groupList;
	sl::BoxIterator<Group*> m_parentGroupListIt;

public:
	Group(Host* host):
		Block(host)
	{
		m_blockKind = BlockKind_Group;
	}

	bool
	isEmpty()
	{
		return m_itemArray.isEmpty() && m_groupList.isEmpty();
	}

	const sl::String&
	getName()
	{
		return m_name;
	}

	sl::Array<handle_t>
	getItemArray()
	{
		return m_itemArray;
	}

	sl::ConstBoxList<Group*>
	getGroupList()
	{
		return m_groupList;
	}

	void
	addItem(handle_t item)
	{
		m_itemArray.append(item);
	}

	sl::BoxIterator<Group*>
	addGroup(Group* group)
	{
		return m_groupList.insertTail(group);
	}

	bool
	generateDocumentation(
		const sl::StringRef& outputDir,
		sl::String* itemXml,
		sl::String* indexXml
		);
};

//..............................................................................

} // namespace dox
} // namespace axl
