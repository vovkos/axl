// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

class CFunctionType;
class CFunctionPointerType;
class CPropertyType;
class CPropertyPointerType;

//.............................................................................

enum EClosure
{
	EClosure_Function,
	EClosure_Property,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t* 
GetClosureKindString (EClosure ClosureKind);

//.............................................................................

class CClosure: public ref::IRefCount
{
protected:
	friend class CValue;

	EClosure m_ClosureKind;
	rtl::CBoxListT <CValue> m_ArgList;

public:
	CClosure ()
	{
		m_ClosureKind = EClosure_Function;
	}

	bool 
	IsEmpty ()
	{
		return m_ArgList.IsEmpty ();
	}

	bool
	IsClassMemberClosure (CValue* pIfaceValue);

	bool
	IsSimpleClassMemberClosure (CValue* pIfaceValue)
	{
		return m_ArgList.GetCount () == 1 && IsClassMemberClosure (pIfaceValue);
	}

	EClosure
	GetClosureKind ()
	{
		return m_ClosureKind;
	}

	rtl::CBoxListT <CValue>*
	GetArgList ()
	{
		return &m_ArgList;
	}

	void
	CombineClosure (rtl::CBoxListT <CValue>* pArgList);

	bool
	Apply (rtl::CBoxListT <CValue>* pArgList);

	CType*
	GetClosureType (CType* pType);

	CFunctionType*
	GetFunctionClosureType (CFunctionType* pType);

	CPropertyType*
	GetPropertyClosureType (CPropertyType* pType);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
