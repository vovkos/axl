// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_MAPPEDFILE_H

#include "axl_rtl_List.h"
#include "axl_rtl_RbTree.h"
#include "axl_io_File.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_io_win_Mapping.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_io_psx_Mapping.h"
#endif

namespace axl {
namespace io {

class CMappedFile;
	
//.............................................................................

class CMappedViewMgr
{
	friend class CMappedFile;

protected:
	// interval trees are not needed here, because views never overlap COMPLETELY
	// therefore, for any 2 views in the map: (Begin1 < Begin2 && End1 < End2)

	struct TViewEntry;

	typedef rtl::CRbTreeMapT <uint64_t, TViewEntry*> CViewMap;

	struct TViewEntry: rtl::TListLink 
	{
#if (_AXL_ENV == AXL_ENV_WIN)
		win::CMappedView m_View;
#elif (_AXL_ENV == AXL_ENV_POSIX)
		psx::CMapping m_View;
#endif
		uint64_t m_Begin;
		uint64_t m_End;
		CViewMap::CIterator m_MapIt; // to optimize deletion
	};

protected:	
	CMappedFile* m_pMappedFile;
	rtl::CStdListT <TViewEntry> m_ViewList;
	CViewMap m_ViewMap; 

protected:
	CMappedViewMgr () // protected construction only
	{		
		m_pMappedFile = NULL;
	}
	
public:
	void*
	Find (
		uint64_t Begin,
		uint64_t End
		);

	void*
	View (
		uint64_t Begin,
		uint64_t End,
		uint64_t OrigBegin,
		uint64_t OrigEnd
		);	

	void
	Clear ()
	{
		m_ViewList.Clear ();
		m_ViewMap.Clear ();
	}

	void
	LimitViewCount (size_t MaxViewCount);
};

//.............................................................................

class CMappedFile
{
	friend class CMappedViewMgr;
	
public:
	enum EDefaults
	{
		EDefaults_MaxDynamicViewCount = 32,
		EDefaults_ReadAheadSize       = 16 * 1024,
	};

protected:
	CFile m_File;

#if (_AXL_ENV == AXL_ENV_WIN)
	mutable win::CMapping m_Mapping;
	mutable uint64_t m_MappedSize;
#endif
	
	mutable CMappedViewMgr m_DynamicViewMgr;
	mutable CMappedViewMgr m_PermanentViewMgr;
	
	size_t m_ReadAheadSize;
	size_t m_MaxDynamicViewCount;

	uint_t m_FileFlags;
	uint64_t m_FileSize;

public:
	CMappedFile ();

	~CMappedFile ()
	{
		Close ();
	}

	bool
	IsOpen () const
	{
		return m_File.IsOpen ();
	}

	bool 
	Open (
		const char* pFileName, 
		uint_t Flags = 0 // EFileFlag 
		);

	void 
	Close ();

	bool
	Setup (
		size_t MaxDynamicViewCount,
		size_t ReadAheadSize
		);

	uint64_t
	GetSize () const
	{
		return m_FileSize;
	}

	bool 
	SetSize (
		uint64_t Size, 
		bool UnmapAndApplyNow = false
		);

	const void* 
	View (
		uint64_t Offset = 0, 
		size_t Size = 0, 
		bool IsPermanent = false
		) const
	{
		return ViewImpl (Offset, Size ? Offset + Size : m_FileSize, IsPermanent);
	}
	
	void* 
	View (
		uint64_t Offset = 0, 
		size_t Size = 0, 
		bool IsPermanent = false
		);
	
	void 
	UnmapAllViews ();
	
protected:
	void*
	ViewImpl (
		uint64_t Offset,
		uint64_t End,
		bool IsPermanent
		) const;	
};

//.............................................................................

} // namespace io
} // namespace axl

