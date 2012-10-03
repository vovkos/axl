// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_PACKER_H

#include "axl_rtl_Pack.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace rtl {

//.............................................................................

[uuid ("22b32a4a-f68c-473b-bc0c-e83c413abbe3")]
struct IPacker: obj::IRoot
{
	virtual
	va_list
	PackV (
		void* p,
		size_t* pSize,
		va_list va
		) = 0;
	
	void
	Pack (
		void* p,
		size_t* pSize,
		...
		)
	{
		PackV (p, pSize, va_start_e (pSize));
	}

	size_t
	CountV (va_list va)
	{
		size_t Size = 0;
		PackV (NULL, &Size, va);
		return Size;
	}

	size_t
	Count (
		int Unused,
		...
		)
	{
		return CountV (va_start_e (Unused));
	}

	ref::CPtrT <mem::TBlock> 
	CreatePackageV (va_list va)
	{
		size_t Size = 0;
		PackV (NULL, &Size, va);

		if (Size == -1)
			return NULL;

		typedef ref::CBoxT <mem::TBlock> CPackage;

		ref::CPtrT <CPackage> Package = AXL_REF_NEW_EXTRA (CPackage, Size);
		PackV (Package + 1, &Size, va);
		Package->m_p = Package + 1;
		Package->m_Size = Size;

		return Package;			
	}

	ref::CPtrT <mem::TBlock> 
	CreatePackage (
		int Unused,
		...
		)
	{
		return CreatePackageV (va_start_e (Unused));
	}
};

//.............................................................................

template <typename TPack>
class IPackerImplT: public IPacker
{
public:
	AXL_OBJ_SIMPLE_CLASS (IPackerImplT, IPacker)

public:
	virtual
	va_list
	PackV (
		void* p,
		size_t* pSize,
		va_list va
		)
	{
		size_t Size;
		return TPack () (p, &Size, va);
	}

	static
	IPackerImplT*
	GetSingleton ()
	{
		return rtl::GetSimpleSingleton <IPackerImplT> ();
	}
};

//.............................................................................

// run-time sequencing

class CPackerSeq: public IPacker
{
public:
	AXL_OBJ_SIMPLE_CLASS (CPackerSeq, IPacker)

protected:
	rtl::CArrayT <IPacker*> m_Sequence;

public:
	virtual
	va_list
	PackV (
		void* p,
		size_t* pSize,
		va_list va
		);

	void
	Clear ()
	{
		m_Sequence.Clear ();
	}

	size_t
	Append (IPacker* pPacker)
	{
		m_Sequence.Append (pPacker);
		return m_Sequence.GetCount ();
	}

	template <typename T>
	size_t
	Append ()
	{
		return Append (IPackerImplT <T>::GetSingleton ());
	}

	// often times it is more convenient to use printf-like format string for sequencing

	size_t
	AppendFormat (const char* pFormat);

	size_t
	Format (const char* pFormat)
	{
		Clear ();
		return AppendFormat (pFormat);
	}
};

//.............................................................................

inline
ref::CPtrT <mem::TBlock> 
FormatPackageV (
	const char* pFormat, 
	va_list va
	)
{
	CPackerSeq Packer;
	Packer.Format (pFormat);
	return Packer.CreatePackageV (va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ref::CPtrT <mem::TBlock> 
FormatPackage (
	const char* pFormat, 
	...
	)
{
	return FormatPackageV (pFormat, va_start_e (pFormat));
}

//.............................................................................

// incremental packer

class CPacker
{
protected:
	axl::rtl::CArrayT <uchar_t> m_Buffer;

public:
	void 
	Clear ()
	{
		m_Buffer.Clear ();
	}

	const uchar_t*
	GetBuffer ()
	{
		return m_Buffer;
	}

	size_t
	GetSize ()
	{
		return m_Buffer.GetCount ();
	}

	size_t
	AppendV (
		IPacker* pPack,
		va_list va
		);

	template <typename TPack>
	size_t
	AppendV (va_list va)
	{
		IPack* pPack = IPackerImplT <TPack>::GetSingleton ();
		return AppendV (pPack, va);
	}

	size_t
	Append (
		const void* p,
		size_t Size
		);

	template <typename T>
	size_t 
	Append (const T& Data)
	{
		IPack* pPack = IPackerImplT <CPackT <T> >::GetSingleton ();
		return Pack (&Data, sizeof (Data));
	}

	size_t
	AppendFormatV (
		const char* pFormat,
		va_list va
		)
	{
		CPackerSeq Packer;
		Packer.Format (pFormat);
		AppendV (&Packer, va);
	}

	size_t
	AppendFormat (
		const char* pFormat,
		...
		)
	{
		return AppendFormatV (pFormat, va_start_e (pFormat));
	}

	size_t
	FormatV (
		const char* pFormat,
		va_list va
		)
	{
		Clear ();
		return AppendFormatV (pFormat, va_start_e (pFormat));
	}

	size_t
	Format (
		const char* pFormat,
		...
		)
	{
		return FormatV (pFormat, va_start_e (pFormat));
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
