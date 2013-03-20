// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

class CFunctionType;
class CPropertyType;

//.............................................................................

class CNamedType: 
	public CType,
	public CNamespace
{
protected:
	friend class CParser;

	CNamespace* m_pExtensionNamespace;

public:
	CNamedType ()
	{
		m_NamespaceKind = ENamespace_Type;
		m_pItemDecl = this;
	}

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

protected:
	virtual
	CModuleItem*
	FindItemTraverseImpl (
		const tchar_t* pName,
		CBaseTypeCoord* pCoord,
		int Flags
		);
};
//.............................................................................

} // namespace jnc {
} // namespace axl {
