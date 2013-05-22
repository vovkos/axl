#pragma once

#include "axl_jnc_Value.h"
#include "axl_rtl_HandleTable.h"

namespace axl {
namespace jnc {

//.............................................................................

class CMulticast: public jnc::TMulticast
{
public:
	void
	Clear ();

	handle_t
	SetHandler (jnc::TFunctionPtr Ptr);

	handle_t
	SetHandler_w (jnc::TFunctionPtr_w Ptr);

	handle_t
	SetHandler_u (void* pf);

	handle_t
	AddHandler (jnc::TFunctionPtr Ptr)
	{
		return Ptr.m_pf ? AddHandlerImpl (Ptr) : NULL;
	}

	handle_t
	AddHandler_w (jnc::TFunctionPtr_w Ptr)
	{
		return Ptr.m_pf ? AddHandlerImpl (Ptr) : NULL;
	}

	handle_t
	AddHandler_u (void* pf)
	{
		return pf ? AddHandlerImpl (pf) : NULL;
	}

	jnc::TFunctionPtr
	RemoveHandler (handle_t Handle)
	{
		return RemoveHandlerImpl <jnc::TFunctionPtr> (Handle);
	}

	jnc::TFunctionPtr_w 
	RemoveHandler_w (handle_t Handle)
	{
		return RemoveHandlerImpl <jnc::TFunctionPtr_w> (Handle);
	}

	void* 
	RemoveHandler_u (handle_t Handle)
	{
		return RemoveHandlerImpl <void*> (Handle);
	}

	jnc::TMcSnapshot
	Snapshot ()
	{
		return SnapshotImpl <jnc::TFunctionPtr> ();
	}

	jnc::TMcSnapshot
	Snapshot_w ()
	{
		return SnapshotImpl <jnc::TFunctionPtr_w> ();
	}

	jnc::TMcSnapshot
	Snapshot_u ()
	{
		return SnapshotImpl <void*> ();
	}

protected:
	rtl::CHandleTableT <size_t>*
	GetHandleTable ();

	bool
	SetCount (
		size_t Count,
		size_t PtrSize
		);

	template <typename T>
	handle_t
	SetHandlerImpl (T Ptr)
	{
		SetCount (1, sizeof (T));
		*(T*) m_pPtrArray = Ptr;
		rtl::CHandleTableT <size_t>* pHandleTable = GetHandleTable ();
		pHandleTable->Clear ();
		return pHandleTable->Add (0);
	}

	template <typename T>
	handle_t
	AddHandlerImpl (T Ptr)
	{
		size_t i = m_Count;
		SetCount (i + 1, sizeof (T));
		*((T*) m_pPtrArray + i) = Ptr;
		return GetHandleTable ()->Add (i);
	}

	template <typename T>
	T
	RemoveHandlerImpl (handle_t Handle)
	{
		T Ptr = { 0 };

		if (!m_pHandleTable)
			return Ptr;

		rtl::CHandleTableT <size_t>* pHandleTable = (rtl::CHandleTableT <size_t>*) m_pHandleTable;
		rtl::CHandleTableT <size_t>::CMapIterator MapIt = pHandleTable->Find (Handle);
		if (!MapIt)
			return Ptr;
	
		rtl::CHandleTableT <size_t>::CListIterator ListIt = MapIt->m_Value;	

		size_t i = ListIt->m_Value;
		ASSERT (i < m_Count);

		Ptr = *((T*) m_pPtrArray + i);

		memmove (((T*) m_pPtrArray + i), ((T*) m_pPtrArray + i + 1),  (m_Count - i) * sizeof (T));
		m_Count--;

		for (ListIt++; ListIt; ListIt++) 
			ListIt->m_Value--;

		pHandleTable->Remove (MapIt);
		return Ptr;
	}

	template <typename T>
	jnc::TMcSnapshot
	SnapshotImpl ()
	{
		jnc::TMcSnapshot Snapshot = {0};
		Snapshot.m_Count = m_Count;
		Snapshot.m_pPtrArray = m_pPtrArray;
		return Snapshot;
	}

};

//.............................................................................

} // namespace axl
} // namespace jnc
