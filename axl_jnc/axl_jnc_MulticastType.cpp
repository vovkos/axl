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
	memset (m_MethodArray, 0, sizeof (m_MethodArray));
}

CStructType* 
CMulticastType::GetMulticastStructType ()
{
	if (m_pMulticastStructType)
		return m_pMulticastStructType;

	m_pMulticastStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pMulticastStructType->m_Tag.Format (_T("multicast"));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_pMulticastStructType->CreateFieldMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_pMulticastStructType->CreateFieldMember (m_pTargetType->GetDataPtrType (EDataPtrType_Unsafe));
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
CMulticastType::GetMethod (
	EMulticastMethod Method,
	EDataPtrType PtrTypeKind
	)
{
	GetMulticastStructType ();

	ASSERT ((size_t) Method < EMulticastMethod__Count);

	size_t i2 = PtrTypeKind == EDataPtrType_Unsafe;

	if (m_MethodArray [Method] [i2])
		return m_MethodArray [Method] [i2];

	CFunction* pFunction;

	switch (Method)
	{
	case EMulticastMethod_Clear:
		pFunction = i2 ? CreateClearMethod_u () : CreateClearMethod ();
		break;

	case EMulticastMethod_Set:
		pFunction = i2 ? CreateSetMethod_u () : CreateSetMethod ();
		break;

	case EMulticastMethod_Add:
		pFunction = i2 ? CreateAddMethod_u () : CreateAddMethod ();
		break;

	case EMulticastMethod_Remove:
		pFunction = i2 ? CreateRemoveMethod_u () : CreateRemoveMethod ();
		break;

	case EMulticastMethod_Snapshot:
		pFunction = i2 ? CreateSnapshotMethod_u () : CreateSnapshotMethod ();
		break;

	case EMulticastMethod_Call:
		pFunction = i2 ? CreateCallMethod_u () : CreateCallMethod ();
		break;

	default:
		ASSERT (false);
		pFunction = NULL;
	}

	m_MethodArray [Method] [i2] = pFunction;
	return pFunction;
}

CFunction* 
CMulticastType::CreateClearMethod ()
{
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastClear_s"), pType);

	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	m_pModule->m_OperatorMgr.PrepareDataPtr (&ArgValue, ERuntimeError_StoreOutOfRange);
	m_pModule->m_LlvmBuilder.CreateStore (GetZeroValue (), ArgValue);
	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastType::CreateClearMethod_u ()
{
	ASSERT (false);

	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Unsafe),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastClear_u"), pType);

	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	m_pModule->m_LlvmBuilder.CreateStore (GetZeroValue (), ArgValue);
	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastType::CreateSetMethod ()
{
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

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Set, EDataPtrType_Unsafe);

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

	return pFunction;
}

CFunction* 
CMulticastType::CreateSetMethod_u ()
{
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

	EFunctionPtrType PtrTypeKind = m_pTargetType->GetPtrTypeKind ();
	EStdFunc FuncKind = 
		PtrTypeKind == EFunctionPtrType_Unsafe ? EStdFunc_MulticastSet_u :
		PtrTypeKind == EFunctionPtrType_Thin ? EStdFunc_MulticastSet_t :
		PtrTypeKind == EFunctionPtrType_Weak ? EStdFunc_MulticastSet_w : EStdFunc_MulticastSet;

	ConvertToSimpleMulticastPtr (&ArgValueArray [0]);
	ConvertToSimpleFunctionPtr (&ArgValueArray [1]);

	CFunction* pStdFunction = m_pModule->m_FunctionMgr.GetStdFunction (FuncKind);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pStdFunction,
		pStdFunction->GetType (),
		ArgValueArray,
		countof (ArgValueArray),
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastType::CreateAddMethod ()
{
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

	m_pModule->m_OperatorMgr.PrepareDataPtr (&ArgValueArray [0], ERuntimeError_StoreOutOfRange);

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Add, EDataPtrType_Unsafe);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValueArray,
		countof (ArgValueArray),
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastType::CreateAddMethod_u ()
{
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

	EFunctionPtrType PtrTypeKind = m_pTargetType->GetPtrTypeKind ();
	EStdFunc FuncKind = 
		PtrTypeKind == EFunctionPtrType_Unsafe ? EStdFunc_MulticastAdd_u :
		PtrTypeKind == EFunctionPtrType_Thin ? EStdFunc_MulticastAdd_t :
		PtrTypeKind == EFunctionPtrType_Weak ? EStdFunc_MulticastAdd_w : EStdFunc_MulticastAdd;

	ConvertToSimpleMulticastPtr (&ArgValueArray [0]);
	ConvertToSimpleFunctionPtr (&ArgValueArray [1]);

	CFunction* pStdFunction = m_pModule->m_FunctionMgr.GetStdFunction (FuncKind);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pStdFunction,
		pStdFunction->GetType (),
		ArgValueArray,
		countof (ArgValueArray),
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastType::CreateRemoveMethod ()
{
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

	m_pModule->m_OperatorMgr.PrepareDataPtr (&ArgValueArray [0], ERuntimeError_StoreOutOfRange);

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Remove, EDataPtrType_Unsafe);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValueArray,
		countof (ArgValueArray),
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastType::CreateRemoveMethod_u ()
{
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

	EFunctionPtrType PtrTypeKind = m_pTargetType->GetPtrTypeKind ();
	EStdFunc FuncKind = 
		PtrTypeKind == EFunctionPtrType_Unsafe ? EStdFunc_MulticastRemove_u :
		PtrTypeKind == EFunctionPtrType_Thin ? EStdFunc_MulticastRemove_t :
		PtrTypeKind == EFunctionPtrType_Weak ? EStdFunc_MulticastRemove_w : EStdFunc_MulticastRemove;

	ConvertToSimpleMulticastPtr (&ArgValueArray [0]);

	CFunction* pStdFunction = m_pModule->m_FunctionMgr.GetStdFunction (FuncKind);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pStdFunction,
		pStdFunction->GetType (),
		ArgValueArray,
		countof (ArgValueArray),
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastType::CreateSnapshotMethod ()
{
	CType* pReturnType = m_pTargetType->GetMcSnapshotType ();
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastSnapshot_s"), pType);

	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	m_pModule->m_OperatorMgr.PrepareDataPtr (&ArgValue, ERuntimeError_StoreOutOfRange);

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Snapshot, EDataPtrType_Unsafe);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValue,
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastType::CreateSnapshotMethod_u ()
{	
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

	// stdfunc returns simple-funciton snapshot so we either 
	// need a real ICastOperator or manually bitcast it to our type
	// since casting to mcsnapshot is not needed anywhere just bitcast it here

	ASSERT (ReturnValue.GetType ()->GetTypeKind () == EType_McSnapshot);

	CValue CountValue;
	CValue PtrPfnValue;

	m_pModule->m_LlvmBuilder.CreateExtractValue (ReturnValue, 0, NULL, &CountValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (ReturnValue, 1, NULL, &PtrPfnValue);
	m_pModule->m_LlvmBuilder.CreateBitCast (PtrPfnValue, m_pTargetType->GetDataPtrType (EDataPtrType_Unsafe), &PtrPfnValue);
	
	ReturnValue = pReturnType->GetUndefValue ();
	m_pModule->m_LlvmBuilder.CreateInsertValue (ReturnValue, CountValue, 0, pReturnType, &ReturnValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (ReturnValue, PtrPfnValue, 1, pReturnType, &ReturnValue);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastType::CreateCallMethod ()
{
	rtl::CArrayT <CType*> ArgTypeArray = m_pTargetType->GetTargetType ()->GetArgTypeArray ();
	ArgTypeArray.Insert (0, GetDataPtrType (EDataPtrType_Normal));
	size_t ArgCount = ArgTypeArray.GetCount ();

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray);
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastCall_s"), pType);

	char Buffer [256];
	rtl::CArrayT <CValue> ArgValueArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgValueArray.SetCount (ArgCount);

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, ArgCount);

	m_pModule->m_OperatorMgr.PrepareDataPtr (&ArgValueArray [0], ERuntimeError_LoadOutOfRange);

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Call, EDataPtrType_Unsafe);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValueArray,
		ArgCount,
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastType::CreateCallMethod_u ()
{
	rtl::CArrayT <CType*> ArgTypeArray = m_pTargetType->GetTargetType ()->GetArgTypeArray ();
	ArgTypeArray.Insert (0, GetDataPtrType (EDataPtrType_Unsafe));
	size_t ArgCount = ArgTypeArray.GetCount ();

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray);
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("MulticastCall_u"), pType);

	char Buffer [256];
	rtl::CArrayT <CValue> ArgValueArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgValueArray.SetCount (ArgCount);

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, ArgCount);

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Snapshot, EDataPtrType_Unsafe);

	rtl::CString s = jnc::GetLlvmTypeString (ArgValueArray [0].GetLlvmValue ()->getType ());

	CValue SnapshotValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValueArray [0],
		&SnapshotValue
		);

	CMcSnapshotType* pMcSnapshotType = m_pTargetType->GetMcSnapshotType ();

	ASSERT (SnapshotValue.GetType ()->Cmp (pMcSnapshotType) == 0);
	ArgValueArray [0] = SnapshotValue;

	pImplFunction = pMcSnapshotType->GetCallMethod ();

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValueArray,
		ArgCount,
		&ReturnValue
		);

	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_FunctionMgr.InternalEpilogue ();

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

	CValue PfnValue;
	CValue ClosureValue;

	m_pModule->m_LlvmBuilder.CreateExtractValue (*pFunctionPtrValue, 0, NULL, &PfnValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (*pFunctionPtrValue, 1, NULL, &ClosureValue);
	m_pModule->m_LlvmBuilder.CreateClosureFunctionPtr (PfnValue, ClosureValue, pFunctionPtrType, pFunctionPtrValue);

	if (PtrTypeKind == EFunctionPtrType_Weak)
	{
		CValue StrengthenValue;
		m_pModule->m_LlvmBuilder.CreateExtractValue (*pFunctionPtrValue, 2, NULL, &StrengthenValue);
		m_pModule->m_LlvmBuilder.CreateInsertValue (*pFunctionPtrValue, StrengthenValue, 2, pFunctionPtrType, pFunctionPtrValue);
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
