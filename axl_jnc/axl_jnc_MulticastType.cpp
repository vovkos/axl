#include "stdafx.h"
#include "axl_jnc_MulticastType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CMulticastType::CMulticastType ()
{
	m_TypeKind = EType_Multicast;
	m_Size = sizeof (TMulticast);
	m_pTargetType = NULL;
	m_pMulticastStructType = NULL;
	m_pSetMethod = NULL;
	m_pSetMethod_u = NULL;
	m_pAddMethod = NULL;
	m_pAddMethod_u = NULL;
	m_pRemoveMethod = NULL;
	m_pRemoveMethod_u = NULL;
	m_pSnapshotMethod = NULL;
	m_pSnapshotMethod_u = NULL;
}

CStructType* 
CMulticastType::GetMulticastStructType ()
{
	if (m_pMulticastStructType)
		return m_pMulticastStructType;

	m_pMulticastStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pMulticastStructType->m_Tag.Format (_T("multicast"));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p));
	m_pMulticastStructType->CreateFieldMember (m_pTargetType->GetDataPtrType (EDataPtrType_Unsafe));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));
	m_pMulticastStructType->CalcLayout ();

	return m_pMulticastStructType;
}

void
CMulticastType::PrepareTypeString ()
{
	m_TypeString += m_pTargetType->GetTypeModifierString ();
	m_TypeString += _T("multicast ");
	m_TypeString += m_pTargetType->GetTargetType ()->GetArgTypeString ();
}

CFunction* 
CMulticastType::GetSetMethodImpl ()
{
	if (m_pSetMethod)
		return m_pSetMethod;

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastSet_s"), pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	m_pModule->m_OperatorMgr.PrepareDataPtr (ArgValue1, ERuntimeError_StoreOutOfRange, &ArgValue1);

	CFunction* pImplFunction = GetSetMethodImpl_u ();

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValue1,
		ArgValue2,
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pSetMethod = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetSetMethodImpl_u ()
{
	if (m_pSetMethod_u)
		return m_pSetMethod_u;

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Unsafe),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastSet_u"), pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	EFunctionPtrType PtrTypeKind = m_pTargetType->GetPtrTypeKind ();
	EStdFunc FuncKind = 
		PtrTypeKind == EFunctionPtrType_Unsafe ? EStdFunc_MulticastSet_u :
		PtrTypeKind == EFunctionPtrType_Thin ? EStdFunc_MulticastSet_t :
		PtrTypeKind == EFunctionPtrType_Weak ? EStdFunc_MulticastSet_w : EStdFunc_MulticastSet;

	ConvertToSimpleMulticastPtr (&ArgValue1);
	ConvertToSimpleFunctionPtr (&ArgValue2);

	CFunction* pImplFunction = m_pModule->m_FunctionMgr.GetStdFunction (FuncKind);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValue1,
		ArgValue2,
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pSetMethod_u = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetAddMethodImpl ()
{
	if (m_pAddMethod)
		return m_pAddMethod;

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastAdd_s"), pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	m_pModule->m_OperatorMgr.PrepareDataPtr (ArgValue1, ERuntimeError_StoreOutOfRange, &ArgValue1);

	CFunction* pImplFunction = GetAddMethodImpl_u ();

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValue1,
		ArgValue2,
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pAddMethod = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetAddMethodImpl_u ()
{
	if (m_pAddMethod_u)
		return m_pAddMethod_u;

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Unsafe),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastAdd_u"), pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	EFunctionPtrType PtrTypeKind = m_pTargetType->GetPtrTypeKind ();
	EStdFunc FuncKind = 
		PtrTypeKind == EFunctionPtrType_Unsafe ? EStdFunc_MulticastAdd_u :
		PtrTypeKind == EFunctionPtrType_Thin ? EStdFunc_MulticastAdd_t :
		PtrTypeKind == EFunctionPtrType_Weak ? EStdFunc_MulticastAdd_w : EStdFunc_MulticastAdd;

	ConvertToSimpleMulticastPtr (&ArgValue1);
	ConvertToSimpleFunctionPtr (&ArgValue2);

	CFunction* pStdFunction = m_pModule->m_FunctionMgr.GetStdFunction (FuncKind);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pStdFunction,
		pStdFunction->GetType (),
		ArgValue1,
		ArgValue2,
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pAddMethod_u = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetRemoveMethodImpl ()
{
	if (m_pRemoveMethod)
		return m_pRemoveMethod;

	CType* pReturnType = m_pTargetType;
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastRemove_s"), pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	m_pModule->m_OperatorMgr.PrepareDataPtr (ArgValue1, ERuntimeError_StoreOutOfRange, &ArgValue1);

	CFunction* pImplFunction = GetRemoveMethodImpl_u ();

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValue1,
		ArgValue2,
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pRemoveMethod = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetRemoveMethodImpl_u ()
{
	if (m_pRemoveMethod_u)
		return m_pRemoveMethod_u;

	CType* pReturnType = m_pTargetType;
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Unsafe),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastRemove_u"), pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	EFunctionPtrType PtrTypeKind = m_pTargetType->GetPtrTypeKind ();
	EStdFunc FuncKind = 
		PtrTypeKind == EFunctionPtrType_Unsafe ? EStdFunc_MulticastRemove_u :
		PtrTypeKind == EFunctionPtrType_Thin ? EStdFunc_MulticastRemove_t :
		PtrTypeKind == EFunctionPtrType_Weak ? EStdFunc_MulticastRemove_w : EStdFunc_MulticastRemove;

	ConvertToSimpleMulticastPtr (&ArgValue1);

	CFunction* pStdFunction = m_pModule->m_FunctionMgr.GetStdFunction (FuncKind);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pStdFunction,
		pStdFunction->GetType (),
		ArgValue1,
		ArgValue2,
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pRemoveMethod_u = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetSnapshotMethodImpl ()
{
	if (m_pSnapshotMethod)
		return m_pSnapshotMethod;

	CType* pReturnType = m_pTargetType->GetMcSnapshotType ();
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastSnapshot_s"), pType);

	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	m_pModule->m_OperatorMgr.PrepareDataPtr (ArgValue, ERuntimeError_StoreOutOfRange, &ArgValue);

	CFunction* pImplFunction = GetSnapshotMethodImpl_u ();

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValue,
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pSnapshotMethod = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::GetSnapshotMethodImpl_u ()
{
	if (m_pSnapshotMethod_u)
		return m_pSnapshotMethod_u;

	CType* pReturnType = m_pTargetType->GetMcSnapshotType ();
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Unsafe),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastSnapshot_u"), pType);

	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	EFunctionPtrType PtrTypeKind = m_pTargetType->GetPtrTypeKind ();
	EStdFunc FuncKind = 
		PtrTypeKind == EFunctionPtrType_Unsafe ? EStdFunc_MulticastSnapshot_u :
		PtrTypeKind == EFunctionPtrType_Thin ? EStdFunc_MulticastSnapshot_t :
		PtrTypeKind == EFunctionPtrType_Weak ? EStdFunc_MulticastSnapshot_w : EStdFunc_MulticastSnapshot;

	ConvertToSimpleMulticastPtr (&ArgValue);

	CFunction* pStdFunction = m_pModule->m_FunctionMgr.GetStdFunction (FuncKind);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pStdFunction,
		pStdFunction->GetType (),
		ArgValue,
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pSnapshotMethod_u = pFunction;
	return pFunction;
}

void
CMulticastType::ConvertToSimpleMulticastPtr (CValue* pMulticastPtrValue)
{
	EFunctionPtrType PtrTypeKind = m_pTargetType->GetPtrTypeKind ();
	CFunctionType* pFuncitonType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	CFunctionPtrType* pFuncitonPtrType = pFuncitonType->GetFunctionPtrType (PtrTypeKind);
	CDataPtrType* pMulticastPtrType = pFuncitonPtrType->GetMulticastType ()->GetDataPtrType (EDataPtrType_Unsafe);

	m_pModule->m_LlvmBuilder.CreateBitCast (*pMulticastPtrValue, pMulticastPtrType, pMulticastPtrValue);
}

void
CMulticastType::ConvertToSimpleFunctionPtr (CValue* pFunctionPtrValue)
{
	EFunctionPtrType PtrTypeKind = m_pTargetType->GetPtrTypeKind ();
	CFunctionType* pFuncitonType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	CFunctionPtrType* pFunctionPtrType = pFuncitonType->GetFunctionPtrType (PtrTypeKind);

	if (PtrTypeKind == EFunctionPtrType_Unsafe || PtrTypeKind == EFunctionPtrType_Thin)
	{
		m_pModule->m_LlvmBuilder.CreateBitCast (*pFunctionPtrValue, pFunctionPtrType, pFunctionPtrValue);	
		return;
	}

	CValue FunctionPtrValue = pFunctionPtrType->GetUndefValue ();

	CValue PfnValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (*pFunctionPtrValue, 0, NULL, &PfnValue);
	m_pModule->m_LlvmBuilder.CreateBitCast (PfnValue, pFuncitonType->GetFunctionPtrType (EFunctionPtrType_Unsafe), &PfnValue);	
	m_pModule->m_LlvmBuilder.CreateInsertValue (FunctionPtrValue, PfnValue, 0, pFunctionPtrType, &FunctionPtrValue);

	CValue ClosureValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (*pFunctionPtrValue, 1, NULL, &ClosureValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (FunctionPtrValue, ClosureValue, 1, pFunctionPtrType, &FunctionPtrValue);

	if (PtrTypeKind == EFunctionPtrType_Weak)
	{
		CValue StrengthenValue;
		m_pModule->m_LlvmBuilder.CreateExtractValue (*pFunctionPtrValue, 2, NULL, &StrengthenValue);
		m_pModule->m_LlvmBuilder.CreateInsertValue (FunctionPtrValue, StrengthenValue, 2, pFunctionPtrType, &FunctionPtrValue);
	}

	*pFunctionPtrValue = FunctionPtrValue;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
