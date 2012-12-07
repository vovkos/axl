// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_BUF_H

#include "axl_ref_Factory.h"
#include "axl_rtl_Func.h"
#include "axl_rtl_BitIdx.h"

namespace axl {
namespace ref {

//.............................................................................

// ref-counted buffer for variable-sized objects 
// could be also used for fixed-sized object as single-writer multiple reader accessor
// can use external buffer (e.g. static or allocated on stack)

enum EBuf
{
	EBuf_Private, // buffer is stack-allocated or an object field, cannot be shared
	EBuf_Static,  // buffer is static or global, can be shared
	EBuf_Stack = EBuf_Private, 
	EBuf_Field = EBuf_Private, 
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TGetSize = rtl::CGetSizeT <T>
	>
class CBufT
{
public:
	class CHdr: public IRefCount
	{
	public:
		size_t m_BufferSize;

		~CHdr ()
		{
			((T*) (this + 1))->~T ();
		}
	};

	enum
	{
		MinBufSize = sizeof (CHdr) + sizeof (T)
	};

protected:
	T* m_p;

public:
	CBufT ()
	{
		m_p = NULL;
	}

	CBufT (const CBufT& Src)
	{
		m_p = NULL;
		Copy (Src);
	}

	CBufT (const T& Src)
	{
		m_p = NULL;
		Copy (Src);
	}
	
	CBufT (
		EBuf Kind,
		void* p, 
		size_t Size
		)
	{
		m_p = NULL;
		SetBuffer (Kind, p, Size);
	}

	~CBufT ()
	{
		Release ();
	}

	operator const T* () const
	{ 
		return m_p; 
	}

	const T* 
	operator -> () const
	{ 
		ASSERT (m_p);
		return m_p; 
	}

	operator T* ()
	{ 
		return m_p; 
	}

	T* 
	operator -> ()
	{ 
		ASSERT (m_p);
		return m_p; 
	}

	CBufT& 
	operator = (const CBufT& Src)
	{ 
		Copy (Src);
		return *this;
	}

	CBufT& 
	operator = (const T& Src)
	{
		Copy (Src);
		return *this;
	}

	void 
	Release ()
	{
		if (!m_p)
			return;

		GetHdr ()->Release ();
		m_p = NULL;
	}

	T* 
	Create (size_t Size = sizeof (T))
	{
		Release ();
		return AllocateBuffer (Size, NULL);
	}

	bool
	Copy (const CBufT& Src)
	{
		if (m_p == Src.m_p)
			return true;

		if (Src.m_p)
		{
			if (Src.GetHdr ()->GetFree () == (mem::FFree) -1)
				return Copy (*Src);

			Src.GetHdr ()->AddRef ();
		}

		if (m_p)
			GetHdr ()->Release ();

		m_p = Src.m_p;
		return true;
	}

	bool
	Copy (const T& Src)
	{
		return m_p == &Src ? true : AllocateBuffer (TGetSize () (Src), &Src) != NULL;
	}

	T* 
	GetBuffer ()
	{
		return AllocateBuffer (m_p ? TGetSize () (*m_p) : sizeof (T), m_p);
	}

	T* 
	GetBuffer (size_t Size, bool SaveContents = false)
	{
		return AllocateBuffer (Size, SaveContents ? m_p : NULL);
	}

	bool
	SetBuffer (
		EBuf Kind,
		void* p,
		size_t Size
		)
	{
		if (Size < sizeof (CHdr) + sizeof (T))
		{
			err::SetError (err::EStatus_BufferTooSmall);
			return false;
		}

		CHdr* pOldHdr = GetHdr ();
		
		CPtrT <CHdr> NewHdr = AXL_REF_NEW_STATIC (CHdr, p);
		NewHdr->m_BufferSize = Size - sizeof (CHdr);
		NewHdr->SetFree (Kind == EBuf_Static ? NULL : (mem::FFree) -1);
		m_p = (T*) (NewHdr + 1);
		NewHdr.Detach ();

		new (m_p) T;

		if (pOldHdr)
			pOldHdr->Release ();

		return true;
	}

	bool 
	EnsureExclusive ()
	{ 
		return m_p ? GetBuffer () != NULL : true; 
	}

protected:
	CHdr*
	GetHdr () const
	{
		return m_p ? (CHdr*) m_p - 1 : NULL;
	}

	T* 
	AllocateBuffer (
		size_t Size,
		const T* pSrc
		)
	{
		ASSERT (Size >= sizeof (T));

		CHdr* pOldHdr = GetHdr ();
		
		if (pOldHdr && 
			pOldHdr->m_BufferSize >= Size &&
			pOldHdr->GetRefCount () == 1
			)
		{
			if (pSrc)
			{
				*m_p = *pSrc;
				CopyExtra (m_p, pSrc);
			}

			return m_p;
		}

		size_t BufferSize = rtl::GetMinPower2Ge (Size);

		CPtrT <CHdr> NewHdr = AXL_REF_NEW_EXTRA (CHdr, BufferSize);
		if (!NewHdr)
			return NULL;

		NewHdr->m_BufferSize = BufferSize;
		m_p = (T*) (NewHdr + 1);
		NewHdr.Detach ();

		if (pSrc)
		{
			new (m_p) T (*pSrc);
			CopyExtra (m_p, pSrc);
		}
		else
		{
			new (m_p) T;
		}

		if (pOldHdr)
			pOldHdr->Release ();

		return m_p;
	}

	static
	void 
	CopyExtra (
		T* pDst,
		const T* pSrc
		)
	{
		size_t Size = TGetSize () (*pSrc);
		if (Size > sizeof (T))
			memcpy (pDst + 1, pSrc + 1, Size - sizeof (T));
	}
};

//.............................................................................

} // namespace ref
} // namespace axl

