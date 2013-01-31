// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BinOp.h"

namespace axl {
namespace jnc {

//.............................................................................

template <typename T>
class CBinOpT_Arithmetic: public IBinaryOperator
{
public:
	enum
	{
		IsIntegerOnly = false
	};

public:
	CBinOpT_Arithmetic ()
	{
		m_OpFlags1 = EOpFlag_Arithmetic;
		m_OpFlags2 = EOpFlag_Arithmetic;
	}

	virtual
	bool
	Operator (
		const CValue& RawOpValue1,
		const CValue& RawOpValue2,
		CValue* pResultValue
		)
	{
		CType* pOpType1 = RawOpValue1.GetType ();
		CType* pOpType2 = RawOpValue2.GetType ();
		CType* pMaxOpType = pOpType1->GetTypeKind () > pOpType2->GetTypeKind () ? pOpType1 : pOpType2;

		CType* pType = GetArithmeticOperatorResultTypeKind (pMaxOpType);
		if (!pType || T::IsIntegerOnly && !pType->IsIntegerType ())
		{
			SetOperatorError (pOpType1, pOpType2);
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
				pResultValue->SetConstInt32 (
					T::ConstOpInt32 (
						OpValue1.GetInt32 (), 
						OpValue2.GetInt32 (), 
						pType->IsUnsignedType ()
						), 
					pType
					);
				break;

			case EType_Int64:
			case EType_Int64_u:
				pResultValue->SetConstInt32 (
					T::ConstOpInt32 (
						OpValue1.GetInt32 (), 
						OpValue2.GetInt32 (), 
						pType->IsUnsignedType ()
						), 
					pType
					);
				break;

			case EType_Float:
				pResultValue->SetConstFloat (T::ConstOpFp32 (OpValue1.GetFloat (), OpValue2.GetFloat ()));
				break;

			case EType_Double:
				pResultValue->SetConstDouble (T::ConstOpFp64 (OpValue1.GetDouble (), OpValue2.GetDouble ()));
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
					pType,
					pResultValue,
					pType->IsUnsignedType ()
					);
				break;

			case EType_Float:
			case EType_Double:
				T::LlvmOpFp (
					m_pModule, 
					OpValue1, 
					OpValue2,
					pType,
					pResultValue
					);
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
class CBinOpT_IntegerOnly: public CBinOpT_Arithmetic <T>
{	
public:
	enum
	{
		IsIntegerOnly = true
	};

public:
	static
	float
	ConstOpFp32 (
		float OpValue1,
		float OpValue2
		)
	{
		return 0;
	}

	static
	double
	ConstOpFp64 (
		double OpValue1,
		double OpValue2
		)
	{
		return 0;
	}

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		ASSERT (false);
		return NULL;
	}
};

//.............................................................................

template <typename T>
class CBinOpT_AddSub: public CBinOpT_Arithmetic <T>
{
protected:
	bool
	PointerIncrementOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		bool Result;

		CValue SizeValue;
		CValue IncrementValue;
		CValue PtrValue;

		CDataPtrType* pType = (CDataPtrType*) OpValue1.GetType ();
		if (pType->GetTypeKind () != EType_DataPtr)
		{
			SetOperatorError (OpValue1.GetType (), OpValue2.GetType ());
			return false;
		}

		if (pType->GetPtrTypeKind () == EDataPtrType_Unsafe)
		{
			m_pModule->m_LlvmBuilder.CreateGep (OpValue1, OpValue2, pType, pResultValue);
			return true;
		}

		SizeValue.SetConstSizeT (pType->GetDataType ()->GetSize ());
		
		Result = m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Mul, OpValue2, SizeValue, &IncrementValue);
		if (!Result)
			return false;

		m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue1, 0, NULL, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateGep (PtrValue, IncrementValue, NULL, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateInsertValue (OpValue1, PtrValue, 0, pType, pResultValue);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Add: public CBinOpT_AddSub <CBinOp_Add>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Add, IBinaryOperator)

public:
	CBinOp_Add ()
	{
		m_OpKind = EBinOp_Add;
	}

	virtual
	bool
	Operator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);

	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return OpValue1 + OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return OpValue1 + OpValue2;
	}

	static
	float
	ConstOpFp32 (
		float OpValue1,
		float OpValue2
		)
	{
		return OpValue1 + OpValue2;
	}

	static
	double
	ConstOpFp64 (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 + OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Sub: public CBinOpT_AddSub <CBinOp_Sub>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Sub, IBinaryOperator)

public:
	CBinOp_Sub ()
	{
		m_OpKind = EBinOp_Sub;
	}

	virtual
	bool
	Operator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);

	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return OpValue1 - OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return OpValue1 - OpValue2;
	}

	static
	float
	ConstOpFp32 (
		float OpValue1,
		float OpValue2
		)
	{
		return OpValue1 - OpValue2;
	}

	static
	double
	ConstOpFp64 (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 - OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		);

protected:
	bool
	PointerDifferenceOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);
};

//.............................................................................

class CBinOp_Mul: public CBinOpT_Arithmetic <CBinOp_Mul>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Mul, IBinaryOperator)

public:
	CBinOp_Mul ()
	{
		m_OpKind = EBinOp_Mul;
	}

	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return OpValue1 * OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return OpValue1 * OpValue2;
	}

	static
	float
	ConstOpFp32 (
		float OpValue1,
		float OpValue2
		)
	{
		return OpValue1 * OpValue2;
	}

	static
	double
	ConstOpFp64 (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 * OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Div: public CBinOpT_Arithmetic <CBinOp_Div>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Div, IBinaryOperator)

public:
	CBinOp_Div ()
	{
		m_OpKind = EBinOp_Div;
	}

	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return IsUnsigned ? (ulong_t) OpValue1 / (ulong_t) OpValue2 : OpValue1 / OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return IsUnsigned ? (uint64_t) OpValue1 / (uint64_t) OpValue2 : OpValue1 / OpValue2;
	}

	static
	float
	ConstOpFp32 (
		float OpValue1,
		float OpValue2
		)
	{
		return OpValue1 / OpValue2;
	}

	static
	double
	ConstOpFp64 (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 / OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Mod: public CBinOpT_IntegerOnly <CBinOp_Mod>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Mod, IBinaryOperator)

public:
	CBinOp_Mod ()
	{
		m_OpKind = EBinOp_Mod;
	}

	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return IsUnsigned ? (ulong_t) OpValue1 % (ulong_t) OpValue2 : OpValue1 % OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return IsUnsigned ? (uint64_t) OpValue1 % (uint64_t) OpValue2 : OpValue1 % OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue,
		bool IsUnsigned
		);
};

//.............................................................................

class CBinOp_Shl: public CBinOpT_IntegerOnly <CBinOp_Shl>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Shl, IBinaryOperator)

public:
	CBinOp_Shl ()
	{
		m_OpKind = EBinOp_Shl;
	}

	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return OpValue1 << OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return OpValue1 << OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue,
		bool IsUnsigned
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Shr: public CBinOpT_IntegerOnly <CBinOp_Shr>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_Shr, IBinaryOperator)

public:
	CBinOp_Shr ()
	{
		m_OpKind = EBinOp_Shr;
	}

	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return OpValue1 >> OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return OpValue1 >> OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue,
		bool IsUnsigned
		);
};

//.............................................................................

class CBinOp_BwAnd: public CBinOpT_IntegerOnly <CBinOp_BwAnd>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_BwAnd, IBinaryOperator)

public:
	CBinOp_BwAnd ()
	{
		m_OpKind = EBinOp_BwAnd;
	}

	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return OpValue1 & OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return OpValue1 & OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue,
		bool IsUnsigned
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_BwOr: public CBinOpT_IntegerOnly <CBinOp_BwOr>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_BwOr, IBinaryOperator)

public:
	CBinOp_BwOr ()
	{
		m_OpKind = EBinOp_BwOr;
	}

	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return OpValue1 | OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return OpValue1 | OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue,
		bool IsUnsigned
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_BwXor: public CBinOpT_IntegerOnly <CBinOp_BwXor>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_BwXor, IBinaryOperator)

public:
	CBinOp_BwXor ()
	{
		m_OpKind = EBinOp_BwXor;
	}

	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2,
		bool IsUnsigned
		) 
	{
		return OpValue1 ^ OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2,
		bool IsUnsigned
		)
	{
		return OpValue1 ^ OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue,
		bool IsUnsigned
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
