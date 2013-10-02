// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BinOp.h"

namespace axl {
namespace jnc {

//.............................................................................

class CBinOp_Assign: public IBinaryOperator
{
public:
	AXL_OBJ_CLASS_0 (CBinOp_Assign, IBinaryOperator)

public:
	CBinOp_Assign ()
	{
		m_OpKind = EBinOp_Assign;
		m_OpFlags1 = EOpFlag_KeepRef;
		m_OpFlags2 = EOpFlag_KeepEnum | EOpFlag_KeepBool;
	}

	virtual
	CType*
	GetResultType (
		const CValue& OpValue1,
		const CValue& OpValue2
		)
	{
		return OpValue1.GetType ();
	}

	virtual
	bool
	Operator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

class CBinOp_OpAssign: public IBinaryOperator
{
public:
	AXL_OBJ_CLASS_0 (CBinOp_OpAssign, IBinaryOperator)

public:
	CBinOp_OpAssign ()
	{
		m_OpFlags1 = EOpFlag_KeepRef;
		m_OpFlags2 = EOpFlag_KeepEnum;
	}

	virtual
	CType*
	GetResultType (
		const CValue& OpValue1,
		const CValue& OpValue2
		)
	{
		return OpValue1.GetType ();
	}

	virtual
	bool
	Operator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

class CBinOp_RefAssign: public IBinaryOperator
{
public:
	AXL_OBJ_CLASS_0 (CBinOp_RefAssign, IBinaryOperator)

public:
	CBinOp_RefAssign ()
	{
		m_OpKind = EBinOp_RefAssign;
		m_OpFlags1 = EOpFlag_KeepRef;
	}

	virtual
	CType*
	GetResultType (
		const CValue& OpValue1,
		const CValue& OpValue2
		)
	{
		err::SetFormatStringError ("'%s' has no overloaded ':=' operator", OpValue1.GetType ()->GetTypeString ().cc ());
		return NULL;
	}

	virtual
	bool
	Operator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		err::SetFormatStringError ("'%s' has no overloaded ':=' operator", OpValue1.GetType ()->GetTypeString ().cc ());
		return false;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
