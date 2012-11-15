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
	EUnOp_None = 0,
	EUnOp_Minus,
	EUnOp_BitwiseNot,
	EUnOp_Addr,
	EUnOp_Indir,
	EUnOp_LogicalNot,
	EUnOp_PreInc,
	EUnOp_PreDec,
	EUnOp_PostInc,
	EUnOp_PostDec,
	EUnOp_Ptr,
	EUnOp__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetUnOpString (EUnOp OpKind);

//.............................................................................

enum EOpFlag
{
	EOpFlag_IntegerOnly    = 0x01,
	EOpFlag_LoadReference  = 0x02,
	EOpFlag_EnumToInt      = 0x10,
	EOpFlag_BoolToInt      = 0x20,
	EOpFlag_ArrayToPointer = 0x40,
};

//.............................................................................

[uuid ("5def51c9-959f-4170-a032-892ac4f7e622")]
struct IUnaryOperator: obj::IRoot
{	
protected:
	CModule* m_pModule;
	EUnOp m_OpKind;
	int m_Flags;

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
	GetFlags ()
	{
		return m_Flags;
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
			GetUnOpString (m_OpKind),
			pOpType->GetTypeString ()
			);
	}

};

//.............................................................................

CType*
GetArithmeticOperatorResultTypeKind (CType* pOpType);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CUnaryArithmeticOperatorT: public IUnaryOperator
{
public:
	CUnaryArithmeticOperatorT ()
	{
		m_Flags = EOpFlag_LoadReference | EOpFlag_EnumToInt | EOpFlag_BoolToInt;
	}

	virtual
	bool
	Operator (
		const CValue& RawOpValue,
		CValue* pResultValue
		)
	{
		CType* pType = GetArithmeticOperatorResultTypeKind (RawOpValue.GetType ());
		if (!pType)
		{
			SetOperatorError (RawOpValue.GetType ());
			return false;
		}

		CValue OpValue;
		bool Result = m_pModule->m_OperatorMgr.CastOperator (RawOpValue, pType, &OpValue);
		if (!Result)
			return false;

		if (OpValue.GetValueKind () == EValue_Const)
		{
			EType TypeKind = pType->GetTypeKind ();
			switch (TypeKind)
			{
			case EType_Int32:
			case EType_Int32_u:
				pResultValue->SetConstInt32 (T::ConstOpInt32 (OpValue.GetInt32 ()), pType);
				break;

			case EType_Int64:
			case EType_Int64_u:
				pResultValue->SetConstInt32 (T::ConstOpInt32 (OpValue.GetInt32 ()), pType);
				break;

			case EType_Float:
				pResultValue->SetConstFloat (T::ConstOpFp32 (OpValue.GetFloat ()));
				break;

			case EType_Double:
				pResultValue->SetConstDouble (T::ConstOpFp64 (OpValue.GetDouble ()));
				break;

			default:
				ASSERT (false);
			}
		}
		else
		{
			EType TypeKind = pType->GetTypeKind ();
			switch (TypeKind)
			{
			case EType_Int32:
			case EType_Int32_u:
			case EType_Int64:
			case EType_Int64_u:
				pResultValue->SetLlvmRegister (T::LlvmOpInt (m_pModule, OpValue.GetLlvmValue ()), pType);
				break;

			case EType_Float:
			case EType_Double:
				pResultValue->SetLlvmRegister (T::LlvmOpFp (m_pModule, OpValue.GetLlvmValue ()), pType);
				break;

			default:
				ASSERT (false);
			}
		}

		return true;
	}
};

//.............................................................................

class CUnOp_Minus: public CUnaryArithmeticOperatorT <CUnOp_Minus>
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_Minus, IUnaryOperator)

public:
	CUnOp_Minus ()
	{
		m_OpKind = EUnOp_Minus;
	}

	static
	long
	ConstOpInt32 (long OpValue)
	{
		return -OpValue;
	}

	static
	int64_t
	ConstOpInt64 (int64_t OpValue)
	{
		return -OpValue;
	}

	static
	float
	ConstOpFp32 (float OpValue)
	{
		return -OpValue;
	}

	static
	double
	ConstOpFp64 (double OpValue)
	{
		return -OpValue;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue
		);
};

//.............................................................................

template <typename T>
class CUnOpT_IntegerOnly: public CUnaryArithmeticOperatorT <T>
{
public:
	CUnOpT_IntegerOnly ()
	{
		m_Flags |= EOpFlag_IntegerOnly;
	}

	static
	float
	ConstOpFp32 (float OpValue)
	{
		return 0;
	}

	static
	double
	ConstOpFp64 (double OpValue)
	{
		return 0;
	}

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue
		)
	{
		return NULL;
	}
};

//.............................................................................

class CUnOp_BitwiseNot: public CUnOpT_IntegerOnly <CUnOp_BitwiseNot>
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_BitwiseNot, IUnaryOperator)

public:
	CUnOp_BitwiseNot ()
	{
		m_OpKind = EUnOp_BitwiseNot;
	};

	static
	long
	ConstOpInt32 (long OpValue)
	{
		return ~OpValue;
	}

	static
	int64_t
	ConstOpInt64 (int64_t OpValue)
	{
		return ~OpValue;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue
		);

};

//.............................................................................

class CUnOp_Addr: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_Addr, IUnaryOperator)

public:
	CUnOp_Addr ()
	{
		m_OpKind = EUnOp_Addr;
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
		m_Flags |= EOpFlag_LoadReference;
	}

	virtual
	bool
	Operator (
		const CValue& OpValue,
		CValue* pResultValue
		);
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
	}

	virtual
	bool
	Operator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		CValue One;
		One.SetConstInt32 (1);
		EBinOp BinOpKind = UnOpKind == EUnOp_PreInc ? EBinOp_Add : EBinOp_Sub;
		
		bool Result = m_pModule->m_OperatorMgr.MoveOperator (One, OpValue, BinOpKind);
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
	}

	virtual
	bool
	Operator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		CValue One;
		One.SetConstInt32 (1);
		EBinOp BinOpKind = UnOpKind == EUnOp_PostInc ? EBinOp_Add : EBinOp_Sub;
		
		bool Result = m_pModule->m_OperatorMgr.MoveOperator (One, OpValue, BinOpKind);
		if (!Result)
			return false;

		*pResultValue = OpValue;
		return true;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
