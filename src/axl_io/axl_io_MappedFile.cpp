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
MappedFile::view(
	uint64_t offset,
	size_t size,
	size_t* actualSize,
	bool isPermanent
) {
	uint64_t end = size ? offset + size : m_file.getSize();
	ViewEntry* viewEntry = viewImpl(isPermanent ? ViewKind_Permanent : ViewKind_Dynamic, offset, end);
	if (!viewEntry)
		return NULL;

	if (actualSize)
		*actualSize = (size_t)(viewEntry->m_end - offset);

	return viewEntry->p(offset);
}

void
MappedFile::unmapAllViews() {
	// only permanent views are allowed here
	while (!m_pinnedViewList.isEmpty()) {
		ViewEntry* viewEntry = m_pinnedViewList.removeHead();
		ASSERT(viewEntry->m_pinCount == viewEntry->m_permanentCount);
		delete viewEntry;
	}

	m_viewList.clear();
	m_viewMap.clear();
	m_lastViewEntry = NULL;

#if (_AXL_OS_WIN)
	m_mapping.close();
	m_mappingSize = 0;
#endif
}

MappedFile::ViewEntry*
MappedFile::viewImpl(
	ViewKind viewKind,
	uint64_t offset,
	uint64_t end
) {
	bool result;

	if (!isOpen())
		return NULL;

	// first, try to find existing view...

	ViewEntry* viewEntry = findView(offset, end);
	if (viewEntry) {
		switch (viewKind) {
		case ViewKind_Permanent:
			viewEntry->m_permanentCount++;
			// and fall through

		case ViewKind_Pinned:
			pinView(viewEntry);
		}

		return viewEntry;
	}

	// ...nope. ok, new view needed.

	// align view base on system allocation granularity and view region size on system page size

	const g::SystemInfo* systemInfo = g::getModule()->getSystemInfo();
	ASSERT(sl::isPowerOf2(systemInfo->m_mappingAlignFactor));

	uint64_t viewBegin = offset & ~((uint64_t)systemInfo->m_mappingAlignFactor - 1);
	uint64_t viewEnd = sl::align(viewKind == ViewKind_Permanent ? end : end + m_readAheadSize, systemInfo->m_pageSize);

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

	return createView(viewKind, viewBegin, viewEnd);
}

MappedFile::ViewEntry*
MappedFile::findView(
	uint64_t begin,
	uint64_t end
) {
	// first check the last view

	if (m_lastViewEntry && m_lastViewEntry->m_begin <= begin && m_lastViewEntry->m_end >= end)
		return m_lastViewEntry;

	// ok, now try to find existing view using the view map...

	ViewMap::Iterator it = m_viewMap.find<sl::RelOpKind_Le>(begin);
	if (!it)
		return NULL;

	if (it->m_value->m_end < end)
		return NULL;

	// ok, this view covers it.
	// move it to the head to mark as recently used and return

	ViewEntry* viewEntry = it->m_value;
	if (!viewEntry->m_pinCount)
		m_viewList.moveToHead(viewEntry);

	m_lastViewEntry = viewEntry;
	return viewEntry;
}

MappedFile::ViewEntry*
MappedFile::createView(
	ViewKind viewKind,
	uint64_t begin,
	uint64_t end
) {
	ViewEntry* viewEntry = new ViewEntry(begin, end);

	size_t size = (size_t)(end - begin);

#if (_AXL_OS_WIN)
	uint_t access = (m_fileFlags & FileFlag_ReadOnly) ?
		FILE_MAP_READ :
		FILE_MAP_READ | FILE_MAP_WRITE;

	void* p = viewEntry->m_view.view(m_mapping, access, begin, size);
#elif (_AXL_OS_POSIX)
	int protection = (m_fileFlags & FileFlag_ReadOnly) ?
		PROT_READ :
		PROT_READ | PROT_WRITE;

	void* p = viewEntry->m_view.map(
			NULL,
			size,
			protection,
			MAP_SHARED,
			m_file.m_file,
			begin
		);
#endif

	if (!p) {
		delete viewEntry;
		return NULL;
	}

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

	m_lastViewEntry = viewEntry;

	switch (viewKind) {
	case ViewKind_Dynamic:
		m_viewList.insertHead(viewEntry);
		evictViews();
		break;

	case ViewKind_Permanent:
		viewEntry->m_permanentCount = 1;
		// and fall through

	case ViewKind_Pinned:
		viewEntry->m_pinCount = 1;
		m_pinnedViewList.insertHead(viewEntry);
		break;
	}

	return viewEntry;
}

void
MappedFile::evictViews() {
	while (m_viewList.getCount() > m_maxDynamicViewCount) {
		ViewEntry* view = m_viewList.removeTail();

		if (view->m_mapIt)
			m_viewMap.erase(view->m_mapIt);

		ASSERT(view != m_lastViewEntry);
		delete view;
	}
}

//..............................................................................

} // namespace io
} // namespace axl
