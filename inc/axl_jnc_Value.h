// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Variable.h"

namespace axl {
namespace jnc {

class CScope;
class CVariable;
class CFunction;
class CFunctionOverload;
class CProperty;
class CClassType;

//.............................................................................
	
enum EValue
{
	EValue_Void = 0,
	EValue_Null,
	EValue_Type,
	EValue_Const,
	EValue_Variable,
	EValue_Function,
	EValue_FunctionOverload,
	EValue_Property,	
	EValue_LlvmRegister,
	EValue_BoolNot,
	EValue_BoolAnd,
	EValue_BoolOr,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetValueKindString (EValue ValueKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CValue
{
protected:
	struct TBufHdr
	{
		size_t m_Size;
	};

	class CGetBufSize 
	{
	public:
		size_t
		operator () (const TBufHdr& Hdr)
		{
			return sizeof (TBufHdr) + Hdr.m_Size;
		}
	};

	typedef ref::CBufT <TBufHdr, CGetBufSize> CConstBuf;

protected:
	EValue m_ValueKind;
	CType* m_pType;	
	CConstBuf m_Const;
	uchar_t m_ConstBuffer [CConstBuf::MinBufSize + 8];

	union
	{
		CVariable* m_pVariable;
		CFunction* m_pFunction;
		CFunctionOverload* m_pFunctionOverload;
		CProperty* m_pProperty;
	};

	mutable llvm::Value* m_pLlvmValue;

public:
	CValue ();

	CValue (const CValue& Value):
		m_Const (ref::EBuf_Field, m_ConstBuffer, sizeof (m_ConstBuffer))
	{
		*this = Value;
	}

	CValue (
		const CValue& Value,
		CType* pType
		):
		m_Const (ref::EBuf_Field, m_ConstBuffer, sizeof (m_ConstBuffer))
	{
		OverrideType (Value, pType);
	}

	CValue (
		const CValue& Value,
		EType TypeKind
		):
		m_Const (ref::EBuf_Field, m_ConstBuffer, sizeof (m_ConstBuffer))
	{
		OverrideType (Value, TypeKind);
	}

	CValue (
		int64_t Value,
		EType TypeKind = EType_Int32
		);

	CValue (
		CType* pType,
		const void* p
		);

	CValue (CType* pType)
	{
		SetType (pType);
	}

	CValue (CVariable* pVariable)
	{
		SetVariable (pVariable);
	}

	CValue (CFunction* pFunction)
	{
		SetFunction (pFunction);
	}

	CValue (CFunctionOverload* pFunctionOverload)
	{
		SetFunctionOverload (pFunctionOverload);
	}

	CValue (CProperty* pProperty)
	{
		SetProperty (pProperty);
	}

	CValue (
		CVariable* pVariable,
		llvm::Value* pLlvmValue,
		CType* pType
		)
	{
		SetVariable (pVariable, pLlvmValue, pType);
	}

	CValue (
		llvm::Value* pLlvmValue,
		CType* pType
		)
	{
		SetLlvmRegister (pLlvmValue, pType);
	}

	CValue (
		llvm::Value* pLlvmValue,
		EType TypeKind
		)
	{
		SetLlvmRegister (pLlvmValue, TypeKind);
	}

	void
	Clear ();

	EValue 
	GetValueKind () const
	{
		return m_ValueKind;
	}

	const tchar_t*
	GetValueKindString () const
	{
		return jnc::GetValueKindString (m_ValueKind);
	}

	CType* 
	GetType () const
	{
		return m_pType;
	}

	CScope*
	GetScope () const
	{
		return m_ValueKind == EValue_Variable ? m_pVariable->GetScope () : NULL;
	}

	CVariable*
	GetVariable () const
	{
		ASSERT (m_ValueKind == EValue_Variable);
		return m_pVariable;
	}

	CFunction*
	GetFunction () const
	{
		ASSERT (m_ValueKind == EValue_Function);
		return m_pFunction;
	}

	CFunctionOverload*
	GetFunctionOverload () const
	{
		ASSERT (m_ValueKind == EValue_FunctionOverload);
		return m_pFunctionOverload;
	}

	CProperty*
	GetProperty () const
	{
		ASSERT (m_ValueKind == EValue_Property);
		return m_pProperty;
	}

	void*
	GetConstData () const
	{
		ASSERT (m_ValueKind == EValue_Const);
		return (void*) (m_Const + 1);
	}

	int32_t
	GetInt32 () const
	{
		ASSERT (m_ValueKind == EValue_Const && m_pType->GetSize () >= sizeof (int32_t));
		return *(int32_t*) GetConstData ();
	}

	int64_t
	GetInt64 () const
	{
		ASSERT (m_ValueKind == EValue_Const && m_pType->GetSize () >= sizeof (int64_t));
		return *(int64_t*) GetConstData ();
	}

	size_t
	GetSizeT () const
	{
		ASSERT (m_ValueKind == EValue_Const && m_pType->GetSize () >= sizeof (size_t));
		return *(size_t*) GetConstData ();
	}

	float
	GetFloat () const
	{
		ASSERT (m_ValueKind == EValue_Const && m_pType->GetSize () >= sizeof (float));
		return *(float*) GetConstData ();
	}

	double
	GetDouble () const
	{
		ASSERT (m_ValueKind == EValue_Const && m_pType->GetSize () >= sizeof (double));
		return *(double*) GetConstData ();
	}

	llvm::Value*
	GetLlvmValue () const;

	static
	llvm::Constant*
	GetLlvmConst (
		CType* pType,
		const void* p
		);

	void
	OverrideType (CType* pType)
	{
		m_pType = pType;
	}

	void
	OverrideType (EType TypeKind);

	void
	OverrideType (
		const CValue& Value,
		CType* pType
		)
	{
		*this = Value;
		OverrideType (pType);
	}

	void
	OverrideType (
		const CValue& Value,
		EType TypeKind
		)
	{
		*this = Value;
		OverrideType (TypeKind);
	}

	void
	SetVoid ()
	{
		Clear ();
	}

	void
	SetNull ();

	void
	SetType (CType* pType);

	void
	SetType (EType TypeKind);

	void
	SetVariable (CVariable* pVariable);

	void
	SetVariable (
		CVariable* pVariable,
		llvm::Value* pLlvmValue,
		CType* pType
		);

	void
	SetFunction (CFunction* pFunction);

	void
	SetFunctionOverload (CFunctionOverload* pFunctionOverload);

	void
	SetProperty (CProperty* pProperty);

	bool
	CreateConst (
		CType* pType,
		const void* p = NULL
		);

	bool
	CreateConst (
		EType Type,
		const void* p = NULL
		);

	void
	SetConstBool (bool Bool)
	{
		CreateConst (EType_Bool, &Bool);
	}

	void
	SetConstInt32 (
		int32_t Integer,
		CType* pType
		)
	{
		CreateConst (pType, &Integer);
	}

	void
	SetConstInt32 (
		int32_t Integer,
		EType TypeKind
		)
	{
		CreateConst (TypeKind, &Integer);
	}

	void
	SetConstInt32 (int32_t Integer)
	{
		SetConstInt32 (Integer, GetInt32TypeKind (Integer));
	}

	void
	SetConstUInt32 (
		uint32_t Integer,
		bool ForceUnsigned = false
		)
	{
		SetConstInt32 (Integer, GetUInt32TypeKind (Integer, ForceUnsigned));
	}

	void
	SetConstInt64 (
		int64_t Integer,
		CType* pType
		)
	{
		CreateConst (pType, &Integer);
	}

	void
	SetConstInt64 (
		int64_t Integer,
		EType TypeKind
		)
	{
		CreateConst (TypeKind, &Integer);
	}

	void
	SetConstInt64 (int64_t Integer)
	{
		SetConstInt64 (Integer, GetInt64TypeKind (Integer));
	}

	void
	SetConstUInt64 (
		uint64_t Integer,
		bool ForceUnsigned = false
		)
	{
		SetConstInt64 (Integer, GetUInt64TypeKind (Integer, ForceUnsigned));
	}

	void
	SetConstSizeT (
		size_t Size, 
		EType TypeKind = EType_SizeT
		)
	{
		CreateConst (TypeKind, &Size);
	}

	void
	SetConstFloat (float Float)
	{
		CreateConst (EType_Float, &Float);
	}

	void
	SetConstDouble (double Double)
	{
		CreateConst (EType_Double, &Double);
	}

	void
	SetLiteralA (
		const char* p,
		size_t Length = -1
		);

	void
	SetLiteralW (
		const wchar_t* p,
		size_t Length = -1
		);

	void
	SetLiteral (
		const tchar_t* p,
		size_t Length = -1
		)
	{
#ifdef _UNICODE
		SetLiteralW (p, Length);
#else
		SetLiteralA (p, Length);
#endif
	}

	void
	SetLlvmRegister (
		llvm::Value* pValue,
		CType* pType
		);

	void
	SetLlvmRegister (
		llvm::Value* pValue,
		EType TypeKind
		);
};

//.............................................................................

// header of class instance

struct TObject
{
	class CClassType* m_pType; // for GC tracing & QueryInterface
	size_t m_ScopeLevel;

	// followed by TInterface of the object
};

// header of interface instance

struct TInterface
{
	TObject* m_pObject; // for GC tracing & QueryInterface
	void** m_pMethodTable; 

	// followed by parents, then by interface data fields
};

//.............................................................................

// *safe, &safe

struct TSafePtr
{
	void* m_p;
	void* m_pRegionBegin;
	void* m_pRegionEnd;
	size_t m_ScopeLevel;
};

// structure backing up function pointer declared like
// typedef void FMethod ();

struct TFunctionPtr
{
	void* m_pfn;
	TInterface* m_pInterface;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ESafePtrError
{
	ESafePtrError_Load = 0,
	ESafePtrError_Store,
	ESafePtrError_ScopeMismatch,
};

//.............................................................................

struct TVariant
{
	CType* m_pType;

	union
	{
		int8_t m_Int8;
		uint8_t m_Int8_u;
		int16_t m_Int16;
		uint16_t m_Int16_u;
		int32_t m_Int32;
		uint32_t m_Int32_u;
		int64_t m_Int64;
		uint64_t m_Int64_u;

		float m_Float;
		double m_Double;

		void* m_p;

		TSafePtr m_SafePtr;
		TFunctionPtr m_FunctionPtr;
	};
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
