// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

//.............................................................................

class CImportType: 
	public CType,
	public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CQualifiedName m_Name;
	CNamespace* m_pAnchorNamespace;
	CType* m_pExternalType;	

public:
	CImportType ()
	{
		m_TypeKind = EType_Import;
		m_pAnchorNamespace = NULL;
		m_pExternalType = NULL;
	}

	const CQualifiedName&
	GetName ()
	{
		return m_Name;
	}

	CNamespace* 
	GetAnchorNamespace ()
	{
		return m_pAnchorNamespace;
	}

	CType* 
	GetExternalType ()
	{
		return m_pExternalType;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
