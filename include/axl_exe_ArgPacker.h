// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_ARGPACKER_H

#include "axl_exe_Arg.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace exe {

//.............................................................................

struct IArgPacker: obj::IRoot
{
	// {409EDC4D-5ADB-48C7-8BAB-28F713F77E7A}
	AXL_OBJ_INTERFACE (
		IArgPacker, 
		0x409edc4d, 0x5adb, 0x48c7, 0x8b, 0xab, 0x28, 0xf7, 0x13, 0xf7, 0x7e, 0x7a
		)

	virtual
	axl_va_list
	PackV (
		void* p,
		size_t* pSize,
		ref::CPtrT <obj::IType>* pAgent,
		axl_va_list va
		) = 0;
	
	void
	Pack (
		void* p,
		size_t* pSize,
		ref::CPtrT <obj::IType>* pAgent,
		...
		)
	{
		AXL_VA_DECL (va, pAgent);
		PackV (p, pSize, pAgent, va);
	}

	size_t
	CountV (axl_va_list va)
	{
		size_t Size = 0;
		PackV (NULL, &Size, NULL, va);
		return Size;
	}

	size_t
	Count (
		int Unused,
		...
		)
	{
		AXL_VA_DECL (va, Unused);
		return CountV (va);
	}

	ref::CPtrT <CArgBlock>
	CreateArgBlockV (axl_va_list va)
	{
		size_t Size = CountV (va);

		ref::CPtrT <CArgBlock> Block = AXL_REF_NEW_EXTRA (CArgBlock, Size);
		Block->m_p = Block + 1;
		Block->m_Size = Size;

		PackV (Block + 1, &Size, &Block->m_Agent, va);

		return Block;
	}

	ref::CPtrT <CArgBlock>
	CreateArgBlock (
		int Unused,
		...
		)
	{
		AXL_VA_DECL (va, Unused);
		return CreateArgBlockV (va);
	}
};

//.............................................................................

template <typename T>
class IArgPackerImplT: public IArgPacker
{
public:
	AXL_OBJ_CLASS_0 (IArgPackerImplT, IArgPacker)

protected:
	typedef ref::CBoxT <typename T::CShadow> CShadow;

public:
	virtual
	axl_va_list
	PackV (
		void* p,
		size_t* pSize,
		ref::CPtrT <obj::IType>* pAgent,
		axl_va_list va
		)
	{
		if (!p || !T::HasShadow)
			return T () (p, pSize, NULL, va);
	
		obj::IType* pType = AXL_OBJ_TYPEOF (T::CType);
		ref::CPtrT <CShadow> Shadow = AXL_REF_NEW (CShadow);		
		pAgent->Copy (pType, Shadow.GetRefCount ());

		return T () (p, pSize, Shadow, va);
	}

	static
	IArgPackerImplT*
	GetSingleton ()
	{
		return rtl::GetSimpleSingleton <IArgPackerImplT> ();
	}
};

//.............................................................................

// run-time sequencing

class CArgPackerSeq: public IArgPacker
{
public:
	AXL_OBJ_CLASS_0 (CArgPackerSeq, IArgPacker)

protected:
	class CAgent: 
		public ref::IRefCount,
		public obj::IType
	{
	public:
		rtl::CArrayT <ref::CPtrT <obj::IType> > m_Sequence;

	public:
		virtual
		void
		Construct (void* p);

		virtual
		void
		Destruct (void* p);

		virtual
		void
		Copy (
			void* p,
			const void* pSrc
			);

		virtual
		size_t
		GetInterfaceOffset (const rtl::TGuid& Guid)
		{
			return -1; // not used in packing
		}
	};

protected:
	rtl::CArrayT <IArgPacker*> m_Sequence;

public:
	virtual
	axl_va_list
	PackV (
		void* p,
		size_t* pSize,
		ref::CPtrT <obj::IType>* pAgent,
		axl_va_list va
		);

	void
	Clear ()
	{
		m_Sequence.Clear ();
	}

	size_t
	Append (IArgPacker* pArgPacker)
	{
		m_Sequence.Append (pArgPacker);
		return m_Sequence.GetCount ();
	}

	template <typename T>
	size_t
	Append ()
	{
		return Append (IArgPackerImplT <T>::GetSingleton ());
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
ref::CPtrT <CArgBlock> 
FormatArgBlockV (
	const char* pFormat, 
	axl_va_list va
	)
{
	CArgPackerSeq Packer;
	Packer.Format (pFormat);
	return Packer.CreateArgBlockV (va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ref::CPtrT <CArgBlock> 
FormatArgBlock (
	const char* pFormat, 
	...
	)
{
	AXL_VA_DECL (va, pFormat);
	return FormatArgBlockV (pFormat, va);
}

//.............................................................................

} // namespace exe
} // namespace axl
