#include "stdafx.h"
#include "axl_jnc_Value.h"
#include "axl_jnc_Closure.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................
	
class CLlvmPodArray: public llvm::ConstantDataSequential 
{
public:
	static
	llvm::Constant*
	Get (
		CArrayType* pType,
		const void* p
		)
	{
		llvm::Type* pLlvmType = pType->GetLlvmType ();
		return getImpl (llvm::StringRef ((char*) p, pType->GetSize ()), pLlvmType);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CLlvmPodStruct: public llvm::ConstantStruct
{
public:
	static
	llvm::Constant*
	Get (
		CStructType* pType,
		const void* p
		)
	{
		llvm::Type* pLlvmType = pType->GetLlvmType ();
		
		char Buffer [256];
		rtl::CArrayT <llvm::Constant*> LlvmMemberArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

		rtl::CIteratorT <CStructMember> Member = pType->GetMemberList ().GetHead ();
		for (; Member; Member++)
		{
			CValue MemberConst ((char*) p + Member->GetOffset (), Member->GetType ());
			LlvmMemberArray.Append ((llvm::Constant*) MemberConst.GetLlvmValue ());
		}

		return get (
			(llvm::StructType*) pLlvmType, 
			llvm::ArrayRef <llvm::Constant*> (LlvmMemberArray, LlvmMemberArray.GetCount ())
			);
	}
};

//.............................................................................

const tchar_t*
GetValueKindString (EValue ValueKind)
{
	static const tchar_t* StringTable [EValue__Count] = 
	{
		_T("void"),          // EValue_Void = 0,
		_T("null"),          // EValue_Null,
		_T("type"),          // EValue_Type,
		_T("const"),         // EValue_Const,
		_T("variable"),      // EValue_Variable,
		_T("function"),      // EValue_Function,
		_T("property"),      // EValue_Property,	
		_T("field"),         // EValue_Field,	
		_T("llvm-register"), // EValue_LlvmRegister,
		_T("bool-not"),      // EValue_BoolNot,
		_T("bool-and"),      // EValue_BoolAnd,
		_T("bool-or"),       // EValue_BoolOr,
	};

	return ValueKind >= 0 && ValueKind < EValue__Count ? 
		StringTable [ValueKind] : 
		_T("undefined-value-kind");
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetAllocKindString (EAlloc AllocKind)
{
	static const tchar_t* StringTable [EAlloc__Count] = 
	{
		_T("undefined-alloc-kind"), // EAlloc_Undefined = 0,
		_T("heap"),                 // EAlloc_Heap,
		_T("stack"),                // EAlloc_Stack,
	};

	return AllocKind >= 0 && AllocKind < EAlloc__Count ? 
		StringTable [AllocKind] : 
		StringTable [EAlloc_Undefined];
}
//.............................................................................

CValue::CValue (
	const void* p,
	CType* pType
	)
{
	Init ();
	CreateConst (p, pType);
}

CValue::CValue (
	int64_t Value,
	EType TypeKind
	)
{	
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pType = pModule->m_TypeMgr.GetPrimitiveType (TypeKind);

	Init ();
	CreateConst (&Value, pType);
}

void
CValue::Clear ()
{
	m_ValueKind = EValue_Void;
	m_pType = NULL;
	m_Flags = 0;
	m_pItem = NULL;
	m_pLlvmValue = NULL;
	m_Closure = NULL;
}

void
CValue::Init ()
{
	m_ValueKind = EValue_Void;
	m_pType = NULL;
	m_Flags = 0;
	memset (m_ConstBuffer, 0, sizeof (m_ConstBuffer));
	m_Const.SetBuffer (ref::EBuf_Field, m_ConstBuffer, sizeof (m_ConstBuffer));
	m_pVariable = NULL;
	m_pLlvmValue = NULL;
}

llvm::Value*
CValue::GetLlvmValue () const
{
	if (m_pLlvmValue)
		return m_pLlvmValue;

	ASSERT (m_ValueKind == EValue_Const);

	m_pLlvmValue = GetLlvmConst (m_pType, GetConstData ());
	return m_pLlvmValue;
}

llvm::Constant* 
GetLlvmPtrConst (
	CType* pType,
	const void* p
	)
{
	int64_t Integer = *(int64_t*) p;

	llvm::Constant* pLlvmConst = llvm::ConstantInt::get (
		pType->GetModule ()->m_TypeMgr.GetPrimitiveType (EType_Int_pu)->GetLlvmType (),
		llvm::APInt (sizeof (void*) * 8, Integer, false)
		);

	return llvm::ConstantExpr::getIntToPtr (pLlvmConst, pType->GetLlvmType ());
}

llvm::Constant*
CValue::GetLlvmConst (
	CType* pType,
	const void* p
	)
{
	int64_t Integer;
	double Double;
	llvm::Constant* pLlvmConst = NULL;

	CModule* pModule = pType->GetModule ();

	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Bool:
	case EType_Int8:
	case EType_Int8_u:
	case EType_Int16:
	case EType_Int16_u:
	case EType_Int32:
	case EType_Int32_u:
	case EType_Int64:
	case EType_Int64_u:
	case EType_Int16_be:
	case EType_Int16_beu:
	case EType_Int32_be:
	case EType_Int32_beu:
	case EType_Int64_be:
	case EType_Int64_beu:
		Integer = *(int64_t*) p;
		pLlvmConst = llvm::ConstantInt::get (
			pType->GetLlvmType (),
			llvm::APInt (pType->GetSize () * 8, Integer, pType->IsSignedIntegerType ())
			);
		break;

	case EType_Float:
		Double = *(float*) p;
		pLlvmConst = llvm::ConstantFP::get (pType->GetLlvmType (), Double);
		break;

	case EType_Double:
		Double = *(double*) p;
		pLlvmConst = llvm::ConstantFP::get (pType->GetLlvmType (), Double);
		break;

	case EType_Array:
		pLlvmConst = CLlvmPodArray::Get ((CArrayType*) pType, p);
		break;

	case EType_Struct:
		pLlvmConst = CLlvmPodStruct::Get ((CStructType*) pType, p);
		break;

	case EType_DataPtr:
	case EType_DataRef:
		if (((CDataPtrType*) pType)->GetPtrTypeKind () == EDataPtrType_Normal)
		{
			pLlvmConst = CLlvmPodStruct::Get (((CDataPtrType*) pType)->GetDataPtrStructType (), p);
		}
		else // thin or unsafe
		{
			pLlvmConst = GetLlvmPtrConst (pType, p);
		}
		break;

	case EType_ClassPtr:
		pLlvmConst = GetLlvmPtrConst (pType, p);
		break;

	default:
		ASSERT (false);
	}

	return pLlvmConst;
}

CClosure*
CValue::CreateClosure ()
{
	m_Closure = AXL_REF_NEW (CClosure);
	return m_Closure;
}

void
CValue::SetClosure (CClosure* pClosure)
{
	m_Closure = pClosure;
}

void
CValue::OverrideType (EType TypeKind)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pType = pModule->m_TypeMgr.GetPrimitiveType (TypeKind);
	OverrideType (pType);
}

void
CValue::SetVoid ()
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	Clear ();

	m_ValueKind = EValue_Void;
	m_pType = pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
}

void
CValue::SetNull ()
{
	Clear ();

	m_ValueKind = EValue_Null;
}

void
CValue::SetType (CType* pType)
{
	Clear ();

	m_ValueKind = EValue_Type;
	m_pType = pType;
}

void
CValue::SetType (EType TypeKind)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);
	
	CType* pType = pModule->m_TypeMgr.GetPrimitiveType (TypeKind);
	SetType (pType);
}

void
CValue::SetVariable (CVariable* pVariable)
{
	return SetLlvmValue (
		pVariable->GetLlvmValue (), 
		pVariable->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin), 
		pVariable,
		EValueFlag_NoDataPtrRangeCheck
		);
}

void
CValue::SetFunction (CFunction* pFunction)
{
	Clear ();

	m_ValueKind = EValue_Function;
	m_pFunction = pFunction;

	if (!pFunction->IsOverloaded ())
		m_pType = pFunction->GetType ()->GetFunctionPtrType (EType_FunctionRef, EFunctionPtrType_Thin);

	if (!pFunction->IsVirtual ())
		m_pLlvmValue = pFunction->GetLlvmFunction ();
}

void
CValue::SetProperty (CProperty* pProperty)
{
	Clear ();

	m_ValueKind = EValue_Property;
	m_pProperty = pProperty;
	m_pType = pProperty->GetType ()->GetPropertyPtrType (EType_PropertyRef, EPropertyPtrType_Thin);

	// don't assign LlvmValue yet cause property LlvmValue is only needed for pointers

}

bool
CValue::CreateConst (
	const void* p,
	CType* pType
	)
{
	Clear ();

	size_t Size = pType->GetSize ();

	bool Result = m_Const.GetBuffer (sizeof (TBufHdr) + Size) != NULL;
	if (!Result)
		return false;

	m_ValueKind = EValue_Const;
	m_pType = pType;
	m_Const->m_Size = Size;

	if (p)
		memcpy (GetConstData (), p, Size);
	else
		memset (GetConstData (), 0, Size);

	return true;	
}

bool
CValue::CreateConst (
	const void* p,
	EType TypeKind
	)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pType = pModule->m_TypeMgr.GetPrimitiveType (TypeKind);
	return CreateConst (p, pType);	
}

void
CValue::SetLiteralA (
	const char* p,
	size_t Length
	)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	if (Length == -1)
		Length = p ? strlen (p) : 0;

	CreateConst (p, pModule->m_TypeMgr.GetLiteralTypeA (Length));
	((char*) GetConstData ()) [Length] = 0;
}

void
CValue::SetLiteralW (
	const wchar_t* p,
	size_t Length
	)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	if (Length == -1)
		Length = p ? wcslen (p) : 0;

	CreateConst (p, pModule->m_TypeMgr.GetLiteralTypeW (Length));
	((wchar_t*) GetConstData ()) [Length] = 0;
}

void
CValue::SetLlvmValue (
	llvm::Value* pValue,
	CType* pType,
	EValue ValueKind
	)
{
	Clear ();

	m_ValueKind = ValueKind;
	m_pType = pType;
	m_pLlvmValue = pValue;
}

void
CValue::SetLlvmValue (
	llvm::Value* pValue,
	EType TypeKind,
	EValue ValueKind
	)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pType = pModule->m_TypeMgr.GetPrimitiveType (TypeKind);
	SetLlvmValue (pValue, pType, ValueKind);
}

void
CValue::SetLlvmValue (		
	llvm::Value* pValue,
	CType* pType,
	CVariable* pVariable,
	int Flags
	)
{
	SetLlvmValue (pValue, pType, EValue_Variable);

	m_pVariable = pVariable;
	m_Flags = Flags;
}

void
CValue::SetLlvmValue (		
	llvm::Value* pValue,
	CType* pType,
	CStructMember* pField,
	int Flags
	)
{
	SetLlvmValue (pValue, pType, EValue_Field);

	m_pField = pField;
	m_Flags = Flags;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
