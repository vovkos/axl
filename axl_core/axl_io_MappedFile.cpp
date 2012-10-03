#include "stdafx.h"
#include "axl_io_MappedFile.h"
#include "axl_win_SystemInfo.h"
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

	CViewMap::CIterator It = m_ViewMap.FindEx <rtl::CLessOrEqualT <size_t> > (Begin);
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
	CMapping* pMapping,
	uint64_t Begin,
	uint64_t End,
	int Flags
	)
{
	TViewEntry* pViewEntry = AXL_MEM_NEW (TViewEntry);

	size_t Size = (size_t) (End - Begin);

	void* p = pViewEntry->m_View.View (pMapping, Begin, Size, Flags);
	if (!p)
		AXL_MEM_DELETE (pViewEntry);

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
	m_IsReadOnly = false;
	m_FileSize = 0;
	m_MappedSize = 0;
	m_ReadAheadSize = EDefaults_ReadAheadSize;
	m_MaxDynamicViewCount = EDefaults_MaxDynamicViewCount;
}

bool
CMappedFile::Open (
	const TCHAR* pFileName,
	int Flags
	)
{
	Close ();
	
	bool Result = m_File.Open (pFileName, Flags);
	if (!Result)
		return false;

	m_IsReadOnly = (Flags & CFile::EFile_ReadOnly) != 0;
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
	
	m_FileSize = 0;
	m_IsReadOnly = false;
}

bool
CMappedFile::SetSize (
	uint64_t Size,
	bool ApplyNow
	)
{
	if (m_IsReadOnly)
		return err::FailWithSystemError (err::EStatus_InvalidDeviceRequest);

	m_FileSize = Size;

	if (!ApplyNow || m_FileSize == m_File.GetSize ())
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
		return err::FailWithSystemError (err::EStatus_InvalidParameter);

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
	bool Result;

	if (!IsOpen ())
		return NULL;

	uint64_t FileSize = GetSize ();
	uint64_t End = Size ? Offset + Size : FileSize; 

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

	// align view on system allocation granularity

	win::TSystemInfo* pSystemInfo = g::GetModule ()->GetSystemInfo ();

	uint64_t ViewBegin = Offset - Offset % pSystemInfo->m_AllocationGranularity;
	uint64_t ViewEnd = End + m_ReadAheadSize;
	
	if (ViewEnd % pSystemInfo->m_AllocationGranularity)
		ViewEnd = ViewEnd - ViewEnd % pSystemInfo->m_AllocationGranularity + pSystemInfo->m_AllocationGranularity;

	// ensure mapping covers the view

	if (!m_Mapping.IsOpen () || End > m_MappedSize)
	{
		if (m_IsReadOnly)
		{
			if (End > m_FileSize)
			{
				err::SetError (err::EStatus_InvalidAddress);
				return NULL;
			}

			if (ViewEnd > m_FileSize)
				ViewEnd = m_FileSize;
		}
		
		int MappingFlags = m_IsReadOnly ? CMapping::EMapping_ReadOnly : 0;

		Result = m_Mapping.CreateFileMapping (&m_File, ViewEnd, MappingFlags);
		if (!Result)
			return false;

		m_MappedSize = ViewEnd;
	}

	// map the view

	int ViewFlags = m_IsReadOnly ? CMappedView::EView_ReadOnly : 0;

	if (IsPermanent)
	{
		p = m_PermanentViewMgr.View (&m_Mapping, ViewBegin, ViewEnd, ViewFlags);
	}
	else
	{
		p = m_DynamicViewMgr.View (&m_Mapping, ViewBegin, ViewEnd, ViewFlags);
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
	m_Mapping.Close ();
	m_MappedSize = 0;
}

//.............................................................................

} // namespace io
} // namespace axl

