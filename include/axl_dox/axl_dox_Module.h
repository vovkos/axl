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

#include "axl_dox_Group.h"

namespace axl {
namespace dox {

//..............................................................................

class Module {
	friend class Parser;

protected:
	struct Target: sl::ListLink {
		Block* m_block;
		int m_tokenKind;
		sl::StringRef m_itemName;
		size_t m_overloadIdx;
	};

protected:
	Host* m_host;

	sl::List<Block> m_blockList;
	sl::List<Group> m_groupList;
	sl::List<Footnote> m_footnoteList;
	sl::StringHashTable<size_t> m_refIdMap;
	sl::StringHashTable<Group*> m_groupMap;
	sl::List<Target> m_targetList;

public:
	Module(Host* host) {
		m_host = host;
	}

	Host*
	getHost() {
		return m_host;
	}

	void
	clear();

	sl::ConstList<Block>
	getBlockList() {
		return m_blockList;
	}

	sl::ConstList<Group>
	getGroupList() {
		return m_groupList;
	}

	Block*
	createBlock(handle_t item = NULL);

	bool
	resolveBlockTargets();

	void
	deleteEmptyGroups();

	sl::String
	adjustRefId(const sl::StringRef& refId);

	bool
	generateDocumentation(
		const sl::StringRef& outputDir,
		const sl::StringRef& indexFileName = "index.xml",
		const sl::StringRef& globalNamespaceFileName = "global.xml"
	);

protected:
	void
	setBlockTarget(
		Block* block,
		int tokenKind,
		const sl::StringRef& itemName,
		size_t overloadIdx
	);

	Group*
	getGroup(const sl::StringRef& name);

	Footnote*
	createFootnote();

	bool
	generateGroupDocumentation(
		const sl::StringRef& outputDir,
		sl::String* indexXml
	);
};

//..............................................................................

} // namespace dox
} // namespace axl
