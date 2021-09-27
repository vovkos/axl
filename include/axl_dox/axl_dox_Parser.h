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

class Module;

//..............................................................................

class Parser {
protected:
	struct GroupStackEntry {
		handle_t m_namespace;
		Group* m_group;
	};

	enum BlockTargetKind {
		BlockTargetKind_None = 0,
		BlockTargetKind_Member,
		BlockTargetKind_Compound,
	};

protected:
	Module* m_module;
	Block* m_block;
	Block* m_parentBlock;
	sl::Array<GroupStackEntry> m_groupStack;
	BlockTargetKind m_blockTargetKind;
	sl::String m_firstIndent;
	sl::String m_indent;
	sl::String m_overloadName;
	lex::LineCol m_lastPos;
	size_t m_overloadIdx;

public:
	Parser(Module* module) {
		init();
		m_module = module;
	}

	void
	reset();

	Block*
	popBlock();

	void
	addComment(
		const sl::StringRef& comment,
		const lex::LineCol& pos,
		bool isSingleLine,
		handle_t lastDeclaredItem
	);

protected:
	void
	init();

	void
	setBlockTarget(
		int tokenKind,
		const sl::StringRef& name
	);
};

//..............................................................................

} // namespace dox
} // namespace axl
