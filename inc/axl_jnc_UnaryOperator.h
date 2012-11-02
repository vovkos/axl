// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_rtl_StringHashTable.h"
#include "axl_jnc_CastOperator.h"

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
	EUnOp_Inc,
	EUnOp_Dec,
	EUnOp__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetUnOpString (EUnOp Op);

//.............................................................................

struct TUnaryOperatorTypeInfo
{
	CType* m_pReturnType;
	CType* m_pOpType;

	ECast m_CastKind;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

[uuid ("5def51c9-959f-4170-a032-892ac4f7e622")]
struct IUnaryOperator: obj::IRoot
{	
protected:
	CModule* m_pModule;
	EUnOp m_OpKind;

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

	virtual
	bool
	GetTypeInfo (
		CType* pOpType,
		TUnaryOperatorTypeInfo* pTypeInfo
		) = 0;

	virtual
	bool
	ConstOperator (
		const CValue& OpValue,
		CValue* pResultValue
		) = 0;

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue,
		CValue* pResultValue
		) = 0;
};

//.............................................................................

class CUnaryOperatorOverload
{
protected:
	friend class COperatorMgr;

	rtl::CStringHashTableMapAT <IUnaryOperator*> m_DirectMap;
	rtl::CStringHashTableMapAT <IUnaryOperator*> m_ImplicitMap;

public:
	void
	Clear ();

	IUnaryOperator*
	GetOperator (
		CType* pOpType,
		TUnaryOperatorTypeInfo* pTypeInfo
		);

	IUnaryOperator*
	AddOperator (
		CType* pOpType,
		IUnaryOperator* pOperator
		);
};

//.............................................................................

bool
GetArithmeticUnaryOperatorTypeInfo (
	CModule* pModule,
	CType* pType,
	CType* pOpType,
	TUnaryOperatorTypeInfo* pTypeInfo
	);

bool
GetArithmeticUnaryOperatorTypeInfo (
	CModule* pModule,
	EType TypeKind,
	CType* pOpType,
	TUnaryOperatorTypeInfo* pTypeInfo
	);

//.............................................................................

template <typename T>
class CUnOpT_i32: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOpT_i32, IUnaryOperator)

public:
	CUnOpT_i32 ()
	{
		m_OpKind = (EUnOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType,
		TUnaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetArithmeticUnaryOperatorTypeInfo (m_pModule, EType_Int32, pOpType, pTypeInfo);
	}

	virtual
	bool
	ConstOperator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		long Integer = T::ConstOpInt32 (OpValue.GetInt32 ());
		pResultValue->SetConstInt32 (Integer);
		return true;
	}

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		llvm::Value* pLlvmOpValue = m_pModule->m_OperatorMgr.LoadValue (OpValue);
		llvm::Value* pLlvmResultValue = T::LlvmOpInt (m_pModule, pLlvmOpValue);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Int32);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CUnOpT_i64: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOpT_i64, IUnaryOperator)

public:
	CUnOpT_i64 ()
	{
		m_OpKind = (EUnOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType,
		TUnaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetArithmeticUnaryOperatorTypeInfo (m_pModule, EType_Int64, pOpType, pTypeInfo);
	}

	virtual
	bool
	ConstOperator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		int64_t Integer = T::ConstOpInt64 (OpValue.GetInt64 ());
		pResultValue->SetConstInt64 (Integer);
		return true;
	}

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		llvm::Value* pLlvmOpValue = m_pModule->m_OperatorMgr.LoadValue (OpValue);
		llvm::Value* pLlvmResultValue = T::LlvmOpInt (m_pModule, pLlvmOpValue);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Int64);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CUnOpT_f32: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOpT_f32, IUnaryOperator)

public:
	CUnOpT_f32 ()
	{
		m_OpKind = (EUnOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType,
		TUnaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetArithmeticUnaryOperatorTypeInfo (m_pModule, EType_Float, pOpType, pTypeInfo);
	}

	virtual
	bool
	ConstOperator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		float Float = T::ConstOpFp32 (OpValue.GetFloat ());
		pResultValue->SetConstFloat (Float);
		return true;
	}

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		llvm::Value* pLlvmOpValue = m_pModule->m_OperatorMgr.LoadValue (OpValue);
		llvm::Value* pLlvmResultValue = T::LlvmOpFp (m_pModule, pLlvmOpValue);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Float);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CUnOpT_f64: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOpT_f64, IUnaryOperator)

public:
	CUnOpT_f64 ()
	{
		m_OpKind = (EUnOp) T::OpKind;
	}

	virtual
	bool
	GetTypeInfo (
		CType* pOpType,
		TUnaryOperatorTypeInfo* pTypeInfo
		)
	{
		return GetArithmeticUnaryOperatorTypeInfo (m_pModule, EType_Double, pOpType, pTypeInfo);
	}

	virtual
	bool
	ConstOperator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		double Double = T::ConstOpFp64 (OpValue.GetDouble ());
		pResultValue->SetConstDouble (Double);
		return true;
	}

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		llvm::Value* pLlvmOpValue = m_pModule->m_OperatorMgr.LoadValue (OpValue);
		llvm::Value* pLlvmResultValue = T::LlvmOpFp (m_pModule, pLlvmOpValue);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Double);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUnOp_addr: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_addr, IUnaryOperator)

public:
	virtual
	bool
	GetTypeInfo (
		CType* pOpType,
		TUnaryOperatorTypeInfo* pTypeInfo
		);

	virtual
	bool
	ConstOperator (
		const CValue& OpValue,
		CValue* pResultValue
		);

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUnOp_indir: public IUnaryOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOp_indir, IUnaryOperator)

public:
	virtual
	bool
	GetTypeInfo (
		CType* pOpType,
		TUnaryOperatorTypeInfo* pTypeInfo
		);

	virtual
	bool
	ConstOperator (
		const CValue& OpValue,
		CValue* pResultValue
		);

	virtual
	bool
	LlvmOperator (
		const CValue& OpValue,
		CValue* pResultValue
		);
};

//.............................................................................

class CUnOp_Minus
{
public:
	enum
	{
		OpKind = EUnOp_Minus
	};

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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUnOp_BitwiseNot
{
public:
	enum
	{
		OpKind = EUnOp_BitwiseNot
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

} // namespace axl {
} // namespace jnc {
