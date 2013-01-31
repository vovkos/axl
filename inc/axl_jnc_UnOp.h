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
	EOpFlag_KeepDataRef     = 0x01,
	EOpFlag_KeepPropertyRef = 0x02,
	EOpFlag_EnumToInt       = 0x10,
	EOpFlag_BitFieldToInt   = 0x20,
	EOpFlag_BoolToInt       = 0x40,
	EOpFlag_Arithmetic      = EOpFlag_EnumToInt | EOpFlag_BitFieldToInt | EOpFlag_BoolToInt
};

//.............................................................................

[uuid ("5def51c9-959f-4170-a032-892ac4f7e622")]
struct IUnaryOperator: obj::IRoot
{	
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

template <EUnOp UnOpKind>
class CUnOpT_PreInc: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOpT_PreInc, IUnaryOperator)

public:
	CUnOpT_PreInc ()
	{
		m_OpKind = UnOpKind;
		m_OpFlags |= EOpFlag_KeepDataRef;
	}

	virtual
	bool
	Operator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		CValue OneValue;
		OneValue.SetConstInt32 (1);
		EBinOp BinOpKind = UnOpKind == EUnOp_PreInc ? EBinOp_Add : EBinOp_Sub;
		
		bool Result = m_pModule->m_OperatorMgr.BinOpMoveOperator (OneValue, OpValue, BinOpKind);
		if (!Result)
			return false;

		*pResultValue = OpValue;
		return true;
	}
};

//.............................................................................

template <EUnOp UnOpKind>
class CUnOpT_PostInc: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOpT_PostInc, IUnaryOperator)

public:
	CUnOpT_PostInc ()
	{
		m_OpKind = UnOpKind;
		m_OpFlags |= EOpFlag_KeepDataRef;
	}

	virtual
	bool
	Operator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		bool Result;

		CValue OldValue;
		Result = m_pModule->m_OperatorMgr.PrepareOperand (OpValue, &OldValue);
		if (!Result)
			return false;

		CValue OneValue;
		OneValue.SetConstInt32 (1);
		EBinOp BinOpKind = UnOpKind == EUnOp_PostInc ? EBinOp_Add : EBinOp_Sub;
		
		Result = m_pModule->m_OperatorMgr.BinOpMoveOperator (OneValue, OpValue, BinOpKind);
		if (!Result)
			return false;

		*pResultValue = OldValue;
		return true;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
