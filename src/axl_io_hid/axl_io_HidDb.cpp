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
#include "axl_io_HidDb.h"
#include "axl_io_HidDbParser.h"
#include "axl_io_MappedFile.h"
#include "axl_io_FilePathUtils.h"

namespace axl {
namespace io {

//..............................................................................

void
HidDbFileLoader::initialize(const sl::StringRef& fileName) {
	m_path = io::getFullFilePath(fileName);
	m_fileName = io::getFileName(m_path);
	m_dir = io::getDir(m_path);

#if (_AXL_OS_WIN)
	if (m_dir.isSuffix('\\'))
		m_dir[m_dir.getLength() - 1] = '/';
#endif

	if (!m_dir.isSuffix('/'))
		m_dir += '/';
}

bool
HidDbFileLoader::load(
	sl::Array<char>* buffer,
	const sl::StringRef& fileName
) const {
	ASSERT(m_dir.isSuffix('/'));
	io::SimpleMappedFile file;
	sl::String filePath = m_dir + fileName;

	bool result =
		file.open(filePath, io::FileFlag_ReadOnly) &&
		buffer->setCount(file.getMappingSize());

	if (!result)
		return false;

	memcpy(*buffer, file.p(), file.getMappingSize());
	return true;
}

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
		mutableSelf->m_format = "Usage 0x%02x";

	return sl::formatString(m_format.sz(), usage);
}

bool
HidUsagePage::load() {
	ASSERT(m_loader && !m_fileName.isEmpty());

	sl::Array<char> buffer;

	return
		m_loader->load(&buffer, m_fileName) &&
		HidUsageIniParser(this).parse(m_fileName, sl::StringRef(buffer, buffer.getCount()));
}

//..............................................................................

void
HidDb::clear() {
	m_usagePageList.clear();
	m_usagePageMap.clear();
}

bool
HidDb::load(HidDbLoader* loader) {
	clear();

	sl::Array<char> buffer;
	return
		loader->load(&buffer, loader->getFileName()) &&
		HidUsagePageDirIniParser(this, loader).parse(sl::StringRef(buffer, buffer.getCount()));
}

const HidUsagePage*
HidDb::getUsagePage(uint_t pageId) const {
	sl::ConstMapIterator<uint_t, const HidUsagePage*> it = m_usagePageMap.find(pageId);
	if (it)
		return it->m_value;

	HidDb* mutableSelf = (HidDb*)this;
	HidUsagePage* page = new HidUsagePage(NULL);
	page->m_id = pageId;
	page->m_name.format("Page 0x%02x", pageId);
	mutableSelf->m_usagePageList.insertTail(page);
	mutableSelf->m_usagePageMap[pageId] = page;
	return page;
}

//..............................................................................

} // namespace io
} // namespace axl
