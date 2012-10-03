// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_ARGPACKER_H

#include "axl_exe_Arg.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace exe {

//.............................................................................

[uuid ("F3E7AE11-D5F1-4911-8F5E-438E2A5AA900")]
struct IArgPacker: obj::IRoot
{
	virtual
	va_list
	PackV (
		void* p,
		size_t* pSize,
		ref::CPtrT <obj::IClass>* pAgent,
		va_list va
		) = 0;
	
	void
	Pack (
		void* p,
		size_t* pSize,
		ref::CPtrT <obj::IClass>* pAgent,
		...
		)
	{
		PackV (p, pSize, pAgent, va_start_e (pAgent));
	}

	size_t
	CountV (va_list va)
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
		return CountV (va_start_e (Unused));
	}

	ref::CPtrT <CArgBlock>
	CreateArgBlockV (va_list va)
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
		return CreateArgBlockV (va_start_e (Unused));
	}
};

//.............................................................................

template <typename T>
class IArgPackerImplT: public IArgPacker
{
public:
	AXL_OBJ_SIMPLE_CLASS (IArgPackerImplT, IArgPacker)

protected:
	typedef ref::CBoxT <typename T::CShadow> CShadow;

public:
	virtual
	va_list
	PackV (
		void* p,
		size_t* pSize,
		ref::CPtrT <obj::IClass>* pAgent,
		va_list va
		)
	{
		if (!p || !T::HasShadow)
			return T () (p, pSize, NULL, va);
	
		obj::IClass* pClass = obj::IClassImplT <T::CClass>::GetSingleton ();
		ref::CPtrT <CShadow> Shadow = AXL_REF_NEW (CShadow);		
		pAgent->Copy (pClass, Shadow.GetRefCount ());

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
	AXL_OBJ_SIMPLE_CLASS (CArgPackerSeq, IArgPacker)

protected:
	class CAgent: 
		public ref::IRefCount,
		public obj::IClass
	{
	public:
		rtl::CArrayT <ref::CPtrT <obj::IClass> > m_Sequence;

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
		GetInterfaceOffset (const GUID& Guid)
		{
			return -1; // not used in packing
		}
	};

protected:
	rtl::CArrayT <IArgPacker*> m_Sequence;

public:
	virtual
	va_list
	PackV (
		void* p,
		size_t* pSize,
		ref::CPtrT <obj::IClass>* pAgent,
		va_list va
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
	va_list va
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
	return FormatArgBlockV (pFormat, va_start_e (pFormat));
}

//.............................................................................

} // namespace exe
} // namespace axl
