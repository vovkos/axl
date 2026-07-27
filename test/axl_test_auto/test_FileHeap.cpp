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
#include "test.h"
#include "axl_io_FileHeap.h"
#include "axl_io_FilePathUtils.h"
#include "axl_g_Module.h"

namespace {

//..............................................................................

uint32_t
lcg(uint32_t* seed) {
	*seed = *seed * 1664525 + 1013904223;
	return *seed >> 16;
}

// a live allocation in the reference model

struct Alloc {
	uint64_t m_offset; // handle (FileHeapPtr::m_offset)
	uint32_t m_size;   // requested size
	uint8_t m_fill;    // base byte of the fill pattern
};

// mostly small (exercise splitting), occasionally large (exercise growth and
// blocks that outrun the read-ahead window)

uint32_t
randomSize(uint32_t* seed) {
	return (lcg(seed) & 0x1f) == 0 ?
		lcg(seed) % (128 * 1024) :
		lcg(seed) % 4000;
}

void
writePattern(
	void* p,
	size_t size,
	uint8_t fill
) {
	uchar_t* b = (uchar_t*)p;
	for (size_t i = 0; i < size; i++)
		b[i] = (uchar_t)(fill + i);
}

void
checkPattern(
	const void* p,
	size_t size,
	uint8_t fill
) {
	const uchar_t* b = (const uchar_t*)p;
	for (size_t i = 0; i < size; i++)
		TEST_ASSERT(b[i] == (uchar_t)(fill + i));
}

// re-map the block by its handle and check its pattern is intact

void
verifyBlock(
	io::FileHeap* heap,
	const Alloc& alloc
) {
	io::FileHeapPtr ptr = heap->materialize(alloc.m_offset);
	TEST_ASSERT(ptr.m_p && ptr.m_offset == alloc.m_offset);
	TEST_ASSERT(ptr.m_size >= sizeof(io::FileHeapBlock) + alloc.m_size); // total block size covers the request
	checkPattern(ptr.m_p, alloc.m_size, alloc.m_fill);
}

void
verifyAll(
	io::FileHeap* heap,
	const sl::Array<Alloc>& live
) {
	TEST_ASSERT(heap->getBlockCount() >= live.getCount()); // total blocks >= allocated blocks

	for (size_t i = 0; i < live.getCount(); i++)
		verifyBlock(heap, live[i]);
}

void
allocOne(
	io::FileHeap* heap,
	sl::Array<Alloc>* live,
	uint32_t* seed,
	uint8_t* fillCounter
) {
	Alloc alloc;
	alloc.m_size = randomSize(seed);
	alloc.m_fill = (*fillCounter)++;

	io::FileHeapPtr ptr = heap->allocate(alloc.m_size);
	TEST_ASSERT(ptr.m_p);
	TEST_ASSERT(ptr.m_size >= sizeof(io::FileHeapBlock) + alloc.m_size);

	alloc.m_offset = ptr.m_offset;
	writePattern(ptr.m_p, alloc.m_size, alloc.m_fill);
	live->append(alloc);
}

void
freeOne(
	io::FileHeap* heap,
	sl::Array<Alloc>* live,
	uint32_t* seed
) {
	size_t count = live->getCount();
	size_t index = lcg(seed) % count;

	verifyBlock(heap, (*live)[index]); // still intact right before freeing
	heap->free((*live)[index].m_offset);

	size_t last = count - 1;
	if (index != last)
		live->rwi()[index] = (*live)[last];
	live->pop();
}

void
churn(
	io::FileHeap* heap,
	sl::Array<Alloc>* live,
	uint32_t* seed,
	uint8_t* fillCounter,
	size_t iterationCount,
	size_t maxLiveTarget
) {
	for (size_t i = 0; i < iterationCount; i++) {
		bool doAlloc = live->isEmpty() ||
			(live->getCount() < maxLiveTarget && (lcg(seed) & 1));

		if (doAlloc)
			allocOne(heap, live, seed, fillCounter);
		else
			freeOne(heap, live, seed);

		if ((i & 0xff) == 0)
			verifyAll(heap, *live);
	}

	verifyAll(heap, *live);
}

// churn a heap, then reopen it read-write (exercising load(): the free index is
// rebuilt and the top block / block count are recomputed by walking the chain);
// every live block must survive, and allocation must still work afterwards

void
test_ReadWrite() {
	enum {
		IterationCount = 4000,
		MaxLiveTarget  = 200,
	};

	sl::String fileName = io::createTempFile();
	TEST_ASSERT(!fileName.isEmpty());
	io::AutoDeleteFile autoDelete(fileName);

	uint32_t seed = 0x12345678;
	uint8_t fillCounter = 1;
	sl::Array<Alloc> live;

	io::FileHeap heap;
	bool result = heap.open(fileName); // empty file -> create
	TEST_ASSERT(result);
	TEST_ASSERT(heap.getBlockCount() == 0);
	TEST_ASSERT(heap.getHeapSize() == 0);

	churn(&heap, &live, &seed, &fillCounter, IterationCount, MaxLiveTarget);

	// reopen read-write: load() must reconstruct everything from the file

	uint64_t heapSizeBeforeReload = heap.getHeapSize();
	heap.close();
	result = heap.open(fileName); // non-empty file -> load
	TEST_ASSERT(result);
	TEST_ASSERT(heap.getHeapSize() == heapSizeBeforeReload);
	verifyAll(&heap, live);

	// keep operating after the reload

	churn(&heap, &live, &seed, &fillCounter, IterationCount, MaxLiveTarget);

	// free everything -> full coalescing collapses the heap into one free block

	while (!live.isEmpty())
		freeOne(&heap, &live, &seed);

	TEST_ASSERT(heap.getBlockCount() == 1);

	// the collapsed free space must be reusable without growing the heap

	uint64_t heapSize = heap.getHeapSize();
	io::FileHeapPtr ptr = heap.allocate(2000);
	TEST_ASSERT(ptr.m_p);
	TEST_ASSERT(heap.getHeapSize() == heapSize);
	heap.free(ptr.m_offset);
	TEST_ASSERT(heap.getBlockCount() == 1);

	heap.close();
}

// write some blocks, close, then reopen the file read-only and read them back:
// the read-only path must never write to the file (header included), and mutating
// operations must be rejected

void
test_ReadOnly() {
	enum {
		BlockCount = 64,
	};

	sl::String fileName = io::createTempFile();
	TEST_ASSERT(!fileName.isEmpty());
	io::AutoDeleteFile autoDelete(fileName);

	uint32_t seed = 0x2468ace0;
	uint8_t fillCounter = 1;
	sl::Array<Alloc> live;

	{
		io::FileHeap heap;
		bool result = heap.open(fileName);
		TEST_ASSERT(result);

		for (size_t i = 0; i < BlockCount; i++) {
			Alloc alloc;
			alloc.m_size = randomSize(&seed);
			alloc.m_fill = fillCounter++;

			io::FileHeapPtr ptr = heap.allocate(alloc.m_size);
			TEST_ASSERT(ptr.m_p);

			alloc.m_offset = ptr.m_offset;
			writePattern(ptr.m_p, alloc.m_size, alloc.m_fill);
			live.append(alloc);
		}

		heap.close();
	}

	// reopen read-only and read the markup back

	io::FileHeap heap;
	bool result = heap.open(fileName, io::FileFlag_ReadOnly);
	TEST_ASSERT(result);
	TEST_ASSERT(heap.getBlockCount() >= live.getCount());

	verifyAll(&heap, live);

	// mutating a read-only heap must fail, not crash

	io::FileHeapPtr ptr = heap.allocate(100);
	TEST_ASSERT(!ptr.m_p);

	heap.close();
}

// every heap operation keeps a view mapped across the other views it takes. with the
// default 64K read-ahead those views overlap, so the reliance never shows. running at
// setup(MinDynamicViewCount, 0) removes both cushions: each block gets a view ending
// at its own page, and the LRU evicts between them

void
test_SmallViews() {
	enum {
		IterationCount = 4000,
		MaxLiveTarget  = 200,
	};

	sl::String fileName = io::createTempFile();
	TEST_ASSERT(!fileName.isEmpty());
	io::AutoDeleteFile autoDelete(fileName);

	uint32_t seed = 0x0badc0de;
	uint8_t fillCounter = 1;
	sl::Array<Alloc> live;

	io::FileHeap heap;
	heap.setup(io::FileHeap::MinDynamicViewCount, 0);

	bool result = heap.open(fileName);
	TEST_ASSERT(result);

	churn(&heap, &live, &seed, &fillCounter, IterationCount, MaxLiveTarget);

	// load() walks the physical chain, so a header written through an evicted or
	// too-short view surfaces here as a broken chain, not merely as lost data

	heap.close();
	result = heap.open(fileName);
	TEST_ASSERT(result);
	verifyAll(&heap, live);

	churn(&heap, &live, &seed, &fillCounter, IterationCount, MaxLiveTarget);

	while (!live.isEmpty())
		freeOne(&heap, &live, &seed);

	TEST_ASSERT(heap.getBlockCount() == 1);
	heap.close();
}

// allocate() splits a block by writing the leftover header immediately past the region
// it mapped. size the request so the block ends exactly on a page boundary: without
// read-ahead the view stops there and the leftover lands outside it

void
test_PageBoundarySplit() {
	enum {
		SeedBlockCount = 4,
		SeedBlockSize  = 128 * 1024, // >64K, so seed views never contain one another
	};

	sl::String fileName = io::createTempFile();
	TEST_ASSERT(!fileName.isEmpty());
	io::AutoDeleteFile autoDelete(fileName);

	sl::Array<Alloc> live;

	io::FileHeap heap;
	heap.setup(io::FileHeap::MinDynamicViewCount, 0);

	bool result = heap.open(fileName);
	TEST_ASSERT(result);

	for (size_t i = 0; i < SeedBlockCount; i++) {
		Alloc alloc;
		alloc.m_size = SeedBlockSize;
		alloc.m_fill = (uint8_t)(i + 1);

		io::FileHeapPtr ptr = heap.allocate(alloc.m_size);
		TEST_ASSERT(ptr.m_p);

		alloc.m_offset = ptr.m_offset;
		writePattern(ptr.m_p, alloc.m_size, alloc.m_fill);
		live.append(alloc);
	}

	// nothing was freed, so the only free block is the tail, right past the last one

	const Alloc& last = live[live.getCount() - 1];
	io::FileHeapPtr lastPtr = heap.materialize(last.m_offset);
	TEST_ASSERT(lastPtr.m_p);

	uint64_t tailOffset = last.m_offset - sizeof(io::FileHeapBlock) + lastPtr.m_size;

	// a full page out, so the leftover is always large enough to be split off

	size_t pageSize = g::getModule()->getSystemInfo()->m_pageSize;
	uint64_t boundary = sl::align(tailOffset + pageSize, pageSize);
	size_t blockSize = (size_t)(boundary - tailOffset);

	// flush the LRU, so the allocation below maps a fresh view of exactly its own
	// size instead of being served from one of the seed views

	for (size_t i = 0; i < io::FileHeap::MinDynamicViewCount; i++)
		TEST_ASSERT(heap.materialize(live[i].m_offset).m_p);

	io::FileHeapPtr ptr = heap.allocate(blockSize - sizeof(io::FileHeapBlock));
	TEST_ASSERT(ptr.m_p);
	TEST_ASSERT(ptr.m_offset == tailOffset + sizeof(io::FileHeapBlock)); // took the tail
	TEST_ASSERT(ptr.m_offset - sizeof(io::FileHeapBlock) + ptr.m_size == boundary);

	writePattern(ptr.m_p, blockSize - sizeof(io::FileHeapBlock), 0xc3);

	// the leftover header must have landed: it has to be allocatable, and the chain
	// has to still walk on reopen

	io::FileHeapPtr leftover = heap.allocate(64);
	TEST_ASSERT(leftover.m_p);
	TEST_ASSERT(leftover.m_offset == boundary + sizeof(io::FileHeapBlock));

	heap.close();
	result = heap.open(fileName);
	TEST_ASSERT(result);
	verifyAll(&heap, live);
	heap.close();
}

// a permanently mapped block stays valid no matter how many views are taken after it;
// a dynamically mapped one would have been evicted long before

void
test_Permanent() {
	enum {
		IterationCount = 2000,
		MaxLiveTarget  = 100,
		PinnedSize     = 8000, // > one page, so the pin is not absorbed by the header view
	};

	sl::String fileName = io::createTempFile();
	TEST_ASSERT(!fileName.isEmpty());
	io::AutoDeleteFile autoDelete(fileName);

	uint32_t seed = 0xfeedface;
	uint8_t fillCounter = 1;
	uint8_t pinnedFill = 0xa5;
	uint8_t repinnedFill = 0x5a;
	sl::Array<Alloc> live;

	io::FileHeap heap;
	heap.setup(io::FileHeap::MinDynamicViewCount, 0);

	bool result = heap.open(fileName);
	TEST_ASSERT(result);

	// pinned at allocation time

	io::FileHeapPtr pinned = heap.allocate(PinnedSize, true);
	TEST_ASSERT(pinned.m_p);
	writePattern(pinned.m_p, PinnedSize, pinnedFill);

	// pinned after the fact, through materialize()

	io::FileHeapPtr dynamic = heap.allocate(PinnedSize);
	TEST_ASSERT(dynamic.m_p);
	writePattern(dynamic.m_p, PinnedSize, repinnedFill);

	io::FileHeapPtr repinned = heap.materialize(dynamic.m_offset, true);
	TEST_ASSERT(repinned.m_p && repinned.m_offset == dynamic.m_offset);

	churn(&heap, &live, &seed, &fillCounter, IterationCount, MaxLiveTarget);

	// neither block was freed, so both pointers must still be mapped and intact

	checkPattern(pinned.m_p, PinnedSize, pinnedFill);
	checkPattern(repinned.m_p, PinnedSize, repinnedFill);

	heap.close();
}

void
run() {
	test_ReadWrite();
	test_ReadOnly();
	test_SmallViews();
	test_PageBoundarySplit();
	test_Permanent();
}

//..............................................................................

ADD_TEST_CASE("test_FileHeap", run)

} // namespace
