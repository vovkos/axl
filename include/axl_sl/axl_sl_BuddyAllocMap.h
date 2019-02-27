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

#define _AXL_SL_BUDDYALLOCMAP_H

#include "axl_sl_Array.h"
#include "axl_sl_List.h"

namespace axl {
namespace sl {

//..............................................................................

class BuddyAllocMap
{
protected:
	struct Page: ListLink
	{
		size_t m_map;
	};

	// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	class Level
	{
	protected:
		AuxList<Page> m_availablePageList;
		Page* m_firstPage;

	public:
		Page*
		getFirstPage()
		{
			return m_firstPage;
		}

		Page*
		getFirstAvailablePage()
		{
			return *m_availablePageList.getHead();
		}

		void
		format(
			Page* page,
			size_t count
			);

		void
		setPageMap(
			Page* page,
			size_t map
			);

		void
		setBit(
			Page* page,
			size_t bit,
			bool value
			);

		void
		setBitRange(
			Page* page,
			size_t from,
			size_t to,
			bool value
			);
	};

protected:
	Array<Page> m_pageArray;
	Array<Level> m_levelArray;

	size_t m_width;
	size_t m_height;
	size_t m_totalSize;
	size_t m_freeSizeTop; // free size if allocating on top level (max alloc size)
	size_t m_freeSizeBottom; // free size if allocating on bottom level (size 1)
	size_t m_maxAllocSize;

public:
	BuddyAllocMap();

	bool
	create(
		size_t width,
		size_t height
		);

	void
	close();

	void
	clear();

	size_t
	getFreeSizeTop()
	{
		return m_freeSizeTop;
	}

	size_t
	getFreeSizeBottom()
	{
		return m_freeSizeBottom;
	}

	size_t
	getTotalSize()
	{
		return m_totalSize;
	}

	bool
	isEmpty()
	{
		return m_freeSizeBottom == m_totalSize; // can compare either (top or bottom)
	}

	size_t
	allocate(size_t size);

	void
	mark(
		size_t address,
		size_t size
		);

	void
	free(
		size_t address,
		size_t size
		);

	bool
	getBit(size_t address)
	{
		return address < m_totalSize ? getBit(m_pageArray, address) : false;
	}

protected:
	static
	bool
	getBit(
		Page* page,
		size_t bit
		)
	{
		size_t pageIdx = bit / AXL_PTR_BITS;
		size_t mask = (size_t) 1 << (bit & (AXL_PTR_BITS - 1));
		return (page[pageIdx].m_map & mask) != 0;
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
