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
	
template <typename TAlloc = mem::CStdAlloc>
class CRtFactoryT
{
protected:
	class CBox: 
		public CRefCount,
		public obj::ITypeSimpleImplT <CBox>
	{
	protected:
		obj::IType* m_type;

	public:
		CBox ()
		{
			m_type = NULL;
		}

		~CBox ()
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
		getInterfaceOffset (const rtl::TGuid& guid)
		{
			ASSERT (m_type);

			if (guid == AXL_OBJ_GUIDOF (CRefCount))
				return 0;

			size_t offset = m_type->getInterfaceOffset (guid);
			return offset != -1 ? offset + sizeof (CBox) : -1;
		}
	};

public:
	typedef TAlloc CAlloc;

	class COperatorNew
	{
	public:
#ifdef _DEBUG
		CPtrT <void> 
		operator () (
			obj::IType* type,
			const char* filePath,
			int line,
			size_t extra = 0
			)
		{
			size_t size = type->getSize () + extra;			
			size_t refCountOffset = type->getInterfaceOffset (AXL_OBJ_GUIDOF (CRefCount));

			if (refCountOffset != -1)
			{
				void* p = TAlloc::alloc (size, type->getName (), filePath, line);
				type->construct (p);
				CRefCount* refCount = (CRefCount*) ((uchar_t*) p + refCountOffset);
				refCount->setFree (&TAlloc::free);
				return CPtrT <void> (p, refCount);
			}
			else
			{
				CPtrT <CBox> box = mem::CStdFactoryT <CBox, TAlloc>::operatorNew (filePath, line, size);
				type->construct (box + 1);
				box->setType (type);
				box->setFree (&TAlloc::free);
				return CPtrT <void> (box + 1, box.getRefCount ());
			}
		}
#else
		CPtrT <void> 
		operator () (
			obj::IType* type,
			size_t extra = 0
			)
		{
			size_t size = type->getSize () + extra;			
			size_t refCountOffset = type->getInterfaceOffset (AXL_OBJ_GUIDOF (CRefCount));

			if (refCountOffset != -1)
			{
				void* p = TAlloc::alloc (size);
				type->construct (p);
				CRefCount* refCount = (CRefCount*) ((uchar_t*) p + refCountOffset);
				refCount->setFree (&TAlloc::free);
				return CPtrT <void> (p, refCount);
			}
			else
			{
				CPtrT <CBox> box = mem::CStdFactoryT <CBox, TAlloc>::operatorNew (size);
				box->setClass (type);
				box->setFree (&TAlloc::free);
				return CPtrT <void> (box + 1, box.getRefCount ());
			}
		}
#endif
	};

public:

#ifdef _DEBUG
	static
	CPtrT <void> 
	operatorNew (
		obj::IType* type,
		const char* filePath,
		int line,
		size_t extra = 0
		)
	{
		return COperatorNew () (type, filePath, line, extra);
	}
#else
	static
	CPtrT <void> 
	operatorNew (
		obj::IType* type,
		size_t extra = 0
		)
	{
		return COperatorNew () (type, extra);
	}
#endif
};

typedef CRtFactoryT <mem::CStdAlloc> CRtFactory;

//.............................................................................

#ifdef _DEBUG

#define AXL_REF_RT_NEW(type) \
	ref::CRtFactory::operatorNew (type, __FILE__, __LINE__)

#define AXL_REF_RT_NEW_EXTRA(type, extra) \
	ref::CRtFactory::operatorNew (type, __FILE__, __LINE__, extra)

#else

#define AXL_REF_RT_NEW(type) \
	ref::CRtFactory::operatorNew (type)

#define AXL_REF_RT_NEW_EXTRA(type, extra) \
	ref::CRtFactory::operatorNew (type, extra)

#endif

//.............................................................................

} // namespace ref
} // namespace axl
