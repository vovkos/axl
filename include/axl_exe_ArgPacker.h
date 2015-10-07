// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_ARGPACKER_H

#include "axl_exe_Arg.h"
#include "axl_sl_Array.h"

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
	packV (
		void* p,
		size_t* size,
		ref::Ptr <obj::IType>* agent,
		axl_va_list va
		) = 0;
	
	void
	pack (
		void* p,
		size_t* size,
		ref::Ptr <obj::IType>* agent,
		...
		)
	{
		AXL_VA_DECL (va, agent);
		packV (p, size, agent, va);
	}

	size_t
	countV (axl_va_list va)
	{
		size_t size = 0;
		packV (NULL, &size, NULL, va);
		return size;
	}

	size_t
	count (
		int unused,
		...
		)
	{
		AXL_VA_DECL (va, unused);
		return countV (va);
	}

	ref::Ptr <ArgBlock>
	createArgBlockV (axl_va_list va)
	{
		size_t size = countV (va);

		ref::Ptr <ArgBlock> block = AXL_REF_NEW_EXTRA (ArgBlock, size);
		block->m_p = block + 1;
		block->m_size = size;

		packV (block + 1, &size, &block->m_agent, va);

		return block;
	}

	ref::Ptr <ArgBlock>
	createArgBlock (
		int unused,
		...
		)
	{
		AXL_VA_DECL (va, unused);
		return createArgBlockV (va);
	}
};

//.............................................................................

template <typename T>
class IArgPackerImplT: public IArgPacker
{
public:
	AXL_OBJ_CLASS_0 (IArgPackerImplT, IArgPacker)

protected:
	typedef ref::Box <typename T::Shadow> Shadow;

public:
	virtual
	axl_va_list
	packV (
		void* p,
		size_t* size,
		ref::Ptr <obj::IType>* agent,
		axl_va_list va
		)
	{
		if (!p || !T::hasShadow)
			return T () (p, size, NULL, va);
	
		obj::IType* type = AXL_OBJ_TYPEOF (T::Type);
		ref::Ptr <Shadow> shadow = AXL_REF_NEW (Shadow);		
		agent->copy (type, shadow.getRefCount ());

		return T () (p, size, shadow, va);
	}

	static
	IArgPackerImplT*
	getSingleton ()
	{
		return sl::getSimpleSingleton <IArgPackerImplT> ();
	}
};

//.............................................................................

// run-time sequencing

class ArgPackerSeq: public IArgPacker
{
public:
	AXL_OBJ_CLASS_0 (ArgPackerSeq, IArgPacker)

protected:
	class Agent: 
		public ref::IRefCount,
		public obj::IType
	{
	public:
		sl::Array <ref::Ptr <obj::IType> > m_sequence;

	public:
		virtual
		void
		construct (void* p);

		virtual
		void
		destruct (void* p);

		virtual
		void
		copy (
			void* p,
			const void* src
			);

		virtual
		size_t
		getInterfaceOffset (const sl::Guid& guid)
		{
			return -1; // not used in packing
		}
	};

protected:
	sl::Array <IArgPacker*> m_sequence;

public:
	virtual
	axl_va_list
	packV (
		void* p,
		size_t* size,
		ref::Ptr <obj::IType>* agent,
		axl_va_list va
		);

	void
	clear ()
	{
		m_sequence.clear ();
	}

	size_t
	append (IArgPacker* argPacker)
	{
		m_sequence.append (argPacker);
		return m_sequence.getCount ();
	}

	template <typename T>
	size_t
	append ()
	{
		return append (IArgPackerImplT <T>::getSingleton ());
	}

	// often times it is more convenient to use printf-like format string for sequencing

	size_t
	appendFormat (const char* format);

	size_t
	format (const char* format)
	{
		clear ();
		return appendFormat (format);
	}
};

//.............................................................................

inline
ref::Ptr <ArgBlock> 
formatArgBlockV (
	const char* format, 
	axl_va_list va
	)
{
	ArgPackerSeq packer;
	packer.format (format);
	return packer.createArgBlockV (va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ref::Ptr <ArgBlock> 
formatArgBlock (
	const char* format, 
	...
	)
{
	AXL_VA_DECL (va, format);
	return formatArgBlockV (format, va);
}

//.............................................................................

} // namespace exe
} // namespace axl
