// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

class CFunctionType;
class CFunctionPtrType;
class CPropertyType;
class CPropertyPtrType;

//.............................................................................

class CClosure: public ref::IRefCount
{
protected:
	friend class CValue;

	rtl::CBoxListT <CValue> m_ArgList;

public:
	rtl::CBoxListT <CValue>*
	GetArgList ()
	{
		return &m_ArgList;
	}

	bool
	IsMemberClosure ()
	{
		return !m_ArgList.IsEmpty () && m_ArgList.GetHead ()->GetType ()->GetTypeKind () == EType_ClassPtr;
	}

	bool
	IsSimpleClosure ()
	{
		return IsMemberClosure () && m_ArgList.GetCount () == 1;
	}

	size_t
	Append (const rtl::CConstBoxListT <CValue>& ArgList);

	bool
	Apply (rtl::CBoxListT <CValue>* pArgList);

	CType*
	GetClosureType (CType* pType);

	CFunctionPtrType*
	GetFunctionClosureType (CFunction* pFunction); // choose the best overload

	CFunctionPtrType*
	GetFunctionClosureType (CFunctionPtrType* pPtrType);

	CPropertyPtrType*
	GetPropertyClosureType (CPropertyPtrType* pPtrType);

protected:
	bool
	GetArgTypeArray (
		CModule* pModule,
		rtl::CArrayT <CType*>* pArgTypeArray
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
