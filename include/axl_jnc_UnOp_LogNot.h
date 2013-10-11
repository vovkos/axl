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

protected:
	bool
	ZeroCmpOperator (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	PtrOperator (
		const CValue& OpValue,
		CValue* pResultValue
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
