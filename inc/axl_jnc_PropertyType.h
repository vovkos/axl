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
	CPropertyPointerType* m_pPropertyPointerType;

	CClassType* m_pParentClassType;
	size_t m_ParentVTableIndex;

	rtl::CString m_AccessorSignature;

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

	rtl::CStringA
	GetAccessorSignature ();

	int 
	CmpAccessorTypes (CPropertyType* pPropertyType)
	{
		return GetAccessorSignature ().Cmp (pPropertyType->GetAccessorSignature ());
	}

	CPropertyPointerType* 
	GetPropertyPointerType ();

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
	friend class CTypeMgr;

	CPropertyType* m_pPropertyType;
	CPropertyType* m_pMemberPropertyType; // with additional abstract interface argument
	CStructType* m_pPointerStructType;

public:
	CPropertyPointerType ();

	llvm::StructType* 
	GetLlvmType ()
	{
		return GetPointerStructType ()->GetLlvmType ();
	}

	CPropertyType* 
	GetPropertyType ()
	{
		return m_pPropertyType;
	}

	CPropertyType* 
	GetMemberPropertyType ();

	CStructType* 
	GetPointerStructType ();
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
