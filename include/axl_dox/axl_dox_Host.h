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

namespace axl {
namespace dox {

class Block;

//..............................................................................

struct Host
{
	virtual
	Block*
	findItemBlock(handle_t item) = 0;

	virtual
	Block*
	getItemBlock(handle_t item) = 0;

	virtual
	void
	setItemBlock(
		handle_t item,
		Block* block
		) = 0;

	virtual
	sl::String
	createItemRefId(handle_t item) = 0;

	virtual
	sl::StringRef
	getItemCompoundElementName(handle_t item) = 0; // empty / innerclass / innernamespace

	virtual
	handle_t
	findItem(
		const sl::StringRef& name,
		size_t overloadIdx
		) = 0;

	virtual
	handle_t
	getCurrentNamespace() = 0;

	virtual
	bool
	generateGlobalNamespaceDocumentation(
		const sl::StringRef& outputDir,
		sl::String* itemXml,
		sl::String* indexXml
		) = 0;
};

//..............................................................................

} // namespace dox
} // namespace axl
