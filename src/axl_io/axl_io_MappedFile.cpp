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
#include "axl_io_MappedFile.h"
#include "axl_g_Module.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//..............................................................................

void*
MappedViewMgr::find(
	uint64_t begin,
	uint64_t end,
	uint64_t* actualEnd
) {
	// first check the last view

	ViewEntry* viewEntry = *m_viewList.getHead();
	if (!viewEntry)
		return NULL;

	if (viewEntry->m_begin <= begin && viewEntry->m_end >= end) {
		*actualEnd = viewEntry->m_end;
		return (char*)(void*)viewEntry->m_view + begin - viewEntry->m_begin;
	}

	// ok, now try to find existing view using the view map...

	ViewMap::Iterator it = m_viewMap.find<sl::RelOpKind_Le>(begin);
	if (!it)
		return NULL;

	viewEntry = it->m_value;
	if (viewEntry->m_end < end)
		return NULL;

	// ok, this view covers it.
	// move it to the head to mark as recently used and return

	m_viewList.moveToHead(viewEntry);
	*actualEnd = viewEntry->m_end;
	return (char*)(void*)viewEntry->m_view + begin - viewEntry->m_begin;
}

void*
MappedViewMgr::view(
	uint64_t begin,
	uint64_t end,
	uint64_t origBegin,
	uint64_t origEnd
) {
	ViewEntry* viewEntry = AXL_MEM_NEW(ViewEntry);

	MappedFile* mappedFile = getMappedFile();
	void* p;
	size_t size = (size_t)(end - begin);

#if (_AXL_OS_WIN)
	uint_t access = (mappedFile->m_fileFlags & FileFlag_ReadOnly) ?
		FILE_MAP_READ :
		FILE_MAP_READ | FILE_MAP_WRITE;

	p = viewEntry->m_view.view(mappedFile->m_mapping, access, begin, size);
#elif (_AXL_OS_POSIX)
	int protection = (mappedFile->m_fileFlags & FileFlag_ReadOnly) ?
		PROT_READ :
		PROT_READ | PROT_WRITE;

	p = viewEntry->m_view.map(
			NULL,
			size,
			protection,
			MAP_SHARED,
			mappedFile->m_file.m_file,
			begin
		);
#endif

	if (!p) {
		AXL_MEM_DELETE(viewEntry);
		return NULL;
	}

	viewEntry->m_begin = begin;
	viewEntry->m_end = end;

	m_viewList.insertHead(viewEntry);

	// update viewmap

	ViewMap::Iterator it = m_viewMap.visit(begin);
	if (it->m_value) {
		ViewEntry* oldViewEntry = it->m_value;

		ASSERT(oldViewEntry->m_mapIt == it);
		ASSERT(oldViewEntry->m_end < end); // otherwise, we should have just used this view!

		oldViewEntry->m_mapIt = NULL; // this view is removed from the map
	}

	viewEntry->m_mapIt = it;
	it->m_value = viewEntry;

	// now, for all the old views that are completely overlapped by this new view --
	// remove them from the map (but do not unmap yet, so recent View () results still valid)

	it++;
	while (it) {
		ViewEntry* oldViewEntry = it->m_value;
		if (oldViewEntry->m_end > end) // nope, not overlapped
			break;

		ASSERT(oldViewEntry->m_mapIt == it);

		// this view is completely overlapped and is not needed for new view requests
		// remove it from map but do not delete it to make sure last N view request are still valid

		ViewMap::Iterator next = it.getInc(1);
		m_viewMap.erase(it);
		oldViewEntry->m_mapIt = NULL;

		it = next;
	}

	return p;
}

void
MappedViewMgr::limitViewCount(size_t maxViewCount) {
	while (m_viewList.getCount() > maxViewCount) {
		ViewEntry* view = m_viewList.removeTail();

		if (view->m_mapIt)
			m_viewMap.erase(view->m_mapIt);

		AXL_MEM_DELETE(view);
	}
}

//..............................................................................

MappedFile::MappedFile():
	m_dynamicViewMgr(offsetof(MappedFile, m_dynamicViewMgr)),
	m_permanentViewMgr(offsetof(MappedFile, m_permanentViewMgr)) {
	m_readAheadSize = DefaultsKind_ReadAheadSize;
	m_maxDynamicViewCount = DefaultsKind_MaxDynamicViewCount;
	m_fileFlags = 0;

#if (_AXL_OS_WIN)
	m_mappingSize = 0;
#endif
}

void
MappedFile::close() {
	if (!isOpen())
		return;

	unmapAllViews();

	m_file.close();
	m_fileFlags = 0;
}

bool
MappedFile::open(
	const sl::StringRef& fileName,
	uint_t flags
) {
	close();

	bool result = m_file.open(fileName, flags);
	if (!result)
		return false;

	m_fileFlags = flags;
	return true;
}

bool
MappedFile::duplicate(
	File::Handle fileHandle,
	uint_t flags
) {
	close();

	bool result = m_file.duplicate(fileHandle);
	if (!result)
		return false;

	m_fileFlags = flags;
	return true;
}

void
MappedFile::attach(
	File::Handle fileHandle,
	uint_t flags
) {
	close();

	m_file.m_file.attach(fileHandle);
	m_fileFlags = flags;
}

File::Handle
MappedFile::detach() {
	if (!isOpen())
		return File::getInvalidHandle();

	unmapAllViews();
	m_fileFlags = 0;

	return m_file.m_file.detach();
}

bool
MappedFile::setSize(uint64_t size) {
	if (!m_permanentViewMgr.isEmpty() ||
		!m_dynamicViewMgr.isEmpty()) {
		err::setError(err::SystemErrorCode_InvalidDeviceState);
		return false;
	}

	return m_file.setSize(size);
}

bool
MappedFile::setup(
	size_t maxDynamicViewCount,
	size_t readAheadSize
) {
	if (!maxDynamicViewCount)
		return err::fail(err::SystemErrorCode_InvalidParameter);

	m_maxDynamicViewCount = maxDynamicViewCount;
	m_readAheadSize = readAheadSize;

	m_dynamicViewMgr.limitViewCount(maxDynamicViewCount);
	return true;
}

void*
MappedFile::view(
	uint64_t offset,
	size_t size,
	size_t* actualSize,
	bool isPermanent
) {
	uint64_t end = size ? offset + size : m_file.getSize();

	uint64_t actualEnd;
	void* p = viewImpl(offset, end, &actualEnd, isPermanent);
	if (!p)
		return NULL;

	if (actualSize)
		*actualSize = (size_t)(actualEnd - offset);

	return p;
}

void*
MappedFile::viewImpl(
	uint64_t offset,
	uint64_t end,
	uint64_t* actualEnd,
	bool isPermanent
) {
	bool result;

	if (!isOpen())
		return NULL;

	// first, try to find existing view...

	void* p = m_permanentViewMgr.find(offset, end, actualEnd);
	if (p)
		return p;

	if (!isPermanent) {
		p = m_dynamicViewMgr.find(offset, end, actualEnd);
		if (p)
			return p;
	}

	// ...nope. ok, new view needed.

	// align view base on system allocation granularity and view region size on system page size

	const g::SystemInfo* systemInfo = g::getModule()->getSystemInfo();
	ASSERT(sl::isPowerOf2(systemInfo->m_mappingAlignFactor));

	uint64_t viewBegin = offset & ~(systemInfo->m_mappingAlignFactor - 1);
	uint64_t viewEnd = sl::align(end + m_readAheadSize, systemInfo->m_pageSize);

	// make sure we don't overextend beyond the end of read-only file

	if (m_fileFlags & FileFlag_ReadOnly) {
		uint64_t fileSize = m_file.getSize();
		if (end > fileSize) {
			err::setError(err::SystemErrorCode_InvalidDeviceRequest);
			return NULL;
		}

		if (viewEnd > fileSize)
			viewEnd = fileSize;
	}

	// ensure mapping covers the view

#if (_AXL_OS_WIN)
	if (!m_mapping.isOpen() || viewEnd > m_mappingSize) {
		uint_t protection = (m_fileFlags & FileFlag_ReadOnly) ? PAGE_READONLY : PAGE_READWRITE;

		result = m_mapping.create(m_file.m_file, NULL, protection, viewEnd);
		if (!result)
			return NULL;

		m_mappingSize = viewEnd;
	}
#elif (_AXL_OS_POSIX)
	if (viewEnd > m_file.getSize()) {
		result = m_file.setSize(viewEnd);
		if (!result)
			return NULL;
	}
#endif

	// map the view

	if (isPermanent) {
		p = m_permanentViewMgr.view(viewBegin, viewEnd, offset, end);
	} else {
		p = m_dynamicViewMgr.view(viewBegin, viewEnd, offset, end);
		m_dynamicViewMgr.limitViewCount(m_maxDynamicViewCount);
	}

	if (!p)
		return NULL;

	*actualEnd = viewEnd;
	return (uchar_t*)p + offset - viewBegin;
}

void
MappedFile::unmapAllViews() {
	m_permanentViewMgr.clear();
	m_dynamicViewMgr.clear();

#if (_AXL_OS_WIN)
	m_mapping.close();
	m_mappingSize = 0;
#endif
}

//..............................................................................

bool
SimpleMappedFile::open(
	const sl::StringRef& fileName,
	uint64_t offset,
	size_t size,
	uint_t flags
) {
	bool result;

	close();

	result = m_file.open(fileName, flags);
	if (!result)
		return false;

	m_openFlags = flags;
	return !size || m_mapping.open(&m_file, offset, size, flags);
}

//..............................................................................

} // namespace io
} // namespace axl
