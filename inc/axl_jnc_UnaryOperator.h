// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"
#include "axl_rtl_StringHashTable.h"

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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

[uuid ("5def51c9-959f-4170-a032-892ac4f7e622")]
struct IUnaryOperatorLo: obj::IRoot
{	
public:
	virtual
	bool
	ConstOperator (
		const CValue& OpValue,
		CValue* pResultValue
		) = 0;

	virtual
	bool
	LlvmOperator (
		CModule* pModule,
		const CValue& OpValue,
		CValue* pResultValue
		) = 0;
};

//.............................................................................

class CUnaryOperator: public rtl::TListLink
{
protected:
	friend class CUnaryOperatorOverload;

	EUnOp m_OpKind;
	CType* m_pReturnType;
	CType* m_pOpType;
	IUnaryOperatorLo* m_pOperatorLo;

public:
	CUnaryOperator ();

	EUnOp 
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
	GetOpType ()
	{
		return m_pOpType;
	}

	bool
	Operator (
		const CValue& OpValue,
		CValue* pReturnValue		
		);

	bool
	Operator (CValue* pValue);
};

//.............................................................................

class CUnaryOperatorOverload
{
protected:
	friend class COperatorMgr;

	EUnOp m_OpKind;
	rtl::CStdListT <CUnaryOperator> m_List;
	rtl::CStringHashTableMapAT <CUnaryOperator*> m_DirectMap;
	rtl::CStringHashTableMapAT <CUnaryOperator*> m_ImplicitMap;

public:
	CUnaryOperatorOverload ()
	{
		m_OpKind = EUnOp_None;
	}

	EUnOp 
	GetOpKind ()
	{
		return m_OpKind;
	}

	void
	Clear ();

	CUnaryOperator*
	FindOperator (CType* pOpType);

	CUnaryOperator*
	AddOperator (
		CType* pReturnType,
		CType* pOpType,
		IUnaryOperatorLo* pOperatorLo
		);
};

//.............................................................................

template <typename T>
class CUnOpT_i32: public IUnaryOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOpT_i32, IUnaryOperatorLo)

public:
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
		CModule* pModule,
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		llvm::Value* pLlvmOpValue = pModule->m_OperatorMgr.LoadValue (OpValue);
		llvm::Value* pLlvmResultValue = T::LlvmOpInt (pModule, pLlvmOpValue);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Int32);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CUnOpT_i64: public IUnaryOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOpT_i64, IUnaryOperatorLo)

public:
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
		CModule* pModule,
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		llvm::Value* pLlvmOpValue = pModule->m_OperatorMgr.LoadValue (OpValue);
		llvm::Value* pLlvmResultValue = T::LlvmOpInt (pModule, pLlvmOpValue);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Int64);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CUnOpT_f32: public IUnaryOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOpT_f32, IUnaryOperatorLo)

public:
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
		CModule* pModule,
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		llvm::Value* pLlvmOpValue = pModule->m_OperatorMgr.LoadValue (OpValue);
		llvm::Value* pLlvmResultValue = T::LlvmOpFp (pModule, pLlvmOpValue);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Float);
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CUnOpT_f64: public IUnaryOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CUnOpT_f64, IUnaryOperatorLo)

public:
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
		CModule* pModule,
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		llvm::Value* pLlvmOpValue = pModule->m_OperatorMgr.LoadValue (OpValue);
		llvm::Value* pLlvmResultValue = T::LlvmOpFp (pModule, pLlvmOpValue);
		pResultValue->SetLlvmRegister (pLlvmResultValue, EType_Double);
		return true;
	}
};

//.............................................................................

class CUnOp_Minus
{
public:
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
