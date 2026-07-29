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
		const FileHeapBlock* block = viewBlockUnpinned(offset);
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
FileHeap::viewBlock(
	uint64_t offset,
	size_t size,
	size_t* actualSize,
	bool isPinned
) {
	FileHeapBlock* block;
	MappedFilePin pin;

	if (!isPinned)
		block = (FileHeapBlock*)m_file.view(offset, size, actualSize);
	else {
		pin = m_file.viewAndPin(offset, size);
		block = (FileHeapBlock*)pin.p();
		if (block && actualSize)
			*actualSize = pin.getSize();
	}

	if (!block || !validateBlock(offset, block))
		return FileHeapPtr();

	uint64_t ptrOffset = offset + sizeof(FileHeapBlock);
	return isPinned ?
		FileHeapPtr(std::move(pin), block + 1, ptrOffset, block->m_size) :
		FileHeapPtr(block + 1, ptrOffset, block->m_size);
}

FileHeapPtr
FileHeap::allocateImpl(
	size_t size, // size_t, so an over-4GB request is rejected rather than truncated
	bool isPinned
) {
	if (!isOpen() || (m_file.getFlags() & FileFlag_ReadOnly)) {
		err::setError(err::SystemErrorCode_InvalidDeviceState);
		return FileHeapPtr();
	}

	if (size > MaxBlockSize - sizeof(FileHeapBlock)) {
		err::setError(err::SystemErrorCode_InvalidParameter);
		return FileHeapPtr();
	}

	// FileHeapHdr & FileHeapBlock are already 8-byte aligned
	// therefore aligning size on 8 is enough to keep all blocks 8-byte aligned

	size_t requiredSize = sl::align<Alignment>(sizeof(FileHeapBlock) + size);
	BlockMap::Iterator it = m_freeBlockMap.find<sl::RelOpKind_Ge>(BlockKey(0, requiredSize));
	if (!it) {
		bool ok = grow(requiredSize);
		if (!ok)
			return FileHeapPtr();

		it = m_freeBlockMap.find<sl::RelOpKind_Ge>(BlockKey(0, requiredSize));
		ASSERT(it);
	}

	BlockKey blockKey = it->getKey();
	ASSERT(blockKey.m_size >= requiredSize);
	ASSERT(blockKey.m_offset + blockKey.m_size <= getEndOffset());

	bool isLastBlock = blockKey.m_offset + blockKey.m_size >= getEndOffset();

	size_t actualSize = 0;
	FileHeapPtr ptr = viewBlock(blockKey.m_offset, requiredSize, &actualSize, isPinned);
	if (!ptr)
		return ptr;

	FileHeapBlock* block = (FileHeapBlock*)ptr.p() - 1;
	if (block->m_flags & FileHeapBlockFlag_Allocated) // marked free in map but actually allocated
		return err::fail(FileHeapPtr(), "corrupted file heap");

	size_t leftoverSize = blockKey.m_size - requiredSize;
	if (leftoverSize < MinBlockSize) { // leftover too small to be useful
		if (!isLastBlock) {
			bool result = updateNextBlock(blockKey.m_offset, blockKey.m_size, true);
			if (!result)
				return FileHeapPtr();
		}

		m_freeBlockMap.erase(it);
	} else { // split off the leftover as a new free block
		uint64_t leftoverOffset = blockKey.m_offset + requiredSize;
		FileHeapBlock* leftoverBlock;
		if (actualSize >= requiredSize + sizeof(FileHeapBlock))
			leftoverBlock = (FileHeapBlock*)((char*)block + requiredSize);
		else {
			leftoverBlock = (FileHeapBlock*)m_file.view(leftoverOffset, sizeof(FileHeapBlock));
			if (!leftoverBlock)
				return FileHeapPtr();
		}

		if (isLastBlock)
			m_hdr->m_lastBlockOffset = leftoverOffset;
		else {
			bool result = updateNextBlock(leftoverOffset, leftoverSize, false);
			if (!result)
				return FileHeapPtr();
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

FileHeapPtr
FileHeap::materializeImpl(
	uint64_t offset0,
	bool isPinned
) {
	if (offset0 < sizeof(FileHeapHdr) + sizeof(FileHeapBlock))
		return FileHeapPtr();

	uint64_t offset = offset0 - sizeof(FileHeapBlock);
	size_t actualSize = 0;
	FileHeapPtr ptr = viewBlock(offset, sizeof(FileHeapBlock), &actualSize, isPinned);
	if (!ptr)
		return ptr;

	FileHeapBlock* block = (FileHeapBlock*)ptr.p() - 1;
	if (block->m_size <= actualSize)
		return ptr;

	return viewBlock(offset, block->m_size, NULL, isPinned);
}

bool
FileHeap::free(uint64_t offset0) {
	ASSERT(isOpen());

	if (m_file.getFlags() & FileFlag_ReadOnly)
		return err::fail(err::SystemErrorCode_InvalidDeviceState);

	uint64_t offset = offset0 - sizeof(FileHeapBlock);
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
		FileHeapBlock* nextBlock = viewBlockUnpinned(nextOffset);
		if (!nextBlock)
			return false;

		if (nextBlock->m_prevSize != block->m_size)
			return err::fail("corrupted file heap chain");

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
		size_t blockSize = block->m_size; // viewBlock() below may evict this block
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

		FileHeapBlock* block = (FileHeapBlock*)m_file.view(Offset, size);
		if (!block)
			return false;

		block->m_signature = FileHeapConst_BlockSignature;
		block->m_size = size;
		block->m_prevSize = 0;
		block->m_flags = FileHeapBlockFlag_PrevAllocated; // can't merge with a non-existent prev block

		setEndOffset(Offset + size);
		addFreeBlock(Offset, size);
		m_hdr->m_blockCount++;
		m_hdr->m_lastBlockOffset = Offset;
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

	size_t lastBlockSize = block->m_size;
	block = (FileHeapBlock*)m_file.view(offset, sizeof(FileHeapBlock));
	if (!block)
		return false;

	block->m_signature = FileHeapConst_BlockSignature;
	block->m_size = size;
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
