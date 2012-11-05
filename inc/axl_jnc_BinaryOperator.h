// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_rtl_StringHashTable.h"
#include "axl_jnc_CastOperator.h"

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

struct TBinaryOperatorTypeInfo
{
	CType* m_pReturnType;
	CType* m_pOpType1;
	CType* m_pOpType2;

	ECast m_CastKind;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

[uuid ("e205fc7b-e75e-4cc6-88f7-336701c9645a")]
struct IBinaryOperator: obj::IRoot
{	
protected:
	CModule* m_pModule;
	EBinOp m_OpKind;

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

	virtual
	bool
	GetTypeInfo (
		CType* pOpType1,
		CType* pOpType2,
		TBinaryOperatorTypeInfo* pTypeInfo
		) = 0;

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

class CBinaryOperatorOverload
{
protected:
	rtl::CBoxListT <rtl::CString> m_SignatureCache;
	rtl::CStringHashTableMapAT <IBinaryOperator*> m_DirectMap;
	rtl::CStringHashTableMapAT <IBinaryOperator*> m_ImplicitMap;

public:
	void
	Clear ();

	IBinaryOperator*
	GetOperator (
		CType* pOpType1,
		CType* pOpType2,
		TBinaryOperatorTypeInfo* pTypeInfo
		);

	IBinaryOperator*
	AddOperator (
		CType* pOpType1,
		CType* pOpType2,
		IBinaryOperator* pOperator
		);
};

//.............................................................................

bool
GetStdBinaryOperatorTypeInfo (
	CModule* pModule,
	CType* pReturnType,
	CType* pOpType,
	CType* pOpType1,
	CType* pOpType2,
	TBinaryOperatorTypeInfo* pTypeInfo
	);

bool
GetStdBinaryOperatorTypeInfo (
	CModule* pModule,
	EType ReturnTypeKind,
	EType OpTypeKind,
	CType* pOpType1,
	CType* pOpType2,
	TBinaryOperatorTypeInfo* pTypeInfo
	);

inline
bool
GetArithmeticBinaryOperatorTypeInfo (
	CModule* pModule,
	CType* pOpType,
	CType* pOpType1,
	CType* pOpType2,
	TBinaryOperatorTypeInfo* pTypeInfo
	)
{
	return GetStdBinaryOperatorTypeInfo (pModule, pOpType, pOpType, pOpType1, pOpType2, pTypeInfo);
}

inline
bool
GetArithmeticBinaryOperatorTypeInfo (
	CModule* pModule,
	EType OpTypeKind,
	CType* pOpType1,
	CType* pOpType2,
	TBinaryOperatorTypeInfo* pTypeInfo
	)
{
	return GetStdBinaryOperatorTypeInfo (pModule, OpTypeKind, OpTypeKind, pOpType1, pOpType2, pTypeInfo);
}

bool
GetCmpBinaryOperatorTypeInfo (
	CModule* pModule,
	CType* pOpType,
	CType* pOpType1,
	CType* pOpType2,
	TBinaryOperatorTypeInfo* pTypeInfo
	);

inline
bool
GetCmpBinaryOperatorTypeInfo (
	CModule* pModule,
	EType OpTypeKind,
	CType* pOpType1,
	CType* pOpType2,
	TBinaryOperatorTypeInfo* pTypeInfo
	)
{
	return GetStdBinaryOperatorTypeInfo (pModule, EType_Bool, OpTypeKind, pOpType1, pOpType2, pTypeInfo);
}

//.............................................................................

template <typename T>
class CBinOpT_i32: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_i32, IBinaryOperator)

public:
	CBinOpT_i32 ()
	{
		m_OpKind = (EBinOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType1,
		CType* pOpType2,
		TBinaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetArithmeticBinaryOperatorTypeInfo (m_pModule, EType_Int32, pOpType1, pOpType2, pTypeInfo);
	}

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
		llvm::Value* pLlvmOpValue1 = m_pModule->m_OperatorMgr.LoadValue (OpValue1);
		llvm::Value* pLlvmOpValue2 = m_pModule->m_OperatorMgr.LoadValue (OpValue2);
		llvm::Value* pLlvmResultValue = T::LlvmOpInt (m_pModule, pLlvmOpValue1, pLlvmOpValue2);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Int32);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CBinOpT_i64: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_i64, IBinaryOperator)

public:
	CBinOpT_i64 ()
	{
		m_OpKind = (EBinOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType1,
		CType* pOpType2,
		TBinaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetArithmeticBinaryOperatorTypeInfo (m_pModule, EType_Int64, pOpType1, pOpType2, pTypeInfo);
	}

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
		llvm::Value* pLlvmOpValue1 = m_pModule->m_OperatorMgr.LoadValue (OpValue1);
		llvm::Value* pLlvmOpValue2 = m_pModule->m_OperatorMgr.LoadValue (OpValue2);
		llvm::Value* pLlvmResultValue = T::LlvmOpInt (m_pModule, pLlvmOpValue1, pLlvmOpValue2);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Int64);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CBinOpT_f32: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_f32, IBinaryOperator)

public:
	CBinOpT_f32 ()
	{
		m_OpKind = (EBinOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType1,
		CType* pOpType2,
		TBinaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetArithmeticBinaryOperatorTypeInfo (m_pModule, EType_Float, pOpType1, pOpType2, pTypeInfo);
	}

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
		llvm::Value* pLlvmOpValue1 = m_pModule->m_OperatorMgr.LoadValue (OpValue1);
		llvm::Value* pLlvmOpValue2 = m_pModule->m_OperatorMgr.LoadValue (OpValue2);
		llvm::Value* pLlvmResultValue = T::LlvmOpFp (m_pModule, pLlvmOpValue1, pLlvmOpValue2);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Float);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CBinOpT_f64: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_f64, IBinaryOperator)

public:
	CBinOpT_f64 ()
	{
		m_OpKind = (EBinOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType1,
		CType* pOpType2,
		TBinaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetArithmeticBinaryOperatorTypeInfo (m_pModule, EType_Double, pOpType1, pOpType2, pTypeInfo);
	}

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
		llvm::Value* pLlvmOpValue1 = m_pModule->m_OperatorMgr.LoadValue (OpValue1);
		llvm::Value* pLlvmOpValue2 = m_pModule->m_OperatorMgr.LoadValue (OpValue2);
		llvm::Value* pLlvmResultValue = T::LlvmOpFp (m_pModule, pLlvmOpValue1, pLlvmOpValue2);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Double);
		return true;
	}
};

//.............................................................................

template <typename T>
class CBinOpT_cmp_i32: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_cmp_i32, IBinaryOperator)

public:
	CBinOpT_cmp_i32 ()
	{
		m_OpKind = (EBinOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType1,
		CType* pOpType2,
		TBinaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetCmpBinaryOperatorTypeInfo (m_pModule, EType_Int32, pOpType1, pOpType2, pTypeInfo);
	}

	virtual
	bool
	ConstOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		bool Bool = T::ConstOpInt32 (OpValue1.GetInt32 (), OpValue2.GetInt32 ());
		pResultValue->SetConstBool (Bool);
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
		llvm::Value* pLlvmOpValue1 = m_pModule->m_OperatorMgr.LoadValue (OpValue1);
		llvm::Value* pLlvmOpValue2 = m_pModule->m_OperatorMgr.LoadValue (OpValue2);
		llvm::Value* pLlvmResultValue = T::LlvmOpInt (m_pModule, pLlvmOpValue1, pLlvmOpValue2);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Bool);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CBinOpT_cmp_i64: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_cmp_i64, IBinaryOperator)

public:
	CBinOpT_cmp_i64 ()
	{
		m_OpKind = (EBinOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType1,
		CType* pOpType2,
		TBinaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetCmpBinaryOperatorTypeInfo (m_pModule, EType_Int64, pOpType1, pOpType2, pTypeInfo);
	}

	virtual
	bool
	ConstOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		bool Bool = T::ConstOpInt64 (OpValue1.GetInt64 (), OpValue2.GetInt64 ());
		pResultValue->SetConstBool (Bool);
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
		llvm::Value* pLlvmOpValue1 = m_pModule->m_OperatorMgr.LoadValue (OpValue1);
		llvm::Value* pLlvmOpValue2 = m_pModule->m_OperatorMgr.LoadValue (OpValue2);
		llvm::Value* pLlvmResultValue = T::LlvmOpInt (m_pModule, pLlvmOpValue1, pLlvmOpValue2);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Bool);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CBinOpT_cmp_f32: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_cmp_f32, IBinaryOperator)

public:
	CBinOpT_cmp_f32 ()
	{
		m_OpKind = (EBinOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType1,
		CType* pOpType2,
		TBinaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetCmpBinaryOperatorTypeInfo (m_pModule, EType_Float, pOpType1, pOpType2, pTypeInfo);
	}

	virtual
	bool
	ConstOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		bool Bool = T::ConstOpFp32 (OpValue1.GetFloat (), OpValue2.GetFloat ());
		pResultValue->SetConstBool (Bool);
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
		llvm::Value* pLlvmOpValue1 = m_pModule->m_OperatorMgr.LoadValue (OpValue1);
		llvm::Value* pLlvmOpValue2 = m_pModule->m_OperatorMgr.LoadValue (OpValue2);
		llvm::Value* pLlvmResultValue = T::LlvmOpFp (m_pModule, pLlvmOpValue1, pLlvmOpValue2);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Bool);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CBinOpT_cmp_f64: public IBinaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CBinOpT_cmp_f64, IBinaryOperator)

public:
	CBinOpT_cmp_f64 ()
	{
		m_OpKind = (EBinOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType1,
		CType* pOpType2,
		TBinaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetCmpBinaryOperatorTypeInfo (m_pModule, EType_Double, pOpType1, pOpType2, pTypeInfo);
	}

	virtual
	bool
	ConstOperator (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue		
		)
	{
		bool Bool = T::ConstOpFp64 (OpValue1.GetDouble (), OpValue2.GetDouble ());
		pResultValue->SetConstBool (Bool);
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
		llvm::Value* pLlvmOpValue1 = m_pModule->m_OperatorMgr.LoadValue (OpValue1);
		llvm::Value* pLlvmOpValue2 = m_pModule->m_OperatorMgr.LoadValue (OpValue2);
		llvm::Value* pLlvmResultValue = T::LlvmOpFp (m_pModule, pLlvmOpValue1, pLlvmOpValue2);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Bool);
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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Sub
{	
public:
	enum
	{
		 OpKind = EBinOp_Sub
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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Div_u
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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Mod_u
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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//.............................................................................

class CBinOp_Eq
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
		llvm::Value* pOpValue2
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Ne
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
		llvm::Value* pOpValue2
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Lt
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
		llvm::Value* pOpValue2
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Lt_u
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
		ulong_t OpValue1,
		ulong_t OpValue2
		) 
	{
		return OpValue1 < OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		uint64_t OpValue1,
		uint64_t OpValue2
		)
	{
		return OpValue1 < OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Le
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
		llvm::Value* pOpValue2
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Le_u
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
		ulong_t OpValue1,
		ulong_t OpValue2
		) 
	{
		return OpValue1 <= OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		uint64_t OpValue1,
		uint64_t OpValue2
		)
	{
		return OpValue1 <= OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
		llvm::Value* pOpValue2
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Gt_u
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
		ulong_t OpValue1,
		ulong_t OpValue2
		) 
	{
		return OpValue1 > OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		uint64_t OpValue1,
		uint64_t OpValue2
		)
	{
		return OpValue1 > OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
		llvm::Value* pOpValue2
		);

	static
	llvm::Value*
	LlvmOpFp (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinOp_Ge_u
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
		ulong_t OpValue1,
		ulong_t OpValue2
		) 
	{
		return OpValue1 >= OpValue2;
	}

	static
	bool
	ConstOpInt64 (
		uint64_t OpValue1,
		uint64_t OpValue2
		)
	{
		return OpValue1 >= OpValue2;
	}

	static
	llvm::Value*
	LlvmOpInt (
		CModule* pModule,
		llvm::Value* pOpValue1,
		llvm::Value* pOpValue2
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
