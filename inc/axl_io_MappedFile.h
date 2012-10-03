// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_MAPPEDFILE_H

#include "axl_io_File.h"
#include "axl_io_Mapping.h"
#include "axl_rtl_List.h"
#include "axl_rtl_RbTree.h"

namespace axl {
namespace io {

//.............................................................................

class CMappedViewMgr
{
protected:
	// interval trees are not needed here, because views never overlap COMPLETELY
	// therefore, for any 2 views in the map: (Begin1 < Begin2 && End1 < End2)

	struct TViewEntry;

	typedef rtl::CRbTreeMapT <uint64_t, TViewEntry*> CViewMap;

	struct TViewEntry: rtl::TListLink 
	{
		CMappedView m_View;
		uint64_t m_Begin;
		uint64_t m_End;
		CViewMap::CIterator m_MapIt; // to optimize deletion
	};

protected:
	rtl::CStdListT <TViewEntry> m_ViewList;
	CViewMap m_ViewMap; 

public:
	void*
	Find (
		uint64_t Begin,
		uint64_t End
		);	

	void*
	View (
		CMapping* pMapping,
		uint64_t Begin,
		uint64_t End,
		int Flags
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
public:
	enum EDefaults
	{
		EDefaults_MaxDynamicViewCount = 32,
		EDefaults_ReadAheadSize       = 16 * 1024,
	};

protected:
	CFile m_File;
	CMapping m_Mapping;
	
	CMappedViewMgr m_DynamicViewMgr;
	CMappedViewMgr m_PermanentViewMgr;

	bool m_IsReadOnly;

	size_t m_ReadAheadSize;
	size_t m_MaxDynamicViewCount;

	uint64_t m_FileSize;
	uint64_t m_MappedSize;

public:
	CMappedFile ();

	~CMappedFile ()
	{
		Close ();
	}

	bool
	IsOpen ()
	{
		return m_File.IsOpen ();
	}

	bool 
	Open (
		const tchar_t* pFileName, 
		int Flags = 0 // CFil
		);

	void 
	Close ();

	bool
	Setup (
		size_t MaxDynamicViewCount,
		size_t ReadAheadSize
		);

	uint64_t
	GetSize ()
	{
		return m_FileSize;
	}

	bool 
	SetSize (
		uint64_t Size, 
		bool ApplyNow = false
		);

	void* 
	View (
		uint64_t Offset = 0, 
		size_t Size = 0, 
		bool IsPermanent = false
		);

	void 
	UnmapAllViews ();
};

//.............................................................................

} // namespace io
} // namespace axl

