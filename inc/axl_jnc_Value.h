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
class CClassType;
class CPropertyType;
class CClosure;

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

enum EValueFlag
{
	EValueFlag_IsVariableOffset = 1
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
	int m_Flags;

	CConstBuf m_Const;
	uchar_t m_ConstBuffer [CConstBuf::MinBufSize + 8];

	union
	{
		CVariable* m_pVariable;
		CFunction* m_pFunction;
		CFunctionOverload* m_pFunctionOverload;
	};

	mutable llvm::Value* m_pLlvmValue;

	ref::CPtrT <CClosure> m_Closure;

public:
	CValue ()
	{
		Init ();
	}

	CValue (const CValue& Value)
	{
		Init ();
		*this = Value;
	}

	CValue (
		const CValue& Value,
		CType* pType
		)
	{
		Init ();
		OverrideType (Value, pType);
	}

	CValue (
		const CValue& Value,
		EType TypeKind
		)
	{
		Init ();
		OverrideType (Value, TypeKind);
	}

	CValue (
		int64_t Value,
		EType TypeKind = EType_Int32
		);

	CValue (
		const void* p,
		CType* pType
		);

	CValue (CType* pType)
	{
		Init ();
		SetType (pType);
	}

	CValue (CVariable* pVariable)
	{
		Init ();
		SetVariable (pVariable);
	}

	CValue (CFunction* pFunction)
	{
		Init ();
		SetFunction (pFunction);
	}

	CValue (CFunctionOverload* pFunctionOverload)
	{
		Init ();
		SetFunctionOverload (pFunctionOverload);
	}

	CValue (
		llvm::Value* pLlvmValue,
		CType* pType
		)
	{
		Init ();
		SetLlvmValue (pLlvmValue, pType);
	}

	CValue (
		llvm::Value* pLlvmValue,
		EType TypeKind
		)
	{
		Init ();
		SetLlvmValue (pLlvmValue, TypeKind);
	}

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

	bool
	IsEmpty () const
	{
		return m_ValueKind == EValue_Void;
	}

	CType* 
	GetType () const
	{
		return m_pType;
	}

	int 
	GetFlags () const
	{
		return m_Flags;
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

	CClosure*
	GetClosure () const
	{
		return m_Closure;
	}

	void
	SetClosure (CClosure* pClosure);

	CClosure*
	CreateClosure ();

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
		m_ValueKind = EValue_Void;
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
		CType* pType,
		bool MakeReference,
		bool IsOffset
		);

	void
	SetProperty (CPropertyType* pPropertyType);

	void
	SetFunction (CFunction* pFunction);

	void
	SetFunctionOverload (CFunctionOverload* pFunctionOverload);

	bool
	CreateConst (
		const void* p,
		CType* pType
		);

	bool
	CreateConst (
		const void* p,
		EType Type
		);

	void
	SetConstBool (bool Bool)
	{
		CreateConst (&Bool, EType_Bool);
	}

	void
	SetConstInt32 (
		int32_t Integer,
		CType* pType
		)
	{
		CreateConst (&Integer, pType);
	}

	void
	SetConstInt32 (
		int32_t Integer,
		EType TypeKind
		)
	{
		CreateConst (&Integer, TypeKind);
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
		CreateConst (&Integer, pType);
	}

	void
	SetConstInt64 (
		int64_t Integer,
		EType TypeKind
		)
	{
		CreateConst (&Integer, TypeKind);
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
		CreateConst (&Size, TypeKind);
	}

	void
	SetConstFloat (float Float)
	{
		CreateConst (&Float, EType_Float);
	}

	void
	SetConstDouble (double Double)
	{
		CreateConst (&Double, EType_Double);
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
	SetLlvmValue (		
		llvm::Value* pValue,
		CType* pType,
		EValue ValueKind = EValue_LlvmRegister
		);

	void
	SetLlvmValue (
		llvm::Value* pValue,
		EType TypeKind,
		EValue ValueKind = EValue_LlvmRegister
		);

protected:
	void
	Init ();
};

//.............................................................................

// header of class instance

struct TObjectHdr
{
	class CClassType* m_pType; // for GC tracing & QueryInterface; after destruction is zeroed
	size_t m_ScopeLevel;

	// followed by TInterface of object
};

// header of interface instance

struct TInterfaceHdr
{
	void** m_pVTable; 
	TObjectHdr* m_pObject; // for GC tracing & QueryInterface

	// followed by parents, then by interface data fields
};

//.............................................................................

// *safe, &safe

struct TSafePtrValidator
{
	void* m_pRegionBegin;
	void* m_pRegionEnd;
	size_t m_ScopeLevel;
};

struct TSafePtr
{
	void* m_p;
	TSafePtrValidator m_Validator;
};

// structure backing up function pointer declared like
// typedef void FTest ();

struct TFunctionPtr
{
	void* m_pfn;
	intptr_t m_CallingConvention;
	TInterfaceHdr* m_pInterface; // NULL, interface of object or interface of closure object
};

// structure backing up property pointer declared like
// typedef int property PTest;

struct TPropertyPtr
{
	void** m_pVTable;
	TInterfaceHdr* m_pInterface; // NULL, interface of object or interface of closure object
};

// structures backing up event declared like
// event OnFire ();

struct TEventHandler
{
	TFunctionPtr m_FunctionPtr;
	TEventHandler* m_pNext;
	TEventHandler* m_pPrev;
};

struct TEvent
{
	TEventHandler* m_pHead;
	TEventHandler* m_pTail;
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

enum ERuntimeError
{
	ERuntimeError_ScopeMismatch,
	ERuntimeError_LoadOutOfRange,
	ERuntimeError_StoreOutOfRange,
	ERuntimeError_NullInterface,
	ERuntimeError_NullFunction,
	ERuntimeError_NullProperty,
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
