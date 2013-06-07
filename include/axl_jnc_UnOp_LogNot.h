// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_UnOp.h"

namespace axl {
namespace jnc {

//.............................................................................

class CUnOp_LogNot: public IUnaryOperator
{
public:
	AXL_OBJ_CLASS_0 (CUnOp_LogNot, IUnaryOperator)

public:
	CUnOp_LogNot ()
	{
		m_OpKind = EUnOp_LogNot;
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