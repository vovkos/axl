// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EThinDataPtrValidator
{
	EThinDataPtrValidator_Undefined,
	EThinDataPtrValidator_Simple,
	EThinDataPtrValidator_Complex,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CThinDataPtrValidator: public ref::IRefCount
{
	friend class CValue;
	friend class CConstMgr;

protected:
	EThinDataPtrValidator m_ValidatorKind;

	CValue m_ScopeValidatorValue;
	CValue m_RangeBeginValue;
	CValue m_SizeValue;

public:
	CThinDataPtrValidator ()
	{
		m_ValidatorKind = EThinDataPtrValidator_Undefined;
	}

	EThinDataPtrValidator 
	GetValidatorKind ()
	{
		return m_ValidatorKind;
	}

	CValue 
	GetScopeValidator ()
	{
		return m_ScopeValidatorValue;
	}

	CValue 
	GetRangeBegin ()
	{
		return m_RangeBeginValue;
	}

	CValue
	GetSizeValue ()
	{
		return m_SizeValue;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
