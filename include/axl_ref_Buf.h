// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_BUF_H

#include "axl_ref_New.h"
#include "axl_sl_Func.h"
#include "axl_sl_BitIdx.h"

namespace axl {
namespace ref {

//.............................................................................

// ref-counted buffer for variable-sized objects 
// could be also used for fixed-sized object as single-writer multiple reader accessor
// can use external buffer (e.g. static or allocated on stack)

enum BufKind
{
	BufKind_Exclusive, // buffer cannot be shared (stack-allocated or object-field) 
	BufKind_Shared,    // buffer can be shared (static or global)

	// aliases

	BufKind_Static = BufKind_Shared, 
	BufKind_Stack  = BufKind_Exclusive,
	BufKind_Field  = BufKind_Exclusive,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum BufHdrFlag
{
	BufHdrFlag_Exclusive = 0x04
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename SizeOf = sl::SizeOf <T>
	>
class Buf
{
public:
	class Hdr: public RefCount
	{
	public:
		size_t m_bufferSize;

		~Hdr ()
		{
			((T*) (this + 1))->~T ();
		}
	};

	enum
	{
		MinBufSize = sizeof (Hdr) + sizeof (T)
	};

protected:
	T* m_p;

public:
	Buf ()
	{
		m_p = NULL;
	}

	Buf (const Buf& src)
	{
		m_p = NULL;
		copy (src);
	}

	Buf (const T* src)
	{
		m_p = NULL;
		copy (src);
	}
	
	Buf (
		BufKind kind,
		void* p, 
		size_t size
		)
	{
		m_p = NULL;
		setBuffer (kind, p, size);
	}

	~Buf ()
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

	Buf& 
	operator = (const Buf& src)
	{ 
		copy (src);
		return *this;
	}

	Buf& 
	operator = (const T* src)
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
	copy (const Buf& src)
	{
		if (m_p == src.m_p)
			return true;

		if (src.m_p)
		{
			if (src.getHdr ()->getFlags () & BufHdrFlag_Exclusive)
				return copy (src.m_p);

			src.getHdr ()->addRef ();
		}

		if (m_p)
			getHdr ()->release ();

		m_p = src.m_p;
		return true;
	}

	bool
	copy (const T* src)
	{
		return m_p == src ? true : allocateBuffer (SizeOf () (src), src) != NULL;
	}

	T* 
	getBuffer ()
	{
		return allocateBuffer (m_p ? SizeOf () (m_p) : sizeof (T), m_p);
	}

	T* 
	getBuffer (
		size_t size, 
		bool saveContents = false
		)
	{
		return allocateBuffer (size, saveContents ? m_p : NULL);
	}

	void
	setBuffer (
		BufKind kind,
		void* p,
		size_t size
		)
	{
		ASSERT (size >= sizeof (Hdr) + sizeof (T));

		Hdr* oldHdr = getHdr ();

		uint_t flags = kind != BufKind_Static ? BufHdrFlag_Exclusive : 0;
		Ptr <Hdr> newHdr = AXL_REF_NEW_INPLACE (Hdr, p, flags);
		newHdr->m_bufferSize = size - sizeof (Hdr);

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
	Hdr*
	getHdr () const
	{
		return m_p ? (Hdr*) m_p - 1 : NULL;
	}

	T* 
	allocateBuffer (
		size_t size,
		const T* src
		)
	{
		ASSERT (size >= sizeof (T));

		Hdr* oldHdr = getHdr ();
		
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

			return m_p;
		}

		size_t bufferSize = sl::getMinPower2Ge (size);

		Ptr <Hdr> newHdr = AXL_REF_NEW_EXTRA (Hdr, bufferSize);
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
		if (!src)
			return;

		size_t size = SizeOf () (src);
		if (size > sizeof (T))
			memcpy (dst + 1, src + 1, size - sizeof (T));
	}
};

//.............................................................................

} // namespace ref
} // namespace axl

