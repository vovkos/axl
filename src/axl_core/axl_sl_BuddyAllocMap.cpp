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
#include "axl_sl_BuddyAllocMap.h"
#include "axl_sl_BitIdx.h"
#include "axl_err_Error.h"

namespace axl {
namespace sl {

//..............................................................................

void
BuddyAllocMap::Level::format(
	Page* page,
	size_t count
	)
{
	size_t i;

	m_availablePageList.clear();
	m_firstPage = page;

	ASSERT(count);
	for (i = 0; i < count; i++, page++)
	{
		m_availablePageList.insertTail(page);
		page->m_map = 0;
	}
}

void
BuddyAllocMap::Level::setPageMap(
	Page* page,
	size_t map
	)
{
	if (map == -1 && page->m_map != -1)
		m_availablePageList.remove(page);
	else if (map != -1 && page->m_map == -1)
		m_availablePageList.insertTail(page);

	page->m_map = map;
}

void
BuddyAllocMap::Level::setBit(
	Page* page,
	size_t bit,
	bool value
	)
{
	size_t mask;

	page += bit / AXL_PTR_BITS;
	bit &= AXL_PTR_BITS - 1;
	mask = (size_t) 1 << bit;

	if (value)
	{
		ASSERT((page->m_map & mask) == 0);
		setPageMap(page, page->m_map | mask);
	}
	else
	{
		ASSERT((page->m_map & mask) == mask);
		setPageMap(page, page->m_map & ~mask);
	}
}

void
BuddyAllocMap::Level::setBitRange(
	Page* page,
	size_t from,
	size_t to,
	bool value
	)
{
	size_t pageIdx = from / AXL_PTR_BITS;
	size_t mask;

	page += pageIdx;
	from -= pageIdx * AXL_PTR_BITS;
	to -= pageIdx * AXL_PTR_BITS;

	if (value)
	{
		if (to < AXL_PTR_BITS)
		{
			mask = getBitmask(from, to);
			// ASSERT ((pPage->m_Map & Mask) == 0);
			setPageMap(page, page->m_map | mask);
			return;
		}

		mask = getHiBitmask(from);
		// ASSERT ((pPage->m_Map & Mask) == 0);
		setPageMap(page, page->m_map | mask);

		to -= AXL_PTR_BITS;
		page++;

		while (to >= AXL_PTR_BITS)
		{
			// ASSERT (pPage->m_Map == 0);
			page->m_map = -1;
			to -= AXL_PTR_BITS;
			page++;
		}

		if (to)
		{
			mask = getLoBitmask(to);
			// ASSERT ((pPage->m_Map & Mask) == 0);
			setPageMap(page, page->m_map | mask);
		}
	}
	else
	{
		if (to < AXL_PTR_BITS)
		{
			mask = getBitmask(from, to);
			// ASSERT ((pPage->m_Map & Mask) == Mask);
			setPageMap(page, page->m_map & ~mask);
			return;
		}

		mask = getHiBitmask(from);
		// ASSERT ((pPage->m_Map & Mask) == Mask);
		setPageMap(page, page->m_map & ~mask);

		to -= AXL_PTR_BITS;
		page++;

		while (to >= AXL_PTR_BITS)
		{
			// ASSERT (pPage->m_Map == -1);
			page->m_map = 0;
			to -= AXL_PTR_BITS;
			page++;
		}

		if (to)
		{
			mask = getLoBitmask(to);
			// ASSERT ((pPage->m_Map & Mask) == Mask);
			setPageMap(page, page->m_map & ~mask);
		}
	}
}

//..............................................................................

BuddyAllocMap::BuddyAllocMap()
{
	m_width = 0;
	m_height = 0;
	m_totalSize = 0;
	m_freeSizeTop = 0;
	m_freeSizeBottom = 0;
	m_maxAllocSize = 0;
}

bool
BuddyAllocMap::create(
	size_t width,
	size_t height
	)
{
	bool_t result;

	size_t totalPageCount;
	size_t pageCount;

	Level* level;
	Level* end;

	Page* page;

	close();

	if (!width)
		width = 1;

	if (!height)
		height = 1;

	totalPageCount = ((1 << height) - 1) * width;

	result = m_pageArray.setCount(totalPageCount) && m_levelArray.setCount(height);
	if (!result)
	{
		close();
		return false;
	}

	page = m_pageArray;
	level = m_levelArray;
	end = level + height;

	pageCount = width << (height - 1); // on the bottom level

	m_width = width;
	m_height = height;
	m_totalSize = pageCount * AXL_PTR_BITS;
	m_freeSizeTop = m_totalSize;
	m_freeSizeBottom = m_totalSize;
	m_maxAllocSize = (size_t) 1 << (height - 1);

	for (; level < end; level++)
	{
		level->format(page, pageCount);

		page += pageCount;
		pageCount /= 2;
	}

	return true;
}

void
BuddyAllocMap::close()
{
	m_pageArray.clear();
	m_levelArray.clear();

	m_maxAllocSize = 0;
	m_totalSize = 0;
	m_freeSizeTop = 0;
	m_freeSizeBottom = 0;
}

void
BuddyAllocMap::clear()
{
	Page* page = m_pageArray;
	Level* level = m_levelArray;
	Level* end = level + m_height;

	size_t pageCount = m_width << (m_height - 1); // on the bottom level

	for (; level < end; level++)
	{
		level->format(page, pageCount);

		page += pageCount;
		pageCount /= 2;
	}

	m_freeSizeTop = m_totalSize;
	m_freeSizeBottom = m_totalSize;
}

size_t
BuddyAllocMap::allocate(size_t size)
{
	Level* level;
	Page* page;

	size_t pageCount;
	size_t bitSize;
	size_t bitIdx;
	size_t address;
	size_t addressEnd;
	size_t i;

	size_t levelIdx;

	if (size > m_maxAllocSize)
	{
		err::setError(err::SystemErrorCode_InvalidParameter);
		return -1;
	}

	levelIdx = getHiBitIdx(size);

	if (levelIdx && (size & ((1 << levelIdx) - 1)))
		levelIdx++;

	ASSERT(levelIdx < m_height);
	level = m_levelArray + levelIdx;

	page = level->getFirstAvailablePage();
	if (!page)
	{
		err::setError(err::SystemErrorCode_InsufficientResources);
		return -1;
	}

	ASSERT(page->m_map != -1);

	bitIdx = getHiBitIdx(((page->m_map + 1) | page->m_map) ^ page->m_map);
	bitSize = (size_t) 1 << levelIdx;

	size_t pageIdx = page - level->getFirstPage();

	address = (pageIdx * AXL_PTR_BITS + bitIdx) * bitSize;
	addressEnd = address + size;

	m_freeSizeBottom -= size;

	// mark starting with the bottom level (level 0)

	level = m_levelArray;
	page = m_pageArray;
	pageCount = m_width << (m_height - 1); // on the bottom level
	bitSize = 1;

	// below the allocation level we mark ranges of clusters

	for (i = 0; i < levelIdx; i++)
	{
		size_t from = address / bitSize;
		size_t to = addressEnd / bitSize;

		ASSERT(!(address % bitSize)); // beacuse we below the allocation level

		if (addressEnd % bitSize)
			to++;

		level->setBitRange(page, from, to, true);

		level++;
		page += pageCount;
		pageCount /= 2;
		bitSize *= 2;
	}

	// from the allocation level and above we only mark one cluster, also we can stop as soon as we find a marked cluster

	for (; i < m_height; i++)
	{
		size_t bit = address / bitSize;

		if (getBit(page, bit))
			break;

		level->setBit(page, bit, true);

		level++;
		page += pageCount;
		pageCount /= 2;
		bitSize *= 2;
	}


	if (i == m_height) // we marked all the way to the top, adjust top level free size
		m_freeSizeTop -= (size_t) 1 << (m_height - 1);

	return address;
}

void
BuddyAllocMap::mark(
	size_t address,
	size_t size
	)
{
	Level* level = m_levelArray;
	Page* page = m_pageArray;

	size_t pageCount = m_width << (m_height - 1); // on the bottom level
	size_t bitSize = 1;
	size_t addressEnd = address + size;
	size_t i;

	size_t levelIdx = getHiBitIdx(size);

	if (levelIdx && (size & ((1 << levelIdx) - 1)))
		levelIdx++;

	m_freeSizeBottom -= size;

	// below the allocation level we mark ranges of clusters

	for (i = 0; i < levelIdx; i++)
	{
		size_t from = address / bitSize;
		size_t to = addressEnd / bitSize;

		ASSERT(!(address % bitSize)); // beacuse we below the allocation level

		if (addressEnd % bitSize)
			to++;

		level->setBitRange(page, from, to, true);

		level++;
		page += pageCount;
		pageCount /= 2;
		bitSize *= 2;
	}

	// from the allocation level and above we only mark one cluster, also we can stop as soon as we find a marked cluster

	for (; i < m_height; i++)
	{
		size_t bit = address / bitSize;

		if (getBit(page, bit))
			break;

		level->setBit(page, bit, true);

		level++;
		page += pageCount;
		pageCount /= 2;
		bitSize *= 2;
	}

	if (i == m_height) // we marked all the way to the top, adjust top level free size
		m_freeSizeTop -= (size_t) 1 << (m_height - 1);
}

void
BuddyAllocMap::free(
	size_t address,
	size_t size
	)
{
	Level* level = m_levelArray;
	Page* page = m_pageArray;
	Page* prevPage = NULL;

	size_t pageCount = m_width << (m_height - 1); // on the bottom level
	size_t bitSize = 1;
	size_t bit;
	size_t prevBit;
	size_t addressEnd = address + size;
	size_t i;

	bool_t isBlocked = false;

	size_t levelIdx = getHiBitIdx(size);

	if (levelIdx && (size & ((1 << levelIdx) - 1)))
		levelIdx++;

	m_freeSizeBottom += size;

	// below the allocation level we unmark ranges of clusters

	for (i = 0; i < levelIdx; i++)
	{
		size_t from = address / bitSize;
		size_t to = addressEnd / bitSize;
		size_t mod = addressEnd % bitSize;

		ASSERT(!(address % bitSize)); // beacuse we below the allocation level

		if (mod && !isBlocked)
		{
			ASSERT(prevPage != NULL);

			if (getBit(prevPage, prevBit))
			{
				isBlocked = true;
			}
			else
			{
				addressEnd += bitSize - mod;
				to++;
			}
		}

		level->setBitRange(page, from, to, false);

		level++;
		prevPage = page;
		prevBit = to;
		page += pageCount;
		pageCount /= 2;
		bitSize *= 2;
	}

	if (isBlocked)
		return;

	// from the allocation level and above we only unmark one cluster, and only if both children are free

	prevBit--; // in the next cycle PrevBit has meaning of the last cleared cluster

	for (; i < m_height; i++)
	{
		// check adjacent cluster

		if (prevPage)
		{
			if (prevBit & 1)
			{
				if (getBit(prevPage, prevBit - 1))
					break;
			}
			else
			{
				if (getBit(prevPage, prevBit + 1))
					break;
			}
		}

		bit = address / bitSize;
		level->setBit(page, bit, false);

		level++;
		prevPage = page;
		prevBit = bit;
		page += pageCount;
		pageCount /= 2;
		bitSize *= 2;
	}

	if (i == m_height) // we marked all the way to the top, adjust top level free size
		m_freeSizeTop += (size_t) 1 << (m_height - 1);
}

//..............................................................................

} // namespace sl
} // namespace axl
