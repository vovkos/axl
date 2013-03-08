// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BinOp.h"

namespace axl {
namespace jnc {

//.............................................................................

class CBinOp_LogAnd: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_LogAnd, IBinaryOperator)

public:
	CBinOp_LogAnd ()
	{
		m_OpKind = EBinOp_LogAnd;
	}

	virtual
	CType*
	GetResultType (
		const CValue& OpValue1,
		const CValue& OpValue2
		);

	virtual
	bool
	Operator (
		const CValue& RawOpValue1,
		const CValue& RawOpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

class CBinOp_LogOr: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_LogOr, IBinaryOperator)

public:
	CBinOp_LogOr ()
	{
		m_OpKind = EBinOp_LogOr;
	}

	virtual
	CType*
	GetResultType (
		const CValue& OpValue1,
		const CValue& OpValue2
		);

	virtual
	bool
	Operator (
		const CValue& RawOpValue1,
		const CValue& RawOpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
