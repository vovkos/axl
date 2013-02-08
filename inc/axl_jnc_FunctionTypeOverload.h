// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"

namespace axl {
namespace jnc {

enum ECast;

//.............................................................................

class CFunctionTypeOverload
{
protected:
	CFunctionType* m_pType;
	rtl::CArrayT <CFunctionType*> m_OverloadArray;

public:
	CFunctionTypeOverload ()
	{
		m_pType = NULL;
	}

	CFunctionTypeOverload (CFunctionType* pType)
	{
		m_pType = pType;
	}

	CFunctionTypeOverload (
		CFunctionType* const* ppTypeArray,
		size_t Count
		)
	{
		Copy (ppTypeArray, Count);
	}

	bool
	IsEmpty () const
	{
		return m_pType == NULL;
	}

	bool 
	IsOverloaded () const
	{
		return m_OverloadArray.IsEmpty ();
	}

	size_t
	GetOverloadCount () const
	{
		return m_OverloadArray.GetCount () + 1;
	}

	CFunctionType*
	GetOverload (size_t Overload = 0) const
	{
		return 
			Overload == 0 ? m_pType : 
			Overload <= m_OverloadArray.GetCount () ? m_OverloadArray [Overload - 1] : NULL;
	}

	size_t
	FindOverload (CFunctionType* pType) const;

	size_t
	FindShortOverload (CFunctionType* pType) const;

	size_t
	ChooseOverload (
		const rtl::CArrayT <CType*>& ArgTypeArray,
		ECast* pCastKind = NULL
		) const
	{
		return ChooseOverload (ArgTypeArray, ArgTypeArray.GetCount (), pCastKind);
	}

	size_t
	ChooseOverload (
		CType* const* ppArgTypeArray,
		size_t Count,
		ECast* pCastKind = NULL
		) const;

	size_t
	ChooseOverload (
		const rtl::CConstBoxListT <CValue>& ArgList,
		ECast* pCastKind = NULL
		) const;

	bool
	AddOverload (CFunctionType* pType);

	void
	Copy (
		CFunctionType* const* ppTypeArray,
		size_t Count
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
