// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BinOp.h"

namespace axl {
namespace jnc {

//.............................................................................

class CBinOp_Idx: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Idx, IBinaryOperator)

public:
	CBinOp_Idx ()
	{
		m_OpKind = EBinOp_Ge;
		m_OpFlags1 = EOpFlag_KeepRef;
	}

	virtual
	bool
	Operator (
		const CValue& RawOpValue1,
		const CValue& RawOpValue2,
		CValue* pResultValue
		);

protected:
	bool
	ArrayIndexOperator (
		const CValue& RawOpValue1,
		CArrayType* pArrayType,
		const CValue& RawOpValue2,
		CValue* pResultValue
		);

	bool
	PropertyIndexOperator (
		const CValue& RawOpValue1,
		const CValue& RawOpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
