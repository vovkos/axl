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
#include "axl_io_HidUsageDb.h"
#include "axl_io_HidUsageDbParser.h"
#include "axl_io_MappedFile.h"
#include "axl_io_FilePathUtils.h"

namespace axl {
namespace io {

//..............................................................................

sl::String
HidUsagePage::getUsageName(uint_t usage) const {
	HidUsagePage* mutableSelf = (HidUsagePage*)this;

	if (!m_isNameMapReady) {
		if (!m_fileName.isEmpty())
			mutableSelf->load();
		mutableSelf->m_isNameMapReady = true;
	}

	sl::ConstMapIterator<uint_t, sl::String> it = m_usageNameMap.find(usage);
	if (it)
		return it->m_value;

	if (m_format.isEmpty())
		mutableSelf->m_format.format("%s 0x%%x", m_name.sz());

	return sl::formatString(m_format.sz(), usage);
}

bool
HidUsagePage::load() {
	ASSERT(!m_fileName.isEmpty());
	ASSERT(m_db->getDir().isSuffix('/'));

	io::SimpleMappedFile file;
	sl::String filePath = m_db->getDir() + m_fileName;

	return
		file.open(filePath, io::FileFlag_ReadOnly) &&
		HidUsageIniParser(this).parse(
			filePath,
			sl::StringRef((char*)file.p(), file.getMappingSize())
		);
}

//..............................................................................

void
HidUsageDb::clear() {
	m_dir.clear();
	m_usagePageList.clear();
	m_usagePageMap.clear();
}

bool
HidUsageDb::load(const sl::StringRef& fileName) {
	io::SimpleMappedFile file;
	bool result =
		file.open(fileName, io::FileFlag_ReadOnly) &&
		HidUsagePageDirIniParser(this).parse(
			fileName,
			sl::StringRef((char*)file.p(), file.getMappingSize())
		);

	if (!result)
		return false;

	m_dir = io::getDir(io::getFullFilePath(fileName));
	return true;
}

const HidUsagePage*
HidUsageDb::getUsagePage(uint_t pageId) const {
	sl::ConstMapIterator<uint_t, const HidUsagePage*> it = m_usagePageMap.find(pageId);
	if (it)
		return it->m_value;

	HidUsageDb* mutableSelf = (HidUsageDb*)this;
	HidUsagePage* page = new HidUsagePage(this);
	page->m_id = pageId;
	page->m_name.format("Page 0x%02x", pageId);
	mutableSelf->m_usagePageList.insertTail(page);
	mutableSelf->m_usagePageMap[pageId] = page;
	return page;
}

//..............................................................................

} // namespace io
} // namespace axl
