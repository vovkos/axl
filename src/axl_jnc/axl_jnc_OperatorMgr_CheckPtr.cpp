#include "pch.h"
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
	static int32_t LlvmIndexArray [] = 
	{
		1, // TDataPtrValidator m_Validator
		2, // size_t m_ScopeLvel
	};

	ASSERT (Value.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pPtrType = (CDataPtrType*) Value.GetType ();
	EDataPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	if (PtrTypeKind == EDataPtrType_Thin)
	{
		GetThinDataPtrScopeLevel (Value, pScopeLevelValue);
	}
	else 
	{
		m_pModule->m_LlvmBuilder.CreateExtractValue (
			Value, 
			LlvmIndexArray, 
			countof (LlvmIndexArray), 
			m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT), 
			pScopeLevelValue
			);
	}
}

void
COperatorMgr::CheckDataPtrRange (
	const CValue& RawPtrValue,
	size_t Size,
	const CValue& RangeBeginValue,
	const CValue& RangeEndValue
	)
{
	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "check data pointer range");

	CValue SizeValue (Size, EType_SizeT);

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateBitCast (RawPtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CValue ArgValueArray [] = 
	{
		PtrValue,
		SizeValue,
		RangeBeginValue,
		RangeEndValue,
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

void
COperatorMgr::CheckDataPtrRange (const CValue& Value)
{
	ASSERT (Value.GetType ()->GetTypeKind () == EType_DataPtr || Value.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pType = (CDataPtrType*) Value.GetType ();
	EDataPtrType PtrTypeKind = pType->GetPtrTypeKind ();

	CDataPtrType* pResultType = pType->GetTargetType ()->GetDataPtrType_c ();

	CValue PtrValue;
	CValue RangeBeginValue;	
	CValue RangeEndValue;	

	if (pType->GetFlags () & (EPtrTypeFlag_Checked | EPtrTypeFlag_Unsafe))
		return;

	switch (PtrTypeKind)
	{
	case EDataPtrType_Thin:
		PtrValue = Value;
		GetThinDataPtrRange (Value, &RangeBeginValue, &RangeEndValue);
		break;

	case EDataPtrType_Normal:
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, pResultType, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 1, NULL, &RangeBeginValue);
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 2, NULL, &RangeEndValue);
		break;

	default:
		ASSERT (false);
	}

	CheckDataPtrRange (PtrValue, pType->GetTargetType ()->GetSize (), RangeBeginValue, RangeEndValue);
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

	if (pPtrType->GetFlags () & EPtrTypeFlag_Unsafe)
		return true;

	if (SrcValue.GetValueKind () == EValue_Variable && DstValue.GetValueKind () == EValue_Variable)
	{
		if (SrcValue.GetVariable ()->GetScopeLevel () > DstValue.GetVariable ()->GetScopeLevel ())
		{
			err::SetFormatStringError ("data pointer scope level mismatch");
			return false;
		}

		return true;
	}

	CValue SrcScopeLevelValue;

	if (PtrTypeKind == EDataPtrType_Thin)
		GetThinDataPtrScopeLevel (SrcValue, &SrcScopeLevelValue);
	else
		m_pModule->m_LlvmBuilder.CreateExtractValue (SrcValue, 3, m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT), &SrcScopeLevelValue);

	CValue DstScopeLevelValue;
	GetDataRefScopeLevel (DstValue, &DstScopeLevelValue);

	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "check data pointer scope level");

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
	ASSERT (Value.GetType ()->GetTypeKindFlags () & ETypeKindFlag_ClassPtr);

	CClassPtrType* pPtrType = (CClassPtrType*) Value.GetType ();
	EClassPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	if (pPtrType->GetFlags () & (EPtrTypeFlag_Checked | EPtrTypeFlag_Unsafe))
		return;

	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "check null class pointer");

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateBitCast (Value, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckNullPtr);	
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pCheckFunction, 
		pCheckFunction->GetType (), 
		PtrValue, 
		CValue (ERuntimeError_NullClassPtr, EType_Int),
		NULL
		);
}

void
COperatorMgr::CheckClassPtrScopeLevel (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	ASSERT (SrcValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_ClassPtr);
	if (SrcValue.GetType ()->GetFlags () & EPtrTypeFlag_Unsafe)
		return;

	CValue DstScopeLevelValue;
	GetDataRefScopeLevel (DstValue, &DstScopeLevelValue);

	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "check class scope level");

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
	ASSERT (Value.GetType ()->GetTypeKindFlags () & ETypeKindFlag_FunctionPtr);

	CFunctionPtrType* pPtrType = (CFunctionPtrType*) Value.GetType ();
	EFunctionPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	if (pPtrType->GetFlags () & (EPtrTypeFlag_Checked | EPtrTypeFlag_Unsafe))
		return;

	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "check null function pointer");

	CValue PtrValue;

	if (PtrTypeKind == EFunctionPtrType_Thin)
		PtrValue = Value;
	else
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, NULL, &PtrValue);
	
	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckNullPtr);	
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pCheckFunction, 
		pCheckFunction->GetType (), 
		PtrValue, 
		CValue (ERuntimeError_NullFunctionPtr, EType_Int),
		NULL
		);
}

void
COperatorMgr::CheckFunctionPtrScopeLevel (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	ASSERT (SrcValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_FunctionPtr);
	CFunctionPtrType* pPtrType = (CFunctionPtrType*) SrcValue.GetType ();

	if (!pPtrType->HasClosure () || (pPtrType->GetFlags () & EPtrTypeFlag_Unsafe))
		return;

	CValue ClosureValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (SrcValue, 1, NULL, &ClosureValue);
	CheckClassPtrScopeLevel (ClosureValue, DstValue);
}

void
COperatorMgr::CheckPropertyPtrNull (const CValue& Value)
{
	ASSERT (Value.GetType ()->GetTypeKindFlags () & ETypeKindFlag_PropertyPtr);

	CPropertyPtrType* pPtrType = (CPropertyPtrType*) Value.GetType ();
	EPropertyPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	if (pPtrType->GetFlags () & (EPtrTypeFlag_Checked | EPtrTypeFlag_Unsafe))
		return;

	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "check null property pointer");

	CValue PtrValue;

	if (PtrTypeKind == EPropertyPtrType_Thin)
		PtrValue = Value;
	else
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, NULL, &PtrValue);

	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckNullPtr);	
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pCheckFunction, 
		pCheckFunction->GetType (), 
		PtrValue, 
		CValue (ERuntimeError_NullPropertyPtr, EType_Int),
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

	if (!pPtrType->HasClosure () || (pPtrType->GetFlags () & EPtrTypeFlag_Unsafe))
		return;

	CValue ClosureValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (SrcValue, 1, NULL, &ClosureValue);
	CheckClassPtrScopeLevel (ClosureValue, DstValue);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
