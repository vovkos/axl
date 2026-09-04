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

	explicit operator bool () const {
		return m_p != NULL;
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
		MinDynamicViewCount = 3, // allocate, reallocate, free -- each need 3 views live at once
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
	reallocate(
		uint64_t offset,
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

	static
	bool
	validateBlockChain(
		const FileHeapBlock* block,
		uint32_t prevSize,
		uint32_t prevFlags
	);

	FileHeapBlock*
	viewBlockUnpinned(uint64_t offset) {
		FileHeapBlock* block = m_file.view<FileHeapBlock>(offset, sizeof(FileHeapBlock));
		return block && validateBlock(offset, block) ? block : NULL;
	}

	FileHeapBlock*
	viewBlockUnpinned(
		uint64_t offset,
		uint32_t prevSize,
		uint32_t prevFlags
	) {
		FileHeapBlock* block = viewBlockUnpinned(offset);
		return block && validateBlockChain(block, prevSize, prevFlags) ? block : NULL;
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
		FileHeapBlock* nextBlock,
		uint32_t size,
		bool isAllocated
	);

	bool
	updateNextBlock(
		uint64_t offset,
		uint32_t size,
		bool isAllocated
	) {
		FileHeapBlock* nextBlock = viewBlockUnpinned(offset + size);
		return nextBlock ? updateNextBlock(nextBlock, size, isAllocated), true : false;
	}

	bool
	createFreeBlock(
		uint64_t offset,
		uint32_t size,
		uint32_t prevSize
	);

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
	ASSERT(isOpen() && !(m_file.getFlags() & FileFlag_ReadOnly));

	if (size > MaxBlockSize - sizeof(FileHeapBlock))
		return err::fail(FileHeapPtr<T>(), err::SystemErrorCode_InvalidParameter);

	// FileHeapHdr & FileHeapBlock are already 8-byte aligned
	// therefore aligning size on 8 is enough to keep all blocks 8-byte aligned

	uint32_t requiredSize = sl::align<Alignment>(sizeof(FileHeapBlock) + size);
	BlockMap::Iterator it = m_freeBlockMap.find<sl::RelOpKind_Ge>(BlockKey(0, requiredSize));
	if (!it) {
		bool result = grow(requiredSize);
		if (!result)
			return FileHeapPtr<T>();

		it = m_freeBlockMap.find<sl::RelOpKind_Ge>(BlockKey(0, requiredSize));
		ASSERT(it);
	}

	BlockKey blockKey = it->getKey();
	ASSERT(blockKey.m_size >= requiredSize);
	ASSERT(blockKey.m_offset + blockKey.m_size <= getEndOffset());

	FileHeapPtr<T> ptr = viewBlock<T>(blockKey.m_offset, requiredSize, NULL, isPinned);
	if (!ptr)
		return ptr;

	FileHeapBlock* block = (FileHeapBlock*)ptr.p() - 1;
	if (block->m_flags & FileHeapBlockFlag_Allocated) { // marked free in map but actually allocated
		err::fail("corrupted file heap");
		return FileHeapPtr<T>();
	}

	uint32_t leftoverSize = blockKey.m_size - requiredSize;
	if (leftoverSize >= MinBlockSize) {
		bool result = createFreeBlock(blockKey.m_offset + requiredSize, leftoverSize, requiredSize);
		if (!result)
			return FileHeapPtr<T>();

		block->m_size = requiredSize;
		ptr.m_size = requiredSize - sizeof(FileHeapBlock);
	} else if (blockKey.m_offset < m_hdr->m_lastBlockOffset) {
		bool result = updateNextBlock(blockKey.m_offset, blockKey.m_size, true);
		if (!result)
			return FileHeapPtr<T>();
	}

	m_freeBlockMap.erase(it);
	block->m_flags |= FileHeapBlockFlag_Allocated;
	return ptr;
}

template <typename T>
FileHeapPtr<T>
FileHeap::reallocate(
	uint64_t offset0,
	size_t size,
	bool isPinned
) {
	ASSERT(isOpen() && !(m_file.getFlags() & FileFlag_ReadOnly));

	if (offset0 == -1)
		return allocate<T>(size, isPinned);

	uint64_t offset = offset0 - sizeof(FileHeapBlock);
	if (offset < sizeof(FileHeapHdr) ||
		offset >= getEndOffset() ||
		size > MaxBlockSize - sizeof(FileHeapBlock)
	)
		return err::fail(FileHeapPtr<T>(), err::SystemErrorCode_InvalidParameter);

	FileHeapBlock* block = viewBlockUnpinned(offset);
	if (!block)
		return FileHeapPtr<T>();

	if (!(block->m_flags & FileHeapBlockFlag_Allocated))
		return err::fail(FileHeapPtr<T>(), "attempt to relocate a non-allocated file heap block");

	// try to reallocate in place (using the next block if possible)

	bool isUpdateNextBlockNeeded = false;
	uint32_t requiredSize = sl::align<Alignment>(sizeof(FileHeapBlock) + size);
	if (offset < m_hdr->m_lastBlockOffset) { // merge with next free block if it's free and big enough
		uint64_t nextOffset = offset + block->m_size;
		FileHeapBlock* nextBlock = viewBlockUnpinned(nextOffset, block->m_size, FileHeapBlockFlag_Allocated);
		if (!nextBlock)
			return FileHeapPtr<T>();

		uint64_t combinedSize = (uint64_t)block->m_size + nextBlock->m_size;
		if (!(nextBlock->m_flags & FileHeapBlockFlag_Allocated) &&
			combinedSize >= requiredSize &&
			combinedSize <= MaxBlockSize
		) {
			removeFreeBlock(nextOffset, nextBlock->m_size);
			block->m_size = (uint32_t)combinedSize;

			if (m_hdr->m_lastBlockOffset == nextOffset)
				m_hdr->m_lastBlockOffset = offset;
			else
				isUpdateNextBlockNeeded = true;

			m_hdr->m_blockCount--;
		}
	}

	if (block->m_size >= requiredSize) { // yes, we can reallocate in place
		uint32_t leftoverSize = block->m_size - requiredSize;
		if (leftoverSize >= MinBlockSize) {
			block->m_size = requiredSize; // createFreeBlock can unmap block
			bool result = createFreeBlock(offset + requiredSize, leftoverSize, requiredSize);
			if (!result)
				return FileHeapPtr<T>();
		} else if (isUpdateNextBlockNeeded) {
			bool result = updateNextBlock(offset, block->m_size, true);
			if (!result)
				return FileHeapPtr<T>();
		}

		return materialize<T>(offset0, isPinned);
	}

	// nope, full path

	FileHeapPtr<T> dst = allocate<T>(size, true); // pin against free
	FileHeapPtr<T> src = materialize<T>(offset0, false);
	if (!src || !dst)
		return FileHeapPtr<T>();

	memcpy(dst.p(), src.p(), src.getSize());

	bool result = free(offset0);
	if (!result)
		return FileHeapPtr<T>();

	if (!isPinned)
		dst.unpin();

	return dst;
}

template <typename T>
FileHeapPtr<T>
FileHeap::materialize(
	uint64_t offset0,
	bool isPinned
) {
	ASSERT(isOpen());

	uint64_t offset = offset0 - sizeof(FileHeapBlock);
	if (offset < sizeof(FileHeapHdr) || offset >= getEndOffset())
		return err::fail(FileHeapPtr<T>(), err::SystemErrorCode_InvalidParameter);

	size_t actualSize = 0;
	FileHeapPtr<T> ptr = viewBlock<T>(offset, sizeof(FileHeapBlock), &actualSize, isPinned);
	if (!ptr)
		return ptr;

	FileHeapBlock* block = (FileHeapBlock*)ptr.p() - 1;
	if (!(block->m_flags & FileHeapBlockFlag_Allocated))
		return err::fail(FileHeapPtr<T>(), "attempt to materialize a non-allocated file heap block");

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

inline
bool
FileHeap::validateBlockChain(
	const FileHeapBlock* block,
	uint32_t prevSize,
	uint32_t prevFlags
) {
	if (block->m_prevSize != prevSize ||
		((block->m_flags & FileHeapBlockFlag_PrevAllocated) >> 1) != (prevFlags & FileHeapBlockFlag_Allocated)
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
	uint64_t payloadOffset = offset + sizeof(FileHeapBlock);
	uint32_t payloadSize = block->m_size - sizeof(FileHeapBlock);

	if (isPinned)
		return FileHeapPtr<T>(std::move(view), p, payloadOffset, payloadSize);
	else
		return FileHeapPtr<T>(p, payloadOffset, payloadSize);
}

inline
void
FileHeap::updateNextBlock(
	FileHeapBlock* nextBlock,
	uint32_t size,
	bool isAllocated
) {
	nextBlock->m_prevSize = size;
	if (isAllocated)
		nextBlock->m_flags |= FileHeapBlockFlag_PrevAllocated;
	else
		nextBlock->m_flags &= ~FileHeapBlockFlag_PrevAllocated;
}

//..............................................................................

} // namespace io
} // namespace axl
