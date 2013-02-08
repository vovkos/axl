// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_UnOp.h"

namespace axl {
namespace jnc {

//.............................................................................

class CUnOp_Addr: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_Addr, IUnaryOperator)

public:
	CUnOp_Addr ()
	{
		m_OpKind = EUnOp_Addr;
		m_OpFlags = EOpFlag_KeepRef;
	}

	virtual
	bool
	Operator (
		const CValue& OpValue,
		CValue* pResultValue
		);
};

//.............................................................................

class CUnOp_Indir: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_Indir, IUnaryOperator)

public:
	CUnOp_Indir ()
	{
		m_OpKind = EUnOp_Indir;
	}

	virtual
	bool
	Operator (
		const CValue& OpValue,
		CValue* pResultValue
		);
};

//.............................................................................

class CUnOp_Ptr: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_Ptr, IUnaryOperator)

public:
	CUnOp_Ptr ()
	{
		m_OpKind = EUnOp_Ptr;
	}

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
