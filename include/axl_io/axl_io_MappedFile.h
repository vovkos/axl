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

template <typename T = void>
class MappedFileView {
	template <typename T2>
	friend class MappedFileView;

protected:
	MappedFile* m_file;
	handle_t m_pin;
	T* m_p;
	size_t m_size;

public:
	MappedFileView() {
		init();
	}

	MappedFileView(
		MappedFile* file,
		handle_t pin,
		T* p,
		size_t size
	) {
		setup(file, pin, p, size);
	}

	MappedFileView(const MappedFileView& src) {
		setup(src.m_file, src.m_pin, src.m_p, src.m_size);
		src.pin();
	}

	MappedFileView(MappedFileView&& src) {
		setup(src.m_file, src.m_pin, src.m_p, src.m_size);
		src.init();
	}

	template <typename T2>
	MappedFileView(const MappedFileView<T2>& src) {
		setup(src.m_file, src.m_pin, src.m_p, src.m_size);
		src.pin();
	}

	template <typename T2>
	MappedFileView(MappedFileView<T2>&& src) {
		setup(src.m_file, src.m_pin, src.m_p, src.m_size);
		src.init();
	}

	~MappedFileView() {
		unpin();
	}

	operator T* () const {
		return m_p;
	}

	T*
	operator -> () const {
		ASSERT(m_p);
		return m_p;
	}

	MappedFileView&
	operator = (const MappedFileView& src) {
		copy(src);
		return *this;
	}

	MappedFileView&
	operator = (MappedFileView&& src) {
		move(std::move(src));
		return *this;
	}

	bool
	isNull() const {
		return m_p == NULL;
	}

	bool
	isPinned() const {
		return m_pin != NULL;
	}

	T*
	p() const {
		return m_p;
	}

	size_t
	getSize() const {
		return m_size;
	}

	void
	unpin();

	void
	clear() {
		unpin();
		init();
	}

	void
	copy(const MappedFileView& src) {
		if (this == &src)
			return;

		unpin();
		setup(src.m_file, src.m_pin, src.m_p, src.m_size);
		src.pin();
	}

	void
	move(MappedFileView&& src) {
		if (this == &src)
			return;

		unpin();
		setup(src.m_file, src.m_pin, src.m_p, src.m_size);
		src.init();
	}

protected:
	void
	init() {
		setup(NULL, NULL, NULL, 0);
	}

	void
	setup(
		MappedFile* file,
		handle_t pin,
		T* p,
		size_t size
	) {
		m_file = file;
		m_pin = pin;
		m_p = p;
		m_size = size;
	}

	void
	pin() const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
using ConstMappedFileView = MappedFileView<const T>;

//..............................................................................

class MappedFile {
	template <typename T>
	friend class MappedFileView;

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

	template <typename T = void>
	const T*
	view(
		uint64_t offset,
		size_t size,
		bool isPermanent = false
	) const {
		return ((MappedFile*)this)->view<T>(offset, size, isPermanent);
	}

	template <typename T = void>
	T*
	view(
		uint64_t offset,
		size_t size,
		bool isPermanent = false
	) {
		return view<T>(offset, size, NULL, isPermanent);
	}

	template <typename T = void>
	const T*
	view(
		uint64_t offset,
		size_t size,
		size_t* actualSize,
		bool isPermanent = false
	) const {
		return ((MappedFile*)this)->view<T>(offset, size, actualSize, isPermanent);
	}

	template <typename T = void>
	T*
	view(
		uint64_t offset,
		size_t size,
		size_t* actualSize,
		bool isPermanent = false
	);

	template <typename T = void>
	ConstMappedFileView<T>
	viewEx(
		uint64_t offset,
		size_t size,
		bool isPinned = true
	) const {
		return ((MappedFile*)this)->viewEx<T>(offset, size, isPinned);
	}

	template <typename T = void>
	MappedFileView<T>
	viewEx(
		uint64_t offset,
		size_t size,
		bool isPinned = true
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

template <typename T>
T*
MappedFile::view(
	uint64_t offset,
	size_t size,
	size_t* actualSize,
	bool isPermanent
) {
	uint64_t end = size ? offset + size : m_file.getSize();
	ViewEntry* viewEntry = viewImpl(isPermanent ? ViewKind_Permanent : ViewKind_Dynamic, offset, end);
	if (!viewEntry)
		return NULL;

	if (actualSize)
		*actualSize = (size_t)(viewEntry->m_end - offset);

	return (T*)viewEntry->p(offset);
}

template <typename T>
MappedFileView<T>
MappedFile::viewEx(
	uint64_t offset,
	size_t size,
	bool isPinned
) {
	uint64_t end = size ? offset + size : m_file.getSize();
	ViewEntry* viewEntry = viewImpl(isPinned ? ViewKind_Pinned : ViewKind_Dynamic , offset, end);
	if (!viewEntry)
		return MappedFileView<T>();

	return MappedFileView<T>(
		this,
		isPinned ? viewEntry : NULL,
		(T*)viewEntry->p(offset),
		viewEntry->m_end - offset
	);
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
MappedFileView<T>::pin() const {
	if (m_pin)
		m_file->pinView((MappedFile::ViewEntry*)m_pin);
}

template <typename T>
void
MappedFileView<T>::unpin() {
	if (m_pin) {
		m_file->unpinView((MappedFile::ViewEntry*)m_pin);
		m_pin = NULL;
	}
}

//..............................................................................

} // namespace io
} // namespace axl
