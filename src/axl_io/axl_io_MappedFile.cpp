#include "pch.h"
#include "axl_io_MappedFile.h"
#include "axl_g_Module.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

void*
MappedViewMgr::find (
	uint64_t begin,
	uint64_t end
	)
{
	// first check the last view

	ViewEntry* viewEntry = *m_viewList.getHead ();
	if (!viewEntry)
		return NULL;
		
	if (viewEntry->m_begin <= begin && viewEntry->m_end >= end)
		return (uchar_t*) (void*) viewEntry->m_view + begin - viewEntry->m_begin;

	// ok, now try to find existing view using the view map...

	ViewMap::Iterator it = m_viewMap.find (begin, sl::BinTreeFindRelOp_Le);
	if (!it)
		return NULL;

	viewEntry = it->m_value;
	if (viewEntry->m_end < end)
		return NULL;

	// ok, this view covers it.
	// move it to the head to mark as recently used and return

	m_viewList.moveToHead (viewEntry);
	return (uchar_t*) (void*) viewEntry->m_view + begin - viewEntry->m_begin;
}

void*
MappedViewMgr::view (
	uint64_t begin,
	uint64_t end,
	uint64_t origBegin,
	uint64_t origEnd
	)
{
	ViewEntry* viewEntry = AXL_MEM_NEW (ViewEntry);

	void* p;
	size_t size = (size_t) (end - begin);
	
#if (_AXL_ENV == AXL_ENV_WIN)
	uint_t access = (m_mappedFile->m_fileFlags & FileFlag_ReadOnly) ? 
		FILE_MAP_READ : 
		FILE_MAP_READ | FILE_MAP_WRITE;	

	p = viewEntry->m_view.view (m_mappedFile->m_mapping, access, begin, size);
#elif (_AXL_ENV == AXL_ENV_POSIX)
	int protection = (m_mappedFile->m_fileFlags & FileFlag_ReadOnly) ? 
		PROT_READ : 
		PROT_READ | PROT_WRITE;	
	
	p = viewEntry->m_view.map (
			NULL, 
			size, 
			protection, 
			MAP_SHARED, 
			m_mappedFile->m_file.m_file, 
			begin
			);
#endif
	
	if (!p)
	{
		AXL_MEM_DELETE (viewEntry);
		return NULL;
	}

	viewEntry->m_begin = begin;
	viewEntry->m_end = end;

	m_viewList.insertHead (viewEntry);

	// update viewmap

	ViewMap::Iterator it = m_viewMap.visit (begin);
	if (it->m_value) 
	{
		ViewEntry* oldViewEntry = it->m_value;
		
		ASSERT (oldViewEntry->m_mapIt == it);
		ASSERT (oldViewEntry->m_end < end); // otherwise, we should have just used this view!

		oldViewEntry->m_mapIt = NULL; // this view is removed from the map
	}

	viewEntry->m_mapIt = it;
	it->m_value = viewEntry;

	// now, for all the old views that are completely overlapped by this new view --
	// remove them from the map (but do not unmap yet, so recent View () results still valid)

	it++;
	while (it)
	{
		ViewEntry* oldViewEntry = it->m_value;
		if (oldViewEntry->m_end > end) // nope, not overlapped
			break;

		ASSERT (oldViewEntry->m_mapIt == it);

		// this view is completely overlapped and is not needed for new view requests
		// remove it from map but do not delete it to make sure last N view request are still valid

		ViewMap::Iterator next = it.getInc (1);
		m_viewMap.erase (it);
		oldViewEntry->m_mapIt = NULL;

		it = next;
	}

	return p;
}

void
MappedViewMgr::limitViewCount (size_t maxViewCount)
{
	while (m_viewList.getCount () > maxViewCount)
	{
		ViewEntry* view = m_viewList.removeTail ();
	
		if (view->m_mapIt)
			m_viewMap.erase (view->m_mapIt);

		AXL_MEM_DELETE (view);
	}
}

//.............................................................................

MappedFile::MappedFile ()
{
	m_fileFlags = 0;
	m_fileSize = 0;
	m_readAheadSize = DefaultsKind_ReadAheadSize;
	m_maxDynamicViewCount = DefaultsKind_MaxDynamicViewCount;
	m_dynamicViewMgr.m_mappedFile = this;
	m_permanentViewMgr.m_mappedFile = this;

#if (_AXL_ENV == AXL_ENV_WIN)
	m_mappingSize = 0;
#endif
}

void
MappedFile::close ()
{
	if (!isOpen ())
		return;

	unmapAllViews ();

	// now that all the views are unmapped, we can update file size

	if (m_fileSize != m_file.getSize ())
		m_file.setSize (m_fileSize);

	m_file.close ();

	m_fileFlags = 0;
	m_fileSize = 0;
}

bool
MappedFile::open (
	const char* fileName,
	uint_t flags
	)
{
	io::File file;

	bool result = file.open (fileName, flags);
	if (!result)
		return false;

	attach (file.m_file.detach (), flags);
	return true;
}

void
MappedFile::attach (
	File::Handle fileHandle,
	uint_t flags
	)
{
	close ();

	m_file.m_file.attach (fileHandle);
	m_fileFlags = flags;
	m_fileSize = m_file.getSize ();
}

bool
MappedFile::setSize (
	uint64_t size,
	bool unmapAndApplyNow
	)
{
	if (m_fileFlags & FileFlag_ReadOnly)
		return err::fail (err::SystemErrorCode_InvalidDeviceRequest);

	m_fileSize = size;

	if (!unmapAndApplyNow || m_fileSize == m_file.getSize ())
		return true;

	unmapAllViews ();
	return m_file.setSize (m_fileSize);
}

bool
MappedFile::setup (
	size_t maxDynamicViewCount,
	size_t readAheadSize
	)
{
	if (!maxDynamicViewCount)
		return err::fail (err::SystemErrorCode_InvalidParameter);

	m_maxDynamicViewCount = maxDynamicViewCount;
	m_readAheadSize = readAheadSize;

	m_dynamicViewMgr.limitViewCount (maxDynamicViewCount);
	return true;
}

void*
MappedFile::view (
	uint64_t offset,
	size_t size,
	bool isPermanent
	)
{
	uint64_t end = size ? offset + size : m_fileSize; 

	if (end > m_fileSize)
	{	
		if (m_fileFlags & FileFlag_ReadOnly)
			return err::fail ((void*) NULL, err::SystemErrorCode_InvalidDeviceRequest);
		
		m_fileSize = end;
	}
	
	return viewImpl (offset, end, isPermanent);
}

size_t
MappedFile::write (
	uint64_t offset,
	const void* p,
	size_t size
	)
{
	size_t result = m_file.writeAt (offset, p, size);
	if (result == -1)
		return -1;

	uint64_t end = offset + result; 
	if (end > m_fileSize)
		m_fileSize = end;

	return result;
}

void*
MappedFile::viewImpl (
	uint64_t offset,
	uint64_t end,
	bool isPermanent
	)
{
	bool result;

	if (!isOpen ())
		return NULL;
	
	// first, try to find existing view...

	void* p = m_permanentViewMgr.find (offset, end);
	if (p)
		return p;

	if (!isPermanent)
	{
		p = m_dynamicViewMgr.find (offset, end);
		if (p)
			return p;
	}

	// ...nope. ok, new view needed.

	// align view base on system allocation granularity

	g::SystemInfo* systemInfo = g::getModule ()->getSystemInfo ();

	uint64_t viewBegin = offset - offset % systemInfo->m_mappingAlignFactor;
	uint64_t viewEnd = end + m_readAheadSize;

	// align view region size on system page size

	size_t remSize = viewEnd % systemInfo->m_pageSize;
	if (viewEnd % systemInfo->m_pageSize)
		viewEnd = viewEnd - remSize + systemInfo->m_pageSize;

	// make sure we don't overextend beyond the end of read-only file

	if (m_fileFlags & FileFlag_ReadOnly)
	{
		if (end > m_fileSize)
		{
			err::setError (err::SystemErrorCode_InvalidDeviceRequest);
			return NULL;
		}

		if (viewEnd > m_fileSize)
			viewEnd = m_fileSize;
	}

	// ensure mapping covers the view

#if (_AXL_ENV == AXL_ENV_WIN)
	if (!m_mapping.isOpen () || viewEnd > m_mappingSize)
	{
		uint_t protection = (m_fileFlags & FileFlag_ReadOnly) ? PAGE_READONLY : PAGE_READWRITE;

		result = m_mapping.create (m_file.m_file, NULL, protection, viewEnd);
		if (!result)
			return NULL;

		m_mappingSize = viewEnd;
	}
#elif (_AXL_ENV == AXL_ENV_POSIX)
	if (viewEnd > m_file.getSize ())
	{
		result = m_file.setSize (viewEnd);
		if (!result)
			return NULL;

		m_fileSize = viewEnd;
	}
#endif

	// map the view

	if (isPermanent)
	{
		p = m_permanentViewMgr.view (viewBegin, viewEnd, offset, end);
	}
	else
	{
		p = m_dynamicViewMgr.view (viewBegin, viewEnd, offset, end);
		m_dynamicViewMgr.limitViewCount (m_maxDynamicViewCount);
	}

	if (!p)
		return NULL;

	return (uchar_t*) p + offset - viewBegin;
}

void
MappedFile::unmapAllViews ()
{
	m_permanentViewMgr.clear ();
	m_dynamicViewMgr.clear ();

#if (_AXL_ENV == AXL_ENV_WIN)
	m_mapping.close ();
	m_mappingSize = 0;
#endif
}

//.............................................................................

bool
SimpleMappedFile::open (
	const char* fileName, 
	uint64_t offset,
	size_t size,
	uint_t flags
	)
{
	close ();

	return 
		m_file.open (fileName, flags) &&
		m_mapping.open (&m_file, offset, size, flags);
}

//.............................................................................

} // namespace io
} // namespace axl
