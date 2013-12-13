// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_ref_RefCount.h"
#include "axl_rtl_List.h"
#include "axl_rtl_Array.h"
#include "axl_rtl_BitMap.h"
#include "axl_rtl_Singleton.h"

namespace axl {
namespace jnc {

class CRuntime;
class CTlsMgr;
struct TTlsPage;
struct TGcShadowStackFrame;

//.............................................................................

struct TTlsData: public rtl::TListLink
{
	CRuntime* m_pRuntime;
	void* m_pStackEpoch;

	// followed by TLS data
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TTls
{
	size_t m_ScopeLevel;
	TGcShadowStackFrame* m_pGcShadowStackTop;

	// followed by user TLS variables
};

//.............................................................................

class CTlsDirectory: public ref::CRefCount
{
protected:
	rtl::CArrayT <TTlsData*> m_Table;

public:
	~CTlsDirectory ();

	TTlsData*
	FindTlsData (CRuntime* pRuntime);

	TTlsData*
	GetTlsData (CRuntime* pRuntime);

	TTlsData*
	NullifyTlsData (CRuntime* pRuntime);
};

//.............................................................................

class CTlsMgr
{
protected:
	uint64_t m_MainThreadId;
	size_t m_TlsSlot;
	rtl::CBitMap m_SlotMap;
	size_t m_SlotCount;
	
public:
	CTlsMgr ();

	// CreateSlot / DestroySlot should only be called from the main thread;
	// all threads containing tls pages on particular slot (n) should be terminated by 
	// the moment of calling DestroySlot (n)

	size_t 
	CreateSlot ();

	void
	DestroySlot (size_t Slot);

	TTlsData*
	FindTlsData (CRuntime* pRuntime);

	TTlsData*
	GetTlsData (CRuntime* pRuntime);

	TTlsData*
	NullifyTlsData (CRuntime* pRuntime);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
CTlsMgr*
GetTlsMgr ()
{
	return rtl::GetSingleton <CTlsMgr> ();
}

//.............................................................................

} // namespace axl 
} // namespace jnc 
