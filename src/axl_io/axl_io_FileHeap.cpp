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

#include "pch.h"
#include "axl_io_FileHeap.h"
#include "axl_err_Error.h"

// FileHeap is a file-backed best-fit allocator. Its header (FileHeapHdr) stays
// mapped at m_hdr, followed by a chain of blocks; each block has a 16-byte header
// with its size, the previous block's size, and Allocated/PrevAllocated flags,
// which let free() merge with either neighbor. Free blocks are kept in an
// in-memory tree keyed by (size, offset), so allocate() best-fits in O(log n).
// Blocks are accessed through mapped views (viewBlock); close() trims the file to
// the heap end so load() can rebuild the tree by walking the chain on reopen.

namespace axl {
namespace io {

//..............................................................................

bool
FileHeap::open(
	const sl::StringRef& fileName,
	uint_t flags
) {
	close();

	bool result = m_file.open(fileName, flags);
	if (!result)
		return false;

	// load an existing (or read-only) file; otherwise create a fresh heap by
	// initializing a permanently-mapped header

	uint64_t fileSize = m_file.getSize();
	if (fileSize || (flags & FileFlag_ReadOnly))
		result = load(fileSize);
	else {
		m_hdr = (FileHeapHdr*)m_file.view(0, sizeof(FileHeapHdr), true);
		if (m_hdr) {
			m_freeBlockMap.clear();
			m_hdr->m_signature = FileHeapConst_FileSignature;
			m_hdr->m_version = FileHeapConst_Version;
			m_hdr->m_heapSize = 0; // no blocks yet (block-area size, header excluded)
			m_hdr->m_blockCount = 0;
			m_hdr->m_lastBlockOffset = 0;
		}

		result = m_hdr != NULL;
	}

	if (!result) {
		m_hdr = NULL;
		m_file.close();
		m_freeBlockMap.clear();
	}

	return result;
}

void
FileHeap::close() {
	if (!isOpen())
		return;

	// trim the file to exactly the heap end so the persisted size is exact

	if (m_hdr && !(m_file.getFlags() & FileFlag_ReadOnly)) {
		uint64_t endOffset = getEndOffset();
		m_file.unmapAllViews();
		m_file.setSize(endOffset);
	}

	m_file.close();
	m_freeBlockMap.clear();
	m_hdr = NULL;
}

bool
FileHeap::load(uint64_t fileSize) {
	if (fileSize < sizeof(FileHeapHdr))
		return err::fail(false, "invalid file heap file");

	// keep the header permanently mapped (read-only if the file is read-only)

	m_hdr = (FileHeapHdr*)m_file.view(0, sizeof(FileHeapHdr), true);
	if (!m_hdr)
		return false;

	if (m_hdr->m_signature != FileHeapConst_FileSignature ||
		m_hdr->m_version != FileHeapConst_Version ||
		m_hdr->m_heapSize + sizeof(FileHeapHdr) != fileSize
	)
		return err::fail(false, "invalid file heap file");

	// a read-only heap needs neither an in-memory free index nor top-block
	// tracking, and its header must not be written -- trust it as persisted

	if (m_file.getFlags() & FileFlag_ReadOnly)
		return true;

	// read-write: rebuild the free-block index and recompute the top block and
	// block count by walking the physical block chain (robust to a prior crash)

	m_freeBlockMap.clear();
	uint64_t offset = sizeof(FileHeapHdr);
	uint64_t prevOffset = 0;
	uint32_t prevSize = 0;
	uint64_t blockCount = 0;
	bool isPrevAllocated = true; // the first block has no free predecessor

	while (offset < fileSize) {
		const FileHeapBlock* block = viewBlock(offset);
		if (!block)
			return false;

		if (block->m_prevSize != prevSize ||
			((block->m_flags & FileHeapBlockFlag_PrevAllocated) != 0) != isPrevAllocated
		)
			return err::fail(false, "corrupted file heap block chain");

		bool isAllocated = (block->m_flags & FileHeapBlockFlag_Allocated) != 0;
		if (!isAllocated)
			addFreeBlock(offset, block->m_size);

		prevOffset = offset;
		prevSize = block->m_size;
		isPrevAllocated = isAllocated;
		offset += block->m_size;
		blockCount++;
	}

	if (offset != fileSize)
		return err::fail(false, "corrupted file heap block chain");

	m_hdr->m_blockCount = blockCount;
	m_hdr->m_lastBlockOffset = prevOffset;
	return true;
}

FileHeapPtr
FileHeap::allocate(size_t size) {
	if (!isOpen() || (m_file.getFlags() & FileFlag_ReadOnly)) {
		err::setError(err::SystemErrorCode_InvalidDeviceState);
		return g_nullFileHeapPtr;
	}

	if (size > MaxBlockSize - sizeof(FileHeapBlock)) {
		err::setError(err::SystemErrorCode_InvalidParameter);
		return g_nullFileHeapPtr;
	}

	// FileHeapHdr & FileHeapBlock are already 8-byte aligned
	// therefore aligning size on 8 is enough to keep all blocks 8-byte aligned

	size_t requiredSize = sl::align<Alignment>(sizeof(FileHeapBlock) + size);
	BlockMap::Iterator it = m_freeBlockMap.find<sl::RelOpKind_Ge>(BlockKey(0, requiredSize));
	if (!it) {
		bool ok = grow(requiredSize);
		if (!ok)
			return g_nullFileHeapPtr;

		it = m_freeBlockMap.find<sl::RelOpKind_Ge>(BlockKey(0, requiredSize));
		ASSERT(it);
	}

	BlockKey blockKey = it->getKey();
	ASSERT(blockKey.m_size >= requiredSize);
	ASSERT(blockKey.m_offset + blockKey.m_size <= getEndOffset());

	bool isLastBlock = blockKey.m_offset + blockKey.m_size >= getEndOffset();
	FileHeapBlock* block = viewBlock(blockKey.m_offset, requiredSize);
	if (!block)
		return g_nullFileHeapPtr;

	if (block->m_flags & FileHeapBlockFlag_Allocated) // marked free in map but actually allocated
		return err::fail(g_nullFileHeapPtr, "corrupted file heap");

	m_freeBlockMap.erase(it);

	block->m_flags |= FileHeapBlockFlag_Allocated;

	size_t leftoverSize = blockKey.m_size - requiredSize;
	if (leftoverSize < MinBlockSize) { // leftover too small to be useful
		if (!isLastBlock) {
			bool result = updateNextBlock(blockKey.m_offset, blockKey.m_size, true);
			if (!result)
				return g_nullFileHeapPtr;
		}
	} else { // split off the leftover as a new free block
		block->m_size = (uint32_t)requiredSize;

		uint64_t leftoverOffset = blockKey.m_offset + requiredSize;
		FileHeapBlock* leftoverBlock = (FileHeapBlock*)((char*)block + block->m_size);
		leftoverBlock->m_signature = FileHeapConst_BlockSignature;
		leftoverBlock->m_size = (uint32_t)leftoverSize;
		leftoverBlock->m_prevSize = (uint32_t)requiredSize;
		leftoverBlock->m_flags = FileHeapBlockFlag_PrevAllocated;

		if (isLastBlock)
			m_hdr->m_lastBlockOffset = leftoverOffset;
		else {
			bool result = updateNextBlock(leftoverOffset, leftoverSize, false);
			if (!result)
				return g_nullFileHeapPtr;
		}

		addFreeBlock(leftoverOffset, leftoverSize);
		m_hdr->m_blockCount++;
	}

	return FileHeapPtr(
		block + 1,
		sizeof(FileHeapBlock) + blockKey.m_offset,
		block->m_size
	);
}

FileHeapPtr
FileHeap::materialize(uint64_t offset0) {
	if (offset0 < sizeof(FileHeapHdr) + sizeof(FileHeapBlock))
		return g_nullFileHeapPtr;

	uint64_t offset = offset0 - sizeof(FileHeapBlock);
	size_t viewSize;
	FileHeapBlock* block = viewBlock(offset, sizeof(FileHeapBlock), &viewSize);
	if (!block)
		return g_nullFileHeapPtr;

	if (block->m_size > viewSize) {
		block = (FileHeapBlock*)m_file.view(offset, block->m_size);
		if (!block)
			return g_nullFileHeapPtr;
	}

	return FileHeapPtr(
		block + 1,
		sizeof(FileHeapBlock) + offset,
		block->m_size
	);
}

bool
FileHeap::free(uint64_t offset0) {
	ASSERT(isOpen());

	if (m_file.getFlags() & FileFlag_ReadOnly)
		return err::fail(err::SystemErrorCode_InvalidDeviceState);

	uint64_t offset = offset0 - sizeof(FileHeapBlock);
	FileHeapBlock* block = viewBlock(offset);
	if (!block)
		return false;

	if (!(block->m_flags & FileHeapBlockFlag_Allocated))
		return err::fail("attempt to free a non-allocated file heap block");

	// attempt to merge with the next block

	uint64_t nextOffset = offset + block->m_size;
	uint64_t endOffset = getEndOffset();
	if (nextOffset < endOffset) {
		FileHeapBlock* nextBlock = viewBlock(nextOffset);
		if (!nextBlock)
			return false;

		if (nextBlock->m_prevSize != block->m_size)
			return err::fail("corrupted file heap chain");

		if (!(nextBlock->m_flags & FileHeapBlockFlag_Allocated) &&
			block->m_size <= MaxBlockSize - nextBlock->m_size
		) {
			removeFreeBlock(nextOffset, nextBlock->m_size);
			m_hdr->m_blockCount--;
			block->m_size += nextBlock->m_size;
		}
	}

	// attempt to merge with the prev block

	if (!(block->m_flags & FileHeapBlockFlag_PrevAllocated) &&
		block->m_size <= MaxBlockSize - block->m_prevSize
	) {
		uint64_t prevOffset = offset - block->m_prevSize;
		FileHeapBlock* prevBlock = viewBlock(prevOffset);
		if (!prevBlock)
			return false;

		if (prevBlock->m_size != block->m_prevSize ||
			(prevBlock->m_flags & FileHeapBlockFlag_Allocated)
		)
			return err::fail("corrupted file heap chain");

		removeFreeBlock(prevOffset, prevBlock->m_size);
		m_hdr->m_blockCount--;
		prevBlock->m_size += block->m_size;
		offset = prevOffset;
		block = prevBlock;
	}

	block->m_flags &= ~FileHeapBlockFlag_Allocated;
	addFreeBlock(offset, block->m_size);

	if (offset + block->m_size < endOffset)
		return updateNextBlock(offset, block->m_size, false);

	m_hdr->m_lastBlockOffset = offset;
	return true;
}

bool
FileHeap::grow(size_t size0) {
	ASSERT(size0 <= MaxBlockSize);
	size_t size = sl::align(size0, m_growSize);
	if (size > MaxBlockSize)
		size = MaxBlockSize;

	if (!m_hdr->m_blockCount) { // empty heap
		enum {
			Offset = sizeof(FileHeapHdr)
		};

		FileHeapBlock* block = (FileHeapBlock*)m_file.view(Offset, size);
		if (!block)
			return false;

		block->m_signature = FileHeapConst_BlockSignature;
		block->m_size = (uint32_t)size;
		block->m_prevSize = 0;
		block->m_flags = FileHeapBlockFlag_PrevAllocated; // can't merge with a non-existent prev block

		setEndOffset(Offset + size);
		addFreeBlock(Offset, size);
		m_hdr->m_blockCount++;
		m_hdr->m_lastBlockOffset = Offset;
		return true;
	}

	uint64_t offset = m_hdr->m_lastBlockOffset;
	FileHeapBlock* block = viewBlock(offset);
	if (!block)
		return false;

	if (!(block->m_flags & FileHeapBlockFlag_Allocated)) {
		ASSERT(block->m_size < size0); // otherwise it would have satisfied the request
		removeFreeBlock(offset, block->m_size);
		block->m_size = (uint32_t)size;
		addFreeBlock(offset, size);
		setEndOffset(offset + size);
		return true;
	}

	offset += block->m_size;
	if (offset != getEndOffset())
		return err::fail("corrupted file heap");

	size_t lastBlockSize = block->m_size;
	block = (FileHeapBlock*)m_file.view(offset, sizeof(FileHeapBlock));
	if (!block)
		return false;

	block->m_signature = FileHeapConst_BlockSignature;
	block->m_size = (uint32_t)size;
	block->m_prevSize = lastBlockSize;
	block->m_flags = FileHeapBlockFlag_PrevAllocated; // prev last block is allocated

	setEndOffset(offset + size);
	addFreeBlock(offset, size);
	m_hdr->m_blockCount++;
	m_hdr->m_lastBlockOffset = offset;
	return true;
}

//..............................................................................

} // namespace io
} // namespace axl
