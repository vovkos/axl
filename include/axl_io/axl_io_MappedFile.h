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

#pragma once

#define _AXL_IO_MAPPEDFILE_H

#include "axl_io_File.h"
#include "axl_io_Mapping.h"

namespace axl {
namespace io {

class MappedFile;

//..............................................................................

class MappedViewMgr
{
	friend class MappedFile;

protected:
	// interval trees are not needed here, because views never overlap COMPLETELY
	// therefore, for any 2 views in the map: (Begin1 < Begin2 && End1 < End2)

	struct ViewEntry;

	typedef sl::RbTree<uint64_t, ViewEntry*> ViewMap;

	struct ViewEntry: sl::ListLink
	{
#if (_AXL_OS_WIN)
		win::MappedView m_view;
#elif (_AXL_OS_POSIX)
		psx::Mapping m_view;
#endif
		uint64_t m_begin;
		uint64_t m_end;
		ViewMap::Iterator m_mapIt; // to optimize deletion
	};

protected:
	sl::List<ViewEntry> m_viewList;
	ViewMap m_viewMap;
	size_t m_parentOffset;

protected:
	MappedViewMgr(size_t offset) // protected construction only
	{
		m_parentOffset = offset;
	}

public:
	MappedFile*
	getMappedFile()
	{
		return (MappedFile*)((char*)this - m_parentOffset);
	}

	bool
	isEmpty()
	{
		return m_viewList.isEmpty();
	}

	void*
	find(
		uint64_t begin,
		uint64_t end,
		uint64_t* actualEnd
		);

	void*
	view(
		uint64_t begin,
		uint64_t end,
		uint64_t origBegin,
		uint64_t origEnd
		);

	void
	clear()
	{
		m_viewList.clear();
		m_viewMap.clear();
	}

	void
	limitViewCount(size_t maxViewCount);
};

//..............................................................................

class MappedFile
{
	friend class MappedViewMgr;

public:
	enum DefaultsKind
	{
		DefaultsKind_MaxDynamicViewCount = 32,
		DefaultsKind_ReadAheadSize       = 64 * 1024, // 64K
	};

protected:
	File m_file;

#if (_AXL_OS_WIN)
	win::Mapping m_mapping;
	uint64_t m_mappingSize;
#endif

	MappedViewMgr m_dynamicViewMgr;
	MappedViewMgr m_permanentViewMgr;

	size_t m_readAheadSize;
	size_t m_maxDynamicViewCount;
	uint_t m_fileFlags;

public:
	MappedFile();

	~MappedFile()
	{
		close();
	}

	bool
	isOpen() const
	{
		return m_file.isOpen();
	}

	uint_t
	getFlags() const
	{
		return m_fileFlags;
	}

	const File*
	getFile() const
	{
		return &m_file;
	}

	uint64_t
	getSize() const
	{
		return m_file.getSize();
	}

	bool
	setSize(uint64_t size); // can only be called when no views are mapped

	void
	close();

	bool
	open(
		const sl::StringRef& fileName,
		uint_t flags = 0 // FileFlag
		);

	bool
	duplicate(
		File::Handle fileHandle,
		uint_t flags = 0 // FileFlag
		);

	bool
	duplicate(
		const File* file,
		uint_t flags = 0 // FileFlag
		)
	{
		ASSERT(file->isOpen());
		return duplicate(file->getHandle(), flags);
	}

	void
	attach(
		File::Handle fileHandle,
		uint_t flags = 0 // FileFlag
		);

	File::Handle
	detach();

	bool
	setup(
		size_t maxDynamicViewCount,
		size_t readAheadSize
		);

	const void*
	view(
		uint64_t offset = 0,
		size_t size = 0,
		bool isPermanent = false
		) const
	{
		return ((MappedFile*)this)->view(offset, size, isPermanent);
	}

	void*
	view(
		uint64_t offset = 0,
		size_t size = 0,
		bool isPermanent = false
		)
	{
		return view(offset, size, NULL, isPermanent);
	}

	const void*
	view(
		uint64_t offset,
		size_t size,
		size_t* actualSize,
		bool isPermanent = false
		) const
	{
		return ((MappedFile*)this)->view(offset, size, actualSize, isPermanent);
	}

	void*
	view(
		uint64_t offset,
		size_t size,
		size_t* actualSize,
		bool isPermanent = false
		);

	void
	unmapAllViews();

protected:
	void*
	viewImpl(
		uint64_t offset,
		uint64_t end,
		uint64_t* actualEnd,
		bool isPermanent
		);
};

//..............................................................................

class SimpleMappedFile
{
protected:
	File m_file;
	Mapping m_mapping;
	uint_t m_openFlags;

public:
	SimpleMappedFile()
	{
		m_openFlags = 0;
	}

	bool
	isOpen()
	{
		return m_mapping.isOpen();
	}

	void*
	p()
	{
		return m_mapping.p();
	}

	operator void* ()
	{
		return m_mapping;
	}

	uint64_t
	getFileSize()
	{
		return m_file.getSize();
	}

	size_t
	getMappingSize()
	{
		return m_mapping.getSize();
	}

	bool
	open(
		const sl::StringRef& fileName,
		uint64_t offset,
		size_t size,
		uint_t flags = 0 // FileFlag
		);

	bool
	open(
		const sl::StringRef& fileName,
		uint_t flags = 0 // FileFlag
		)
	{
		return open(fileName, 0, -1, flags);
	}

	void*
	view(
		uint64_t offset,
		size_t size
		)
	{
		return m_mapping.open(&m_file, offset, size, m_openFlags);
	}

	void
	close()
	{
		m_file.close();
		m_mapping.close();
		m_openFlags = 0;
	}
};

//..............................................................................

} // namespace io
} // namespace axl
