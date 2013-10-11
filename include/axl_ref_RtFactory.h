// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
		obj::IType* m_pType;

	public:
		CBox ()
		{
			m_pType = NULL;
		}

		~CBox ()
		{
			ASSERT (m_pType);
			m_pType->Destruct (this + 1);
		}

		void
		SetType (obj::IType* pType)
		{
			m_pType = pType;
			m_pGuid = pType->GetGuid ();
		}
			
		// IRoot

		virtual
		void*
		GetObject (obj::IType** ppType)
		{
			if (ppType)
				*ppType = this;

			return this;
		}

		// IType

		virtual
		size_t
		GetInterfaceOffset (const rtl::TGuid& Guid)
		{
			ASSERT (m_pType);

			if (Guid == AXL_OBJ_GUIDOF (CRefCount))
				return 0;

			size_t Offset = m_pType->GetInterfaceOffset (Guid);
			return Offset != -1 ? Offset + sizeof (CBox) : -1;
		}
	};

public:
	typedef TAlloc CAlloc;

	class CNew
	{
	public:
#ifdef _DEBUG
		CPtrT <void> 
		operator () (
			obj::IType* pType,
			const char* pFilePath,
			int Line,
			size_t Extra = 0
			)
		{
			size_t Size = pType->GetSize () + Extra;			
			size_t RefCountOffset = pType->GetInterfaceOffset (AXL_OBJ_GUIDOF (CRefCount));

			if (RefCountOffset != -1)
			{
				void* p = TAlloc::Alloc (Size, pType->GetName (), pFilePath, Line);
				pType->Construct (p);
				CRefCount* pRefCount = (CRefCount*) ((uchar_t*) p + RefCountOffset);
				pRefCount->SetFree (&TAlloc::Free);
				return CPtrT <void> (p, pRefCount);
			}
			else
			{
				CPtrT <CBox> Box = mem::CStdFactoryT <CBox, TAlloc>::New (pFilePath, Line, Size);
				pType->Construct (Box + 1);
				Box->SetType (pType);
				Box->SetFree (&TAlloc::Free);
				return CPtrT <void> (Box + 1, Box.GetRefCount ());
			}
		}
#else
		CPtrT <void> 
		operator () (
			obj::IType* pType,
			size_t Extra = 0
			)
		{
			size_t Size = pType->GetSize () + Extra;			
			size_t RefCountOffset = pType->GetInterfaceOffset (AXL_OBJ_GUIDOF (CRefCount));

			if (RefCountOffset != -1)
			{
				void* p = TAlloc::Alloc (Size);
				pType->Construct (p);
				CRefCount* pRefCount = (CRefCount*) ((uchar_t*) p + RefCountOffset);
				pRefCount->SetFree (&TAlloc::Free);
				return CPtrT <void> (p, pRefCount);
			}
			else
			{
				CPtrT <CBox> Box = mem::CStdFactoryT <CBox, TAlloc>::New (Size);
				Box->SetClass (pType);
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
		obj::IType* pType,
		const char* pFilePath,
		int Line,
		size_t Extra = 0
		)
	{
		return CNew () (pType, pFilePath, Line, Extra);
	}
#else
	static
	CPtrT <void> 
	New (
		obj::IType* pType,
		size_t Extra = 0
		)
	{
		return CNew () (pType, Extra);
	}
#endif
};

typedef CRtFactoryT <mem::CStdAlloc> CRtFactory;

//.............................................................................

#ifdef _DEBUG

#define AXL_REF_RT_NEW(pType) \
	ref::CRtFactory::New (pType, __FILE__, __LINE__)

#define AXL_REF_RT_NEW_EXTRA(pType, Extra) \
	ref::CRtFactory::New (pType, __FILE__, __LINE__, Extra)

#else

#define AXL_REF_RT_NEW(pType) \
	ref::CRtFactory::New (pType)

#define AXL_REF_RT_NEW_EXTRA(pType, Extra) \
	ref::CRtFactory::New (pType, Extra)

#endif

//.............................................................................

} // namespace ref
} // namespace axl
