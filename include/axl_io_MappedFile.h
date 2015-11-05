// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_MAPPEDFILE_H

#include "axl_sl_List.h"
#include "axl_sl_RbTree.h"
#include "axl_io_File.h"
#include "axl_io_Mapping.h"

namespace axl {
namespace io {

class MappedFile;

//.............................................................................

class MappedViewMgr
{
	friend class MappedFile;

protected:
	// interval trees are not needed here, because views never overlap COMPLETELY
	// therefore, for any 2 views in the map: (Begin1 < Begin2 && End1 < End2)

	struct ViewEntry;

	typedef sl::RbTreeMap <uint64_t, ViewEntry*> ViewMap;

	struct ViewEntry: sl::ListLink 
	{
#if (_AXL_ENV == AXL_ENV_WIN)
		win::MappedView m_view;
#elif (_AXL_ENV == AXL_ENV_POSIX)
		psx::Mapping m_view;
#endif
		uint64_t m_begin;
		uint64_t m_end;
		ViewMap::Iterator m_mapIt; // to optimize deletion
	};

protected:	
	MappedFile* m_mappedFile;
	sl::StdList <ViewEntry> m_viewList;
	ViewMap m_viewMap; 

protected:
	MappedViewMgr () // protected construction only
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

class MappedFile
{
	friend class MappedViewMgr;
	
public:
	enum DefaultsKind
	{
		DefaultsKind_MaxDynamicViewCount = 32,
		DefaultsKind_ReadAheadSize       = 16 * 1024,
	};

protected:
	File m_file;

#if (_AXL_ENV == AXL_ENV_WIN)
	win::Mapping m_mapping;
	uint64_t m_mappingSize;
#endif
	
	MappedViewMgr m_dynamicViewMgr;
	MappedViewMgr m_permanentViewMgr;
	
	size_t m_readAheadSize;
	size_t m_maxDynamicViewCount;

	uint_t m_fileFlags;
	uint64_t m_fileSize;

public:
	MappedFile ();

	~MappedFile ()
	{
		close ();
	}

	File::Handle
	getFileHandle () const
	{
		return m_file.getHandle ();
	}

	bool
	isOpen () const
	{
		return m_file.isOpen ();
	}

	void
	close ();

	bool 
	open (
		const char* fileName, 
		uint_t flags = 0 // FileFlag
		);

	void
	attach (
		File::Handle fileHandle,
		uint_t flags = 0 // FileFlag
		);

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
		return ((MappedFile*) this)->viewImpl (
			offset,
			size ? offset + size : m_fileSize,
			isPermanent
			);
	}
	
	void* 
	view (
		uint64_t offset = 0, 
		size_t size = 0, 
		bool isPermanent = false
		);

	size_t
	write (
		uint64_t offset,
		const void* p,
		size_t size
		);

	void 
	unmapAllViews ();
	
protected:
	void*
	viewImpl (
		uint64_t offset,
		uint64_t end,
		bool isPermanent
		);
};

//.............................................................................

class SimpleMappedFile
{
protected:
	File m_file;
	Mapping m_mapping;

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

