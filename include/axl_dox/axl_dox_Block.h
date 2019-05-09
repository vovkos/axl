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

#include "axl_dox_Pch.h"

namespace axl {
namespace dox {

struct Host;
class Module;
class Block;
class Group;

//..............................................................................

struct Param: sl::ListLink
{
	sl::String m_name;
	sl::String m_description;
};

//..............................................................................

enum BlockKind
{
	BlockKind_Normal,
	BlockKind_Group,
	BlockKind_Footnote,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct BlockData
{
	BlockKind m_blockKind;
	Group* m_group;

	sl::String m_source;
	sl::String m_refId;
	sl::String m_title;
	sl::String m_briefDescription;
	sl::String m_detailedDescription;
	sl::String m_seeAlsoDescription;
	sl::String m_internalDescription;
	sl::String m_returnDescription;
	sl::List<Param> m_paramList;
	sl::BoxList<sl::String> m_importList;
	sl::Array<Block*> m_footnoteArray;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Block:
	public sl::ListLink,
	protected BlockData
{
	friend class Module;
	friend class Parser;

protected:
	Host* m_host;

public:
	handle_t m_item;

public:
	Block(Host* host);

	BlockKind
	getBlockKind()
	{
		return m_blockKind;
	}

	Group*
	getGroup()
	{
		return m_group;
	}

	handle_t
	getItem()
	{
		return m_item;
	}

	const sl::String&
	getSource()
	{
		return m_source;
	}

	const sl::String&
	getRefId();

	const sl::String&
	getTitle()
	{
		return m_title;
	}

	const sl::String&
	getBriefDescription()
	{
		return m_briefDescription;
	}

	const sl::String&
	getDetailedDescription()
	{
		return m_detailedDescription;
	}

	const sl::String&
	getSeeAlsoDescription()
	{
		return m_seeAlsoDescription;
	}

	const sl::String&
	getInternalDescription()
	{
		return m_internalDescription;
	}

	bool
	isDescriptionEmpty()
	{
		return m_briefDescription.isEmpty() && m_detailedDescription.isEmpty();
	}

	sl::String
	getDescriptionString();

	sl::String
	getFootnoteString();

	sl::String
	getImportString();
};

//..............................................................................

class Footnote: public Block
{
	friend class Parser;

protected:
	Block* m_parent;

public:
	Footnote(Host* host):
		Block(host)
	{
		m_blockKind = BlockKind_Footnote;
		m_parent = NULL;
	}

	Block*
	getParent()
	{
		return m_parent;
	}
};

//..............................................................................

void
appendXmlElementContents(
	sl::String* string,
	const sl::StringRef& contents
	);

//..............................................................................

} // namespace dox
} // namespace axl
