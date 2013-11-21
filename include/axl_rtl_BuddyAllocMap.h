// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_BUDDYALLOCMAP_H

#include "axl_rtl_Array.h"
#include "axl_rtl_List.h"

namespace axl {
namespace rtl {

//.............................................................................

class CBuddyAllocMap
{
protected:
	struct TPage: TListLink
	{
		size_t m_Map;
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	class CLevel
	{
	protected:
		CAuxListT <TPage> m_AvailablePageList;
		TPage* m_pFirstPage;

	public:
		TPage*
		GetFirstPage ()
		{
			return m_pFirstPage;
		}

		TPage*
		GetFirstAvailablePage ()
		{
			return *m_AvailablePageList.GetHead ();
		}

		void
		Format (
			TPage* pPage,
			size_t Count
			);

		void
		SetPageMap (
			TPage* pPage,
			size_t Map
			);

		void
		SetBit (
			TPage* pPage,
			size_t Bit,
			bool Value
			);

		void
		SetBitRange (
			TPage* pPage,
			size_t From,
			size_t To,
			bool Value
			);
	};

protected:
	CArrayT <TPage> m_PageArray;
	CArrayT <CLevel> m_LevelArray;

	size_t m_Width;
	size_t m_Height;
	size_t m_TotalSize;
	size_t m_FreeSizeTop; // free size if allocating on top level (max alloc size)
	size_t m_FreeSizeBottom; // free size if allocating on bottom level (size 1)
	size_t m_MaxAllocSize;

public:
	CBuddyAllocMap ();

	bool
	Create (
		size_t Width,
		size_t Height
		);

	void
	Close ();

	void
	Clear ();

	size_t
	GetFreeSizeTop ()
	{
		return m_FreeSizeTop;
	}

	size_t
	GetFreeSizeBottom ()
	{
		return m_FreeSizeBottom;
	}

	size_t
	GetTotalSize ()
	{
		return m_TotalSize;
	}

	bool
	IsEmpty ()
	{
		return m_FreeSizeBottom == m_TotalSize; // can compare either (top or bottom)
	}

	size_t
	Allocate (size_t Size);

	void
	Mark (
		size_t Address,
		size_t Size
		);

	void
	Free (
		size_t Address,
		size_t Size
		);

	bool
	GetBit (size_t Address)
	{
		return Address < m_TotalSize ? GetBit (m_PageArray, Address) : false;
	}

protected:
	static
	bool
	GetBit (
		TPage* pPage,
		size_t Bit
		)
	{
		size_t Page = Bit / _AXL_PTR_BITNESS;
		size_t Mask = (size_t) 1 << (Bit & (_AXL_PTR_BITNESS - 1));
		return (pPage [Page].m_Map & Mask) != 0;
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

