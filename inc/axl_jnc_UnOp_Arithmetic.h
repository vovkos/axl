// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_UnOp.h"

namespace axl {
namespace jnc {

//.............................................................................

CType*
GetArithmeticOperatorResultTypeKind (CType* pOpType);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CUnOpT_Arithmetic: public IUnaryOperator
{
public:
	enum
	{
		IsIntegerOnly = false
	};

public:
	virtual
	bool
	Operator (
		const CValue& RawOpValue,
		CValue* pResultValue
		)
	{
		CType* pType = GetArithmeticOperatorResultTypeKind (RawOpValue.GetType ());
		if (!pType || T::IsIntegerOnly && !pType->IsIntegerType ())
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
				T::LlvmOpInt (m_pModule, OpValue, pType, pResultValue);
				break;

			case EType_Float:
			case EType_Double:
				T::LlvmOpFp (m_pModule, OpValue, pType, pResultValue);
				break;

			default:
				ASSERT (false);
			}

			if (!Result)
				return false;
		}

		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CUnOpT_IntegerOnly: public CUnOpT_Arithmetic <T>
{
public:
	enum
	{
		IsIntegerOnly = true
	};

public:
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
		const CValue& OpValue,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		ASSERT (false);
		return NULL;
	}
};

//.............................................................................

class CUnOp_Plus: public CUnOpT_Arithmetic <CUnOp_Plus>
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_Plus, IUnaryOperator)

public:
	CUnOp_Plus ()
	{
		m_OpKind = EUnOp_Plus;
	}

	static
	long
	ConstOpInt32 (long OpValue)
	{
		return +OpValue;
	}

	static
	int64_t
	ConstOpInt64 (int64_t OpValue)
	{
		return +OpValue;
	}

	static
	float
	ConstOpFp32 (float OpValue)
	{
		return +OpValue;
	}

	static
	double
	ConstOpFp64 (double OpValue)
	{
		return +OpValue;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		*pResultValue = OpValue;
		return pResultValue->GetLlvmValue ();
	}

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		*pResultValue = OpValue;
		return pResultValue->GetLlvmValue ();
	}
};

//.............................................................................

class CUnOp_Minus: public CUnOpT_Arithmetic <CUnOp_Minus>
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
		const CValue& OpValue,
		CType* pResultType,
		CValue* pResultValue
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue,
		CType* pResultType,
		CValue* pResultValue
		);
};

//.............................................................................

class CUnOp_BwNot: public CUnOpT_IntegerOnly <CUnOp_BwNot>
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_BwNot, IUnaryOperator)

public:
	CUnOp_BwNot ()
	{
		m_OpKind = EUnOp_BwNot;
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
		const CValue& OpValue,
		CType* pResultType,
		CValue* pResultValue
		);

};

//.............................................................................

} // namespace jnc {
} // namespace axl {
