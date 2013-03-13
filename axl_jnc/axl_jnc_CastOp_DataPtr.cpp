#include "stdafx.h"
#include "axl_jnc_CastOp_DataPtr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

ECast
CCast_DataPtr_FromArray::GetCastKind (
	const CValue& OpValue,
	CType* pType
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_Array);
	ASSERT (pType->GetTypeKind () == EType_DataPtr);

	CArrayType* pSrcType = (CArrayType*) OpValue.GetType ();
	CDataPtrType* pDstType = (CDataPtrType*) pType;

	CType* pArrayElementType = pSrcType->GetElementType ();
	CType* pPtrDataType = pDstType->GetTargetType ();
	
	return 
		pArrayElementType->Cmp (pPtrDataType) == 0 ? ECast_Implicit : 
		(pArrayElementType->GetFlags () & ETypeFlag_Pod) &&
		(pPtrDataType->GetFlags () & ETypeFlag_Pod) ? ECast_Explicit : ECast_None;
}

bool
CCast_DataPtr_FromArray::ConstCast (
	const CValue& OpValue,
	CType* pType,
	void* pDst
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_Array);
	ASSERT (pType->GetTypeKind () == EType_DataPtr);

	CArrayType* pSrcType = (CArrayType*) OpValue.GetType ();
	CDataPtrType* pDstType = (CDataPtrType*) pType;
	EDataPtrType PtrTypeKind = pDstType->GetPtrTypeKind ();

	const CValue& SavedOpValue = m_pModule->m_ConstMgr.SaveValue (OpValue);
	void* p = SavedOpValue.GetConstData ();

	// #pragma AXL_TODO ("create a global constant holding the array")

	if (PtrTypeKind == EDataPtrType_Normal)
	{
		TDataPtr* pPtr = (TDataPtr*) pDst;
		pPtr->m_p = p;
		pPtr->m_Validator.m_pRegionBegin = p;
		pPtr->m_Validator.m_pRegionEnd = (char*) p + pSrcType->GetSize ();
		pPtr->m_Validator.m_ScopeLevel = 0;
	}
	else if (PtrTypeKind == EDataPtrType_Unsafe)
	{
		*(void**) pDst = p;
	}
	else
	{
		ASSERT (false); // cannot cast to thin
	}

	return true;
}

//.............................................................................

ECast
CCast_DataPtr_Base::GetCastKind (
	const CValue& OpValue,
	CType* pType
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataPtr && pType->GetTypeKind () == EType_DataPtr);

	CDataPtrType* pSrcType = (CDataPtrType*) OpValue.GetType ();
	CDataPtrType* pDstType = (CDataPtrType*) pType;

	if ((pSrcType->GetFlags () & EPtrTypeFlag_Const) != 0 && 
		(pDstType->GetFlags () & EPtrTypeFlag_Const) == 0 &&
		pDstType->GetPtrTypeKind () != EDataPtrType_Unsafe)
		return ECast_None;

	CType* pSrcDataType = pSrcType->GetTargetType ();
	CType* pDstDataType = pDstType->GetTargetType ();

	if (pSrcDataType->Cmp (pDstDataType) == 0)
		return ECast_Implicit;

	if (pSrcDataType->GetTypeKind () != EType_Struct)
		return ECast_Explicit;

	return ((CStructType*) pSrcDataType)->FindBaseTypeTraverse (pDstDataType) ?
		ECast_Implicit :
		ECast_Explicit;
}

intptr_t
CCast_DataPtr_Base::GetOffset (
	CDataPtrType* pSrcType,
	CDataPtrType* pDstType,
	CBaseTypeCoord* pCoord
	)
{	
	CType* pSrcDataType = pSrcType->GetTargetType ();
	CType* pDstDataType = pDstType->GetTargetType ();

	if (pSrcDataType->Cmp (pDstDataType) == 0 ||
		pSrcDataType->GetTypeKind () != EType_Struct ||
		pDstDataType->GetTypeKind () != EType_Struct)
	{
		return 0;
	}

	CStructType* pSrcStructType = (CStructType*) pSrcDataType;
	CStructType* pDstStructType = (CStructType*) pDstDataType;

	if (pSrcStructType->FindBaseTypeTraverse (pDstStructType, pCoord))
		return pCoord->m_Offset;

	CBaseTypeCoord Coord;
	if (pDstStructType->FindBaseTypeTraverse (pSrcStructType, &Coord))
		return -(intptr_t) Coord.m_Offset;

	return 0;
}

intptr_t
CCast_DataPtr_Base::GetOffsetUnsafePtrValue (
	const CValue& PtrValue, 
	CDataPtrType* pSrcType,
	CDataPtrType* pDstType,
	CValue* pResultValue
	)
{
	CBaseTypeCoord Coord;
	intptr_t Offset = GetOffset (pSrcType, pDstType, &Coord);

	if (!Coord.m_LlvmIndexArray.IsEmpty ())
	{
		Coord.m_LlvmIndexArray.Insert (0, 0);
		m_pModule->m_LlvmBuilder.CreateGep (
			PtrValue, 
			Coord.m_LlvmIndexArray,
			Coord.m_LlvmIndexArray.GetCount (),
			pDstType, 
			pResultValue
			);

		return Offset;
	}

	if (!Offset)
	{
		m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pDstType, pResultValue);
		return Offset;
	}

	ASSERT (Offset < 0);

	CValue BytePtrValue;
	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &BytePtrValue);
	m_pModule->m_LlvmBuilder.CreateGep (BytePtrValue, Offset, NULL, &BytePtrValue);
	m_pModule->m_LlvmBuilder.CreateBitCast (BytePtrValue, pDstType, pResultValue);
	return Offset;
}

//.............................................................................

bool
CCast_DataPtr_Normal2Normal::ConstCast (
	const CValue& OpValue,
	CType* pType,
	void* pDst
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataPtr);
	ASSERT (pType->GetTypeKind () == EType_DataPtr);

	intptr_t Offset = GetOffset ((CDataPtrType*) OpValue.GetType (), (CDataPtrType*) pType, NULL);

	TDataPtr* pDstPtr = (TDataPtr*) pDst;
	TDataPtr* pSrcPtr = (TDataPtr*) OpValue.GetConstData ();
	pDstPtr->m_p = (char*) pSrcPtr->m_p + Offset;
	pDstPtr->m_Validator = pSrcPtr->m_Validator;
	return true;
}

bool
CCast_DataPtr_Normal2Normal::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataPtr);
	ASSERT (pType->GetTypeKind () == EType_DataPtr);

	CValue PtrValue;
	CValue ValidatorValue;

	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 1, NULL, &ValidatorValue);

	CDataPtrType* pUnsafePtrType = ((CDataPtrType*) pType)->GetTargetType ()->GetDataPtrType (EDataPtrType_Unsafe);
	GetOffsetUnsafePtrValue (PtrValue, (CDataPtrType*) OpValue.GetType (), pUnsafePtrType, &PtrValue);

	CValue ResultValue = pType->GetUndefValue ();
	m_pModule->m_LlvmBuilder.CreateInsertValue (ResultValue, PtrValue, 0, NULL, &ResultValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (ResultValue, ValidatorValue, 1, pType, pResultValue);
	return true;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
CCast_DataPtr_Thin2Normal::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	CValue PtrValue;
	CDataPtrType* pUnsafePtrType = ((CDataPtrType*) pType)->GetTargetType ()->GetDataPtrType (EDataPtrType_Unsafe);
	GetOffsetUnsafePtrValue (OpValue, (CDataPtrType*) OpValue.GetType (), pUnsafePtrType, &PtrValue);

	CValue ValidatorValue;
	m_pModule->m_OperatorMgr.GetThinDataPtrValidator (OpValue, &ValidatorValue);

	m_pModule->m_LlvmBuilder.CreateDataPtr (PtrValue, ValidatorValue, (CDataPtrType*) pType, pResultValue);
	return true;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
CCast_DataPtr_Normal2Unsafe::ConstCast (
	const CValue& OpValue,
	CType* pType,
	void* pDst
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataPtr);
	ASSERT (pType->GetTypeKind () == EType_DataPtr);

	intptr_t Offset = GetOffset ((CDataPtrType*) OpValue.GetType (), (CDataPtrType*) pType, NULL);
	*(char**) pDst = *(char**) OpValue.GetConstData () + Offset;
	return true;
}

bool
CCast_DataPtr_Normal2Unsafe::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataPtr);
	ASSERT (pType->GetTypeKind () == EType_DataPtr);

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, NULL, &PtrValue);
	GetOffsetUnsafePtrValue (PtrValue, (CDataPtrType*) OpValue.GetType (), (CDataPtrType*) pType, pResultValue);
	return true;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
CCast_DataPtr_Unsafe2Unsafe::ConstCast (
	const CValue& OpValue,
	CType* pType,
	void* pDst
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataPtr);
	ASSERT (pType->GetTypeKind () == EType_DataPtr);

	intptr_t Offset = GetOffset ((CDataPtrType*) OpValue.GetType (), (CDataPtrType*) pType, NULL);
	*(char**) pDst = *(char**) OpValue.GetConstData () + Offset;
	return true;
}

bool
CCast_DataPtr_Unsafe2Unsafe::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataPtr);
	ASSERT (pType->GetTypeKind () == EType_DataPtr);

	GetOffsetUnsafePtrValue (OpValue, (CDataPtrType*) OpValue.GetType (), (CDataPtrType*) pType, pResultValue);
	return true;
}

//.............................................................................

CCast_DataPtr::CCast_DataPtr ()
{
	memset (m_OperatorTable, 0, sizeof (m_OperatorTable));

	m_OperatorTable [EDataPtrType_Normal] [EDataPtrType_Normal] = &m_Normal2Normal;
	m_OperatorTable [EDataPtrType_Thin] [EDataPtrType_Normal]   = &m_Thin2Normal;
	m_OperatorTable [EDataPtrType_Thin] [EDataPtrType_Unsafe]   = &m_Normal2Unsafe;
	m_OperatorTable [EDataPtrType_Thin] [EDataPtrType_Unsafe]   = &m_Unsafe2Unsafe;
	m_OperatorTable [EDataPtrType_Unsafe] [EDataPtrType_Unsafe] = &m_Unsafe2Unsafe;
}

ICastOperator*
CCast_DataPtr::GetCastOperator (
	const CValue& OpValue,
	CType* pType
	)
{
	ASSERT (pType->GetTypeKind () == EType_DataPtr);
	
	CDataPtrType* pDstPtrType = (CDataPtrType*) pType;
	EDataPtrType DstPtrTypeKind = pDstPtrType->GetPtrTypeKind ();

	CType* pSrcType = OpValue.GetType ();

	if (pSrcType->IsIntegerType ())
	{
		return DstPtrTypeKind == EDataPtrType_Unsafe ? 
			m_pModule->m_OperatorMgr.GetStdCastOperator (EStdCast_PtrFromInt) : 
			NULL;
	}
	else if (pSrcType->GetTypeKind () == EType_Array)
	{
		return &m_FromArray;
	}
	else if (pSrcType->GetTypeKind () != EType_DataPtr)
	{
		return NULL;
	}
		
	CDataPtrType* pSrcPtrType = (CDataPtrType*) pSrcType;
	EDataPtrType SrcPtrTypeKind = pSrcPtrType->GetPtrTypeKind ();

	if (DstPtrTypeKind == EDataPtrType_Normal &&
		(pSrcPtrType->GetTargetType ()->GetFlags () & ETypeFlag_Pod) != 
		(pDstPtrType->GetTargetType ()->GetFlags () & ETypeFlag_Pod))
	{
		return NULL;
	}

	ASSERT ((size_t) SrcPtrTypeKind < EDataPtrType__Count);
	ASSERT ((size_t) DstPtrTypeKind < EDataPtrType__Count);

	return m_OperatorTable [SrcPtrTypeKind] [DstPtrTypeKind];
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
