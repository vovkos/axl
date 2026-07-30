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

// re-map the block by its handle and check its pattern is intact.
//
// the churn helpers below deliberately pass isPinned = false: a pinned block is never
// evicted, which would defeat test_SmallViews and test_PageBoundarySplit -- both exist
// to provoke exactly the eviction the pin suppresses

void
verifyBlock(
	io::FileHeap* heap,
	const Alloc& alloc
) {
	io::FileHeapPtr<> ptr = heap->materialize<void>(alloc.m_offset, false);
	TEST_ASSERT(ptr.p() && ptr.getOffset() == alloc.m_offset);
	TEST_ASSERT(ptr.getSize() >= alloc.m_size); // payload covers the request
	checkPattern(ptr.p(), alloc.m_size, alloc.m_fill);
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
	uint32_t size,
	uint8_t* fillCounter
) {
	Alloc alloc;
	alloc.m_size = size;
	alloc.m_fill = (*fillCounter)++;

	io::FileHeapPtr<> ptr = heap->allocate<void>(alloc.m_size, false);
	TEST_ASSERT(ptr.p() && !ptr.isPinned());
	TEST_ASSERT(ptr.getSize() >= alloc.m_size);

	alloc.m_offset = ptr.getOffset();
	writePattern(ptr.p(), alloc.m_size, alloc.m_fill);
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

// reallocate an existing block in place of allocating a new one -- the payload the old
// and new sizes have in common has to come across intact either way

void
reallocOne(
	io::FileHeap* heap,
	sl::Array<Alloc>* live,
	uint32_t size,
	uint32_t* seed
) {
	size_t index = lcg(seed) % live->getCount();
	Alloc& alloc = live->rwi()[index];

	io::FileHeapPtr<> ptr = heap->reallocate<void>(alloc.m_offset, size, false);
	TEST_ASSERT(ptr.p() && !ptr.isPinned());
	TEST_ASSERT(ptr.getSize() >= size);

	checkPattern(ptr.p(), size < alloc.m_size ? size : alloc.m_size, alloc.m_fill);

	alloc.m_offset = ptr.getOffset();
	alloc.m_size = size;
	writePattern(ptr.p(), size, alloc.m_fill);
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
			allocOne(heap, live, randomSize(seed), fillCounter);
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
	io::FileHeapPtr<> ptr = heap.allocate<void>(2000, false);
	TEST_ASSERT(ptr.p());
	TEST_ASSERT(heap.getHeapSize() == heapSize);
	heap.free(ptr.getOffset());
	TEST_ASSERT(heap.getBlockCount() == 1);

	heap.close();
}

// write some blocks, close, then reopen the file read-only and read them back:
// the read-only path must never write to the file, header included -- load() skips
// both the free index and the top-block recomputation for exactly that reason

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

			io::FileHeapPtr<> ptr = heap.allocate<void>(alloc.m_size, false);
			TEST_ASSERT(ptr.p());

			alloc.m_offset = ptr.getOffset();
			writePattern(ptr.p(), alloc.m_size, alloc.m_fill);
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

	// mutating a read-only heap is a caller error, not a runtime failure -- allocate(),
	// reallocate() and free() all assert on it, so there is nothing to exercise here

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

		io::FileHeapPtr<> ptr = heap.allocate<void>(alloc.m_size, false);
		TEST_ASSERT(ptr.p());

		alloc.m_offset = ptr.getOffset();
		writePattern(ptr.p(), alloc.m_size, alloc.m_fill);
		live.append(alloc);
	}

	// nothing was freed, so the only free block is the tail, right past the last one

	const Alloc& last = live[live.getCount() - 1];
	io::FileHeapPtr<> lastPtr = heap.materialize<void>(last.m_offset, false);
	TEST_ASSERT(lastPtr.p());

	uint64_t tailOffset = last.m_offset + lastPtr.getSize();

	// a full page out, so the leftover is always large enough to be split off

	size_t pageSize = g::getModule()->getSystemInfo()->m_pageSize;
	uint64_t boundary = sl::align(tailOffset + pageSize, pageSize);
	size_t blockSize = (size_t)(boundary - tailOffset);

	// flush the LRU, so the allocation below maps a fresh view of exactly its own
	// size instead of being served from one of the seed views

	for (size_t i = 0; i < io::FileHeap::MinDynamicViewCount; i++)
		TEST_ASSERT(heap.materialize<void>(live[i].m_offset, false).p());

	io::FileHeapPtr<> ptr = heap.allocate<void>(blockSize - sizeof(io::FileHeapBlock), false);
	TEST_ASSERT(ptr.p());
	TEST_ASSERT(ptr.getOffset() == tailOffset + sizeof(io::FileHeapBlock)); // took the tail
	TEST_ASSERT(ptr.getOffset() + ptr.getSize() == boundary);

	writePattern(ptr.p(), blockSize - sizeof(io::FileHeapBlock), 0xc3);

	// the leftover header must have landed: it has to be allocatable, and the chain
	// has to still walk on reopen

	io::FileHeapPtr<> leftover = heap.allocate<void>(64, false);
	TEST_ASSERT(leftover.p());
	TEST_ASSERT(leftover.getOffset() == boundary + sizeof(io::FileHeapBlock));

	heap.close();
	result = heap.open(fileName);
	TEST_ASSERT(result);
	verifyAll(&heap, live);
	heap.close();
}

// a pinned FileHeapPtr -- allocate()/materialize() with the default isPinned -- keeps
// its block mapped no matter how many views are taken afterwards; the isPinned = false
// forms would have been evicted long before. run at setup(MinDynamicViewCount, 0) so
// the LRU is as hostile as it gets

void
test_Pin() {
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
	uint8_t allocFill = 0xa5;
	uint8_t materializeFill = 0x5a;
	sl::Array<Alloc> live;

	io::FileHeap heap;
	heap.setup(io::FileHeap::MinDynamicViewCount, 0);

	bool result = heap.open(fileName);
	TEST_ASSERT(result);

	{
		// pinned at allocation time -- allocate() pins by default

		io::FileHeapPtr<> allocated = heap.allocate(PinnedSize);
		TEST_ASSERT(allocated.p() && allocated.isPinned());
		writePattern(allocated.p(), PinnedSize, allocFill);

		// pinned after the fact: allocate(..., false) hands back no pin, so the block is
		// only held once the materialize() below picks it up

		io::FileHeapPtr<> dynamic = heap.allocate<void>(PinnedSize, false);
		TEST_ASSERT(dynamic.p() && !dynamic.isPinned());
		writePattern(dynamic.p(), PinnedSize, materializeFill);

		io::FileHeapPtr<> materialized = heap.materialize(dynamic.getOffset());
		TEST_ASSERT(materialized.p() && materialized.isPinned());
		TEST_ASSERT(materialized.getOffset() == dynamic.getOffset());

		// a copy pins the same block a second time; a move transfers the pin and
		// leaves the source null

		io::FileHeapPtr<> copied = allocated;
		TEST_ASSERT(copied.isPinned());
		TEST_ASSERT(copied.p() == allocated.p() && copied.getOffset() == allocated.getOffset());

		io::FileHeapPtr<> moved = std::move(copied);
		TEST_ASSERT(moved.isPinned() && moved.p() == allocated.p());
		TEST_ASSERT(copied.isNull() && !copied.isPinned() && copied.getOffset() == (uint64_t)-1);

		churn(&heap, &live, &seed, &fillCounter, IterationCount, MaxLiveTarget);

		// nothing was freed, so every pinned pointer must still be mapped and intact

		checkPattern(allocated.p(), PinnedSize, allocFill);
		checkPattern(moved.p(), PinnedSize, allocFill);
		checkPattern(materialized.p(), PinnedSize, materializeFill);

		// dropping one pin of a doubly-pinned block must not unmap it

		moved.clear();
		TEST_ASSERT(moved.isNull());
		checkPattern(allocated.p(), PinnedSize, allocFill);
	}

	// every pin is gone, but the blocks are still reachable through the heap

	verifyAll(&heap, live);
	heap.close();
}

// the typed forms are the same pointer with a payload type attached: allocate<T>()
// hands back a FileHeapPtr<T> you dereference directly, and the pin travels with it
// across the T -> void and T -> const T conversions

struct Rec {
	uint32_t m_signature;
	uint32_t m_index;
	uint64_t m_payload;
};

void
test_Typed() {
	enum {
		RecCount  = 200,
		Signature = 0x5ec0ffee,
	};

	sl::String fileName = io::createTempFile();
	TEST_ASSERT(!fileName.isEmpty());
	io::AutoDeleteFile autoDelete(fileName);

	sl::Array<uint64_t> offsetArray;

	io::FileHeap heap;
	heap.setup(io::FileHeap::MinDynamicViewCount, 0); // hostile LRU, as in test_Pin

	bool result = heap.open(fileName);
	TEST_ASSERT(result);

	// write through operator -> on a pinned typed pointer

	for (size_t i = 0; i < RecCount; i++) {
		io::FileHeapPtr<Rec> rec = heap.allocate<Rec>(sizeof(Rec));
		TEST_ASSERT(rec.p() && rec.isPinned());
		TEST_ASSERT(rec.getSize() >= sizeof(Rec));

		rec->m_signature = Signature;
		rec->m_index = (uint32_t)i;
		rec->m_payload = i * 0x0101010101010101ull;
		offsetArray.append(rec.getOffset());
	}

	// read them back -- unpinned this time, so the LRU churns between records

	for (size_t i = 0; i < RecCount; i++) {
		io::FileHeapPtr<Rec> rec = heap.materialize<Rec>(offsetArray[i], false);
		TEST_ASSERT(rec.p() && !rec.isPinned());
		TEST_ASSERT(rec->m_signature == Signature);
		TEST_ASSERT(rec->m_index == i);
		TEST_ASSERT(rec->m_payload == i * 0x0101010101010101ull);
	}

	{
		// the pin survives the T -> void and T -> const T conversions, and a pinned
		// record stays readable through every conversion after heavy churn

		io::FileHeapPtr<Rec> rec = heap.materialize<Rec>(offsetArray[0]);
		TEST_ASSERT(rec.isPinned());

		io::FileHeapPtr<void> raw = rec;             // converting copy: Rec* -> void*
		io::ConstFileHeapPtr<Rec> constRec = rec;    // converting copy: Rec* -> const Rec*
		TEST_ASSERT(raw.isPinned() && constRec.isPinned());
		TEST_ASSERT(raw.p() == rec.p() && constRec.p() == rec.p());
		TEST_ASSERT(raw.getOffset() == rec.getOffset() && raw.getSize() == rec.getSize());

		for (size_t i = 1; i < RecCount; i++)
			TEST_ASSERT(heap.materialize<Rec>(offsetArray[i], false).p());

		TEST_ASSERT(rec->m_signature == Signature && rec->m_index == 0);
		TEST_ASSERT(constRec->m_signature == Signature);

		// a converting move leaves the source null and keeps the block pinned

		io::FileHeapPtr<void> movedRaw = std::move(rec);
		TEST_ASSERT(movedRaw.isPinned() && rec.isNull() && !rec.isPinned());
		TEST_ASSERT(constRec->m_index == 0); // still mapped, held by constRec/movedRaw
	}

	heap.close();
}

// reallocate() must preserve the payload across a move, hand back a block that is
// actually big enough, and leave the heap walkable -- checked by reopening at the end,
// which fails on a broken block chain rather than merely on lost data.
//
// it takes a payload handle rather than a FileHeapPtr, so there is no pinned/unpinned
// source distinction to cover: it always materializes internally

void
test_Reallocate() {
	enum {
		StepCount = 64,
		StepSize  = 300, // > Alignment, so most steps really do have to move
	};

	sl::String fileName = io::createTempFile();
	TEST_ASSERT(!fileName.isEmpty());
	io::AutoDeleteFile autoDelete(fileName);

	uint32_t seed = 0x1234abcd;
	uint8_t fillCounter = 1;
	sl::Array<Alloc> live;

	io::FileHeap heap;
	heap.setup(io::FileHeap::MinDynamicViewCount, 0); // hostile LRU, no read-ahead

	bool result = heap.open(fileName);
	TEST_ASSERT(result);

	// a null handle allocates

	io::FileHeapPtr<> grown = heap.reallocate((uint64_t)-1, StepSize);
	TEST_ASSERT(grown.p() && grown.isPinned() && grown.getSize() >= StepSize);
	writePattern(grown.p(), StepSize, 0x11);

	// grow repeatedly, interleaved with churn so the block is rarely the last one and
	// the copy has to survive an LRU that evicts between the two views.
	//
	// writePattern() lays down (fill + i), so appending at `size` with a base of
	// (0x11 + size) keeps one continuous run over the whole payload

	size_t size = StepSize;
	for (size_t i = 0; i < StepCount; i++) {
		churn(&heap, &live, &seed, &fillCounter, 8, 32);

		size_t newSize = size + StepSize;
		io::FileHeapPtr<> next = heap.reallocate(grown.getOffset(), newSize);
		TEST_ASSERT(next.p() && next.isPinned());
		TEST_ASSERT(next.getSize() >= newSize);

		checkPattern(next.p(), size, 0x11); // everything written so far survived
		writePattern((char*)next.p() + size, StepSize, (uint8_t)(0x11 + size));

		grown = std::move(next);
		size = newSize;
	}

	checkPattern(grown.p(), size, 0x11); // the whole accumulated payload

	// every block must stay 8-byte aligned: an unaligned m_size would shift every
	// later block in the chain and only surface as corruption on reopen

	TEST_ASSERT(grown.getOffset() % io::FileHeap::Alignment == 0);
	TEST_ASSERT((grown.getOffset() + grown.getSize()) % io::FileHeap::Alignment == 0);

	// a request that already fits keeps the same block

	uint64_t offset = grown.getOffset();
	io::FileHeapPtr<> same = heap.reallocate(offset, StepSize);
	TEST_ASSERT(same.p() && same.getOffset() == offset);
	checkPattern(same.p(), StepSize, 0x11);

	// honoring isPinned = false

	io::FileHeapPtr<> unpinned = heap.reallocate(offset, StepSize, false);
	TEST_ASSERT(unpinned.p() && !unpinned.isPinned());

	same.clear();
	unpinned.clear();
	grown.clear();

	// the chain must still walk after all that reallocation

	verifyAll(&heap, live);
	heap.close();
	result = heap.open(fileName);
	TEST_ASSERT(result);
	verifyAll(&heap, live);
	heap.close();
}

// MinDynamicViewCount is 3 because allocate(), reallocate() and free() each need three
// views live at once -- but all three take a *fourth* view along the way, and only get
// away with it because the one the LRU drops has already gone dead: free() copies its
// block's size and prev-size into locals before viewing the prev block, and both
// allocate() and reallocate() finish writing the block header before createFreeBlock()
// views the successor of the leftover.
//
// none of that shows unless all four views are separate entries. a view is based on the
// system allocation granule, so anything smaller shares a view with its neighbours and
// the eviction simply never happens -- which is why the two tests below allocate
// granule-sized blocks with the read-ahead turned off

size_t
getViewGranule() {
	return g::getModule()->getSystemInfo()->m_mappingAlignFactor;
}

// churn at granule size: allocate() splitting a free block that is not the last one and
// free() merging in both directions both run with every view distinct and the LRU at its
// minimum

void
test_ViewPressure() {
	enum {
		IterationCount = 400,
		MaxLiveTarget  = 24,
	};

	sl::String fileName = io::createTempFile();
	TEST_ASSERT(!fileName.isEmpty());
	io::AutoDeleteFile autoDelete(fileName);

	size_t granule = getViewGranule();

	uint32_t seed = 0x5a17ed00;
	uint8_t fillCounter = 1;
	sl::Array<Alloc> live;

	io::FileHeap heap;
	heap.setup(io::FileHeap::MinDynamicViewCount, 0);

	bool result = heap.open(fileName);
	TEST_ASSERT(result);

	for (size_t i = 0; i < IterationCount; i++) {
		uint32_t size = (uint32_t)(granule + lcg(&seed) % granule);

		if (live.isEmpty())
			allocOne(&heap, &live, size, &fillCounter);
		else switch (lcg(&seed) % 4) {
		case 0:
		case 1:
			// reallocate() is the one that takes four views: it merges the free
			// successor in, then splits the leftover back off and has to reach the
			// block past it

			reallocOne(&heap, &live, size, &seed);
			break;

		case 2:
			if (live.getCount() < MaxLiveTarget) {
				allocOne(&heap, &live, size, &fillCounter);
				break;
			}

			// and fall through

		default:
			freeOne(&heap, &live, &seed);
		}

		if ((i & 0x1f) == 0)
			verifyAll(&heap, live);
	}

	verifyAll(&heap, live);

	// a header written through an evicted view breaks load(), not merely the data

	heap.close();
	result = heap.open(fileName);
	TEST_ASSERT(result);
	verifyAll(&heap, live);
	heap.close();
}

// the same two paths built by hand rather than hoped for -- churn covers them only by
// luck, and never the reallocate() one, which needs a free successor to absorb

void
test_MergeViews() {
	enum {
		BlockCount = 8,
	};

	sl::String fileName = io::createTempFile();
	TEST_ASSERT(!fileName.isEmpty());
	io::AutoDeleteFile autoDelete(fileName);

	size_t granule = getViewGranule();

	io::FileHeap heap;
	heap.setup(io::FileHeap::MinDynamicViewCount, 0);

	bool result = heap.open(fileName);
	TEST_ASSERT(result);

	// a fresh heap splits blocks off the front, so these come out physically adjacent --
	// asserted below, since both layouts rest on it. [0..3] go to free(), [4..7] to
	// reallocate(); [5] is double-sized so absorbing it still leaves a leftover to split

	uint64_t offsets[BlockCount];
	uint32_t sizes[BlockCount];

	for (size_t i = 0; i < BlockCount; i++) {
		sizes[i] = (uint32_t)(i == 5 ? granule * 2 : granule);

		io::FileHeapPtr<> ptr = heap.allocate<void>(sizes[i], false);
		TEST_ASSERT(ptr.p() && ptr.getSize() == sizes[i]); // exact: the leftover always splits

		offsets[i] = ptr.getOffset();
		writePattern(ptr.p(), sizes[i], (uint8_t)(0x40 + i));

		if (i)
			TEST_ASSERT(offsets[i] == offsets[i - 1] + sizes[i - 1] + sizeof(io::FileHeapBlock));
	}

	// free(): with [0] and [2] already free, freeing [1] merges in both directions and
	// still has to reach [3], the block past the successor it swallows. four views, and
	// [1]'s own is the one that dies

	TEST_ASSERT(heap.free(offsets[0])); // heap start before it, allocated block after
	TEST_ASSERT(heap.free(offsets[2])); // both neighbours allocated
	TEST_ASSERT(heap.free(offsets[1])); // merges both ways

	io::FileHeapPtr<> survivor = heap.materialize<void>(offsets[3], false);
	TEST_ASSERT(survivor.p());
	checkPattern(survivor.p(), sizes[3], 0x43);

	// the three must have collapsed into exactly one block -- two headers absorbed. it is
	// the smallest free block that fits, so allocate() has to hand back this one

	uint32_t mergedSize =
		sizes[0] + sizes[1] + sizes[2] +
		(uint32_t)sizeof(io::FileHeapBlock) * 2;

	io::FileHeapPtr<> merged = heap.allocate<void>(mergedSize, false);
	TEST_ASSERT(merged.p() && merged.getOffset() == offsets[0]);
	TEST_ASSERT(merged.getSize() == mergedSize); // an exact fit -- nothing left to split

	// reallocate(): free [5] so [4] can absorb it, then grow [4] over both. the request
	// leaves a granule of leftover whose successor is [6] -- the fourth view -- and [6]
	// is not the last block, so createFreeBlock() really does have to update it

	TEST_ASSERT(heap.free(offsets[5]));

	uint32_t newSize = sizes[4] + sizes[5] / 2;

	io::FileHeapPtr<> grown = heap.reallocate(offsets[4], newSize);
	TEST_ASSERT(grown.p() && grown.getOffset() == offsets[4]); // absorbed [5] in place
	TEST_ASSERT(grown.getSize() == newSize);
	checkPattern(grown.p(), sizes[4], 0x44); // [4]'s payload survived the merge

	io::FileHeapPtr<> next = heap.materialize<void>(offsets[6], false);
	TEST_ASSERT(next.p());
	checkPattern(next.p(), sizes[6], 0x46);

	io::FileHeapPtr<> last = heap.materialize<void>(offsets[7], false);
	TEST_ASSERT(last.p());
	checkPattern(last.p(), sizes[7], 0x47);

	grown.clear(); // close() needs every pin released

	// and the chain still has to walk

	heap.close();
	result = heap.open(fileName);
	TEST_ASSERT(result);

	io::FileHeapPtr<> reloaded = heap.materialize<void>(offsets[6], false);
	TEST_ASSERT(reloaded.p());
	checkPattern(reloaded.p(), sizes[6], 0x46);

	reloaded = heap.materialize<void>(offsets[7], false);
	TEST_ASSERT(reloaded.p());
	checkPattern(reloaded.p(), sizes[7], 0x47);
	heap.close();
}

//..............................................................................

void
run() {
	test_ReadWrite();
	test_ReadOnly();
	test_SmallViews();
	test_PageBoundarySplit();
	test_Pin();
	test_Typed();
	test_Reallocate();
	test_ViewPressure();
	test_MergeViews();
}

//..............................................................................

ADD_TEST_CASE("test_FileHeap", run)

} // namespace
