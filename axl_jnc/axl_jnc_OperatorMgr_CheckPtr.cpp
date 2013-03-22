#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

void
COperatorMgr::GetDataRefScopeLevel (
	const CValue& Value,
	CValue* pScopeLevelValue
	)
{
	static size_t ScopeLevelIndexArray [] = 
	{
		1, // TDataPtrValidator m_Validator
		2, // size_t m_ScopeLvel
	};

	ASSERT (Value.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pPtrType = (CDataPtrType*) Value.GetType ();
	EDataPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	switch (PtrTypeKind)
	{
	case EDataPtrType_Normal:
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, ScopeLevelIndexArray, countof (ScopeLevelIndexArray), m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT), pScopeLevelValue);
		break;

	case EDataPtrType_Thin:
		GetThinDataPtrScopeLevel (Value, pScopeLevelValue);
		break;

	case EDataPtrType_Unsafe:
	default:
		ASSERT (false); // should not be called for unsafe references
		pScopeLevelValue->SetConstSizeT (0); 
	}
}

void
COperatorMgr::CheckDataPtrRange (
	const CValue& RawPtrValue,
	size_t Size,
	const CValue& RangeBeginValue,
	const CValue& RangeEndValue,
	ERuntimeError Error
	)
{
	m_pModule->m_LlvmBuilder.CreateComment ("check safe pointer range");

	CValue SizeValue (Size, EType_SizeT);
	CValue ErrorValue (Error, EType_Int);

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateBitCast (RawPtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CValue ArgValueArray [] = 
	{
		PtrValue,
		SizeValue,
		RangeBeginValue,
		RangeEndValue,
		ErrorValue,
	};

	CFunction* pCheckDataPtrRange = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckDataPtrRange);

	m_pModule->m_LlvmBuilder.CreateCall (
		pCheckDataPtrRange,
		pCheckDataPtrRange->GetType (),
		ArgValueArray,
		countof (ArgValueArray),
		NULL
		);
}

bool
COperatorMgr::CheckDataPtrScopeLevel (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	ASSERT (SrcValue.GetType ()->GetTypeKind () == EType_DataPtr);
	CDataPtrType* pPtrType = (CDataPtrType*) SrcValue.GetType ();
	EDataPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	CValue SrcScopeLevelValue;

	switch (PtrTypeKind)
	{
	case EDataPtrType_Normal:
		m_pModule->m_LlvmBuilder.CreateExtractValue (SrcValue, 3, m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT), &SrcScopeLevelValue);
		break;

	case EDataPtrType_Thin:
		if (SrcValue.GetValueKind () == EValue_Variable && DstValue.GetValueKind () == EValue_Variable)
		{
			if (SrcValue.GetVariable ()->GetScopeLevel () > DstValue.GetVariable ()->GetScopeLevel ())
			{
				err::SetFormatStringError (_T("data pointer scope level mismatch"));
				return false;
			}

			return true;
		}

		GetThinDataPtrScopeLevel (SrcValue, &SrcScopeLevelValue);
		break;

	case EDataPtrType_Unsafe:
	default:
		return true;
	}

	CValue DstScopeLevelValue;
	GetDataRefScopeLevel (DstValue, &DstScopeLevelValue);

	m_pModule->m_LlvmBuilder.CreateComment ("check data pointer scope level");

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckScopeLevel);
	
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pCheckFunction,
		pCheckFunction->GetType (),
		SrcScopeLevelValue,
		DstScopeLevelValue,
		NULL
		);

	return true;
}

void
COperatorMgr::CheckClassPtrNull (const CValue& Value)
{
	ASSERT (Value.GetType ()->GetTypeKind () == EType_ClassPtr);
	CClassPtrType* pPtrType = (CClassPtrType*) Value.GetType ();
	EClassPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	if (PtrTypeKind == EClassPtrType_Unsafe || !(pPtrType->GetFlags () & EPtrTypeFlag_Nullable))
		return;

	m_pModule->m_LlvmBuilder.CreateComment ("check null pointer");

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateBitCast (Value, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckNullPtr);	
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pCheckFunction, 
		pCheckFunction->GetType (), 
		PtrValue, 
		CValue (ERuntimeError_NullInterface, EType_Int),
		NULL
		);
}

void
COperatorMgr::CheckClassPtrScopeLevel (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CValue DstScopeLevelValue;
	GetDataRefScopeLevel (DstValue, &DstScopeLevelValue);

	m_pModule->m_LlvmBuilder.CreateComment ("check class scope level");

	CValue IfaceValue;
	m_pModule->m_LlvmBuilder.CreateBitCast (SrcValue, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr), &IfaceValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckClassPtrScopeLevel);

	m_pModule->m_LlvmBuilder.CreateCall2 (
		pCheckFunction,
		pCheckFunction->GetType (),
		IfaceValue,
		DstScopeLevelValue,
		NULL
		);
}

void
COperatorMgr::CheckFunctionPtrNull (const CValue& Value)
{
	ASSERT (Value.GetType ()->GetTypeKind () == EType_FunctionPtr);
	CFunctionPtrType* pPtrType = (CFunctionPtrType*) Value.GetType ();
	EFunctionPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	if (PtrTypeKind == EFunctionPtrType_Unsafe || !(pPtrType->GetFlags () & EPtrTypeFlag_Nullable))
		return;

	CValue PtrValue;

	switch (PtrTypeKind)
	{
	case EFunctionPtrType_Normal:
	case EFunctionPtrType_Weak:
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, NULL, &PtrValue);
		break;
			
	case EFunctionPtrType_Thin:
		PtrValue = Value;
		break;
	
	case EFunctionPtrType_Unsafe:
	default:
		return;
	}
	
	m_pModule->m_LlvmBuilder.CreateComment ("check null pointer");

	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckNullPtr);	
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pCheckFunction, 
		pCheckFunction->GetType (), 
		PtrValue, 
		CValue (ERuntimeError_NullFunction, EType_Int),
		NULL
		);
}

void
COperatorMgr::CheckFunctionPtrScopeLevel (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	ASSERT (SrcValue.GetType ()->GetTypeKind () == EType_FunctionPtr);
	CFunctionPtrType* pPtrType = (CFunctionPtrType*) SrcValue.GetType ();

	if (!pPtrType->HasClosure ())
		return;

	CValue ClosureValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (SrcValue, 1, NULL, &ClosureValue);
	CheckClassPtrScopeLevel (ClosureValue, DstValue);
}

void
COperatorMgr::CheckPropertyPtrNull (const CValue& Value)
{
	ASSERT (Value.GetType ()->GetTypeKind () == EType_FunctionPtr);
	CFunctionPtrType* pPtrType = (CFunctionPtrType*) Value.GetType ();
	EFunctionPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	if (PtrTypeKind == EPropertyPtrType_Unsafe || !(pPtrType->GetFlags () & EPtrTypeFlag_Nullable))
		return;

	CValue PtrValue;

	switch (PtrTypeKind)
	{
	case EFunctionPtrType_Normal:
	case EFunctionPtrType_Weak:
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, NULL, &PtrValue);
		break;
			
	case EFunctionPtrType_Thin:
		PtrValue = Value;
		break;
	
	case EFunctionPtrType_Unsafe:
	default:
		return;
	}

	if (pPtrType->GetTargetType ()->GetFlags () & EPropertyTypeFlag_Augmented)
		m_pModule->m_LlvmBuilder.CreateExtractValue (PtrValue, 0, NULL, &PtrValue);

	m_pModule->m_LlvmBuilder.CreateComment ("check null pointer");

	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckNullPtr);	
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pCheckFunction, 
		pCheckFunction->GetType (), 
		PtrValue, 
		CValue (ERuntimeError_NullFunction, EType_Int),
		NULL
		);
}

void
COperatorMgr::CheckPropertyPtrScopeLevel (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	ASSERT (SrcValue.GetType ()->GetTypeKind () == EType_PropertyPtr);
	CPropertyPtrType* pPtrType = (CPropertyPtrType*) SrcValue.GetType ();

	if (!pPtrType->HasClosure ())
		return;

	CValue ClosureValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (SrcValue, 1, NULL, &ClosureValue);
	CheckClassPtrScopeLevel (ClosureValue, DstValue);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
