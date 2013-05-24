// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EUnOp
{
	EUnOp_Undefined = 0,
	EUnOp_Plus,
	EUnOp_Minus,
	EUnOp_BwNot,	
	EUnOp_Addr,
	EUnOp_Indir,	
	EUnOp_LogNot,
	EUnOp_PreInc,
	EUnOp_PreDec,
	EUnOp_PostInc,
	EUnOp_PostDec,	
	EUnOp_Ptr,
	EUnOp__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetUnOpKindString (EUnOp OpKind);

//.............................................................................

enum EOpFlag
{
	EOpFlag_KeepDataRef      = 0x01,
	EOpFlag_KeepPropertyRef  = 0x02,
	EOpFlag_KeepArrayRef     = 0x04,
	EOpFlag_KeepBool         = 0x10,
	EOpFlag_KeepEnum         = 0x20,

	EOpFlag_KeepRef          = EOpFlag_KeepDataRef | EOpFlag_KeepPropertyRef,
};

//.............................................................................

struct IUnaryOperator: obj::IRoot
{
	friend class COperatorMgr;

	// {6CB89F38-6712-4172-ACCD-DA8CD196EE6F}
	AXL_OBJ_INTERFACE (
		IUnaryOperator,
		0x6cb89f38, 0x6712, 0x4172, 0xac, 0xcd, 0xda, 0x8c, 0xd1, 0x96, 0xee, 0x6f
		)

protected:
	CModule* m_pModule;
	EUnOp m_OpKind;
	int m_OpFlags;

public:
	IUnaryOperator ();

	CModule*
	GetModule ()
	{
		return m_pModule;
	}

	EUnOp 
	GetOpKind ()
	{
		return m_OpKind;
	}

	int 
	GetOpFlags ()
	{
		return m_OpFlags;
	}

	bool
	GetResultType (
		const CValue& OpValue,
		CValue* pResultValue
		);

	virtual
	CType*
	GetResultType (const CValue& OpValue) = 0;

	virtual
	bool
	Operator (
		const CValue& OpValue,
		CValue* pResultValue
		) = 0;

	err::CError
	SetOperatorError (CType* pOpType)
	{
		return err::SetFormatStringError (
			"unary '%s' cannot be applied to '%s'",
			GetUnOpKindString (m_OpKind),
			pOpType->GetTypeString ().cc () // thanks a lot gcc
			);
	}

	err::CError
	SetOperatorError (const CValue& OpValue)
	{
		return SetOperatorError (OpValue.GetType ());
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
