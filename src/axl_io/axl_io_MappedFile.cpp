#include "pch.h"
#include "axl_io_MappedFile.h"
#include "axl_g_Module.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

void*
CMappedViewMgr::Find (
	uint64_t Begin,
	uint64_t End
	)
{
	// first check the last view

	TViewEntry* pViewEntry = *m_ViewList.GetHead ();
	if (!pViewEntry)
		return NULL;
		
	if (pViewEntry->m_Begin <= Begin && pViewEntry->m_End >= End)
		return (uchar_t*) (void*) pViewEntry->m_View + Begin - pViewEntry->m_Begin;

	// ok, now try to find existing view using the view map...

	CViewMap::CIterator It = m_ViewMap.FindEx (Begin, rtl::EBinTreeFindEx_Le);
	if (!It)
		return NULL;

	pViewEntry = It->m_Value;
	if (pViewEntry->m_End < End)
		return NULL;

	// ok, this view covers it.
	// move it to the head to mark as recently used and return

	m_ViewList.MoveToHead (pViewEntry);
	return (uchar_t*) (void*) pViewEntry->m_View + Begin - pViewEntry->m_Begin;
}

void*
CMappedViewMgr::View (
	uint64_t Begin,
	uint64_t End,
	uint64_t OrigBegin,
	uint64_t OrigEnd
	)
{
	TViewEntry* pViewEntry = AXL_MEM_NEW (TViewEntry);

	void* p;
	size_t Size = (size_t) (End - Begin);
	
#if (_AXL_ENV == AXL_ENV_WIN)
	uint_t Access = (m_pMappedFile->m_FileFlags & EFileFlag_ReadOnly) ? 
		FILE_MAP_READ : 
		FILE_MAP_READ | FILE_MAP_WRITE;	

	p = pViewEntry->m_View.View (m_pMappedFile->m_Mapping, Access, Begin, Size);
#elif (_AXL_ENV == AXL_ENV_POSIX)
	int Protection = (m_pMappedFile->m_FileFlags & EFileFlag_ReadOnly) ? 
		PROT_READ : 
		PROT_READ | PROT_WRITE;	
	
	p = pViewEntry->m_View.Map (
			NULL, 
			Size, 
			Protection, 
			MAP_SHARED, 
			m_pMappedFile->m_File.m_File, 
			Begin
			);
#endif
	
	if (!p)
	{
		AXL_MEM_DELETE (pViewEntry);
		return NULL;
	}

	pViewEntry->m_Begin = Begin;
	pViewEntry->m_End = End;

	m_ViewList.InsertHead (pViewEntry);

	// update viewmap

	CViewMap::CIterator It = m_ViewMap.Goto (Begin);
	if (It->m_Value) 
	{
		TViewEntry* pOldViewEntry = It->m_Value;
		
		ASSERT (pOldViewEntry->m_MapIt == It);
		ASSERT (pOldViewEntry->m_End < End); // otherwise, we should have just used this view!

		pOldViewEntry->m_MapIt = NULL; // this view is removed from the map
	}

	pViewEntry->m_MapIt = It;
	It->m_Value = pViewEntry;

	// now, for all the old views that are completely overlapped by this new view --
	// remove them from the map (but do not unmap yet, so recent View () results still valid)

	It++;
	while (It)
	{
		TViewEntry* pOldViewEntry = It->m_Value;
		if (pOldViewEntry->m_End > End) // nope, not overlapped
			break;

		ASSERT (pOldViewEntry->m_MapIt == It);

		// this view is completely overlapped and is not needed for new view requests
		// remove it from map but do not delete it to make sure last N view request are still valid

		CViewMap::CIterator Next = It.GetInc (1);
		m_ViewMap.Delete (It);
		pOldViewEntry->m_MapIt = NULL;

		It = Next;
	}

	return p;
}

void
CMappedViewMgr::LimitViewCount (size_t MaxViewCount)
{
	while (m_ViewList.GetCount () > MaxViewCount)
	{
		TViewEntry* pView = m_ViewList.RemoveTail ();
	
		if (pView->m_MapIt)
			m_ViewMap.Delete (pView->m_MapIt);

		AXL_MEM_DELETE (pView);
	}
}

//.............................................................................

CMappedFile::CMappedFile ()
{
	m_FileFlags = 0;
	m_FileSize = 0;
	m_ReadAheadSize = EDefaults_ReadAheadSize;
	m_MaxDynamicViewCount = EDefaults_MaxDynamicViewCount;
	m_DynamicViewMgr.m_pMappedFile = this;
	m_PermanentViewMgr.m_pMappedFile = this;

#if (_AXL_ENV == AXL_ENV_WIN)
	m_MappedSize = 0;
#endif
}

bool
CMappedFile::Open (
	const char* pFileName,
	uint_t Flags
	)
{
	Close ();
	
	bool Result = m_File.Open (pFileName, Flags);
	if (!Result)
		return false;

	m_FileFlags = Flags;
	m_FileSize = m_File.GetSize ();
	return true;
}

void
CMappedFile::Close ()
{
	if (!IsOpen ())
		return;

	UnmapAllViews ();

	// now that all the views are unmapped, we can update file size

	if (m_FileSize != m_File.GetSize ())
		m_File.SetSize (m_FileSize);

	m_File.Close ();
	
	m_FileFlags = 0;
	m_FileSize = 0;
}

bool
CMappedFile::SetSize (
	uint64_t Size,
	bool UnmapAndApplyNow
	)
{
	if (m_FileFlags & EFileFlag_ReadOnly)
		return err::Fail (err::EStatus_InvalidDeviceRequest);

	m_FileSize = Size;

	if (!UnmapAndApplyNow || m_FileSize == m_File.GetSize ())
		return true;

	UnmapAllViews ();
	return m_File.SetSize (m_FileSize);
}

bool
CMappedFile::Setup (
	size_t MaxDynamicViewCount,
	size_t ReadAheadSize
	)
{
	if (!MaxDynamicViewCount)
		return err::Fail (err::EStatus_InvalidParameter);

	m_MaxDynamicViewCount = MaxDynamicViewCount;
	m_ReadAheadSize = ReadAheadSize;

	m_DynamicViewMgr.LimitViewCount (MaxDynamicViewCount);
	return true;
}

void*
CMappedFile::View (
	uint64_t Offset,
	size_t Size,
	bool IsPermanent
	)
{
	uint64_t End = Size ? Offset + Size : m_FileSize; 

	if (End > m_FileSize)
	{	
		if (m_FileFlags & EFileFlag_ReadOnly)
			return err::Fail ((void*) NULL, err::EStatus_InvalidDeviceRequest);
		
		m_FileSize = End;
		
#if (_AXL_ENV == AXL_ENV_POSIX)
		bool Result = m_File.SetSize (End);
		if (!Result)
			return NULL;
#endif
	}
	
	return ViewImpl (Offset, End, IsPermanent);
}

void*
CMappedFile::ViewImpl (
	uint64_t Offset,
	uint64_t End,
	bool IsPermanent
	) const
{
	if (!IsOpen ())
		return NULL;
	
	// first, try to find existing view...

	void* p = m_PermanentViewMgr.Find (Offset, End);
	if (p)
		return p;

	if (!IsPermanent)
	{
		p = m_DynamicViewMgr.Find (Offset, End);
		if (p)
			return p;
	}

	// ...nope. ok, new view needed.

	// align view base on system allocation granularity

	g::TSystemInfo* pSystemInfo = g::GetModule ()->GetSystemInfo ();

	uint64_t ViewBegin = Offset - Offset % pSystemInfo->m_MappingAlignFactor;
	uint64_t ViewEnd = End + m_ReadAheadSize;

	// align view region size on system page size

	if (ViewEnd % pSystemInfo->m_PageSize)
		ViewEnd = ViewEnd - ViewEnd % pSystemInfo->m_PageSize + pSystemInfo->m_PageSize;

#if (_AXL_ENV == AXL_ENV_WIN)
	// ensure mapping covers the view

	if (!m_Mapping.IsOpen () || ViewEnd > m_MappedSize)
	{
		uint_t Protection;

		if (m_FileFlags & EFileFlag_ReadOnly)
		{
			if (End > m_FileSize)
			{
				err::SetError (err::EStatus_InvalidAddress);
				return NULL;
			}

			if (ViewEnd > m_FileSize)
				ViewEnd = m_FileSize;

			Protection = PAGE_READONLY;
		}
		else
		{
			Protection = PAGE_READWRITE;
		}
		
		bool Result = m_Mapping.Create (m_File.m_File, NULL, Protection, ViewEnd);
		if (!Result)
			return false;

		m_MappedSize = ViewEnd;
	}
#endif
	
	// map the view

	if (IsPermanent)
	{
		p = m_PermanentViewMgr.View (ViewBegin, ViewEnd, Offset, End);
	}
	else
	{
		p = m_DynamicViewMgr.View (ViewBegin, ViewEnd, Offset, End);
		m_DynamicViewMgr.LimitViewCount (m_MaxDynamicViewCount);
	}

	if (!p)
		return NULL;

	return (uchar_t*) p + Offset - ViewBegin;
}

void
CMappedFile::UnmapAllViews ()
{
	m_PermanentViewMgr.Clear ();
	m_DynamicViewMgr.Clear ();

#if (_AXL_ENV == AXL_ENV_WIN)
	m_Mapping.Close ();
	m_MappedSize = 0;
#endif
}

//.............................................................................

} // namespace io
} // namespace axl
