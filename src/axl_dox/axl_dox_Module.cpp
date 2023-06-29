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
#include "axl_dox_Module.h"
#include "axl_dox_Host.h"

namespace axl {
namespace dox {

//..............................................................................

void
Module::clear() {
	m_blockList.clear();
	m_groupList.clear();
	m_footnoteList.clear();
	m_refIdMap.clear();
	m_groupMap.clear();
	m_targetList.clear();
}

Group*
Module::getGroup(const sl::StringRef& name) {
	sl::StringHashTableIterator<Group*> it = m_groupMap.visit(name);
	if (it->m_value)
		return it->m_value;

	sl::String refId;
	refId.format("group_%s", name.sz());
	refId.replace('-', '_');

	Group* group = new Group(m_host);
	group->m_name = name;
	group->m_refId = adjustRefId(refId);

	m_groupList.insertTail(group);
	it->m_value = group;
	return  group;
}

Block*
Module::createBlock(handle_t item) {
	Block* block = new Block(m_host);
	block->m_item = item;
	m_blockList.insertTail(block);
	return  block;
}

Footnote*
Module::createFootnote() {
	Footnote* footnote = new Footnote(m_host);
	m_blockList.insertTail(footnote);
	return footnote;
}

sl::String
Module::adjustRefId(const sl::StringRef& refId) {
	sl::StringHashTableIterator<size_t> it = m_refIdMap.visit(refId);
	if (!it->m_value) { // no collisions
		it->m_value = 2; // start with index 2
		return refId;
	}

	sl::String adjustedRefId;
	adjustedRefId.format("%s_%d", refId.sz(), it->m_value);

	it->m_value++;
	return adjustedRefId;
}

void
Module::setBlockTarget(
	Block* block,
	int tokenKind,
	const sl::StringRef& itemName,
	size_t overloadIdx
) {
	Target* target = new Target;
	target->m_block = block;
	target->m_tokenKind = tokenKind;
	target->m_itemName = itemName;
	target->m_overloadIdx = overloadIdx;
	m_targetList.insertTail(target);
}

bool
Module::resolveBlockTargets() {
	bool result = true;

	sl::Iterator<Target> it = m_targetList.getHead();
	for (; it; it++) {
		Target* target = *it;

		handle_t item = m_host->findItem(target->m_itemName, target->m_overloadIdx);
		if (!item) {
			result = false;
			continue;
		}

		// keep the group of the original item (usually, we use @{ ... @} for grouping)

		Block* block = m_host->findItemBlock(item);
		if (block && block->m_group && !target->m_block->m_group)
			target->m_block->m_group = block->m_group;

		m_host->setItemBlock(item, target->m_block);
	}

	if (!result)
		err::setError("documentation target(s) not found");

	return result;
}

void
Module::deleteEmptyGroups() {
	bool isGroupDeleted;

	do {
		isGroupDeleted = false;

		sl::Iterator<Group> groupIt = m_groupList.getHead();
		while (groupIt) {
			sl::Iterator<Group> nextIt = groupIt.getNext();

			if (groupIt->isEmpty()) {
				if (groupIt->m_group)
					groupIt->m_group->m_groupList.remove(groupIt->m_parentGroupListIt);

				m_groupMap.eraseKey(groupIt->m_name);
				m_groupList.erase(groupIt);
				isGroupDeleted = true;
			}

			groupIt = nextIt;
		}
	} while (isGroupDeleted);
}

bool
Module::generateDocumentation(
	const sl::StringRef& outputDir,
	const sl::StringRef& indexFileName,
	const sl::StringRef& globalNamespaceFileName
) {
	static char indexFileHdr[] =
		"<?xml version='1.0' encoding='UTF-8' standalone='no'?>\n"
		"<doxygenindex>\n";

	static char indexFileTerm[] = "</doxygenindex>\n";

	static char compoundFileHdr[] =
		"<?xml version='1.0' encoding='UTF-8' standalone='no'?>\n"
		"<doxygen>\n";

	static char compoundFileTerm[] = "</doxygen>\n";

	bool result;

	result = io::ensureDirExists(outputDir);
	if (!result)
		return false;

	result = resolveBlockTargets();
	if (!result) {
		// generate a warning about orphan doxy blocks?
	}

	sl::String nspaceXml;
	sl::String indexXml;

	result = m_host->generateGlobalNamespaceDocumentation(outputDir, &nspaceXml, &indexXml);
	if (!result)
		return false;

	if (nspaceXml.isEmpty()) {
		err::setError("module does not contain any documentable items");
		return false;
	}

	result = generateGroupDocumentation(outputDir, &indexXml);
	if (!result)
		return false;

	sl::String nspaceFilePath = io::concatFilePath(outputDir, globalNamespaceFileName);
	sl::String indexFilePath = io::concatFilePath(outputDir, indexFileName);

	io::File file;
	return
		file.open(nspaceFilePath, io::FileFlag_Clear) &&
		file.write(compoundFileHdr, lengthof(compoundFileHdr)) != -1 &&
		file.write(nspaceXml, nspaceXml.getLength()) != -1 &&
		file.write(compoundFileTerm, lengthof(compoundFileTerm)) != -1 &&

		file.open(indexFilePath, io::FileFlag_Clear) &&
		file.write(indexFileHdr, lengthof(indexFileHdr)) != -1 &&
		file.write(indexXml, indexXml.getLength()) != -1 &&
		file.write(indexFileTerm, lengthof(indexFileTerm)) != -1;
}

bool
Module::generateGroupDocumentation(
	const sl::StringRef& outputDir,
	sl::String* indexXml
) {
	bool result;

	static char compoundFileHdr[] =
		"<?xml version='1.0' encoding='UTF-8' standalone='no'?>\n"
		"<doxygen>\n";

	static char compoundFileTerm[] = "</doxygen>\n";

	sl::String itemXml;

	sl::Iterator<Group> groupIt = m_groupList.getHead();
	for (; groupIt; groupIt++) {
		result = groupIt->generateDocumentation(outputDir, &itemXml, indexXml);
		if (!result)
			return false;

		sl::String refId = groupIt->getRefId();
		sl::String fileName = sl::String(outputDir) + "/" + refId + ".xml";

		io::File compoundFile;
		result =
			compoundFile.open(fileName, io::FileFlag_Clear) &&
			compoundFile.write(compoundFileHdr, lengthof(compoundFileHdr)) != -1 &&
			compoundFile.write(itemXml, itemXml.getLength()) != -1 &&
			compoundFile.write(compoundFileTerm, lengthof(compoundFileTerm)) != -1;

		if (!result)
			return false;
	}

	return true;
}

//..............................................................................

} // namespace dox
} // namespace axl
