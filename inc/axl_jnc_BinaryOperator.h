// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"

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
GetBinOpString (EBinOp Op);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

[uuid ("e205fc7b-e75e-4cc6-88f7-336701c9645a")]
struct IBinaryOperatorLo: obj::IRoot
{	
public:
	virtual
	bool
	ConstOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue	
		) = 0;

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue	
		) = 0;
};

//.............................................................................

class CBinaryOperator: public rtl::TListLink
{
protected:
	friend class CBinaryOperatorOverload;

	EBinOp m_OpKind;
	CType* m_pReturnType;
	CType* m_pOpType1;
	CType* m_pOpType2;
	IBinaryOperatorLo* m_pOperatorLo;

public:
	CBinaryOperator ();

	EBinOp 
	GetOpKind ()
	{
		return m_OpKind;
	}

	CType* 
	GetReturnType () 
	{
		return m_pReturnType;
	}

	CType* 
	GetOpType1 ()
	{
		return m_pOpType1;
	}

	CType* 
	GetOpType2 ()
	{
		return m_pOpType2;
	}

	bool
	Operator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pReturnValue		
		);

	bool
	Operator (
		CValue* pValue,
		const CValue& OpValue2
		);
};

//.............................................................................

class CBinaryOperatorOverload
{
protected:
	EBinOp m_OpKind;
	rtl::CStdListT <CBinaryOperator> m_List;
	rtl::CBoxListT <rtl::CString> m_SignatureCache;
	rtl::CStringHashTableMapAT <CBinaryOperator*> m_DirectMap;
	rtl::CStringHashTableMapAT <CBinaryOperator*> m_ImplicitMap;

public:
	CBinaryOperatorOverload ()
	{
		m_OpKind = EBinOp_None;
	}

	void
	Clear ();

	CBinaryOperator*
	FindOperator (
		CType* pOpType1,
		CType* pOpType2
		);

	CBinaryOperator*
	AddOperator (
		CType* pReturnType,
		CType* pOpType1,
		CType* pOpType2,
		IBinaryOperatorLo* pOperatorLo
		);
};

//.............................................................................

template <typename T>
class CBinOpT_i32: public IBinaryOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_i32, IBinaryOperatorLo)

public:
	virtual
	bool
	ConstOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		long Integer = T::ConstOpInt32 (OpValue1.GetInt32 (), OpValue2.GetInt32 ());
		pResultValue->SetConstInt32 (Integer);
		return true;
	}

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		llvm::Instruction* pLlvmValue = T::LlvmOpInt (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue ());
		pResultValue->SetLlvmValue (pLlvmValue, EType_Int32);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CBinOpT_i64: public IBinaryOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_i64, IBinaryOperatorLo)

public:
	virtual
	bool
	ConstOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		int64_t Integer = T::ConstOpInt64 (OpValue1.GetInt64 (), OpValue2.GetInt64 ());
		pResultValue->SetConstInt64 (Integer);
		return true;
	}

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		llvm::Instruction* pLlvmValue = T::LlvmOpInt (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue ());
		pResultValue->SetLlvmValue (pLlvmValue, EType_Int64);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CBinOpT_f32: public IBinaryOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_f32, IBinaryOperatorLo)

public:
	virtual
	bool
	ConstOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		float Float = T::ConstOpFp32 (OpValue1.GetFloat (), OpValue2.GetFloat ());
		pResultValue->SetConstFloat (Float);
		return true;
	}

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		llvm::Instruction* pLlvmValue = T::LlvmOpFp (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue ());
		pResultValue->SetLlvmValue (pLlvmValue, EType_Float);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CBinOpT_f64: public IBinaryOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_f64, IBinaryOperatorLo)

public:
	virtual
	bool
	ConstOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		double Double = T::ConstOpFp64 (OpValue1.GetDouble (), OpValue2.GetDouble ());
		pResultValue->SetConstDouble (Double);
		return true;
	}

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		llvm::Instruction* pLlvmValue = T::LlvmOpFp (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue ());
		pResultValue->SetLlvmValue (pLlvmValue, EType_Double);
		return true;
	}
};

//.............................................................................

class CBinOp_Add
{	
public:
	enum
	{
		 OpKind = EBinOp_Add
	};

public:
	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 + OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
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
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateAdd (pOpValue1, pOpValue2);
	}

	static
	llvm::Instruction*
	LlvmOpFp (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateFAdd (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CBinOp_Sub
{	
public:
	enum
	{
		 OpKind = EBinOp_Add
	};

public:
	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 - OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
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
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateSub (pOpValue1, pOpValue2);
	}

	static
	llvm::Instruction*
	LlvmOpFp (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateFSub (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CBinOp_Mul
{	
public:
	enum
	{
		 OpKind = EBinOp_Mul
	};

public:
	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 * OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
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
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateMul (pOpValue1, pOpValue2);
	}

	static
	llvm::Instruction*
	LlvmOpFp (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateFMul (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CBinOp_Div
{	
public:
	enum
	{
		 OpKind = EBinOp_Div
	};

public:
	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 / OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 / OpValue2;
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
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateSDiv (pOpValue1, pOpValue2);
	}

	static
	llvm::Instruction*
	LlvmOpFp (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateFDiv (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CBinOp_Mod
{	
public:
	enum
	{
		 OpKind = EBinOp_Mod
	};

public:
	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 % OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 % OpValue2;
	}

	static
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateSRem (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CBinOp_DivU
{	
public:
	enum
	{
		 OpKind = EBinOp_Div
	};

public:
	static
	ulong_t
	ConstOpInt32 (
		ulong_t OpValue1,
		ulong_t OpValue2
		) 
	{
		return OpValue1 / OpValue2;
	}

	static
	uint64_t
	ConstOpInt64 (
		uint64_t OpValue1,
		uint64_t OpValue2
		)
	{
		return OpValue1 / OpValue2;
	}

	static
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateUDiv (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CBinOp_ModU
{	
public:
	enum
	{
		 OpKind = EBinOp_Mod
	};

public:
	static
	ulong_t
	ConstOpInt32 (
		ulong_t OpValue1,
		ulong_t OpValue2
		) 
	{
		return OpValue1 % OpValue2;
	}

	static
	uint64_t
	ConstOpInt64 (
		uint64_t OpValue1,
		uint64_t OpValue2
		)
	{
		return OpValue1 % OpValue2;
	}

	static
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateURem (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CBinOp_Shl
{	
public:
	enum
	{
		 OpKind = EBinOp_Shl
	};

public:
	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 << OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 << OpValue2;
	}

	static
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateShl (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CBinOp_Shr
{	
public:
	enum
	{
		 OpKind = EBinOp_Shr
	};

public:
	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 >> OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 >> OpValue2;
	}

	static
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateLShr (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CBinOp_BitwiseAnd
{	
public:
	enum
	{
		 OpKind = EBinOp_BitwiseAnd
	};

public:
	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 & OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 & OpValue2;
	}

	static
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateAnd (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CBinOp_BitwiseOr
{	
public:
	enum
	{
		 OpKind = EBinOp_BitwiseOr
	};

public:
	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 | OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 | OpValue2;
	}

	static
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateOr (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CBinOp_BitwiseXor
{	
public:
	enum
	{
		 OpKind = EBinOp_BitwiseXor
	};

public:
	static
	long
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 ^ OpValue2;
	}

	static
	int64_t
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 ^ OpValue2;
	}

	static
	llvm::Instruction*
	LlvmOpInt (
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		)
	{
		return llvm::BinaryOperator::CreateXor (pOpValue1, pOpValue2);
	}
};

//.............................................................................

class CRelOp_Eq
{	
public:
	enum
	{
		OpKind = EBinOp_Eq
	};

public:
	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 == OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 == OpValue2;
	}

	static
	bool
	ConstOpFp (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 == OpValue2;
	}
};

//.............................................................................

class CRelOp_Ne
{	
public:
	enum
	{
		OpKind = EBinOp_Ne
	};

public:
	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 != OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 != OpValue2;
	}

	static
	bool
	ConstOpFp (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 != OpValue2;
	}
};

//.............................................................................

class CRelOp_Lt
{	
public:
	enum
	{
		OpKind = EBinOp_Lt
	};

public:
	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 < OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 < OpValue2;
	}

	static
	bool
	ConstOpFp (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 < OpValue2;
	}
};

//.............................................................................

class CRelOp_Le
{	
public:
	enum
	{
		OpKind = EBinOp_Le
	};

public:
	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 <= OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 <= OpValue2;
	}

	static
	bool
	ConstOpFp (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 <= OpValue2;
	}
};

//.............................................................................

class CBinOp_Gt
{	
public:
	enum
	{
		OpKind = EBinOp_Gt
	};

public:
	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 > OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 > OpValue2;
	}

	static
	bool
	ConstOpFp (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 > OpValue2;
	}
};

//.............................................................................

class CBinOp_Ge
{	
public:
	enum
	{
		OpKind = EBinOp_Ge
	};

public:
	static
	bool
	ConstOpInt32 (
		long OpValue1,
		long OpValue2
		) 
	{
		return OpValue1 >= OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		int64_t OpValue1,
		int64_t OpValue2
		)
	{
		return OpValue1 >= OpValue2;
	}

	static
	bool
	ConstOpFp (
		double OpValue1,
		double OpValue2
		)
	{
		return OpValue1 >= OpValue2;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
