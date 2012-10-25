#include "stdafx.h"
#include "axl_jnc_Value.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CValue::CValue ()
{
	m_ValueKind = EValue_Void;
	m_pType = NULL;
	m_pLlvmValue = NULL;

	memset (m_ConstBuffer, 0, sizeof (m_ConstBuffer));
	m_Const.SetBuffer (ref::EBuf_Field, m_ConstBuffer, sizeof (m_ConstBuffer));
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

	if (m_pType->IsIntegerType ())
	{
		const void* p2 = m_ConstBuffer;
		int64_t* p = (int64_t*) GetConstData ();
		int64_t Integer = *(int64_t*) GetConstData ();
		m_pLlvmValue = llvm::ConstantInt::get (
			m_pType->GetLlvmType (),
			llvm::APInt (m_pType->GetSize () * 8, Integer, m_pType->IsSignedType ())
			);
	}

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
	m_ValueKind = EValue_GlobalProperty;
	m_pType = pVariable->GetType ();
	m_pVariable = pVariable;
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
CValue::SetLlvmValue (
	llvm::Value* pValue,
	CType* pType
	)
{
	m_ValueKind = EValue_Llvm;
	m_pType = pType;
	m_pLlvmValue = pValue;
}

void
CValue::SetLlvmValue (
	llvm::Value* pValue,
	EType TypeKind,
	CModule* pModule
	)
{
	if (!pModule)
	{
		pModule = GetCurrentThreadModule ();
		ASSERT (pModule);
	}

	CType* pType = pModule->m_TypeMgr.GetBasicType (TypeKind);
	SetLlvmValue (pValue, pType);
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
