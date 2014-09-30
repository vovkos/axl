// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
	EBuf_Exclusive, // buffer cannot be shared (stack-allocated or object-field) 
	EBuf_Shared,    // buffer can be shared (static or global)

	// aliases

	EBuf_Static = EBuf_Shared, 
	EBuf_Stack  = EBuf_Exclusive,
	EBuf_Field  = EBuf_Exclusive,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TGetSize = rtl::CGetSizeT <T>
	>
class CBufT
{
public:
	class CHdr: public CRefCount
	{
	public:
		size_t m_bufferSize;

		~CHdr ()
		{
			((T*) (this + 1))->~T ();
		}
	};

	enum
	{
		minBufSize = sizeof (CHdr) + sizeof (T)
	};

protected:
	T* m_p;

public:
	CBufT ()
	{
		m_p = NULL;
	}

	CBufT (const CBufT& src)
	{
		m_p = NULL;
		copy (src);
	}

	CBufT (const T& src)
	{
		m_p = NULL;
		copy (src);
	}
	
	CBufT (
		EBuf kind,
		void* p, 
		size_t size
		)
	{
		m_p = NULL;
		setBuffer (kind, p, size);
	}

	~CBufT ()
	{
		release ();
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
	operator = (const CBufT& src)
	{ 
		copy (src);
		return *this;
	}

	CBufT& 
	operator = (const T& src)
	{
		copy (src);
		return *this;
	}

	void 
	release ()
	{
		if (!m_p)
			return;

		getHdr ()->release ();
		m_p = NULL;
	}

	T* 
	create (size_t size = sizeof (T))
	{
		release ();
		return allocateBuffer (size, NULL);
	}

	bool
	copy (const CBufT& src)
	{
		if (m_p == src.m_p)
			return true;

		if (src.m_p)
		{
			if (src.getHdr ()->getFree () == (mem::FFree*) -1)
				return copy (*src);

			src.getHdr ()->addRef ();
		}

		if (m_p)
			getHdr ()->release ();

		m_p = src.m_p;
		return true;
	}

	bool
	copy (const T& src)
	{
		return m_p == &src ? true : allocateBuffer (TGetSize () (src), &src) != NULL;
	}

	T* 
	getBuffer ()
	{
		return allocateBuffer (m_p ? TGetSize () (*m_p) : sizeof (T), m_p);
	}

	T* 
	getBuffer (size_t size, bool saveContents = false)
	{
		return allocateBuffer (size, saveContents ? m_p : NULL);
	}

	void
	setBuffer (
		EBuf kind,
		void* p,
		size_t size
		)
	{
		ASSERT (size >= sizeof (CHdr) + sizeof (T));

		CHdr* oldHdr = getHdr ();

		mem::FFree* pfFree = kind == ref::EBuf_Static ? NULL : (mem::FFree*) -1;
		CPtrT <CHdr> newHdr = AXL_REF_NEW_INPLACE (CHdr, p, pfFree);
		newHdr->m_bufferSize = size - sizeof (CHdr);

		m_p = (T*) (newHdr + 1);
		newHdr.detach ();

		new (m_p) T;

		if (oldHdr)
			oldHdr->release ();
	}

	bool 
	ensureExclusive ()
	{ 
		return m_p ? getBuffer () != NULL : true; 
	}

protected:
	CHdr*
	getHdr () const
	{
		return m_p ? (CHdr*) m_p - 1 : NULL;
	}

	T* 
	allocateBuffer (
		size_t size,
		const T* src
		)
	{
		ASSERT (size >= sizeof (T));

		CHdr* oldHdr = getHdr ();
		
		if (oldHdr && 
			oldHdr->m_bufferSize >= size &&
			oldHdr->getRefCount () == 1
			)
		{
			if (src)
			{
				*m_p = *src;
				copyExtra (m_p, src);
			}

			CHdr* srcHdr = (CHdr*) src - 1;

			return m_p;
		}

		size_t bufferSize = rtl::getMinPower2Ge (size);

		CPtrT <CHdr> newHdr = AXL_REF_NEW_EXTRA (CHdr, bufferSize);
		if (!newHdr)
			return NULL;

		newHdr->m_bufferSize = bufferSize;
		m_p = (T*) (newHdr + 1);
		newHdr.detach ();

		if (src)
		{
			new (m_p) T (*src);
			copyExtra (m_p, src);
		}
		else
		{
			new (m_p) T;
		}

		if (oldHdr)
			oldHdr->release ();

		return m_p;
	}

	static
	void 
	copyExtra (
		T* dst,
		const T* src
		)
	{
		size_t size = TGetSize () (*src);
		if (size > sizeof (T))
			memcpy (dst + 1, src + 1, size - sizeof (T));
	}
};

//.............................................................................

} // namespace ref
} // namespace axl

