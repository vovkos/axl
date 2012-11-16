// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_UnaryOperator.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EBinOp
{
	EBinOp_None = 0,
	EBinOp_Add,
	EBinOp_Sub,
	EBinOp_Mul,
	EBinOp_Div,
	EBinOp_Mod,	
	EBinOp_Shl,
	EBinOp_Shr,	
	EBinOp_BitwiseAnd,
	EBinOp_BitwiseOr,
	EBinOp_BitwiseXor,	
	EBinOp_Eq,
	EBinOp_Ne,
	EBinOp_Lt,
	EBinOp_Le,
	EBinOp_Gt,
	EBinOp_Ge,
	EBinOp_LogicalAnd,
	EBinOp_LogicalOr,
	EBinOp_Idx,

	EBinOp__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetBinOpString (EBinOp OpKind);

//.............................................................................

[uuid ("e205fc7b-e75e-4cc6-88f7-336701c9645a")]
struct IBinaryOperator: obj::IRoot
{	
protected:
	CModule* m_pModule;
	EBinOp m_OpKind;
	int m_Flags;

public:
	IBinaryOperator ();

	CModule*
	GetModule ()
	{
		return m_pModule;
	}

	EBinOp 
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
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		) = 0;

	err::CError
	SetOperatorError (		
		CType* pOpType1,
		CType* pOpType2
		)
	{
		return err::SetFormatStringError (
			_T("binary '%s' cannot be applied to '%s' and '%s'"),
			GetBinOpString (m_OpKind),
			pOpType1->GetTypeString (),
			pOpType2->GetTypeString ()
			);
	}
};

//.............................................................................

template <typename T>
class CBinaryArithmeticOperatorT: public IBinaryOperator
{
public:
	CBinaryArithmeticOperatorT ()
	{
		m_Flags = EOpFlag_LoadReference | EOpFlag_EnumToInt | EOpFlag_BoolToInt;
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
		if (!pType)
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
				pResultValue->SetLlvmRegister (
					T::LlvmOpInt (
						m_pModule, 
						OpValue1.GetLlvmValue (), 
						OpValue2.GetLlvmValue (), 
						pType->IsUnsignedType ()
						), 
					pType
					);
				break;

			case EType_Float:
			case EType_Double:
				pResultValue->SetLlvmRegister (
					T::LlvmOpFp (
						m_pModule, 
						OpValue1.GetLlvmValue (), 
						OpValue2.GetLlvmValue ()
						), 
					pType
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

template <typename T>
class CBinaryRelationalOperatorT: public IBinaryOperator
{
public:
	CBinaryRelationalOperatorT ()
	{
		m_Flags = EOpFlag_LoadReference | EOpFlag_EnumToInt | EOpFlag_BoolToInt;
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
		if (!pType)
		{
			err::SetFormatStringError (
				_T("relational '%s' cannot be applied to '%s' and '%s'"),
				GetBinOpString (m_OpKind),
				pOpType1->GetTypeString (),
				pOpType2->GetTypeString ()
				);
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
						pType->IsUnsignedType ()
						)
					);
				break;

			case EType_Int64:
			case EType_Int64_u:
				pResultValue->SetConstBool (
					T::ConstOpInt32 (
						OpValue1.GetInt32 (), 
						OpValue2.GetInt32 (), 
						pType->IsUnsignedType ()
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
				pResultValue->SetLlvmRegister (
					T::LlvmOpInt (
						m_pModule, 
						OpValue1.GetLlvmValue (), 
						OpValue2.GetLlvmValue (), 
						pType->IsUnsignedType ()
						), 
					EType_Bool
					);
				break;

			case EType_Float:
			case EType_Double:
				pResultValue->SetLlvmRegister (
					T::LlvmOpFp (
						m_pModule, 
						OpValue1.GetLlvmValue (), 
						OpValue2.GetLlvmValue ()
						), 
					EType_Bool
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

template <typename T>
class CBinOpT_AddSub: public CBinaryArithmeticOperatorT <T>
{
protected:
	CBinOpT_AddSub ()
	{
		m_Flags |= EOpFlag_ArrayToPointer;
	}

	bool
	PointerIncrementOperator (
		const CValue& PointerValue,
		const CValue& RawIncrementValue,
		CValue* pResultValue
		)
	{
		bool Result;

		CValue IncrementValue = RawIncrementValue;
		CValue SizeValue;

		llvm::Value* pLlvmValue;

		EType PointerTypeKind = PointerValue.GetType ()->GetTypeKind ();
		switch (PointerTypeKind)
		{
		case EType_Pointer:
			SizeValue.SetConstSizeT (((CPointerType*) PointerValue.GetType ())->GetBaseType ()->GetSize ());
		
			Result = m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Mul, &IncrementValue, SizeValue);
			if (!Result)
				return false;

			pLlvmValue = m_pModule->m_LlvmBuilder.CreateExtractValue (PointerValue.GetLlvmValue (), 0, "sptr_ptr");
			pLlvmValue = m_pModule->m_LlvmBuilder.CreateGEP (pLlvmValue, IncrementValue.GetLlvmValue (), "sptr_ptr_inc");
			pLlvmValue = m_pModule->m_LlvmBuilder.CreateInsertValue (PointerValue.GetLlvmValue (), pLlvmValue, 0, "sptr_ptr");
			break;

		case EType_Pointer_u:
			pLlvmValue = m_pModule->m_LlvmBuilder.CreateGEP (PointerValue.GetLlvmValue (), IncrementValue.GetLlvmValue (), "ptr_inc");
			break;

		default:
			SetOperatorError (PointerValue.GetType (), IncrementValue.GetType ());
			return false;
		}

		pResultValue->SetLlvmRegister (pLlvmValue, PointerValue.GetType ());
		return true;
	}
};

//.............................................................................

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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//.............................................................................

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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
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

class CBinOp_Mul: public CBinaryArithmeticOperatorT <CBinOp_Mul>
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//.............................................................................

class CBinOp_Div: public CBinaryArithmeticOperatorT <CBinOp_Div>
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//.............................................................................

template <typename T>
class CBinOpT_IntegerOnly: public CBinaryArithmeticOperatorT <T>
{	
public:
	CBinOpT_IntegerOnly ()
	{
		 m_Flags |= EOpFlag_IntegerOnly;
	}

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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return NULL;
	}
};

//.............................................................................

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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);
};

//.............................................................................

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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);
};

//.............................................................................

class CBinOp_BitwiseAnd: public CBinOpT_IntegerOnly <CBinOp_BitwiseAnd>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_BitwiseAnd, IBinaryOperator)

public:
	CBinOp_BitwiseAnd ()
	{
		m_OpKind = EBinOp_BitwiseAnd;
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);
};

//.............................................................................

class CBinOp_BitwiseOr: public CBinOpT_IntegerOnly <CBinOp_BitwiseOr>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_BitwiseOr, IBinaryOperator)

public:
	CBinOp_BitwiseOr ()
	{
		m_OpKind = EBinOp_BitwiseOr;
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_BitwiseXor: public CBinOpT_IntegerOnly <CBinOp_BitwiseXor>
{	
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOp_BitwiseXor, IBinaryOperator)

public:
	CBinOp_BitwiseXor ()
	{
		m_OpKind = EBinOp_BitwiseXor;
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);
};

//.............................................................................

class CBinOp_Eq: public CBinaryRelationalOperatorT <CBinOp_Eq>
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//.............................................................................

class CBinOp_Ne: public CBinaryRelationalOperatorT <CBinOp_Ne>
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//.............................................................................

class CBinOp_Lt: public CBinaryRelationalOperatorT <CBinOp_Lt>
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//.............................................................................

class CBinOp_Le: public CBinaryRelationalOperatorT <CBinOp_Le>
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//.............................................................................

class CBinOp_Gt: public CBinaryRelationalOperatorT <CBinOp_Gt>
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//.............................................................................

class CBinOp_Ge: public CBinaryRelationalOperatorT <CBinOp_Ge>
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
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2,
		bool IsUnsigned
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
