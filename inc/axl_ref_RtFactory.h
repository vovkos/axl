// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_RTFACTORY_H

#include "axl_ref_Ptr.h"
#include "axl_mem_Factory.h"

namespace axl {
namespace ref {

//.............................................................................
	
class CRtBox: 
	public IRefCount,
	public obj::IClassImplT <CRtBox>
{
protected:
	obj::IClass* m_pClass;

public:
	CRtBox ()
	{
		m_pClass = NULL;
	}

	~CRtBox ()
	{
		if (m_pClass)
			m_pClass->Destruct (this + 1);
	}

	void
	SetClass (obj::IClass* pClass)
	{
		ASSERT (!m_pClass);
		
		m_pClass = pClass;

		if (m_pClass)
			m_pClass->Construct (this + 1);
	}

	// IRoot

	virtual
	void*
	GetObject (obj::IClass** ppClass)
	{
		if (ppClass)
			*ppClass = this;

		return this;
	}

	// IClass

	virtual
	size_t
	GetInterfaceOffset (const GUID& Guid)
	{
		return
			m_pClass ? m_pClass->GetInterfaceOffset (Guid) : 
			InlineIsEqualGUID (Guid, __uuidof (IRefCount)) ? offsetof_class (CRtBox, IRefCount) : -1;
	}
};

//.............................................................................

template <typename TAlloc = mem::CStdAlloc>
class CRtFactoryT
{
public:
	typedef TAlloc CAlloc;

	class CNew
	{
	public:
#ifdef _DEBUG
		CPtrT <void> 
		operator () (
			obj::IClass* pClass,
			const char* pFilePath,
			int Line,
			size_t Extra = 0
			)
		{
			size_t Size = pClass->GetSize () + Extra;
			
			size_t RefCountOffset = pClass->GetInterfaceOffset (__uuidof (IRefCount));

			if (RefCountOffset != -1)
			{
				void* p = TAlloc::Alloc (Size, pClass->GetName (), pFilePath, Line);
				pClass->Construct (p);
				IRefCount* pRefCount = (IRefCount*) ((uchar_t*) p + RefCountOffset);
				pRefCount->SetFree (&TAlloc::Free);
				return CPtrT <void> (p, pRefCount);
			}
			else
			{
				CPtrT <CRtBox> Box = mem::CStdFactoryT <CRtBox, TAlloc>::New (pFilePath, Line, Size);
				Box->SetClass (pClass);
				Box->SetFree (&TAlloc::Free);
				return CPtrT <void> (Box + 1, Box.GetRefCount ());
			}
		}
#else
		CPtrT <void> 
		operator () (
			obj::IClass* pClass,
			size_t Extra = 0
			)
		{
			size_t Size = pClass->GetSize () + Extra;
			
			size_t RefCountOffset = pClass->GetInterfaceOffset (__uuidof (IRefCount));

			if (RefCountOffset != -1)
			{
				void* p = TAlloc::Alloc (Size);
				pClass->Construct (p);
				IRefCount* pRefCount = (IRefCount*) ((uchar_t*) p + RefCountOffset);
				pRefCount->SetFree (&TAlloc::Free);
				return CPtrT <void> (p, pRefCount);
			}
			else
			{
				CPtrT <CRtBox> Box = mem::CStdFactoryT <CRtBox, TAlloc>::New (Size);
				Box->SetClass (pClass);
				Box->SetFree (&TAlloc::Free);
				return CPtrT <void> (Box + 1, Box.GetRefCount ());
			}
		}
#endif
	};

public:

#ifdef _DEBUG
	static
	CPtrT <void> 
	New (
		obj::IClass* pClass,
		const char* pFilePath,
		int Line,
		size_t Extra = 0
		)
	{
		return CNew () (pClass, pFilePath, Line, Extra);
	}
#else
	static
	CPtrT <void> 
	New (
		obj::IClass* pClass,
		size_t Extra = 0
		)
	{
		return CNew () (pClass, Extra);
	}
#endif
};

typedef CRtFactoryT <mem::CStdAlloc> CRtFactory;

//.............................................................................

#ifdef _DEBUG

#define AXL_REF_RT_NEW(pClass) \
	ref::CRtFactory::New (pClass, __FILE__, __LINE__)

#define AXL_REF_RT_NEW_EXTRA(Class, Extra) \
	ref::CRtFactory::New (pClass, __FILE__, __LINE__, Extra)

#else

#define AXL_REF_RT_NEW(pClass) \
	ref::CRtFactory::New (pClass)

#define AXL_REF_RT_NEW_EXTRA(Class, Extra) \
	ref::CRtFactory::New (pClass, Extra)

#endif

//.............................................................................

} // namespace ref
} // namespace axl
