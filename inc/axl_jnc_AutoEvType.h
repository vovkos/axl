// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CAutoEvType: public CType
{
protected:
	friend class CTypeMgr;

	CFunctionType* m_pStarterType;
	CFunctionType* m_pStopperType;

public:
	CAutoEvType ();

	CFunctionType*
	GetStarterType ()
	{
		return m_pStarterType;
	}

	CFunctionType*
	GetStopperType ()
	{
		return m_pStopperType;
	}

	static
	rtl::CStringA
	CreateSignature (
		CFunctionType* pStarterType,
		CFunctionType* pStopperType
		)
	{ 
		return rtl::CStringA::Format_s ("V%s%s", pStarterType->GetSignature (), pStopperType->GetSignature ()); 
	}

protected:
	virtual 
	void
	PrepareTypeString ()
	{
		m_TypeString.Format (_T("autoev %s"), m_pStarterType->GetArgTypeString ());
	}

	virtual 
	void
	PrepareLlvmType ()
	{
		ASSERT (false);
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
