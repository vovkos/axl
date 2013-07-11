// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_mem_Block.h"
#include "axl_ref_RefCount.h"
#include "axl_rtl_BitMap.h"

namespace axl {
namespace jnc {

class CRuntime;
class CTlsMgr;
struct TTlsPage;

//.............................................................................

class CTlsMgr
{
protected:
	class CDirectory: public ref::IRefCount
	{
	public:
		rtl::CArrayT <mem::TBlock> m_Table;

		~CDirectory ();
	};

protected:
	uint_t m_MainThreadId;
	size_t m_TlsSlot;
	rtl::CBitMap m_SlotMap;
	size_t m_SlotCount;
	
public:
	CTlsMgr ();

	// CreatePageSlot / DestroyPageSlot should only be called from the main thread;
	// all threads containing tls pages on particular slot (n) should be terminated by 
	// the moment of calling DestroyPageSlot (n)

	size_t 
	CreateSlot ();

	void
	DestroySlot (size_t Slot);

	void*
	GetThreadVariableData (
		size_t Slot,
		size_t Size
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
CTlsMgr*
GetTlsMgr ()
{
	return rtl::GetSimpleSingleton <CTlsMgr> ();
}

//.............................................................................

} // namespace axl 
} // namespace jnc 
