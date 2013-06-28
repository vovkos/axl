// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

class CScope;
class CVariable;
class CFunction;
class CFunctionTypeOverload;
class CProperty;
class CStructField;
class CClassType;
class CClosure;
class CThinDataPtrValidator;

//.............................................................................
	
enum EValue
{
	EValue_Void = 0,
	EValue_Null,
	EValue_Type,
	EValue_Const,
	EValue_Variable,
	EValue_Function,
	EValue_FunctionTypeOverload,
	EValue_Property,	
	EValue_LlvmRegister,
	EValue_BoolNot,
	EValue_BoolAnd,
	EValue_BoolOr,
	EValue__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetValueKindString (EValue ValueKind);

//.............................................................................

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
		CModuleItem* m_pItem;
		CVariable* m_pVariable;
		CFunction* m_pFunction;
		CFunctionTypeOverload* m_pFunctionTypeOverload;
		CProperty* m_pProperty;
	};

	mutable llvm::Value* m_pLlvmValue;

	ref::CPtrT <CClosure> m_Closure;
	ref::CPtrT <CThinDataPtrValidator> m_ThinDataPtrValidator;

public:
	CValue ()
	{
		Init ();
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
		EType TypeKind
		)
	{
		Init ();
		CreateConst (&Value, TypeKind);
	}

	CValue (
		int64_t Value,
		CType* pType
		)
	{
		Init ();
		CreateConst (&Value, pType);
	}

	CValue (
		const void* p,
		CType* pType
		)
	{
		Init ();
		CreateConst (p, pType);
	}

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

	CValue (CFunctionTypeOverload* pFunctionTypeOverload)
	{
		Init ();
		SetFunctionTypeOverload (pFunctionTypeOverload);
	}

	CValue (CProperty* pProperty)
	{
		Init ();
		SetProperty (pProperty);
	}

	CValue (
		llvm::Value* pLlvmValue,
		CType* pType,
		EValue ValueKind = EValue_LlvmRegister
		)
	{
		Init ();
		SetLlvmValue (pLlvmValue, pType, ValueKind);
	}

	CValue (
		llvm::Value* pLlvmValue,
		EType TypeKind,
		EValue ValueKind = EValue_LlvmRegister
		)
	{
		Init ();
		SetLlvmValue (pLlvmValue, TypeKind, ValueKind);
	}

	operator bool () const
	{
		return !IsEmpty ();
	}

	EValue 
	GetValueKind () const
	{
		return m_ValueKind;
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

	CFunctionTypeOverload* 
	GetFunctionTypeOverload () const
	{
		ASSERT (m_ValueKind == EValue_FunctionTypeOverload);
		return m_pFunctionTypeOverload;
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

	int
	GetInt () const
	{
		ASSERT (m_ValueKind == EValue_Const && m_pType->GetSize () >= sizeof (int));
		return *(int*) GetConstData ();
	}

	intptr_t
	GetIntPtr () const
	{
		ASSERT (m_ValueKind == EValue_Const && m_pType->GetSize () >= sizeof (intptr_t));
		return *(intptr_t*) GetConstData ();
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

	bool
	HasLlvmValue () const
	{
		return m_pLlvmValue != NULL || m_ValueKind == EValue_Const;
	}

	llvm::Value*
	GetLlvmValue () const;

	rtl::CString 
	GetLlvmTypeString ()
	{
		llvm::Value* pLlvmValue = GetLlvmValue ();
		return pLlvmValue ? jnc::GetLlvmTypeString (pLlvmValue->getType ()) : rtl::CString ();
	}

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

	CClosure*
	CreateClosure ();

	void
	SetClosure (CClosure* pClosure);

	void
	ClearClosure ()
	{
		m_Closure = ref::EPtr_Null;
	}

	void
	InsertToClosureHead (const CValue& Value);

	void
	InsertToClosureTail (const CValue& Value);

	CType*
	GetClosureAwareType () const;

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
	Clear ();

	void
	SetVoid ();

	void
	SetNull ();

	void
	SetType (CType* pType);

	void
	SetType (EType TypeKind);

	void
	SetVariable (CVariable* pVariable);

	void
	SetFunction (CFunction* pFunction);

	void
	SetFunctionTypeOverload (CFunctionTypeOverload* pFunctionTypeOverload);

	void
	SetProperty (CProperty* pProperty);

	void
	SetLlvmValue (		
		llvm::Value* pLlvmValue,
		CType* pType,
		EValue ValueKind = EValue_LlvmRegister
		);

	void
	SetLlvmValue (
		llvm::Value* pLlvmValue,
		EType TypeKind,
		EValue ValueKind = EValue_LlvmRegister
		);

	CThinDataPtrValidator*
	GetThinDataPtrValidator () const
	{
		return m_ThinDataPtrValidator;
	}

	void
	SetThinDataPtrValidator (CThinDataPtrValidator* pValidator);

	void
	SetThinDataPtrValidator (const CValue& ValidatorValue);

	void
	SetThinDataPtrValidator (		
		const CValue& ScopeValidatorValue,
		const CValue& RangeBeginValue,
		size_t Size
		);

	void
	SetThinDataPtr (
		llvm::Value* pLlvmValue,
		CDataPtrType* pType,
		CThinDataPtrValidator* pValidator
		)
	{
		SetLlvmValue (pLlvmValue, (CType*) pType);
		SetThinDataPtrValidator (pValidator);
	}

	void
	SetThinDataPtr (
		llvm::Value* pLlvmValue,
		CDataPtrType* pType,
		const CValue& ValidatorValue
		)
	{
		SetLlvmValue (pLlvmValue, (CType*) pType);
		SetThinDataPtrValidator (ValidatorValue);
	}

	void
	SetThinDataPtr (		
		llvm::Value* pLlvmValue,
		CDataPtrType* pType,
		const CValue& ScopeValidatorValue,
		const CValue& RangeBeginValue,
		size_t Size
		)
	{
		SetLlvmValue (pLlvmValue, (CType*) pType);
		SetThinDataPtrValidator (ScopeValidatorValue, RangeBeginValue, Size);
	}

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
	SetConstUInt32 (uint32_t Integer)
	{
		SetConstInt32 (Integer, GetInt32TypeKind_u (Integer));
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
	SetConstInt64_u (uint64_t Integer)
	{
		SetConstInt64 (Integer, GetInt64TypeKind_u (Integer));
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
	SetLiteral (
		const char* p,
		size_t Length = -1
		);

	void
	SetLiteral_w (
		const wchar_t* p,
		size_t Length = -1
		);

protected:
	void
	Init ();
};

//.............................................................................

enum EObjectFlag
{
	EObjectFlag_Alive  = 0x01,
	EObjectFlag_Static = 0x02,
	EObjectFlag_Stack  = 0x04,
	EObjectFlag_UHeap  = 0x08,
	EObjectFlag_CallMemberDestructors = EObjectFlag_Static | EObjectFlag_Stack | EObjectFlag_UHeap,
};

// master header of class instance

struct TObject
{
	class CClassType* m_pType; // for GC tracing & QueryInterface
	size_t m_ScopeLevel;
	intptr_t m_Flags;

	// followed by TInterface of object
};

// header of class interface

struct TInterface
{
	void** m_pVTable; 
	TObject* m_pObject; // back pointer to master header

	// followed by parents, then by interface data fields
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structures backing up multicast, e.g.:
// mutlicast OnFire ();

struct TMulticast: TInterface
{
	volatile intptr_t m_Lock;
	size_t m_MaxCount;
	size_t m_Count;
	void* m_pPtrArray; // array of function closure, weak or unsafe pointers
	void* m_pHandleTable;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// multicast snapshot returns function pointer with this closure:

struct TMcSnapshot: TInterface
{
	size_t m_Count;
	void* m_pPtrArray; // array of function closure or unsafe pointers
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structure backing up fat data pointer, e.g.:
// int* p;int* p;

struct TDataPtr
{
	void* m_p;
	void* m_pRangeBegin;
	void* m_pRangeEnd;
	size_t m_ScopeLevel;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structures backing up fat function pointers, e.g.:
// int function* pfTest (int, int);
// int function weak* pfTest (int, int);

struct TFunctionPtr
{
	void* m_pf;
	TInterface* m_pClosure; 
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structures backing up property pointers, e.g.:
// int property* pxTest;
// int property weak* pxTest;

struct TPropertyPtr
{
	void** m_pVTable;
	TInterface* m_pClosure; 
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TAutoEvBindSite
{
	TInterface* m_pOnChange;
	intptr_t m_Cookie;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
		intptr_t (__cdecl* m_pf) (...);

		TDataPtr m_DataPtr;
		TInterface* m_pClassPtr;
		TFunctionPtr m_FunctionPtr;
		TPropertyPtr m_PropertyPtr;
	};
};

//.............................................................................

enum ERuntimeError
{
	ERuntimeError_ScopeMismatch,
	ERuntimeError_DataPtrOutOfRange,
	ERuntimeError_NullClassPtr,
	ERuntimeError_NullFunctionPtr,
	ERuntimeError_NullPropertyPtr,
	ERuntimeError_AbstractFunction,
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
