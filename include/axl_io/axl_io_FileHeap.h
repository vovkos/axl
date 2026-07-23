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

// 4GB max alloc size -- more than enough

struct FileHeapBlock {
	uint32_t m_signature;
	uint32_t m_flags;
	uint32_t m_size;
	uint32_t m_prevSize;

	// followed by the block data
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct FileHeapPtr {
	void* m_p;
	uint64_t m_offset;
	uint32_t m_size;

	FileHeapPtr(
		void* p,
		uint64_t offset,
		uint32_t size
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
FileHeapPtr::FileHeapPtr(
	void* p,
	uint64_t offset,
	uint32_t size
) {
	m_p = p;
	m_offset = offset;
	m_size = size;
}

AXL_SELECT_ANY FileHeapPtr g_nullFileHeapPtr = { NULL, (uint64_t)-1, 0 };

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class FileHeap {
public:
	enum: size_t {
		Alignment    = 8,
		MinBlockSize = sizeof(FileHeapBlock) + Alignment,
		MaxBlockSize = 0xffffffff - Alignment + 1,
		DefGrowSize  = 16 * 1024, // grow by 16K at a time
	};

	struct BlockKey {
		uint64_t m_offset;
		size_t m_size;

		BlockKey(): BlockKey(0, 0) {}

		BlockKey(
			uint64_t offset,
			size_t size
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

protected:
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

	FileHeapPtr
	allocate(size_t size);

	FileHeapPtr
	materialize(uint64_t offset);

	bool
	free(uint64_t offset);

protected:
	bool
	load(uint64_t fileSize);

	bool
	grow(size_t size);

	FileHeapBlock*
	viewBlock(
		uint64_t offset,
		size_t size = sizeof(FileHeapBlock),
		size_t* viewSize = NULL
	);

	bool
	updateNextBlock(
		uint64_t offset,
		size_t size,
		bool isAllocated
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
	ASSERT(growSize > sizeof(FileHeapBlock) && sl::isPowerOf2(growSize));
	m_file.setup(maxDynamicViewCount, readAheadSize);
	m_growSize = growSize;
}

inline
FileHeapBlock*
FileHeap::viewBlock(
	uint64_t offset,
	size_t size,
	size_t* viewSize
) {
	FileHeapBlock* block = (FileHeapBlock*)m_file.view(offset, size, viewSize);
	if (!block)
		return NULL;

	if (block->m_signature != FileHeapConst_BlockSignature ||
		block->m_size < sizeof(FileHeapBlock) ||
		block->m_size > MaxBlockSize ||
		block->m_size + offset > getEndOffset()
	)
		return err::fail<FileHeapBlock*>(NULL, "corrupted file heap");

	return block;
}

inline
bool
FileHeap::updateNextBlock(
	uint64_t offset,
	size_t size,
	bool isAllocated
) {
	FileHeapBlock* nextBlock = viewBlock(offset + size);
	if (!nextBlock)
		return false;

	nextBlock->m_prevSize = (uint32_t)size;
	if (isAllocated)
		nextBlock->m_flags |= FileHeapBlockFlag_PrevAllocated;
	else
		nextBlock->m_flags &= ~FileHeapBlockFlag_PrevAllocated;

	return true;
}

//..............................................................................

} // namespace io
} // namespace axl
