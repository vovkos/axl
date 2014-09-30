// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_PACKER_H

#include "axl_rtl_Pack.h"
#include "axl_rtl_Array.h"
#include "axl_rtl_SimpleSingleton.h"

namespace axl {
namespace rtl {
	
//.............................................................................

class CPacker
{
public:
	virtual
	axl_va_list
	pack_va (
		void* p,
		size_t* size,
		axl_va_list va
		) = 0;
	
	void
	pack (
		void* p,
		size_t* size,
		...
		)
	{
		AXL_VA_DECL (va, size);
		pack_va (p, size, va);
	}

	size_t
	count_va (axl_va_list va)
	{
		size_t size = 0;
		pack_va (NULL, &size, va);
		return size;
	}

	size_t
	count (
		int unused,
		...
		)
	{
		AXL_VA_DECL (va, unused);
		return count_va (va);
	}

	ref::CPtrT <mem::TBlock> 
	createPackage_va (axl_va_list va)
	{
		size_t size = 0;
		pack_va (NULL, &size, va);

		if (size == -1)
			return ref::EPtr_Null;

		typedef ref::CBoxT <mem::TBlock> CPackage;
		ref::CPtrT <CPackage> package = AXL_REF_NEW_EXTRA (CPackage, size);
		pack_va (package + 1, &size, va);
		package->m_p = package + 1;
		package->m_size = size;

		return package;			
	}

	ref::CPtrT <mem::TBlock> 
	createPackage (
		int unused,
		...
		)
	{
		AXL_VA_DECL (va, unused);
		return createPackage_va (va);
	}
};

//.............................................................................

template <typename TPack>
class CPackerImplT: public CPacker
{
public:
	virtual
	axl_va_list
	pack_va (
		void* p,
		size_t* size,
		axl_va_list va
		)
	{
		return TPack () (p, size, va);
	}

	static
	CPackerImplT*
	getSingleton ()
	{
		return rtl::getSimpleSingleton <CPackerImplT> ();
	}
};

//.............................................................................

// run-time sequencing

class CPackerSeq: public CPacker
{
protected:
	rtl::CArrayT <CPacker*> m_sequence;

public:
	virtual
	axl_va_list
	pack_va (
		void* p,
		size_t* size,
		axl_va_list va
		);

	void
	clear ()
	{
		m_sequence.clear ();
	}

	size_t
	append (CPacker* packer)
	{
		m_sequence.append (packer);
		return m_sequence.getCount ();
	}

	template <typename T>
	size_t
	append ()
	{
		return append (CPackerImplT <T>::getSingleton ());
	}

	// often times it is more convenient to use printf-like format string for sequencing

	size_t
	appendFormat (const char* formatString);

	size_t
	format (const char* formatString)
	{
		clear ();
		return appendFormat (formatString);
	}
};

//.............................................................................

inline
ref::CPtrT <mem::TBlock> 
formatPackage_va (
	const char* formatString, 
	axl_va_list va
	)
{
	CPackerSeq packer;
	packer.format (formatString);
	return packer.createPackage_va (va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ref::CPtrT <mem::TBlock> 
formatPackage (
	const char* formatString, 
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return formatPackage_va (formatString, va);
}

//.............................................................................

// package: 

class CPackage
{
protected:
	axl::rtl::CArrayT <uchar_t> m_buffer;

public:
	void 
	clear ()
	{
		m_buffer.clear ();
	}

	const uchar_t*
	getBuffer ()
	{
		return m_buffer;
	}

	size_t
	getSize ()
	{
		return m_buffer.getCount ();
	}

	size_t
	append_va (
		CPacker* pack,
		axl_va_list va
		);

	template <typename TPack>
	size_t
	append_va (axl_va_list va)
	{
		CPacker* pack = CPackerImplT <TPack>::getSingleton ();
		return append_va (pack, va);
	}

	size_t
	append (
		const void* p,
		size_t size
		);
/*
	template <typename T>
	size_t 
	append (const T& data)
	{
		CPacker* pack = CPackerImplT <CPackT <T> >::getSingleton ();
		return pack (&data, sizeof (data));
	}
*/
	size_t
	appendFormat_va (
		const char* formatString,
		axl_va_list va
		)
	{
		CPackerSeq packer;
		packer.format (formatString);
		return append_va (&packer, va);
	}

	size_t
	appendFormat (
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return appendFormat_va (formatString, va);
	}

	size_t
	format_va (
		const char* formatString,
		axl_va_list va
		)
	{
		clear ();
		return appendFormat_va (formatString, va);
	}

	size_t
	format (
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return format_va (formatString, va);
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
