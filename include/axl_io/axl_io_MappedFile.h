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

template <typename T>
class MappedFilePinBase {
	template <typename T2>
	friend class MappedFilePinBase;

protected:
	MappedFile* m_file;
	handle_t m_view;
	T* m_p;

public:
	MappedFilePinBase() {
		init();
	}

	MappedFilePinBase(
		MappedFile* file,
		handle_t view,
		T* p
	) {
		setup(file, view, p);
	}

	MappedFilePinBase(const MappedFilePinBase& src) {
		setup(src.m_file, src.m_view, src.m_p);
		src.pin();
	}

	MappedFilePinBase(MappedFilePinBase&& src) {
		setup(src.m_file, src.m_view, src.m_p);
		src.init();
	}

	template <typename T2>
	MappedFilePinBase(const MappedFilePinBase<T2>& src) {
		setup(src.m_file, src.m_view, src.m_p);
		src.pin();
	}

	template <typename T2>
	MappedFilePinBase(MappedFilePinBase<T2>&& src) {
		setup(src.m_file, src.m_view, src.m_p);
		src.init();
	}

	~MappedFilePinBase() {
		unpin();
	}

	operator T* () const {
		return m_p;
	}

	MappedFilePinBase&
	operator = (const MappedFilePinBase& src) {
		copy(src);
		return *this;
	}

	MappedFilePinBase&
	operator = (MappedFilePinBase&& src) {
		move(std::move(src));
		return *this;
	}

	bool
	isNull() const {
		return m_p == NULL;
	}

	T*
	p() const {
		return m_p;
	}

	size_t
	getSize() const;

	void
	copy(const MappedFilePinBase& src) {
		if (this == &src)
			return;

		unpin();
		setup(src.m_file, src.m_view, src.m_p);
		src.pin();
	}

	void
	move(MappedFilePinBase&& src) {
		if (this == &src)
			return;

		unpin();
		setup(src.m_file, src.m_view, src.m_p);
		src.init();
	}

	void
	unpin();

protected:
	void
	init() {
		setup(NULL, NULL, NULL);
	}

	void
	setup(
		MappedFile* file,
		handle_t view,
		T* p
	);

	void
	pin() const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
void
MappedFilePinBase<T>::setup(
	MappedFile* file,
	handle_t view,
	T* p
) {
	m_file = file;
	m_view = view;
	m_p = p;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef MappedFilePinBase<void> MappedFilePin;
typedef MappedFilePinBase<const void> ConstMappedFilePin;

//..............................................................................

class MappedFile {
	template <typename T>
	friend class MappedFilePinBase;

public:
	enum Def {
		Def_MaxDynamicViewCount = 32,
		Def_ReadAheadSize       = 64 * 1024, // 64K
	};

protected:
	enum ViewKind {
		ViewKind_Dynamic = 0,
		ViewKind_Pinned,
		ViewKind_Permanent
	};

	// interval trees are not needed here: views never overlap COMPLETELY
	// for any 2 views in the map: (Begin1 < Begin2 && End1 < End2)

	struct ViewEntry;
	typedef sl::RbTree<uint64_t, ViewEntry*> ViewMap;

	struct ViewEntry: sl::ListLink {
#if (_AXL_OS_WIN)
		win::MappedView m_view;
#elif (_AXL_OS_POSIX)
		psx::Mapping m_view;
#endif
		uint64_t m_begin;
		uint64_t m_end;
		size_t m_pinCount;
		size_t m_permanentCount;
		ViewMap::Iterator m_mapIt; // to optimize deletion

		ViewEntry(
			uint64_t begin,
			uint64_t end
		);

		void* p(uint64_t offset) {
			ASSERT(offset >= m_begin);
			return (char*)(void*)m_view + offset - m_begin;
		}
	};

protected:
	File m_file;
#if (_AXL_OS_WIN)
	win::Mapping m_mapping;
	uint64_t m_mappingSize;
#endif
	ViewMap m_viewMap;
	sl::List<ViewEntry> m_viewList;
	sl::List<ViewEntry> m_pinnedViewList;
	ViewEntry* m_lastViewEntry;
	size_t m_maxDynamicViewCount;
	size_t m_readAheadSize;
	uint_t m_fileFlags;

public:
	MappedFile();

	~MappedFile() {
		close();
	}

	bool
	isOpen() const {
		return m_file.isOpen();
	}

	size_t
	getMaxDynamicViewCount() const {
		return m_maxDynamicViewCount;
	}

	size_t
	getReadAheadSize() const {
		return m_readAheadSize;
	}

	uint_t
	getFlags() const {
		return m_fileFlags;
	}

	const File*
	getFile() const {
		return &m_file;
	}

	uint64_t
	getSize() const {
		return m_file.getSize();
	}

	bool
	setSize(uint64_t size) { // can only be called when no views are mapped
		ASSERT(m_viewList.isEmpty() && m_pinnedViewList.isEmpty());
		return m_file.setSize(size);
	}

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
	) {
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

	void
	setup(
		size_t maxDynamicViewCount,
		size_t readAheadSize
	);

	const void*
	view(
		uint64_t offset = 0,
		size_t size = 0,
		bool isPermanent = false
	) const {
		return ((MappedFile*)this)->view(offset, size, isPermanent);
	}

	void*
	view(
		uint64_t offset = 0,
		size_t size = 0,
		bool isPermanent = false
	) {
		return view(offset, size, NULL, isPermanent);
	}

	const void*
	view(
		uint64_t offset,
		size_t size,
		size_t* actualSize,
		bool isPermanent = false
	) const {
		return ((MappedFile*)this)->view(offset, size, actualSize, isPermanent);
	}

	void*
	view(
		uint64_t offset,
		size_t size,
		size_t* actualSize,
		bool isPermanent = false
	);

	ConstMappedFilePin
	viewAndPin(
		uint64_t offset = 0,
		size_t size = 0
	) const {
		return ((MappedFile*)this)->viewAndPin(offset, size);
	}

	MappedFilePin
	viewAndPin(
		uint64_t offset = 0,
		size_t size = 0
	);

	void
	unmapAllViews();

protected:
	ViewEntry*
	viewImpl(
		ViewKind viewKind,
		uint64_t offset,
		uint64_t end
	);

	ViewEntry*
	findView(
		uint64_t begin,
		uint64_t end
	);

	ViewEntry*
	createView(
		ViewKind viewKind,
		uint64_t begin,
		uint64_t end
	);

	void
	pinView(ViewEntry* viewEntry);

	void
	unpinView(ViewEntry* viewEntry);

	void
	evictViews();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
MappedFile::MappedFile() {
	m_maxDynamicViewCount = Def_MaxDynamicViewCount;
	m_readAheadSize = Def_ReadAheadSize;
	m_fileFlags = 0;
	m_lastViewEntry = NULL;
#if (_AXL_OS_WIN)
	m_mappingSize = 0;
#endif
}

inline
MappedFile::ViewEntry::ViewEntry(
	uint64_t begin,
	uint64_t end
) {
	m_begin = begin;
	m_end = end;
	m_permanentCount = 0;
	m_pinCount = 0;
}

inline
void
MappedFile::close() {
	if (!isOpen())
		return;

	unmapAllViews();

	m_file.close();
	m_fileFlags = 0;
}

inline
bool
MappedFile::open(
	const sl::StringRef& fileName,
	uint_t flags
) {
	close();

	bool result = m_file.open(fileName, flags);
	if (!result)
		return false;

	m_fileFlags = flags;
	return true;
}

inline
bool
MappedFile::duplicate(
	File::Handle fileHandle,
	uint_t flags
) {
	close();

	bool result = m_file.duplicate(fileHandle);
	if (!result)
		return false;

	m_fileFlags = flags;
	return true;
}

inline
void
MappedFile::attach(
	File::Handle fileHandle,
	uint_t flags
) {
	close();

	m_file.m_file.attach(fileHandle);
	m_fileFlags = flags;
}

inline
File::Handle
MappedFile::detach() {
	if (!isOpen())
		return File::getInvalidHandle();

	unmapAllViews();
	m_fileFlags = 0;

	return m_file.m_file.detach();
}

inline
void
MappedFile::setup(
	size_t maxDynamicViewCount,
	size_t readAheadSize
) {
	ASSERT(maxDynamicViewCount);
	m_maxDynamicViewCount = maxDynamicViewCount;
	m_readAheadSize = readAheadSize;
	evictViews();
}

inline
MappedFilePin
MappedFile::viewAndPin(
	uint64_t offset,
	size_t size
) {
	uint64_t end = size ? offset + size : m_file.getSize();
	ViewEntry* viewEntry = viewImpl(ViewKind_Pinned, offset, end);
	return viewEntry ? MappedFilePin(this, viewEntry, viewEntry->p(offset)) : MappedFilePin();
}

inline
void
MappedFile::pinView(ViewEntry* viewEntry) {
	if (!viewEntry->m_pinCount++) {
		m_viewList.remove(viewEntry);
		m_pinnedViewList.insertHead(viewEntry);
	}
}

inline
void
MappedFile::unpinView(ViewEntry* viewEntry) {
	if (!--viewEntry->m_pinCount) {
		m_pinnedViewList.remove(viewEntry);
		if (!m_viewList.isEmpty() && m_viewList.getHead() == m_lastViewEntry)
			m_lastViewEntry = viewEntry;
		m_viewList.insertHead(viewEntry);
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
void
MappedFilePinBase<T>::pin() const {
	if (!isNull())
		m_file->pinView((MappedFile::ViewEntry*)m_view);
}

template <typename T>
void
MappedFilePinBase<T>::unpin() {
	if (!isNull()) {
		m_file->unpinView((MappedFile::ViewEntry*)m_view);
		init();
	}
}

template <typename T>
size_t
MappedFilePinBase<T>::getSize() const {
	ASSERT(!isNull());
	MappedFile::ViewEntry* view = (MappedFile::ViewEntry*)m_view;
	return (char*)(void*)view->m_view + view->m_end - view->m_begin - (char*)m_p;
}

//..............................................................................

} // namespace io
} // namespace axl
