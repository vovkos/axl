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
		m_hdr = m_file.view<FileHeapHdr>(0, sizeof(FileHeapHdr), true);
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

	// unmapAllViews() below drops every view, so all FileHeapPtr-s handed out by
	// allocate()/materialize() must be released before closing the heap

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

	m_hdr = m_file.view<FileHeapHdr>(0, sizeof(FileHeapHdr), true);
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
	uint32_t prevFlags = FileHeapBlockFlag_Allocated; // the first block has no free predecessor
	uint64_t blockCount = 0;

	while (offset < fileSize) {
		const FileHeapBlock* block = viewBlockUnpinned(offset, prevSize, prevFlags);
		if (!block)
			return false;

		if (!(block->m_flags & FileHeapBlockFlag_Allocated))
			addFreeBlock(offset, block->m_size);

		prevOffset = offset;
		prevSize = block->m_size;
		prevFlags = block->m_flags;
		offset += block->m_size;
		blockCount++;
	}

	if (offset != fileSize)
		return err::fail(false, "corrupted file heap block chain");

	m_hdr->m_blockCount = blockCount;
	m_hdr->m_lastBlockOffset = prevOffset;
	return true;
}

bool
FileHeap::free(uint64_t offset0) {
	ASSERT(isOpen() && !(m_file.getFlags() & FileFlag_ReadOnly));

	if (offset0 == -1)
		return true;

	uint64_t offset = offset0 - sizeof(FileHeapBlock);
	if (offset < sizeof(FileHeapHdr) || offset >= getEndOffset())
		return err::fail(err::SystemErrorCode_InvalidParameter);

	FileHeapBlock* block = viewBlockUnpinned(offset);
	if (!block)
		return false;

	if (!(block->m_flags & FileHeapBlockFlag_Allocated))
		return err::fail("attempt to free a non-allocated file heap block");

	FileHeapBlock* updateNextBlockTarget = NULL;
	uint64_t mergeOffsets[2];
	size_t mergeSizes[2] = { 0 };
	size_t mergeSize = 0;
	size_t mergeCount = 0;

	// attempt to merge with the next block

	uint64_t nextOffset = offset + block->m_size;
	uint64_t endOffset = getEndOffset();
	if (nextOffset < endOffset) {
		FileHeapBlock* nextBlock = viewBlockUnpinned(nextOffset, block->m_size, FileHeapBlockFlag_Allocated);
		if (!nextBlock)
			return false;

		if (!(nextBlock->m_flags & FileHeapBlockFlag_Allocated) &&
			block->m_size <= MaxBlockSize - nextBlock->m_size
		) {
			uint64_t nextNextOffset = nextOffset + nextBlock->m_size;
			if (nextNextOffset < endOffset) {
				updateNextBlockTarget = viewBlockUnpinned(nextNextOffset);
				if (!updateNextBlockTarget)
					return false;
			}

			mergeOffsets[0] = nextOffset;
			mergeSizes[0] = nextBlock->m_size;
			mergeSize = nextBlock->m_size;
			mergeCount = 1;
		} else
			updateNextBlockTarget = nextBlock;
	}

	// attempt to merge with the prev block

	if (!(block->m_flags & FileHeapBlockFlag_PrevAllocated) &&
		block->m_size <= MaxBlockSize - block->m_prevSize
	) {
		size_t blockSize = block->m_size; // viewBlockUnpinned() below may evict this block
		size_t prevSize = block->m_prevSize;
		uint64_t prevOffset = offset - prevSize;
		FileHeapBlock* prevBlock = viewBlockUnpinned(prevOffset);
		if (!prevBlock)
			return false;

		if (prevBlock->m_size != prevSize ||
			(prevBlock->m_flags & FileHeapBlockFlag_Allocated)
		)
			return err::fail("corrupted file heap chain");

		mergeOffsets[mergeCount] = prevOffset;
		mergeSizes[mergeCount] = prevSize;
		mergeSize += blockSize;
		mergeCount++;
		offset = prevOffset;
		block = prevBlock;
	}

	block->m_flags &= ~FileHeapBlockFlag_Allocated;
	block->m_size += mergeSize;

	m_hdr->m_blockCount -= mergeCount;

	for (size_t i = 0; i < mergeCount; i++)
		removeFreeBlock(mergeOffsets[i], mergeSizes[i]);

	addFreeBlock(offset, block->m_size);

	if (updateNextBlockTarget)
		updateNextBlock(updateNextBlockTarget, block->m_size, false);
	else
		m_hdr->m_lastBlockOffset = offset;

	return true;
}

bool
FileHeap::grow(uint32_t size0) {
	ASSERT(size0 <= MaxBlockSize);
	uint32_t size = sl::align(size0, m_growSize);
	if (size > MaxBlockSize)
		size = MaxBlockSize;

	if (!m_hdr->m_blockCount) { // empty heap
		enum {
			Offset = sizeof(FileHeapHdr)
		};

		bool result = createFreeBlock(Offset, size, 0);
		if (!result)
			return false;

		setEndOffset(Offset + size);
		return true;
	}

	uint64_t offset = m_hdr->m_lastBlockOffset;
	FileHeapBlock* block = viewBlockUnpinned(offset);
	if (!block)
		return false;

	if (!(block->m_flags & FileHeapBlockFlag_Allocated)) {
		ASSERT(block->m_size < size0); // otherwise it would have satisfied the request
		removeFreeBlock(offset, block->m_size);
		block->m_size = size;
		addFreeBlock(offset, size);
		setEndOffset(offset + size);
		return true;
	}

	offset += block->m_size;
	if (offset != getEndOffset())
		return err::fail("corrupted file heap");

	bool result = createFreeBlock(offset, size, block->m_size);
	if (!result)
		return false;

	setEndOffset(offset + size);
	return true;
}

bool
FileHeap::createFreeBlock(
	uint64_t offset,
	uint32_t size,
	uint32_t prevSize
) {
	ASSERT(size >= MinBlockSize);

	FileHeapBlock* leftoverBlock = m_file.view<FileHeapBlock>(offset, sizeof(FileHeapBlock));
	if (!leftoverBlock)
		return false;

	if (offset > m_hdr->m_lastBlockOffset)
		m_hdr->m_lastBlockOffset = offset;
	else {
		bool result = updateNextBlock(offset, size, false);
		if (!result)
			return false;
	}

	leftoverBlock->m_signature = FileHeapConst_BlockSignature;
	leftoverBlock->m_size = size;
	leftoverBlock->m_prevSize = prevSize;
	leftoverBlock->m_flags = FileHeapBlockFlag_PrevAllocated; // can't be two free blocks in a row

	addFreeBlock(offset, size);
	m_hdr->m_blockCount++;
	return true;
}

//..............................................................................

} // namespace io
} // namespace axl
