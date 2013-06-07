// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

class CFunctionType;
class CPropertyType;
class CStructType;
class CVariable;

//.............................................................................

class CNamedType: 
	public CType,
	public CNamespace
{
	friend class CParser;

protected:
	CNamespace* m_pExtensionNamespace;

public:
	CNamedType ();

	CNamespace* 
	GetExtensionNamespace ()
	{
		return m_pExtensionNamespace;
	}

	CType*
	GetThisArgType (int ThisArgTypeFlags = 0);

	CFunctionType* 
	GetMemberMethodType (
		CFunctionType* pShortType,
		int ThisArgTypeFlags = 0
		);

	CPropertyType* 
	GetMemberPropertyType (CPropertyType* pShortType);

	virtual
	bool
	CalcLayout ();

protected:
	void
	ApplyExtensionNamespace ();

	virtual
	CModuleItem*
	FindItemTraverseImpl (
		const char* pName,
		CBaseTypeCoord* pCoord,
		uint_t Flags
		);
};
//.............................................................................

} // namespace jnc {
} // namespace axl {