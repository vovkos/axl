// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_UnOp.h"

namespace axl {
namespace jnc {

//.............................................................................

class CUnOp_PreInc: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_PreInc, IUnaryOperator)

public:
	CUnOp_PreInc ()
	{
		m_OpFlags = EOpFlag_KeepRef;
	}

	virtual
	CType*
	GetResultType (const CValue& OpValue);

	virtual
	bool
	Operator (
		const CValue& OpValue,
		CValue* pResultValue
		);
};

//.............................................................................

class CUnOp_PostInc: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_PostInc, IUnaryOperator)

public:
	CUnOp_PostInc ()
	{
		m_OpFlags = EOpFlag_KeepRef;
	}

	virtual
	CType*
	GetResultType (const CValue& OpValue);

	virtual
	bool
	Operator (
		const CValue& OpValue,
		CValue* pResultValue
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
