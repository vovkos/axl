// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BinOp.h"

namespace axl {
namespace jnc {

//.............................................................................

template <typename T>
class CBinOpT_Cmp: public IBinaryOperator
{
public:
	virtual
	CType*
	GetResultType (
		const CValue& OpValue1,
		const CValue& OpValue2
		)
	{
		return m_pModule->m_TypeMgr.GetPrimitiveType (EType_Bool);
	}

	virtual
	bool
	Operator (
		const CValue& RawOpValue1,
		const CValue& RawOpValue2,
		CValue* pResultValue
		)
	{
		CType* pType = GetArithmeticOperatorResultTypeKind (RawOpValue1, RawOpValue2);
		if (!pType)
		{
			SetOperatorError (RawOpValue1, RawOpValue2);
			return false;
		}

		CValue OpValue1;
		CValue OpValue2;

		bool Result = 
			m_pModule->m_OperatorMgr.CastOperator (RawOpValue1, pType, &OpValue1) &&
			m_pModule->m_OperatorMgr.CastOperator (RawOpValue2, pType, &OpValue2);
		
		if (!Result)
			return false;

		if (OpValue1.GetValueKind () == EValue_Const && OpValue2.GetValueKind () == EValue_Const)
		{
			EType TypeKind = pType->GetTypeKind ();
			switch (TypeKind)
			{
			case EType_Int32:
			case EType_Int32_u:
				pResultValue->SetConstBool (
					T::ConstOpInt32 (
						OpValue1.GetInt32 (), 
						OpValue2.GetInt32 (), 
						pType->IsUnsignedIntegerType ()
						)
					);
				break;

			case EType_Int64:
			case EType_Int64_u:
				pResultValue->SetConstBool (
					T::ConstOpInt32 (
						OpValue1.GetInt32 (), 
						OpValue2.GetInt32 (), 
						pType->IsUnsignedIntegerType ()
						)
					);
				break;

			case EType_Float:
				pResultValue->SetConstBool (T::ConstOpFp32 (OpValue1.GetFloat (), OpValue2.GetFloat ()));
				break;

			case EType_Double:
				pResultValue->SetConstBool (T::ConstOpFp64 (OpValue1.GetDouble (), OpValue2.GetDouble ()));
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
				T::LlvmOpInt (
					m_pModule, 
					OpValue1, 
					OpValue2, 
					pResultValue,
					pType->IsUnsignedIntegerType ()
					);
				break;

			case EType_Float:
			case EType_Double:
				T::LlvmOpFp (
					m_pModule, 
					OpValue1, 
					OpValue2,
					pResultValue
					);
				break;

			default:
				ASSERT (false);
			}
		}

		return true;
	}
};

//.............................................................................

class CBinOp_Eq: public CBinOpT_Cmp <CBinOp_Eq>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Eq, IBinaryOperator)

public:
	CBinOp_Eq ()
	{
		m_OpKind = EBinOp_Eq;
	}

	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return OpValue1 == OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return OpValue1 == OpValue2;
	}

	static
	bool
	ConstOpFp32 (
		float OpValue1,
		float OpValue2
		)
	{
		return OpValue1 == OpValue2;
	}

	static
	bool
	ConstOpFp64 (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 == OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

class CBinOp_Ne: public CBinOpT_Cmp <CBinOp_Ne>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Ne, IBinaryOperator)

public:
	CBinOp_Ne ()
	{
		m_OpKind = EBinOp_Ne;
	}

	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return OpValue1 != OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return OpValue1 != OpValue2;
	}

	static
	bool
	ConstOpFp32 (
		float OpValue1,
		float OpValue2
		)
	{
		return OpValue1 != OpValue2;
	}

	static
	bool
	ConstOpFp64 (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 != OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

class CBinOp_Lt: public CBinOpT_Cmp <CBinOp_Lt>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Lt, IBinaryOperator)

public:
	CBinOp_Lt ()
	{
		m_OpKind = EBinOp_Lt;
	}

	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return IsUnsigned ? (ulong_t) OpValue1 < (ulong_t) OpValue2 : OpValue1 < OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return IsUnsigned ? (uint64_t) OpValue1 < (uint64_t) OpValue2 : OpValue1 < OpValue2;
	}

	static
	bool
	ConstOpFp32 (
		float OpValue1,
		float OpValue2
		)
	{
		return OpValue1 < OpValue2;
	}

	static
	bool
	ConstOpFp64 (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 < OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

class CBinOp_Le: public CBinOpT_Cmp <CBinOp_Le>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Le, IBinaryOperator)

public:
	CBinOp_Le ()
	{
		m_OpKind = EBinOp_Le;
	}

	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return IsUnsigned ? (ulong_t) OpValue1 <= (ulong_t) OpValue2 : OpValue1 <= OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return IsUnsigned ? (uint64_t) OpValue1 <= (uint64_t) OpValue2 : OpValue1 <= OpValue2;
	}

	static
	bool
	ConstOpFp32 (
		float OpValue1,
		float OpValue2
		)
	{
		return OpValue1 <= OpValue2;
	}

	static
	bool
	ConstOpFp64 (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 <= OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

class CBinOp_Gt: public CBinOpT_Cmp <CBinOp_Gt>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Gt, IBinaryOperator)

public:
	CBinOp_Gt ()
	{
		m_OpKind = EBinOp_Gt;
	}

	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return IsUnsigned ? (ulong_t) OpValue1 > (ulong_t) OpValue2 : OpValue1 > OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return IsUnsigned ? (uint64_t) OpValue1 > (uint64_t) OpValue2 : OpValue1 > OpValue2;
	}

	static
	bool
	ConstOpFp32 (
		float OpValue1,
		float OpValue2
		)
	{
		return OpValue1 > OpValue2;
	}

	static
	bool
	ConstOpFp64 (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 > OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

class CBinOp_Ge: public CBinOpT_Cmp <CBinOp_Ge>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Ge, IBinaryOperator)

public:
	CBinOp_Ge ()
	{
		m_OpKind = EBinOp_Ge;
	}

	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return IsUnsigned ? (ulong_t) OpValue1 >= (ulong_t) OpValue2 : OpValue1 >= OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return IsUnsigned ? (uint64_t) OpValue1 >= (uint64_t) OpValue2 : OpValue1 >= OpValue2;
	}

	static
	bool
	ConstOpFp32 (
		float OpValue1,
		float OpValue2
		)
	{
		return OpValue1 >= OpValue2;
	}

	static
	bool
	ConstOpFp64 (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 >= OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
