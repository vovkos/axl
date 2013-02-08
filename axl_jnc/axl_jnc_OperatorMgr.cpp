#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Closure.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

COperatorMgr::COperatorMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	// operator tables

	memset (m_UnaryOperatorTable, 0, sizeof (m_UnaryOperatorTable));
	memset (m_BinaryOperatorTable, 0, sizeof (m_BinaryOperatorTable));
	memset (m_CastOperatorTable, 0, sizeof (m_CastOperatorTable));

	// unary arithmetics

	m_UnaryOperatorTable [EUnOp_Plus]     = &m_UnOp_Plus;
	m_UnaryOperatorTable [EUnOp_Minus]    = &m_UnOp_Minus;
	m_UnaryOperatorTable [EUnOp_BwNot]    = &m_UnOp_BwNot;
	m_UnaryOperatorTable [EUnOp_LogNot]   = &m_UnOp_LogNot;

	// pointer operators

	m_UnaryOperatorTable [EUnOp_Addr]     = &m_UnOp_Addr;
	m_UnaryOperatorTable [EUnOp_Indir]    = &m_UnOp_Indir;
	m_UnaryOperatorTable [EUnOp_Ptr]      = &m_UnOp_Ptr;
	
	// increment operators

	m_UnOp_PreInc.m_OpKind  = EUnOp_PreInc;
	m_UnOp_PreDec.m_OpKind  = EUnOp_PreDec;
	m_UnOp_PostInc.m_OpKind = EUnOp_PostInc;
	m_UnOp_PostDec.m_OpKind = EUnOp_PostInc;

	m_UnaryOperatorTable [EUnOp_PreInc]   = &m_UnOp_PreInc;
	m_UnaryOperatorTable [EUnOp_PreDec]   = &m_UnOp_PreDec;
	m_UnaryOperatorTable [EUnOp_PostInc]  = &m_UnOp_PostInc;
	m_UnaryOperatorTable [EUnOp_PostDec]  = &m_UnOp_PostDec;

	// binary arithmetics

	m_BinaryOperatorTable [EBinOp_Add]    = &m_BinOp_Add;
	m_BinaryOperatorTable [EBinOp_Sub]    = &m_BinOp_Sub;
	m_BinaryOperatorTable [EBinOp_Mul]    = &m_BinOp_Mul;
	m_BinaryOperatorTable [EBinOp_Div]    = &m_BinOp_Div;
	m_BinaryOperatorTable [EBinOp_Mod]    = &m_BinOp_Mod;
	m_BinaryOperatorTable [EBinOp_Shl]    = &m_BinOp_Shl;
	m_BinaryOperatorTable [EBinOp_Shr]    = &m_BinOp_Shr;
	m_BinaryOperatorTable [EBinOp_BwAnd]  = &m_BinOp_BwAnd;
	m_BinaryOperatorTable [EBinOp_BwXor]  = &m_BinOp_BwXor;
	m_BinaryOperatorTable [EBinOp_BwOr]   = &m_BinOp_BwOr;
	
	// special operators

	m_BinaryOperatorTable [EBinOp_At]     = &m_BinOp_At;
	m_BinaryOperatorTable [EBinOp_Idx]    = &m_BinOp_Idx;

	// binary logic operators

	m_BinaryOperatorTable [EBinOp_LogAnd] = &m_BinOp_LogAnd;
	m_BinaryOperatorTable [EBinOp_LogOr]  = &m_BinOp_LogOr;

	// comparison operators

	m_BinaryOperatorTable [EBinOp_Eq]     = &m_BinOp_Eq;
	m_BinaryOperatorTable [EBinOp_Ne]     = &m_BinOp_Ne;
	m_BinaryOperatorTable [EBinOp_Lt]     = &m_BinOp_Lt;
	m_BinaryOperatorTable [EBinOp_Le]     = &m_BinOp_Le;
	m_BinaryOperatorTable [EBinOp_Gt]     = &m_BinOp_Gt;
	m_BinaryOperatorTable [EBinOp_Ge]     = &m_BinOp_Ge;
	 
	// assignment operators

	m_BinOp_AddAssign.m_OpKind = EBinOp_AddAssign;
	m_BinOp_SubAssign.m_OpKind = EBinOp_SubAssign;
	m_BinOp_MulAssign.m_OpKind = EBinOp_MulAssign;
	m_BinOp_DivAssign.m_OpKind = EBinOp_DivAssign;
	m_BinOp_ModAssign.m_OpKind = EBinOp_ModAssign;
	m_BinOp_ShlAssign.m_OpKind = EBinOp_ShlAssign;
	m_BinOp_ShrAssign.m_OpKind = EBinOp_ShrAssign;
	m_BinOp_AndAssign.m_OpKind = EBinOp_AndAssign;
	m_BinOp_XorAssign.m_OpKind = EBinOp_XorAssign;
	m_BinOp_OrAssign.m_OpKind  = EBinOp_OrAssign;
	m_BinOp_AtAssign.m_OpKind  = EBinOp_AtAssign;


	m_BinaryOperatorTable [EBinOp_Assign]      = &m_BinOp_Assign;
	m_BinaryOperatorTable [EBinOp_AddAssign]   = &m_BinOp_AddAssign;
	m_BinaryOperatorTable [EBinOp_SubAssign]   = &m_BinOp_SubAssign;
	m_BinaryOperatorTable [EBinOp_MulAssign]   = &m_BinOp_MulAssign;
	m_BinaryOperatorTable [EBinOp_DivAssign]   = &m_BinOp_DivAssign;
	m_BinaryOperatorTable [EBinOp_ModAssign]   = &m_BinOp_ModAssign;
	m_BinaryOperatorTable [EBinOp_ShlAssign]   = &m_BinOp_ShlAssign;
	m_BinaryOperatorTable [EBinOp_ShrAssign]   = &m_BinOp_ShrAssign;
	m_BinaryOperatorTable [EBinOp_AndAssign]   = &m_BinOp_AndAssign;
	m_BinaryOperatorTable [EBinOp_XorAssign]   = &m_BinOp_XorAssign;
	m_BinaryOperatorTable [EBinOp_OrAssign]    = &m_BinOp_OrAssign;
	m_BinaryOperatorTable [EBinOp_AtAssign]    = &m_BinOp_AtAssign;

	// cast operators

	m_StdCastOperatorTable [EStdCast_Copy] = &m_Cast_Copy;
	m_StdCastOperatorTable [EStdCast_SwapByteOrder] = &m_Cast_SwapByteOrder;
	m_StdCastOperatorTable [EStdCast_PtrFromInt] = &m_Cast_PtrFromInt;
	m_StdCastOperatorTable [EStdCast_Int] = &m_Cast_Int;
	m_StdCastOperatorTable [EStdCast_Fp] = &m_Cast_Fp;

	m_CastOperatorTable [EType_Bool] = &m_Cast_Bool;

	for (size_t i = EType_Int8; i <= EType_Int64_u; i++)
		m_CastOperatorTable [i] = &m_Cast_Int;

	for (size_t i = EType_Int16_be; i <= EType_Int64_beu; i++)
		m_CastOperatorTable [i] = &m_Cast_BeInt;

	m_CastOperatorTable [EType_Float]       = &m_Cast_Fp;
	m_CastOperatorTable [EType_Double]      = &m_Cast_Fp;
	m_CastOperatorTable [EType_Array]       = &m_Cast_Array;	
	m_CastOperatorTable [EType_Enum]        = &m_Cast_Enum;
	m_CastOperatorTable [EType_DataPtr]     = &m_Cast_DataPtr;
	m_CastOperatorTable [EType_ClassPtr]    = &m_Cast_ClassPtr;
	m_CastOperatorTable [EType_FunctionPtr] = &m_Cast_FunctionPtr;
	m_CastOperatorTable [EType_PropertyPtr] = &m_Cast_PropertyPtr;
}

bool
COperatorMgr::UnaryOperator (
	EUnOp OpKind,
	const CValue& RawOpValue,
	CValue* pResultValue
	)
{
	ASSERT (OpKind >= 0 && OpKind < EUnOp__Count);

	IUnaryOperator* pOperator = m_UnaryOperatorTable [OpKind];	
	if (!pOperator)
	{
		err::SetFormatStringError (_T("unary '%s' is not supported"), GetUnOpKindString (OpKind));
		return false;
	}

	CValue OpValue;
	CValue UnusedResultValue;

	if (!pResultValue)
		pResultValue = &UnusedResultValue;

	return 
		PrepareOperand (RawOpValue, &OpValue, pOperator->GetOpFlags ()) &&
		pOperator->Operator (OpValue, pResultValue);
}

bool
COperatorMgr::BinaryOperator (
	EBinOp OpKind,
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	ASSERT (OpKind >= 0 && OpKind < EBinOp__Count);
	
	IBinaryOperator* pOperator = m_BinaryOperatorTable [OpKind];	
	if (!pOperator)
	{
		err::SetFormatStringError (_T("binary '%s' is not supported"), GetBinOpKindString (OpKind));
		return false;
	}

	CValue OpValue1;
	CValue OpValue2;
	CValue UnusedResultValue;

	if (!pResultValue)
		pResultValue = &UnusedResultValue;

	return
		PrepareOperand (RawOpValue1, &OpValue1, pOperator->GetOpFlags1 ()) &&
		PrepareOperand (RawOpValue2, &OpValue2, pOperator->GetOpFlags2 ()) &&
		pOperator->Operator (OpValue1, OpValue2, pResultValue);
}

/*
	overloads!

	CType* pTargetType = PrepareOperandType (DstValue.GetType ());
	EType TargetTypeKind = pTargetType->GetTypeKind ();

	switch (TargetTypeKind)
	{
	case EType_Event:
		switch (OpKind)
		{
		case EBinOp_Add:
			return EventOperator (DstValue, SrcValue, EEventOp_AddHandler);

		case EBinOp_Sub:
			return EventOperator (DstValue, SrcValue, EEventOp_RemoveHandler);

		default:
			err::SetFormatStringError (_T("invalid operator for events"), GetBinOpKindString (OpKind));
			return false;
		}

		break;

	case EType_ClassPtr:
//		#pragma AXL_TODO ("overloaded operators for interfaces & classes")
		break;
	}

 */

/*

	if (pTargetType->GetTypeKind () == EType_Event)
		return EventOperator (DstValue, RawSrcValue, EEventOp_SetHandler);


 */


bool
COperatorMgr::CastOperator (
	EAlloc AllocKind,
	const CValue& RawOpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	bool Result;

	EType TypeKind = pType->GetTypeKind ();
	ASSERT (TypeKind >= 0 && TypeKind < EType__Count);

	ICastOperator* pOperator = m_CastOperatorTable [TypeKind];
	if (!pOperator)
	{
		SetCastError (RawOpValue, pType);
		return false;
	}

	CValue OpValue;
	CValue UnusedResultValue;

	if (!pResultValue)
		pResultValue = &UnusedResultValue;

	Result = PrepareOperand (RawOpValue, &OpValue, pOperator->GetOpFlags ());
	if (!Result)
		return false;

	if (OpValue.GetType ()->Cmp (pType) == 0) // identity, try to shortcut
	{
		if (OpValue.HasLlvmValue ())
		{
			*pResultValue = OpValue;
			return true;
		}

		if (OpValue.GetValueKind () == EValue_Property)
		{
			ASSERT (pType->GetTypeKind () == EType_PropertyPtr); 
			return GetPropertyThinPtr (OpValue.GetProperty (), OpValue.GetClosure (), (CPropertyPtrType*) pType, pResultValue);
		}

		// nope, need to go through full cast
	}

	return pOperator->Cast (AllocKind, OpValue, pType, pResultValue);
}

bool
COperatorMgr::CastOperator (
	EAlloc AllocKind,
	const CValue& OpValue,
	EType TypeKind,
	CValue* pResultValue
	)
{
	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (TypeKind);
	return CastOperator (AllocKind, OpValue, pType, pResultValue);
}

ECast
COperatorMgr::GetCastKind (
	const CValue& RawOpValue,
	CType* pType
	)
{
	EType TypeKind = pType->GetTypeKind ();
	ASSERT (TypeKind >= 0 && TypeKind < EType__Count);

	ICastOperator* pOperator = m_CastOperatorTable [TypeKind];
	if (!pOperator)
		return ECast_None;

	CValue OpValue;
	PrepareOperandType (
		RawOpValue, 
		&OpValue, 
		pOperator->GetOpFlags ()
		);

	if (OpValue.GetType ()->Cmp (pType) == 0) // identity!
		return ECast_Identitiy;

	return pOperator->GetCastKind (OpValue, pType);
}

ECast
COperatorMgr::GetArgCastKind (
	CFunctionType* pFunctionType,
	CType* const* ppActualArgTypeArray,
	size_t ActualArgCount
	)
{
	rtl::CArrayT <CType*> FormalArgTypeArray = pFunctionType->GetArgTypeArray ();
	size_t FormalArgCount = FormalArgTypeArray.GetCount ();

	if (ActualArgCount < FormalArgCount || 
		ActualArgCount > FormalArgCount && !(pFunctionType->GetFlags () & EFunctionTypeFlag_VarArg))
	{
		return ECast_None;
	}

	ECast WorstCastKind = ECast_Identitiy;

	for (size_t i = 0; i < FormalArgCount; i++)
	{
		CType* pFormalArgType = FormalArgTypeArray [i];
		CType* pActualArgType = ppActualArgTypeArray [i];;

		ECast CastKind = GetCastKind (pActualArgType, pFormalArgType);
		if (!CastKind)
			return ECast_None;

		if (CastKind < WorstCastKind)
			WorstCastKind = CastKind;
	}

	return WorstCastKind;
}

ECast
COperatorMgr::GetArgCastKind (
	CFunctionType* pFunctionType,
	const rtl::CConstBoxListT <CValue>& ArgList
	)
{
	size_t ActualArgCount = ArgList.GetCount ();

	rtl::CArrayT <CType*> FormalArgTypeArray = pFunctionType->GetArgTypeArray ();
	size_t FormalArgCount = FormalArgTypeArray.GetCount ();

	if (ActualArgCount < FormalArgCount || 
		ActualArgCount > FormalArgCount && !(pFunctionType->GetFlags () & EFunctionTypeFlag_VarArg))
	{
		return ECast_None;
	}

	ECast WorstCastKind = ECast_Identitiy;

	const rtl::CBoxIteratorT <CValue>& Arg = ArgList.GetHead ();
	for (size_t i = 0; i < FormalArgCount; i++)
	{
		CType* pFormalArgType = FormalArgTypeArray [i];

		ECast CastKind = GetCastKind (*Arg, pFormalArgType);
		if (!CastKind)
			return ECast_None;

		if (CastKind < WorstCastKind)
			WorstCastKind = CastKind;
	}

	return WorstCastKind;
}

ECast
COperatorMgr::GetFunctionCastKind (
	CFunctionType* pSrcType,
	CFunctionType* pDstType
	)
{
	ECast ArgCastKind = GetArgCastKind (pSrcType, pDstType->GetArgTypeArray ());
	if (!ArgCastKind)
		return ECast_None;

	CType* pSrcReturnType = pSrcType->GetReturnType ();
	CType* pDstReturnType = pDstType->GetReturnType ();
	
	if (pDstReturnType->GetTypeKind () == EType_Void)
		return ArgCastKind;

	ECast ReturnCastKind = GetCastKind (pSrcReturnType, pDstReturnType);
	return min (ArgCastKind, ReturnCastKind);
}

ECast
COperatorMgr::GetPropertyCastKind (
	CPropertyType* pSrcType,
	CPropertyType* pDstType
	)
{
	ECast CastKind = GetFunctionCastKind (pSrcType->GetGetterType (), pDstType->GetGetterType ());
	if (!CastKind)
		return ECast_None;

	CFunctionTypeOverload* pSrcSetterType = pSrcType->GetSetterType ();
	CFunctionTypeOverload* pDstSetterType = pDstType->GetSetterType ();

	ECast WorstCastKind = CastKind;

	size_t Count = pDstSetterType->GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pDstOverload = pDstSetterType->GetOverload (i);
		
		size_t j = pSrcSetterType->ChooseOverload (pDstOverload->GetArgTypeArray (), &CastKind);
		if (j == -1)
			return ECast_None;

		if (CastKind < WorstCastKind)
			WorstCastKind = CastKind;
	}

	return WorstCastKind;
}

bool
COperatorMgr::CheckCastKind (
	const CValue& OpValue,
	CType* pType
	)
{
	ECast CastKind = GetCastKind (OpValue, pType);
	switch (CastKind)
	{
	case ECast_Explicit:
		err::SetFormatStringError (
			_T("conversion from '%s' to '%s' requires explicit cast"),
			OpValue.GetType ()->GetTypeString (),
			pType->GetTypeString ()
			);
		return false;

	case ECast_None:
		SetCastError (OpValue, pType);
		return false;
	}

	return true;
}

bool
COperatorMgr::StackNewOperator (
	CType* pType,
	CValue* pResultValue
	)
{
	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	if (!pScope)
	{
		err::SetFormatStringError (_T("'stack new' operator could not be called at global scope"));
		return false;
	}

	if (pType->GetTypeKind () == EType_Class)
	{
		CClassType* pClassType = (CClassType*) pType;

		CValue ObjPtrValue;
		m_pModule->m_LlvmBuilder.CreateAlloca (pClassType->GetClassStructType (), _T("new"), NULL, &ObjPtrValue);
		
		bool Result = m_pModule->m_LlvmBuilder.InitializeObject (ObjPtrValue, pClassType, pScope);
		if (!Result)
			return false;

		m_pModule->m_LlvmBuilder.CreateGep2 (ObjPtrValue, 1, pClassType->GetClassPtrType (), pResultValue);

		if (pClassType->GetDestructor ())
			pScope->AddToDestructList (*pResultValue);
	}
	else
	{
		CDataPtrType* pResultType = m_pModule->m_TypeMgr.GetDataPtrType (pType, EType_DataPtr);

		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateAlloca (pType, _T("new"), NULL, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateStore (pType->GetZeroValue (), PtrValue);

		m_pModule->m_LlvmBuilder.CreateDataPtr (
			PtrValue, 
			PtrValue, 
			pType->GetSize (),
			pScope,
			pResultType,
			pResultValue
			);			
	}

	return true;
}

bool
COperatorMgr::HeapNewOperator (
	CType* pType,
	CValue* pResultValue
	)
{
	CValue TypeValue (&pType, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));

	CFunction* pHeapAllocate = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_HeapAllocate);

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pHeapAllocate,
		pHeapAllocate->GetType (),
		TypeValue,
		&PtrValue
		);

	if (pType->GetTypeKind () == EType_Class)
	{
		CClassType* pClassType = (CClassType*) pType;

		CDataPtrType* pPointerType = pClassType->GetClassStructType ()->GetDataPtrType (EDataPtrType_Unsafe);
		
		m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pPointerType, &PtrValue);

		bool Result = m_pModule->m_LlvmBuilder.InitializeObject (PtrValue, pClassType, NULL);
		if (!Result)
			return false;

		m_pModule->m_LlvmBuilder.CreateGep2 (PtrValue, 1, pClassType->GetClassPtrType (), pResultValue);
	}
	else
	{
		CDataPtrType* pPointerType = pType->GetDataPtrType (EDataPtrType_Unsafe);
		CDataPtrType* pResultType = pType->GetDataPtrType ();

		m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pPointerType, &PtrValue);

		m_pModule->m_LlvmBuilder.CreateDataPtr (
			PtrValue, 
			PtrValue, 
			pType->GetSize (),
			NULL,
			pResultType,
			pResultValue
			);			
	}

	return true;
}

bool
COperatorMgr::ProcessDestructList (rtl::CBoxListT <CValue>* pList)
{
	rtl::CBoxIteratorT <CValue> It = pList->GetHead ();
	for (; It; It++)
	{
		CValue Value = *It;
		ASSERT (Value.GetType ()->GetTypeKind () == EType_ClassPtr);
		
		CClassType* pType = ((CClassPtrType*) Value.GetType ())->GetTargetType ();

		CFunction* pDestructor = pType->GetDestructor ();
		ASSERT (pDestructor);

		m_pModule->m_LlvmBuilder.CreateCall (pDestructor, pDestructor->GetType (), Value, NULL);		
	}

	return true;
}

void 
COperatorMgr::PrepareOperandType (
	const CValue& OpValue,
	CValue* pOpValue,
	int OpFlags
	) 
{
	CValue Value = OpValue;
	
	for (;;)
	{
		CType* pType = OpValue.GetType ();
		CType* pPrevType = pType;

		EType TypeKind = pType->GetTypeKind ();
		switch (TypeKind)
		{			
		case EType_DataRef:
			if (!(OpFlags & EOpFlag_KeepDataRef))
			{
				CDataPtrType* pPtrType = (CDataPtrType*) pType;
				CType* pTargetType = pPtrType->GetTargetType ();
				EType TargetTypeKind = pTargetType->GetTypeKind ();

				if (TargetTypeKind == EType_BitField)
				{
					CBitFieldType* pBitFieldType = (CBitFieldType*) pTargetType;
					Value = pBitFieldType->GetBaseType ();
				}
				else if (TargetTypeKind != EType_Array)
				{
					Value = ((CDataPtrType*) pType)->GetTargetType ();
				}
				else if (!(OpFlags & EOpFlag_KeepArrayRef))
				{
					CArrayType* pArrayType = (CArrayType*) pTargetType;
					Value = pArrayType->GetElementType ()->GetDataPtrType (
						EType_DataPtr, 
						pPtrType->GetPtrTypeKind (), 
						pPtrType->GetFlags ()
						);
				}
			}

			break;

		case EType_PropertyRef:
			if (!(OpFlags & EOpFlag_KeepPropertyRef))
			{
				CPropertyPtrType* pPropertyPtrType = (CPropertyPtrType*) pType;

				CClosure* pClosure = Value.GetClosure ();
				if (pClosure)
				{
					pPropertyPtrType = pClosure->GetPropertyClosureType (pPropertyPtrType);
					if (!pPropertyPtrType)
						break;
				}
			
				CPropertyType* pPropertyType = pPropertyPtrType->GetTargetType ();
				if (!pPropertyType->IsIndexed ())
					Value = pPropertyType->GetReturnType ();
			}

			break;

		case EType_Bool:
			if (!(OpFlags & EOpFlag_KeepBool))
				Value = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int8);

			break;

		case EType_Enum:
			if (!(OpFlags & EOpFlag_KeepEnum))
				Value = ((CEnumType*) pType)->GetBaseType ();

			break;
		}

		if (Value.GetType () == pType)
			break;
	}

	*pOpValue = Value;
}

bool 
COperatorMgr::PrepareOperand (
	const CValue& OpValue,
	CValue* pOpValue,
	int OpFlags
	) 
{
	bool Result;

	CValue Value = OpValue;
	for (;;)
	{
		CType* pType = Value.GetType ();

		EType TypeKind = pType->GetTypeKind ();
		switch (TypeKind)
		{			
		case EType_DataRef:
			if (!(OpFlags & EOpFlag_KeepDataRef))
			{
				CDataPtrType* pPtrType = (CDataPtrType*) pType;
				if (pPtrType->GetTargetType ()->GetTypeKind () != EType_Array)
				{
					Result = LoadDataRef (&Value);
					if (!Result)
						return false;
				}
				else if (!(OpFlags & EOpFlag_KeepArrayRef))
				{
					CArrayType* pArrayType = (CArrayType*) pPtrType->GetTargetType ();
					pType = pArrayType->GetElementType ()->GetDataPtrType (
						EType_DataPtr, 
						pPtrType->GetPtrTypeKind (), 
						pPtrType->GetFlags ()
						);

					Value.OverrideType (pType);
				}
			}

			break;

		case EType_PropertyRef:
			if (!(OpFlags & EOpFlag_KeepPropertyRef))
			{
				CPropertyPtrType* pPropertyPtrType = (CPropertyPtrType*) pType;

				CClosure* pClosure = Value.GetClosure ();
				if (pClosure)
				{
					pPropertyPtrType = pClosure->GetPropertyClosureType (pPropertyPtrType);
					if (!pPropertyPtrType)
						break;
				}
			
				CPropertyType* pPropertyType = pPropertyPtrType->GetTargetType ();
				if (!pPropertyType->IsIndexed ())
				{
					Result = GetProperty (Value, &Value);
					if (!Result)
						return false;
				}
			}

			break;

		case EType_Bool:
			if (!(OpFlags & EOpFlag_KeepBool))
			{
				Result = CastOperator (&Value, EType_Int8);
				if (!Result)
					return false;
			}

			break;

		case EType_Enum:
			if (!(OpFlags & EOpFlag_KeepEnum))
				Value.OverrideType (((CEnumType*) pType)->GetBaseType ());

			break;
		}

		if (Value.GetType () == pType)
			break;
	}

	*pOpValue = Value;
	return true;
}

bool
COperatorMgr::EventOperator (
	const CValue& Event,
	const CValue& RawHandler,
	EEventOp OpKind
	)
{
	ASSERT (Event.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pReferenceType = (CDataPtrType*) Event.GetType ();

	CType* pTargetType = pReferenceType->GetTargetType ();
	
	CEventType* pEventType = (CEventType*) pTargetType;
	ASSERT (pEventType->GetTypeKind () == EType_Event);

	CValue EventPtrValue;
	bool Result = PrepareDataRef (Event, ERuntimeError_StoreOutOfRange, &EventPtrValue);
	if (!Result)
		return false;

	if (RawHandler.GetValueKind () == EValue_Null)
	{
		if (OpKind == EEventOp_SetHandler)
			m_pModule->m_LlvmBuilder.CreateStore (
				pEventType->GetZeroValue (), 
				EventPtrValue, 
				(pReferenceType->GetFlags () & EPtrTypeFlag_Volatile) != 0
				);

		// else ignore += null or -= null;
		return true;
	}

	CValue Handler;
	Result = CastOperator (RawHandler, pEventType->GetFunctionPtrType (), &Handler);
	if (!Result)
		return false;

	CValue FunctionPtr;
	CValue InterfacePtr;
	m_pModule->m_LlvmBuilder.CreateExtractValue (Handler, 0, NULL, &FunctionPtr);
	m_pModule->m_LlvmBuilder.CreateExtractValue (Handler, 1, NULL, &InterfacePtr);
	m_pModule->m_LlvmBuilder.CreateBitCast (FunctionPtr, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &FunctionPtr);
	m_pModule->m_LlvmBuilder.CreateBitCast (EventPtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleEvent)->GetDataPtrType (EDataPtrType_Unsafe), &EventPtrValue);

	CFunction* pFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_EventOperator);

	m_pModule->m_LlvmBuilder.CreateCall4 (
		pFunction,
		pFunction->GetType (),
		EventPtrValue,
		FunctionPtr,
		InterfacePtr,
		CValue (OpKind, EType_Int),
		NULL
		);
	
	return true;
}

bool
COperatorMgr::CreateClosureObject (
	EAlloc AllocKind,
	const CValue& OpValue, // function or property thin ptr
	rtl::CArrayT <size_t>* pClosureMap,
	CValue* pResultValue
	)
{
	bool Result;

	CFunctionType* pSrcFunctionType;
	if (OpValue.GetType ()->IsFunctionPtrType ())
	{
		pSrcFunctionType = ((CFunctionPtrType*) OpValue.GetType ())->GetTargetType ();
	}
	else
	{
		ASSERT (OpValue.GetType ()->IsPropertyPtrType ());
		pSrcFunctionType = ((CPropertyPtrType*) OpValue.GetType ())->GetTargetType ()->GetGetterType ();
	}

	CClassType* pClosureType = m_pModule->m_TypeMgr.CreateUnnamedClassType ();

	bool IsIndirect = 
		OpValue.GetValueKind () != EValue_Function &&
		OpValue.GetValueKind () != EValue_Property;
	
	if (IsIndirect)
		pClosureType->CreateFieldMember (OpValue.GetType ());

	CClosure* pClosure = OpValue.GetClosure ();

	const rtl::CBoxListT <CValue>* pClosureArgList = NULL;
	
	if (pClosure)
	{
		pClosureArgList = pClosure->GetArgList ();
		pClosureMap->Reserve (pClosureArgList->GetCount ());

		rtl::CBoxIteratorT <CValue> ClosureArg = pClosureArgList->GetHead ();
		rtl::CArrayT <CType*> SrcArgTypeArray = pSrcFunctionType->GetArgTypeArray ();
	
		if (pClosureArgList->GetCount () > SrcArgTypeArray.GetCount ())
		{
			err::SetFormatStringError (_T("closure is too big for '%s'"), pSrcFunctionType->GetTypeString ());
			return false;
		}

		for (size_t i = 0; ClosureArg; ClosureArg++, i++)
		{
			if (ClosureArg->IsEmpty ())
				continue;

			ASSERT (i < SrcArgTypeArray.GetCount ());
			pClosureMap->Append (i);
			pClosureType->CreateFieldMember (SrcArgTypeArray [i]);
		}
	}

	Result = pClosureType->CalcLayout ();
	if (!Result)
		return false;
	
	CValue ClosureValue;
	Result = m_pModule->m_OperatorMgr.NewOperator (AllocKind, pClosureType, &ClosureValue);
	if (!Result)
		return false;

	// save pfn & arguments in the closure
	
	CStructType* pClosureFieldStructType = pClosureType->GetFieldStructType ();
	ASSERT (pClosureFieldStructType);

	rtl::CIteratorT <CStructMember> ClosureMember = pClosureFieldStructType->GetMemberList ().GetHead ();
	if (IsIndirect)
	{
		CValue PtrValue = OpValue;
		PtrValue.SetClosure (NULL); // remove closure

		Result = m_pModule->m_OperatorMgr.SetClassFieldMemberValue (
			ClosureValue, 
			*ClosureMember, 
			PtrValue
			);

		if (!Result)
			return false;
		
		ClosureMember++;
	}

	if (pClosure)
	{
		rtl::CBoxIteratorT <CValue> ClosureArg = pClosureArgList->GetHead ();
		for (; ClosureArg; ClosureArg++)
		{
			if (ClosureArg->IsEmpty ())
				continue;

			ASSERT (ClosureMember);

			Result = m_pModule->m_OperatorMgr.SetClassFieldMemberValue (
				ClosureValue, 
				*ClosureMember, 
				*ClosureArg
				);

			if (!Result)
				return false;

			ClosureMember++;
		}
	}

	*pResultValue = ClosureValue;
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
