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

		rtl::CIteratorT <CStructMember> Member = pType->GetFirstMember ();
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
	switch (ValueKind)
	{
	case EValue_Void:
		return _T("void");

	case EValue_Null:	
		return _T("null");

	case EValue_Type:
		return _T("type");

	case EValue_Const:
		return _T("const");

	case EValue_Variable:
		return _T("variable");

	case EValue_Function:
		return _T("function");

	case EValue_FunctionOverload:
		return _T("function-overload");

	case EValue_Property:
		return _T("property");

	case EValue_LlvmRegister:
		return _T("llvm-register");

	case EValue_BoolNot:
		return _T("bool-not");

	case EValue_BoolAnd:
		return _T("bool-and");

	case EValue_BoolOr:
		return _T("bool-or");

	default:
		return _T("<undefined>");
	}
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
CValue::Init ()
{
	m_ValueKind = EValue_Void;
	m_pType = NULL;
	m_Flags = NULL;
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
			llvm::APInt (pType->GetSize () * 8, Integer, pType->IsSignedType ())
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

	case EType_Pointer:
	case EType_Reference:
		pLlvmConst = CLlvmPodStruct::Get (((CPointerType*) pType)->GetPointerStructType (), p);
		break;

	case EType_Interface:
	case EType_Class:
		pLlvmConst = CLlvmPodStruct::Get (((CClassType*) pType)->GetPointerStructType (), p);
		break;

	case EType_Pointer_u:
		Integer = *(int64_t*) p;

		pLlvmConst = llvm::ConstantInt::get (
			pModule->m_TypeMgr.GetPrimitiveType (EType_Int_pu)->GetLlvmType (),
			llvm::APInt (sizeof (void*) * 8, Integer, false)
			);

		pLlvmConst = llvm::ConstantExpr::getIntToPtr (
			pLlvmConst, 
			pType->GetLlvmType ()
			);

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
CValue::SetNull ()
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	m_ValueKind = EValue_Null;
	m_pType = pModule->m_TypeMgr.GetStdType (EStdType_BytePtr);
}

void
CValue::SetType (CType* pType)
{
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
CValue::SetVariable (
	CVariable* pVariable,
	llvm::Value* pLlvmValue,
	CType* pType,
	bool MakeReference,
	bool IsOffset
	)
{
	m_ValueKind = EValue_Variable;
	m_pType = MakeReference ? pType->GetPointerType (EType_Reference) : pType;
	m_pVariable = pVariable;
	m_pLlvmValue = pLlvmValue;
	m_Flags = IsOffset ? EValueFlag_IsVariableOffset : 0;
}

void
CValue::SetVariable (CVariable* pVariable)
{
	return SetVariable (pVariable, pVariable->GetLlvmValue (), pVariable->GetType (), true, false);
}

void
CValue::SetFunction (CFunction* pFunction)
{
	m_ValueKind = EValue_Function;
	m_pType = pFunction->GetType ()->GetPointerType (EType_Pointer_u);
	m_pLlvmValue = pFunction->GetLlvmFunction ();
}

void
CValue::SetFunctionOverload (CFunctionOverload* pFunctionOverload)
{
	m_ValueKind = EValue_FunctionOverload;
	m_pFunctionOverload = pFunctionOverload;

	if (pFunctionOverload->GetOverloadCount () == 1)
	{
		CFunction* pFunction = pFunctionOverload->GetFunction ();
		m_pType = pFunction->GetType ()->GetPointerType (EType_Pointer_u);
		m_pLlvmValue = pFunction->GetLlvmFunction ();
	}
}

void
CValue::SetProperty (CPropertyType* pPropertyType)
{
	m_ValueKind = EValue_Property;
	m_pType = pPropertyType;
}

bool
CValue::CreateConst (
	const void* p,
	CType* pType
	)
{
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

//.............................................................................

} // namespace axl {
} // namespace jnc {
