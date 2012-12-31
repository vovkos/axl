// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_PropertyType.h"

namespace axl {
namespace jnc {

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
