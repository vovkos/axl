// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_rtl_BuddyAllocMap.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

class CRuntime;

//.............................................................................

class CGcHeap
{
protected:
	struct TRoot
	{
		void* m_p;
		CType* m_pType;

		TRoot ()
		{
		}

		TRoot (
			void* p,
			CType* pType
			)
		{
			m_p = p;
			m_pType = pType;
		}
	};

protected:
	void* m_pHeap;
	size_t m_HeapSize;
	size_t m_BlockSize;
	rtl::CBuddyAllocMap m_Map;
	rtl::CAuxListT <TObject, CObjectHeapLink> m_ObjectList;	
	rtl::CArrayT <TRoot> m_StaticRootArray;
	rtl::CArrayT <TRoot> m_RootArray [2];
	size_t m_CurrentRootArrayIdx;
	void* m_pShadowStack;

public:
	CGcHeap ();
	
	~CGcHeap ()
	{
		Clear ();
	}

	bool
	Create (
		CRuntime* pRuntime,
		size_t BlockSize,
		size_t Width,
		size_t Height
		);

	void 
	Clear ();

	void
	DropStaticRoots ()
	{
		m_StaticRootArray.Clear ();
	}

	void*
	Allocate (CType* pType);

	void
	AddObject (TObject* pObject);

	bool
	RunGc ();

	bool
	ShouldMark (void* p)
	{
		return IsInHeap (p) && !IsMarked (p);
	}

	void 
	MarkValue (
		void* p,
		CType* pType
		);

	void 
	MarkRange (
		void* p,
		size_t Size
		)
	{
		ASSERT (ShouldMark (p));
		m_Map.Mark (GetAddress (p), GetBlockCount (Size));
	}

	void
	AddRoot (
		void* p,
		CType* pType
		)
	{
		m_RootArray [m_CurrentRootArrayIdx].Append (TRoot (p, pType));
	}

protected:
	size_t 
	GetAddress (void* p)
	{
		ASSERT (p >= m_pHeap);	
		return ((char*) p - (char*) m_pHeap) / m_BlockSize;	
	}

	size_t
	GetBlockCount (size_t Size)
	{
		return Size % m_BlockSize ? Size / m_BlockSize + 1 : Size / m_BlockSize;
	}

	bool	
	IsInHeap (void* p)
	{
		return p >= m_pHeap && p < (char*) m_pHeap + m_HeapSize;
	}

	bool
	IsMarked (void* p)
	{	
		return m_Map.GetBit (GetAddress (p));
	}
};

//.............................................................................

} // namespace axl 
} // namespace jnc 
