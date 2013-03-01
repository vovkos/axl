// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

class CAutoEv: public CNamedModuleItem
{
protected:
	CFunctionType* m_pType;
	CFunction* m_pStart;
	CFunction* m_pStop;
	CClassType* m_pParentClassType;
	CStructField* m_pParentClassFieldMember;
	CStructType* m_pFieldStructType;
	CVariable* m_pStaticDataVariable;

public:
	CAutoEv ();

	CFunctionType* 
	GetType ()
	{
		return m_pType;
	}

	CFunction* 
	GetStart ()
	{
		return m_pStart;
	}

	CFunction* 
	GetStop ()
	{
		return m_pStop;
	}

	CStructType* 
	GetFieldStructType ()
	{
		return m_pFieldStructType;
	}

	CClassType* 
	GetParentClassType ()
	{
		return m_pParentClassType;
	}

	CVariable* 
	GetStaticDataVariable ()
	{
		return m_pStaticDataVariable;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
