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

#define _AXL_IO_FILEHEAP_H

#include "axl_io_MappedFile.h"
#include "axl_sl_RbTree.h"
#include "axl_sl_Array.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

class FileHeap;

//..............................................................................

enum FileHeapConst {
	FileHeapConst_FileSignature  = ':aeh',
	FileHeapConst_BlockSignature = ':klb',
	FileHeapConst_Version        = 0x010000, // 1.0.0
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum FileHeapBlockFlag {
	FileHeapBlockFlag_Allocated     = 0x00000001,
	FileHeapBlockFlag_PrevAllocated = 0x00000002,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct FileHeapHdr {
	uint32_t m_signature;
	uint32_t m_version;
	uint64_t m_heapSize;
	uint64_t m_blockCount;
	uint64_t m_lastBlockOffset;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// 4GB max block size -- more than enough

struct FileHeapBlock {
	uint32_t m_signature;
	uint32_t m_flags;
	uint32_t m_size;
	uint32_t m_prevSize;

	// followed by the block data
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T = void>
class FileHeapPtr {
	friend class FileHeap;

	template <typename T2>
	friend class FileHeapPtr;

protected:
	MappedFileView<FileHeapBlock> m_pin;
	T* m_p;
	uint64_t m_offset;
	uint32_t m_size;

public:
	FileHeapPtr() {
		init();
	}

	FileHeapPtr(
		T* p,
		uint64_t offset,
		uint32_t size
	) {
		setup(p, offset, size);
	}

	FileHeapPtr(
		MappedFileView<FileHeapBlock>&& pin,
		T* p,
		uint64_t offset,
		uint32_t size
	):
		m_pin(std::move(pin)) {
		setup(p, offset, size);
	}

	FileHeapPtr(const FileHeapPtr& src):
		m_pin(src.m_pin) {
		setup(src.m_p, src.m_offset, src.m_size);
	}

	FileHeapPtr(FileHeapPtr&& src):
		m_pin(std::move(src.m_pin)) {
		setup(src.m_p, src.m_offset, src.m_size);
		src.init();
	}

	template <typename T2>
	FileHeapPtr(const FileHeapPtr<T2>& src):
		m_pin(src.m_pin) {
		setup(src.m_p, src.m_offset, src.m_size);
	}

	template <typename T2>
	FileHeapPtr(FileHeapPtr<T2>&& src):
		m_pin(std::move(src.m_pin)) {
		setup(src.m_p, src.m_offset, src.m_size);
		src.init();
	}

	operator T* () const {
		return m_p;
	}

	T*
	operator -> () const {
		ASSERT(m_p);
		return m_p;
	}

	FileHeapPtr&
	operator = (const FileHeapPtr& src) {
		copy(src);
		return *this;
	}

	FileHeapPtr&
	operator = (FileHeapPtr&& src) {
		move(std::move(src));
		return *this;
	}

	bool
	isNull() const {
		return m_p == NULL;
	}

	bool
	isPinned() const {
		return m_pin.isPinned();
	}

	T*
	p() const {
		return m_p;
	}

	uint64_t
	getOffset() const {
		return m_offset;
	}

	uint32_t
	getSize() const {
		return m_size;
	}

	void
	unpin() {
		m_pin.unpin();
	}

	void
	clear() {
		unpin();
		init();
	}

	void
	copy(const FileHeapPtr& src) {
		if (this == &src)
			return;

		m_pin.copy(src.m_pin);
		setup(src.m_p, src.m_offset, src.m_size);
	}

	void
	move(FileHeapPtr&& src) {
		if (this == &src)
			return;

		m_pin.move(std::move(src.m_pin));
		setup(src.m_p, src.m_offset, src.m_size);
		src.init();
	}

protected:
	void
	init() {
		setup(NULL, (uint64_t)-1, 0);
	}

	void
	setup(
		T* p,
		uint64_t offset,
		uint32_t size
	) {
		m_p = p;
		m_offset = offset;
		m_size = size;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
using ConstFileHeapPtr = FileHeapPtr<const T>;

//..............................................................................

class FileHeap {
public:
	enum: size_t {
		Alignment           = 8,
		MinBlockSize        = sizeof(FileHeapBlock) + Alignment,
		MaxBlockSize        = 0xffffffff - Alignment + 1,
		MinDynamicViewCount = 3, // free() and allocate<T, false>() each keep 3 views live at once
		DefGrowSize         = 16 * 1024, // grow by 16K at a time
	};

protected:
	struct BlockKey {
		uint64_t m_offset;
		uint32_t m_size;

		BlockKey(): BlockKey(0, 0) {}

		BlockKey(
			uint64_t offset,
			uint32_t size
		) {
			m_offset = offset;
			m_size = size;
		}

		bool operator < (const BlockKey& key) const {
			return
				m_size < key.m_size || // first order by size
				m_size == key.m_size && m_offset < key.m_offset; // then by offset
		}
	};

	typedef sl::RbTree<BlockKey, bool> BlockMap;

protected:
	MappedFile m_file;
	BlockMap m_freeBlockMap;
	FileHeapHdr* m_hdr;
	size_t m_growSize;

public:
	FileHeap();

	~FileHeap() {
		close();
	}

	bool
	isOpen() const {
		return m_file.isOpen();
	}

	uint64_t
	getHeapSize() const {
		ASSERT(m_hdr);
		return m_hdr->m_heapSize;
	}

	size_t
	getBlockCount() const {
		ASSERT(m_hdr);
		return m_hdr->m_blockCount;
	}

	size_t
	getMaxDynamicViewCount() const {
		return m_file.getMaxDynamicViewCount();
	}

	size_t
	getReadAheadSize() const {
		return m_file.getReadAheadSize();
	}

	size_t
	getGrowSize() const {
		return m_growSize;
	}

	bool
	open(
		const sl::StringRef& fileName,
		uint_t flags = 0 // FileFlag
	);

	void
	close();

	void
	setup(
		size_t maxDynamicViewCount,
		size_t readAheadSize,
		size_t growSize = DefGrowSize
	);

	template <typename T = void>
	FileHeapPtr<T>
	allocate(
		size_t size,
		bool isPinned = true
	);

	template <typename T = void>
	FileHeapPtr<T>
	materialize(
		uint64_t offset,
		bool isPinned = true
	);

	bool
	free(uint64_t offset);

protected:
	bool
	load(uint64_t fileSize);

	bool
	grow(uint32_t size);

	bool
	validateBlock(
		uint64_t offset,
		const FileHeapBlock* block
	) const;

	FileHeapBlock*
	viewBlockUnpinned(uint64_t offset) {
		FileHeapBlock* block = m_file.view<FileHeapBlock>(offset, sizeof(FileHeapBlock));
		return block && validateBlock(offset, block) ? block : NULL;
	}

	template <typename T>
	FileHeapPtr<T>
	viewBlock(
		uint64_t offset,
		size_t size,
		size_t* actualSize,
		bool isPinned
	);

	void
	updateNextBlock(
		FileHeapBlock* block,
		uint32_t size,
		bool isAllocated
	);

	bool
	updateNextBlock(
		uint64_t offset,
		uint32_t size,
		bool isAllocated
	) {
		FileHeapBlock* block = viewBlockUnpinned(offset + size);
		return block ? updateNextBlock(block, size, isAllocated), true : false;
	}

	void
	addFreeBlock(
		uint64_t offset,
		size_t size
	) {
		m_freeBlockMap.visit(BlockKey(offset, size));
	}

	void
	removeFreeBlock(
		uint64_t offset,
		size_t size
	) {
		bool result = m_freeBlockMap.eraseKey(BlockKey(offset, size));
		ASSERT(result);
	}

	// m_heapSize excludes the header; the file offset of the heap end is:

	uint64_t
	getEndOffset() const {
		ASSERT(m_hdr);
		return sizeof(FileHeapHdr) + m_hdr->m_heapSize;
	}

	void
	setEndOffset(uint64_t endOffset) {
		ASSERT(m_hdr);
		m_hdr->m_heapSize = endOffset - sizeof(FileHeapHdr);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
FileHeap::FileHeap() {
	m_hdr = NULL;
	m_growSize = DefGrowSize;
}

inline
void
FileHeap::setup(
	size_t maxDynamicViewCount,
	size_t readAheadSize,
	size_t growSize
) {
	ASSERT(
		maxDynamicViewCount >= MinDynamicViewCount &&
		growSize > sizeof(FileHeapBlock) &&
		growSize <= MaxBlockSize &&
		sl::isPowerOf2(growSize)
	);

	m_file.setup(maxDynamicViewCount, readAheadSize);
	m_growSize = growSize;
}

template <typename T>
FileHeapPtr<T>
FileHeap::allocate(
	size_t size,
	bool isPinned
) {
	if (!isOpen() || (m_file.getFlags() & FileFlag_ReadOnly)) {
		err::setError(err::SystemErrorCode_InvalidDeviceState);
		return FileHeapPtr<T>();
	}

	if (size > MaxBlockSize - sizeof(FileHeapBlock)) {
		err::setError(err::SystemErrorCode_InvalidParameter);
		return FileHeapPtr<T>();
	}

	// FileHeapHdr & FileHeapBlock are already 8-byte aligned
	// therefore aligning size on 8 is enough to keep all blocks 8-byte aligned

	size_t requiredSize = sl::align<Alignment>(sizeof(FileHeapBlock) + size);
	BlockMap::Iterator it = m_freeBlockMap.find<sl::RelOpKind_Ge>(BlockKey(0, requiredSize));
	if (!it) {
		bool ok = grow(requiredSize);
		if (!ok)
			return FileHeapPtr<T>();

		it = m_freeBlockMap.find<sl::RelOpKind_Ge>(BlockKey(0, requiredSize));
		ASSERT(it);
	}

	BlockKey blockKey = it->getKey();
	ASSERT(blockKey.m_size >= requiredSize);
	ASSERT(blockKey.m_offset + blockKey.m_size <= getEndOffset());

	bool isLastBlock = blockKey.m_offset + blockKey.m_size >= getEndOffset();

	size_t actualSize = 0;
	FileHeapPtr<T> ptr = viewBlock<T>(blockKey.m_offset, requiredSize, &actualSize, isPinned);
	if (!ptr)
		return ptr;

	FileHeapBlock* block = (FileHeapBlock*)ptr.p() - 1;
	if (block->m_flags & FileHeapBlockFlag_Allocated) { // marked free in map but actually allocated
		err::fail("corrupted file heap");
		return FileHeapPtr<T>();
	}

	size_t leftoverSize = blockKey.m_size - requiredSize;
	if (leftoverSize < MinBlockSize) { // leftover too small to be useful
		if (!isLastBlock) {
			bool result = updateNextBlock(blockKey.m_offset, blockKey.m_size, true);
			if (!result)
				return FileHeapPtr<T>();
		}

		m_freeBlockMap.erase(it);
	} else { // split off the leftover as a new free block
		uint64_t leftoverOffset = blockKey.m_offset + requiredSize;
		FileHeapBlock* leftoverBlock;
		if (actualSize >= requiredSize + sizeof(FileHeapBlock))
			leftoverBlock = (FileHeapBlock*)((char*)block + requiredSize);
		else {
			leftoverBlock = m_file.view<FileHeapBlock>(leftoverOffset, sizeof(FileHeapBlock));
			if (!leftoverBlock)
				return FileHeapPtr<T>();
		}

		if (isLastBlock)
			m_hdr->m_lastBlockOffset = leftoverOffset;
		else {
			bool result = updateNextBlock(leftoverOffset, leftoverSize, false);
			if (!result)
				return FileHeapPtr<T>();
		}

		block->m_size = (uint32_t)requiredSize;
		ptr.m_size = block->m_size;

		leftoverBlock->m_signature = FileHeapConst_BlockSignature;
		leftoverBlock->m_size = (uint32_t)leftoverSize;
		leftoverBlock->m_prevSize = (uint32_t)requiredSize;
		leftoverBlock->m_flags = FileHeapBlockFlag_PrevAllocated;

		m_freeBlockMap.erase(it);
		addFreeBlock(leftoverOffset, leftoverSize);
		m_hdr->m_blockCount++;
	}

	block->m_flags |= FileHeapBlockFlag_Allocated;
	return ptr;
}

template <typename T>
FileHeapPtr<T>
FileHeap::materialize(
	uint64_t offset0,
	bool isPinned
) {
	if (offset0 < sizeof(FileHeapHdr) + sizeof(FileHeapBlock))
		return FileHeapPtr<T>();

	uint64_t offset = offset0 - sizeof(FileHeapBlock);
	size_t actualSize = 0;
	FileHeapPtr<T> ptr = viewBlock<T>(offset, sizeof(FileHeapBlock), &actualSize, isPinned);
	if (!ptr)
		return ptr;

	FileHeapBlock* block = (FileHeapBlock*)ptr.p() - 1;
	if (block->m_size <= actualSize)
		return ptr;

	return viewBlock<T>(offset, block->m_size, NULL, isPinned);
}

inline
bool
FileHeap::validateBlock(
	uint64_t offset,
	const FileHeapBlock* block
) const {
	if (block->m_signature != FileHeapConst_BlockSignature ||
		block->m_size < sizeof(FileHeapBlock) ||
		block->m_size > MaxBlockSize ||
		block->m_size + offset > getEndOffset()
	)
		return err::fail("corrupted file heap");

	return true;
}

template <typename T>
FileHeapPtr<T>
FileHeap::viewBlock(
	uint64_t offset,
	size_t size,
	size_t* actualSize,
	bool isPinned
) {
	MappedFileView<FileHeapBlock> view = m_file.viewEx<FileHeapBlock>(offset, size, isPinned);
	FileHeapBlock* block = view.p();
	if (!block || !validateBlock(offset, block))
		return FileHeapPtr<T>();

	if (actualSize)
		*actualSize = view.getSize();

	T* p = (T*)(block + 1);
	offset += sizeof(FileHeapBlock);

	if (isPinned)
		return FileHeapPtr<T>(std::move(view), p, offset, block->m_size);
	else
		return FileHeapPtr<T>(p, offset, block->m_size);
}

inline
void
FileHeap::updateNextBlock(
	FileHeapBlock* block,
	uint32_t size,
	bool isAllocated
) {
	block->m_prevSize = size;
	if (isAllocated)
		block->m_flags |= FileHeapBlockFlag_PrevAllocated;
	else
		block->m_flags &= ~FileHeapBlockFlag_PrevAllocated;
}

//..............................................................................

} // namespace io
} // namespace axl
