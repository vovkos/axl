// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_VTableType.h"

namespace axl {
namespace jnc {

class CClassType;
class CPropertyPointerType;

//.............................................................................

class CPropertyType: public CVTableType
{
protected:
	friend class CTypeMgr;
	friend class CClassType;
	friend class CParser;

	CFunction* m_pGetter;
	CFunctionOverload m_Setter;
	CPropertyPointerType* m_pPointerType;

	CClassType* m_pParentClassType;
	size_t m_ParentVTableIndex;

public:
	CPropertyType ();

	bool
	IsReadOnly ()
	{
		return m_Setter.IsEmpty ();
	}

	CFunction*
	GetGetter ()
	{
		return m_pGetter;
	}

	CFunctionOverload*
	GetSetter ()
	{
		return &m_Setter;
	}
	
	CClassType* 
	GetParentClassType ()
	{
		return m_pParentClassType;
	}

	size_t
	GetParentVTableIndex ()
	{
		return m_ParentVTableIndex;
	}

	CPropertyPointerType* 
	GetPointerType ()
	{
		return m_pPointerType;
	}

	rtl::CStringA
	CreateSignature ();

	rtl::CString
	CreateTypeString ();

	void
	TagAccessors ();

	bool
	CalcLayout ();
};

//.............................................................................

class CPropertyPointerType:
	public CType,
	public rtl::TListLink
{
protected:
	CPropertyType* m_pPropertyType;
	CStructType* m_pPointerStructType;

public:
	CPropertyPointerType ();

	CPropertyType* 
	GetPropertyType ()
	{
		return m_pPropertyType;
	}

	CStructType* 
	GetPointerStructType ()
	{
		ASSERT (m_pPointerStructType);
		return m_pPointerStructType;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
