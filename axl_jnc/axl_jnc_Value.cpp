#include "stdafx.h"
#include "axl_jnc_Value.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................
	
class CLlvmPodConst: public llvm::ConstantDataSequential 
{
public:
	static
	llvm::Constant*
	Get (
		CType* pType,
		const void* p
		)
	{
		llvm::Type* pLlvmType = pType->GetLlvmType ();
		return getImpl (llvm::StringRef ((char*) p, pType->GetSize ()), pLlvmType);
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

	case EValue_LlvmRegister:
		return _T("llvm-register");

	case EValue_GlobalFunction:
		return _T("global-function");

	case EValue_GlobalProperty:
		return _T("global-property");

	case EValue_Cmp:
		return _T("cmp");

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

CValue::CValue ()
{
	m_ValueKind = EValue_Void;
	m_pType = NULL;
	m_pLlvmValue = NULL;

	memset (m_ConstBuffer, 0, sizeof (m_ConstBuffer));
	m_Const.SetBuffer (ref::EBuf_Field, m_ConstBuffer, sizeof (m_ConstBuffer));
}

CValue::CValue (
	CType* pType,
	const void* p
	):
	m_Const (ref::EBuf_Field, m_ConstBuffer, sizeof (m_ConstBuffer))
{
	m_pLlvmValue = NULL;

	CreateConst (pType, p);
}

llvm::Value*
CValue::GetLlvmValue () const
{
	if (m_pLlvmValue)
		return m_pLlvmValue;

	if (m_ValueKind == EValue_Void)
		return NULL;

	ASSERT (m_ValueKind == EValue_Const);
	ASSERT (m_pType);

	int64_t Integer;
	double Double;
	llvm::Constant* pConstant = NULL;

	EType TypeKind = m_pType->GetTypeKind ();
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
		Integer = *(int64_t*) GetConstData ();
		pConstant = llvm::ConstantInt::get (
			m_pType->GetLlvmType (),
			llvm::APInt (m_pType->GetSize () * 8, Integer, m_pType->IsSignedType ())
			);
		break;

	case EType_Float:
		Double = *(float*) GetConstData ();
		pConstant = llvm::ConstantFP::get (m_pType->GetLlvmType (), Double);
		break;

	case EType_Double:
		Double = *(double*) GetConstData ();
		pConstant = llvm::ConstantFP::get (m_pType->GetLlvmType (), Double);
		break;

	case EType_Array:
		pConstant = CLlvmPodConst::Get (m_pType, GetConstData ());
		break;

	case EType_Struct:
	case EType_Union:
		pConstant = CLlvmPodConst::Get (m_pType, GetConstData ());
		break;

	case EType_Pointer_c:
		Integer = *(int64_t*) GetConstData ();

		pConstant = llvm::ConstantInt::get (
			m_pType->GetModule ()->m_TypeMgr.GetBasicType (EType_Int_pu)->GetLlvmType (),
			llvm::APInt (sizeof (void*) * 8, Integer, false)
			);

		pConstant = llvm::ConstantExpr::getIntToPtr (
			pConstant, 
			m_pType->GetLlvmType ()
			);

		break;

	default:
		ASSERT (false);
	}

	m_pLlvmValue = pConstant;
	return m_pLlvmValue;
}

void
CValue::SetType (CType* pType)
{
	m_ValueKind = EValue_Type;
	m_pType = pType;
}

void
CValue::SetVariable (CVariable* pVariable)
{
	m_ValueKind = EValue_Variable;
	m_pType = pVariable->GetType ();
	m_pVariable = pVariable;
	m_pLlvmValue = pVariable->GetLlvmValue ();
}

void
CValue::SetGlobalFunction (CGlobalFunction* pFunction)
{
	m_ValueKind = EValue_GlobalFunction;
	m_pType = pFunction->GetOverloadCount () == 1 ? pFunction->GetFunction ()->GetType () : NULL;
	m_pGlobalFunction = pFunction;
}

void
CValue::SetGlobalProperty (CGlobalProperty* pProperty)
{
	m_ValueKind = EValue_GlobalProperty;
	m_pType = pProperty->GetProperty ()->GetType ();
	m_pGlobalProperty = pProperty;
}

bool
CValue::CreateConst (
	CType* pType,
	const void* p
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

	return true;	
}

void
CValue::SetConstBool (bool Bool)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pType = pModule->m_TypeMgr.GetBasicType (EType_Bool);
	CreateConst (pType, &Bool);
}

void
CValue::SetConstInt32 (int32_t Integer)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pType = pModule->m_TypeMgr.GetInt32Type (Integer);
	CreateConst (pType, &Integer);
}

void
CValue::SetConstUInt32 (
	uint32_t Integer,
	bool ForceUnsigned
	)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pType = pModule->m_TypeMgr.GetUInt32Type (Integer, ForceUnsigned);
	CreateConst (pType, &Integer);
}

void
CValue::SetConstInt64 (int64_t Integer)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pType = pModule->m_TypeMgr.GetInt64Type (Integer);
	CreateConst (pType, &Integer);
}

void
CValue::SetConstUInt64 (
	uint64_t Integer,
	bool ForceUnsigned
	)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pType = pModule->m_TypeMgr.GetUInt64Type (Integer, ForceUnsigned);
	CreateConst (pType, &Integer);
}

void
CValue::SetConstFloat (float Float)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CreateConst (pModule->m_TypeMgr.GetBasicType (EType_Float));
	*(float*) GetConstData () = Float;
}

void
CValue::SetConstDouble (double Double)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CreateConst (pModule->m_TypeMgr.GetBasicType (EType_Double));
	*(double*) GetConstData () = Double;
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

	CreateConst (pModule->m_TypeMgr.GetLiteralTypeA (Length));
	char* pDst = (char*) GetConstData ();
	memcpy (pDst, p, Length * sizeof (char));
	pDst [Length] = 0;
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

	CreateConst (pModule->m_TypeMgr.GetLiteralTypeW (Length));
	wchar_t* pDst = (wchar_t*) GetConstData ();
	memcpy (pDst, p, Length * sizeof (wchar_t));
	pDst [Length] = 0;
}

void
CValue::SetLlvmRegister (
	llvm::Value* pValue,
	CType* pType
	)
{
	m_ValueKind = EValue_LlvmRegister;
	m_pType = pType;
	m_pLlvmValue = pValue;
}

void
CValue::SetLlvmRegister (
	llvm::Value* pValue,
	EType TypeKind
	)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pType = pModule->m_TypeMgr.GetBasicType (TypeKind);
	SetLlvmRegister (pValue, pType);
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
