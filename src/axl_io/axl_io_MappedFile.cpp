#include "pch.h"
#include "axl_io_MappedFile.h"
#include "axl_g_Module.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

void*
CMappedViewMgr::find (
	uint64_t begin,
	uint64_t end
	)
{
	// first check the last view

	TViewEntry* viewEntry = *m_viewList.getHead ();
	if (!viewEntry)
		return NULL;
		
	if (viewEntry->m_begin <= begin && viewEntry->m_end >= end)
		return (uchar_t*) (void*) viewEntry->m_view + begin - viewEntry->m_begin;

	// ok, now try to find existing view using the view map...

	CViewMap::CIterator it = m_viewMap.findEx (begin, rtl::EBinTreeFindEx_Le);
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
CMappedViewMgr::view (
	uint64_t begin,
	uint64_t end,
	uint64_t origBegin,
	uint64_t origEnd
	)
{
	TViewEntry* viewEntry = AXL_MEM_NEW (TViewEntry);

	void* p;
	size_t size = (size_t) (end - begin);
	
#if (_AXL_ENV == AXL_ENV_WIN)
	uint_t access = (m_mappedFile->m_fileFlags & EFileFlag_ReadOnly) ? 
		FILE_MAP_READ : 
		FILE_MAP_READ | FILE_MAP_WRITE;	

	p = viewEntry->m_view.view (m_mappedFile->m_mapping, access, begin, size);
#elif (_AXL_ENV == AXL_ENV_POSIX)
	int protection = (m_mappedFile->m_fileFlags & EFileFlag_ReadOnly) ? 
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

	CViewMap::CIterator it = m_viewMap.visit (begin);
	if (it->m_value) 
	{
		TViewEntry* oldViewEntry = it->m_value;
		
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
		TViewEntry* oldViewEntry = it->m_value;
		if (oldViewEntry->m_end > end) // nope, not overlapped
			break;

		ASSERT (oldViewEntry->m_mapIt == it);

		// this view is completely overlapped and is not needed for new view requests
		// remove it from map but do not delete it to make sure last N view request are still valid

		CViewMap::CIterator next = it.getInc (1);
		m_viewMap.erase (it);
		oldViewEntry->m_mapIt = NULL;

		it = next;
	}

	return p;
}

void
CMappedViewMgr::limitViewCount (size_t maxViewCount)
{
	while (m_viewList.getCount () > maxViewCount)
	{
		TViewEntry* view = m_viewList.removeTail ();
	
		if (view->m_mapIt)
			m_viewMap.erase (view->m_mapIt);

		AXL_MEM_DELETE (view);
	}
}

//.............................................................................

CMappedFile::CMappedFile ()
{
	m_fileFlags = 0;
	m_fileSize = 0;
	m_readAheadSize = EDefaults_ReadAheadSize;
	m_maxDynamicViewCount = EDefaults_MaxDynamicViewCount;
	m_dynamicViewMgr.m_mappedFile = this;
	m_permanentViewMgr.m_mappedFile = this;

#if (_AXL_ENV == AXL_ENV_WIN)
	m_mappedSize = 0;
#endif
}

bool
CMappedFile::open (
	const char* fileName,
	uint_t flags
	)
{
	close ();
	
	bool result = m_file.open (fileName, flags);
	if (!result)
		return false;

	m_fileFlags = flags;
	m_fileSize = m_file.getSize ();
	return true;
}

void
CMappedFile::close ()
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
CMappedFile::setSize (
	uint64_t size,
	bool unmapAndApplyNow
	)
{
	if (m_fileFlags & EFileFlag_ReadOnly)
		return err::fail (err::EStatus_InvalidDeviceRequest);

	m_fileSize = size;

	if (!unmapAndApplyNow || m_fileSize == m_file.getSize ())
		return true;

	unmapAllViews ();
	return m_file.setSize (m_fileSize);
}

bool
CMappedFile::setup (
	size_t maxDynamicViewCount,
	size_t readAheadSize
	)
{
	if (!maxDynamicViewCount)
		return err::fail (err::EStatus_InvalidParameter);

	m_maxDynamicViewCount = maxDynamicViewCount;
	m_readAheadSize = readAheadSize;

	m_dynamicViewMgr.limitViewCount (maxDynamicViewCount);
	return true;
}

void*
CMappedFile::view (
	uint64_t offset,
	size_t size,
	bool isPermanent
	)
{
	uint64_t end = size ? offset + size : m_fileSize; 

	if (end > m_fileSize)
	{	
		if (m_fileFlags & EFileFlag_ReadOnly)
			return err::fail ((void*) NULL, err::EStatus_InvalidDeviceRequest);
		
		m_fileSize = end;
		
#if (_AXL_ENV == AXL_ENV_POSIX)
		bool result = m_file.setSize (end);
		if (!result)
			return NULL;
#endif
	}
	
	return viewImpl (offset, end, isPermanent);
}

void*
CMappedFile::viewImpl (
	uint64_t offset,
	uint64_t end,
	bool isPermanent
	) const
{
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

	g::TSystemInfo* systemInfo = g::getModule ()->getSystemInfo ();

	uint64_t viewBegin = offset - offset % systemInfo->m_mappingAlignFactor;
	uint64_t viewEnd = end + m_readAheadSize;

	// align view region size on system page size

	if (viewEnd % systemInfo->m_pageSize)
		viewEnd = viewEnd - viewEnd % systemInfo->m_pageSize + systemInfo->m_pageSize;

#if (_AXL_ENV == AXL_ENV_WIN)
	// ensure mapping covers the view

	if (!m_mapping.isOpen () || viewEnd > m_mappedSize)
	{
		uint_t protection;

		if (m_fileFlags & EFileFlag_ReadOnly)
		{
			if (end > m_fileSize)
			{
				err::setError (err::EStatus_InvalidAddress);
				return NULL;
			}

			if (viewEnd > m_fileSize)
				viewEnd = m_fileSize;

			protection = PAGE_READONLY;
		}
		else
		{
			protection = PAGE_READWRITE;
		}
		
		bool result = m_mapping.create (m_file.m_file, NULL, protection, viewEnd);
		if (!result)
			return false;

		m_mappedSize = viewEnd;
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
CMappedFile::unmapAllViews ()
{
	m_permanentViewMgr.clear ();
	m_dynamicViewMgr.clear ();

#if (_AXL_ENV == AXL_ENV_WIN)
	m_mapping.close ();
	m_mappedSize = 0;
#endif
}

//.............................................................................

bool
CSimpleMappedFile::open (
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
