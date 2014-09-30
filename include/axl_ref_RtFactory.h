// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_RTFACTORY_H

#include "axl_ref_Ptr.h"
#include "axl_mem_Factory.h"

namespace axl {
namespace ref {

//.............................................................................
	
template <typename Alloc = mem::StdAlloc>
class RtFactory
{
protected:
	class Box: 
		public RefCount,
		public obj::ITypeSimpleImplT <Box>
	{
	protected:
		obj::IType* m_type;

	public:
		Box ()
		{
			m_type = NULL;
		}

		~Box ()
		{
			ASSERT (m_type);
			m_type->destruct (this + 1);
		}

		void
		setType (obj::IType* type)
		{
			m_type = type;
			m_guid = type->getGuid ();
		}
			
		// IRoot

		virtual
		void*
		getObject (obj::IType** type_o)
		{
			if (type_o)
				*type_o = this;

			return this;
		}

		// IType

		virtual
		size_t
		getInterfaceOffset (const rtl::Guid& guid)
		{
			ASSERT (m_type);

			if (guid == AXL_OBJ_GUIDOF (RefCount))
				return 0;

			size_t offset = m_type->getInterfaceOffset (guid);
			return offset != -1 ? offset + sizeof (Box) : -1;
		}
	};

public:
	typedef Alloc Alloc;

	class OperatorNew
	{
	public:
#ifdef _DEBUG
		Ptr <void> 
		operator () (
			obj::IType* type,
			const char* filePath,
			int line,
			size_t extra = 0
			)
		{
			size_t size = type->getSize () + extra;			
			size_t refCountOffset = type->getInterfaceOffset (AXL_OBJ_GUIDOF (RefCount));

			if (refCountOffset != -1)
			{
				void* p = Alloc::alloc (size, type->getName (), filePath, line);
				type->construct (p);
				RefCount* refCount = (RefCount*) ((uchar_t*) p + refCountOffset);
				refCount->setFree (&Alloc::free);
				return Ptr <void> (p, refCount);
			}
			else
			{
				Ptr <Box> box = mem::StdFactory <Box, Alloc>::operatorNew (filePath, line, size);
				type->construct (box + 1);
				box->setType (type);
				box->setFree (&Alloc::free);
				return Ptr <void> (box + 1, box.getRefCount ());
			}
		}
#else
		Ptr <void> 
		operator () (
			obj::IType* type,
			size_t extra = 0
			)
		{
			size_t size = type->getSize () + extra;			
			size_t refCountOffset = type->getInterfaceOffset (AXL_OBJ_GUIDOF (RefCount));

			if (refCountOffset != -1)
			{
				void* p = Alloc::alloc (size);
				type->construct (p);
				RefCount* refCount = (RefCount*) ((uchar_t*) p + refCountOffset);
				refCount->setFree (&Alloc::free);
				return Ptr <void> (p, refCount);
			}
			else
			{
				Ptr <Box> box = mem::StdFactory <Box, Alloc>::operatorNew (size);
				box->setClass (type);
				box->setFree (&Alloc::free);
				return Ptr <void> (box + 1, box.getRefCount ());
			}
		}
#endif
	};

public:

#ifdef _DEBUG
	static
	Ptr <void> 
	operatorNew (
		obj::IType* type,
		const char* filePath,
		int line,
		size_t extra = 0
		)
	{
		return OperatorNew () (type, filePath, line, extra);
	}
#else
	static
	Ptr <void> 
	operatorNew (
		obj::IType* type,
		size_t extra = 0
		)
	{
		return OperatorNew () (type, extra);
	}
#endif
};

typedef RtFactory <mem::StdAlloc> RtFactory;

//.............................................................................

#ifdef _DEBUG

#define AXL_REF_RT_NEW(type) \
	ref::RtFactory::operatorNew (type, __FILE__, __LINE__)

#define AXL_REF_RT_NEW_EXTRA(type, extra) \
	ref::RtFactory::operatorNew (type, __FILE__, __LINE__, extra)

#else

#define AXL_REF_RT_NEW(type) \
	ref::RtFactory::operatorNew (type)

#define AXL_REF_RT_NEW_EXTRA(type, extra) \
	ref::RtFactory::operatorNew (type, extra)

#endif

//.............................................................................

} // namespace ref
} // namespace axl
