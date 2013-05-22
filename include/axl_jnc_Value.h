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
class CAutoEv;
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
	EValue_AutoEv,	
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
		CAutoEv* m_pAutoEv;
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

	CValue (CAutoEv* pAutoEv)
	{
		Init ();
		SetAutoEv (pAutoEv);
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

	CAutoEv* 
	GetAutoEv () const
	{
		ASSERT (m_ValueKind == EValue_AutoEv);
		return m_pAutoEv;
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

	bool
	HasClosure () const
	{
		return m_Closure != NULL;
	}

	CClosure*
	GetClosure () const
	{
		return m_Closure;
	}

	void
	SetClosure (CClosure* pClosure);
		
	CClosure*
	CreateClosure ();

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
	SetAutoEv (CAutoEv* pAutoEv);

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

	void
	SetThinDataPtr (		
		llvm::Value* pLlvmValue,
		CDataPtrType* pType,
		CThinDataPtrValidator* pValidator
		);

	void
	SetThinDataPtr (		
		llvm::Value* pLlvmValue,
		CDataPtrType* pType,
		const CValue& ValidatorValue // EValue_Variable or EDataPtrType_Normal
		);

	void
	SetThinDataPtr (		
		llvm::Value* pLlvmValue,
		CDataPtrType* pType,
		const CValue& ScopeValidatorValue,
		const CValue& RangeBeginValue,
		size_t Size
		);

	CThinDataPtrValidator*
	GetThinDataPtrValidator () const
	{
		return m_ThinDataPtrValidator;
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

enum EThinDataPtrValidator
{
	EThinDataPtrValidator_Undefined,
	EThinDataPtrValidator_Simple,
	EThinDataPtrValidator_Complex,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CThinDataPtrValidator: public ref::IRefCount
{
protected:
	friend class CValue;

	EThinDataPtrValidator m_ValidatorKind;

	CValue m_ScopeValidatorValue;
	CValue m_RangeBeginValue;
	size_t m_Size;

public:
	CThinDataPtrValidator ()
	{
		m_ValidatorKind = EThinDataPtrValidator_Undefined;
		m_Size = 0;
	}

	EThinDataPtrValidator 
	GetValidatorKind ()
	{
		return m_ValidatorKind;
	}

	CValue 
	GetScopeValidator ()
	{
		return m_ScopeValidatorValue;
	}

	CValue 
	GetRangeBegin ()
	{
		return m_RangeBeginValue;
	}

	size_t
	GetSize ()
	{
		return m_Size;
	}
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

// header of class instance

struct TObject
{
	class CClassType* m_pType; // for GC tracing & QueryInterface; after destruction is zeroed
	size_t m_ScopeLevel;
	intptr_t m_Flags;

	// followed by TInterface of object
};

// header of interface instance

struct TInterface
{
	void** m_pVTable; 
	TObject* m_pObject; // for GC tracing & QueryInterface

	// followed by parents, then by interface data fields
};

// function that converts weak to strong

typedef
TInterface* 
(*FStrengthen) (TInterface*);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structures backing up multicast declared like:
// mutlicast OnFire ();

struct TMulticast
{
	volatile intptr_t m_Lock;
	size_t m_MaxCount;
	size_t m_Count;
	void* m_pPtrArray; // array of function closure, weak or unsafe pointers
	void* m_pHandleTable;
};

struct TMcSnapshot
{
	size_t m_Count;
	void* m_pPtrArray; // array of function closure or unsafe pointers
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structures backing up safe data pointer declared like:
// int* p;

struct TDataPtr
{
	void* m_p;
	void* m_pRangeBegin;
	void* m_pRangeEnd;
	size_t m_ScopeLevel;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structure backing up function closure pointer declared like:
// int function* pfTest (int, int);
// int function weak* pfTest (int, int);

struct TFunctionPtr
{
	void* m_pf;
	TInterface* m_pClosure; 
};

struct TFunctionPtr_w
{
	void* m_pf;
	TInterface* m_pClosure; 
	FStrengthen m_pfStrengthen;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structures backing up property closure pointer declared like:
// int property* pyTest;
// int property weak* pyTest;

struct TPropertyPtr
{
	void** m_pVTable;
	TInterface* m_pClosure; 
};

struct TPropertyPtr_w
{
	void** m_pVTable;
	TInterface* m_pClosure; 
	FStrengthen m_pfStrengthen;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structure backing up augmented property (bindable or autoget) thin pointer declared like:
// int autoget property thin* pyTest;
// if both bindable & autoget modifiers used then onchange event goes first followed by propvalue

struct TAuPropertyPtr_t
{
	void** m_pVTable;
	TDataPtr m_AuDataPtr;
};

// structures backing up augmented property (bindable or autoget) pointer declared like:
// int bindable property* pyTest;

struct TAuPropertyPtr
{
	void** m_pVTable;
	TDataPtr m_AuDataPtr;
	TInterface* m_pClosure; 
};

struct TAuPropertyPtr_w
{
	void** m_pVTable;
	TDataPtr m_AuDataPtr;
	TInterface* m_pClosure; 
	FStrengthen m_pfStrengthen;
};

// structure backing up augmented property (bindable or autoget) unsafe pointer declared like:
// int autoget property unsafe* pyTest;

struct TAuPropertyPtr_u
{
	void** m_pVTable;
	void* m_pAuData;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structures backing up autoev declared like:
// autoev TestAutoEv ();

struct TAutoEv
{
	volatile intptr_t m_Lock;
	size_t m_State;

	// followed by starter arguments
	// followed by bind site array
};

struct TAutoEvBindSite
{
	TDataPtr m_OnChangePtr;
	intptr_t m_Cookie;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structures backing up autoev closure pointer declared like:
// int autoev* paeTest (int);
// int autoev weak* paeTest (int);

struct TAutoEvPtr
{
	void** m_pVTable;
	TInterface* m_pClosure; 
};

struct TAutoEvPtr_w
{
	void** m_pVTable;
	TInterface* m_pClosure; 
	FStrengthen m_pfStrengthen;
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

		TInterface* m_pIface;
		TDataPtr m_DataPtr;
		TFunctionPtr m_FunctionPtr;
		TFunctionPtr_w m_FunctionPtr_w;
		TPropertyPtr m_PropertyPtr;
		TPropertyPtr_w m_PropertyPtr_w;
		TAuPropertyPtr m_AuPropertyPtr;
		TAuPropertyPtr_w m_AuPropertyPtr_w;
		TAuPropertyPtr_t m_AuPropertyPtr_t;
		TAuPropertyPtr_u m_AuPropertyPtr_u;
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
	ERuntimeError_AbstractFunction,
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
