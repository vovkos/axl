// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BinOp.h"

namespace axl {
namespace jnc {

//.............................................................................

class CBinOp_At: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_At, IBinaryOperator)

public:
	CBinOp_At ()
	{
		m_OpKind = EBinOp_At;
	}

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
