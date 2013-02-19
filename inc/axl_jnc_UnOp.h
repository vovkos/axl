// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
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

const tchar_t*
GetUnOpKindString (EUnOp OpKind);

//.............................................................................

enum EOpFlag
{
	EOpFlag_KeepDataRef      = 0x01,
	EOpFlag_KeepPropertyRef  = 0x02,
	EOpFlag_KeepArrayRef     = 0x04,
	EOpFlag_KeepBool         = 0x10,
	EOpFlag_KeepEnum         = 0x20,
	EOpFlag_PropertyRefToPtr = 0x40,

	EOpFlag_KeepRef          = EOpFlag_KeepDataRef | EOpFlag_KeepPropertyRef,
};

//.............................................................................

[uuid ("5def51c9-959f-4170-a032-892ac4f7e622")]
struct IUnaryOperator: obj::IRoot
{	
protected:
	friend class COperatorMgr;

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
			_T("unary '%s' cannot be applied to '%s'"),
			GetUnOpKindString (m_OpKind),
			pOpType->GetTypeString ()
			);
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
