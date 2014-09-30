// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_MAPPEDFILE_H

#include "axl_rtl_List.h"
#include "axl_rtl_RbTree.h"
#include "axl_io_File.h"
#include "axl_io_Mapping.h"

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
		win::CMappedView m_view;
#elif (_AXL_ENV == AXL_ENV_POSIX)
		psx::CMapping m_view;
#endif
		uint64_t m_begin;
		uint64_t m_end;
		CViewMap::CIterator m_mapIt; // to optimize deletion
	};

protected:	
	CMappedFile* m_mappedFile;
	rtl::CStdListT <TViewEntry> m_viewList;
	CViewMap m_viewMap; 

protected:
	CMappedViewMgr () // protected construction only
	{		
		m_mappedFile = NULL;
	}
	
public:
	void*
	find (
		uint64_t begin,
		uint64_t end
		);

	void*
	view (
		uint64_t begin,
		uint64_t end,
		uint64_t origBegin,
		uint64_t origEnd
		);	

	void
	clear ()
	{
		m_viewList.clear ();
		m_viewMap.clear ();
	}

	void
	limitViewCount (size_t maxViewCount);
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
	CFile m_file;

#if (_AXL_ENV == AXL_ENV_WIN)
	mutable win::CMapping m_mapping;
	mutable uint64_t m_mappedSize;
#endif
	
	mutable CMappedViewMgr m_dynamicViewMgr;
	mutable CMappedViewMgr m_permanentViewMgr;
	
	size_t m_readAheadSize;
	size_t m_maxDynamicViewCount;

	uint_t m_fileFlags;
	uint64_t m_fileSize;

public:
	CMappedFile ();

	~CMappedFile ()
	{
		close ();
	}

	bool
	isOpen () const
	{
		return m_file.isOpen ();
	}

	bool 
	open (
		const char* fileName, 
		uint_t flags = 0 // EFileFlag 
		);

	void 
	close ();

	bool
	setup (
		size_t maxDynamicViewCount,
		size_t readAheadSize
		);

	uint64_t
	getSize () const
	{
		return m_fileSize;
	}

	bool 
	setSize (
		uint64_t size, 
		bool unmapAndApplyNow = false
		);

	const void* 
	view (
		uint64_t offset = 0, 
		size_t size = 0, 
		bool isPermanent = false
		) const
	{
		return viewImpl (offset, size ? offset + size : m_fileSize, isPermanent);
	}
	
	void* 
	view (
		uint64_t offset = 0, 
		size_t size = 0, 
		bool isPermanent = false
		);
	
	void 
	unmapAllViews ();
	
protected:
	void*
	viewImpl (
		uint64_t offset,
		uint64_t end,
		bool isPermanent
		) const;	
};

//.............................................................................

class CSimpleMappedFile
{
protected:
	CFile m_file;
	CMapping m_mapping;

public:
	bool
	isOpen ()
	{
		return m_mapping.isOpen ();
	}

	void*
	p ()
	{
		return m_mapping.p ();
	}

	operator void* ()
	{
		return m_mapping;
	}

	size_t 
	getSize ()
	{
		return m_mapping.getSize ();
	}

	bool
	open (
		const char* fileName, 
		uint64_t offset,
		size_t size,
		uint_t flags = 0 // EFileFlag 
		);

	bool
	open (
		const char* fileName, 
		uint_t flags = 0 // EFileFlag 
		)
	{
		return open (fileName, 0, -1, flags);
	}

	void 
	close ()
	{
		m_file.close ();
		m_mapping.close ();
	}
};

//.............................................................................

} // namespace io
} // namespace axl

