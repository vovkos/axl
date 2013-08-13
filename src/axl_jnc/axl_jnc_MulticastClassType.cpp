#include "pch.h"
#include "axl_jnc_MulticastClassType.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Runtime.h"

namespace axl {
namespace jnc {

//.............................................................................

CMulticastClassType::CMulticastClassType ()
{
	m_ClassTypeKind = EClassType_Multicast;
	m_pTargetType = NULL;
	m_pSnapshotType = NULL;
	m_pEventClassPtrTypeTuple = NULL;
	memset (m_FieldArray, 0, sizeof (m_FieldArray));
	memset (m_MethodArray, 0, sizeof (m_MethodArray));
}

void
CMulticastClassType::PrepareTypeString ()
{
	m_TypeString = m_pTargetType->GetTypeModifierString ();
	m_TypeString.AppendFormat ("multicast %s", m_pTargetType->GetTargetType ()->GetArgString ().cc ());
}

bool
CMulticastClassType::CompileCallMethod ()
{
	CFunction* pFunction = m_MethodArray [EMulticastMethod_Call];

	size_t ArgCount = pFunction->GetType ()->GetArgArray ().GetCount ();

	char Buffer [256];
	rtl::CArrayT <CValue> ArgValueArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgValueArray.SetCount (ArgCount);

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, ArgCount);

	CFunction* pGetSnapshot = m_MethodArray [EMulticastMethod_GetSnapshot];

	CValue SnapshotValue;
	m_pModule->m_LlvmIrBuilder.CreateCall (
		pGetSnapshot,
		pGetSnapshot->GetType (),
		ArgValueArray [0],
		&SnapshotValue
		);

	rtl::CBoxListT <CValue> ArgList;
	for (size_t i = 1; i < ArgCount; i++)
		ArgList.InsertTail (ArgValueArray [i]);

	m_pModule->m_OperatorMgr.CallOperator (SnapshotValue, &ArgList);

	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_FunctionMgr.InternalEpilogue ();		

	return true;
}

void
CMulticastClassType::EnumGcRoots (
	CRuntime* pRuntime,
	void* _p
	)
{
	TObject* pObject = (TObject*) _p;
	ASSERT (pObject->m_pType == this);

	TMulticast* pMulticast = (TMulticast*) (pObject + 1);
	if (!(m_pTargetType->GetFlags () & ETypeFlag_GcRoot) || !pMulticast->m_Count)
		return;

	char* p = (char*) pMulticast->m_pPtrArray;
	size_t Size = m_pTargetType->GetSize ();

	for (size_t i = 0; i < pMulticast->m_Count; i++, p += Size)
		pRuntime->AddGcRoot (p, m_pTargetType);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

/*

CStructType* 
CMulticastClassType::GetMulticastStructType ()
{
	if (m_pMulticastStructType)
		return m_pMulticastStructType;

	m_pMulticastStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pMulticastStructType->m_Tag.Format ("multicast");

	m_FieldArray [EMulticastField_Lock] = m_pMulticastStructType->CreateField (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p));
	m_FieldArray [EMulticastField_Count] = m_pMulticastStructType->CreateField (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_FieldArray [EMulticastField_MaxCount] = m_pMulticastStructType->CreateField (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_FieldArray [EMulticastField_PtrArray] = m_pMulticastStructType->CreateField (m_pTargetType->GetDataPtrType_c ());
	m_FieldArray [EMulticastField_HandleTable] = m_pMulticastStructType->CreateField (m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));
	
	m_pMulticastStructType->EnsureLayout ();

	return m_pMulticastStructType;
}

void
CMulticastClassType::PrepareTypeString ()
{
	m_TypeString += m_pTargetType->GetTypeModifierString ();
	m_TypeString += "multicast ";
	m_TypeString += m_pTargetType->GetTargetType ()->GetArgString ();
}

CFunction* 
CMulticastClassType::GetMethod (
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
CMulticastClassType::CreateClearMethod ()
{
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastClear_s", pType);

	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	m_pModule->m_OperatorMgr.PrepareDataPtr (&ArgValue);
	m_pModule->m_LlvmIrBuilder.CreateStore (GetZeroValue (), ArgValue);
	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastClassType::CreateClearMethod_u ()
{
	ASSERT (false);

	CType* ArgTypeArray [] =
	{
		GetDataPtrType_c (),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastClear_u", pType);

	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	m_pModule->m_LlvmIrBuilder.CreateStore (GetZeroValue (), ArgValue);
	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastClassType::CreateSetMethod ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastSet_s", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	m_pModule->m_OperatorMgr.PrepareDataPtr (ArgValue1, &ArgValue1);

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Set, EDataPtrType_Unsafe);

	CValue ReturnValue;
	m_pModule->m_LlvmIrBuilder.CreateCall2 (
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
CMulticastClassType::CreateSetMethod_u ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType_c (),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastSet_u", pType);

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
	m_pModule->m_LlvmIrBuilder.CreateCall (
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
CMulticastClassType::CreateAddMethod ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastAdd_s", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	m_pModule->m_OperatorMgr.PrepareDataPtr (&ArgValueArray [0]);

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Add, EDataPtrType_Unsafe);

	CValue ReturnValue;
	m_pModule->m_LlvmIrBuilder.CreateCall (
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
CMulticastClassType::CreateAddMethod_u ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType_c (),
		m_pTargetType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastAdd_u", pType);

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
	m_pModule->m_LlvmIrBuilder.CreateCall (
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
CMulticastClassType::CreateRemoveMethod ()
{
	CType* pReturnType = m_pTargetType;
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastRemove_s", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	m_pModule->m_OperatorMgr.PrepareDataPtr (&ArgValueArray [0]);

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Remove, EDataPtrType_Unsafe);

	CValue ReturnValue;
	m_pModule->m_LlvmIrBuilder.CreateCall (
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
CMulticastClassType::CreateRemoveMethod_u ()
{
	CType* pReturnType = m_pTargetType;
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastRemove_u", pType);

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
	m_pModule->m_LlvmIrBuilder.CreateCall (
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
CMulticastClassType::CreateSnapshotMethod ()
{
	CType* pReturnType = m_pTargetType->GetMcSnapshotType ();
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType (EDataPtrType_Normal),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastSnapshot_s", pType);

	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	m_pModule->m_OperatorMgr.PrepareDataPtr (&ArgValue);

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Snapshot, EDataPtrType_Unsafe);

	CValue ReturnValue;
	m_pModule->m_LlvmIrBuilder.CreateCall (
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
CMulticastClassType::CreateSnapshotMethod_u ()
{	
	CType* pReturnType = m_pTargetType->GetMcSnapshotType ();
	
	CType* ArgTypeArray [] =
	{
		GetDataPtrType_c (),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastSnapshot_u", pType);

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
	m_pModule->m_LlvmIrBuilder.CreateCall (
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

	m_pModule->m_LlvmIrBuilder.CreateExtractValue (ReturnValue, 0, NULL, &CountValue);
	m_pModule->m_LlvmIrBuilder.CreateExtractValue (ReturnValue, 1, NULL, &PtrPfnValue);
	m_pModule->m_LlvmIrBuilder.CreateBitCast (PtrPfnValue, m_pTargetType->GetDataPtrType_c (), &PtrPfnValue);
	
	ReturnValue = pReturnType->GetUndefValue ();
	m_pModule->m_LlvmIrBuilder.CreateInsertValue (ReturnValue, CountValue, 0, pReturnType, &ReturnValue);
	m_pModule->m_LlvmIrBuilder.CreateInsertValue (ReturnValue, PtrPfnValue, 1, pReturnType, &ReturnValue);

	m_pModule->m_ControlFlowMgr.Return (ReturnValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return pFunction;
}

CFunction* 
CMulticastClassType::CreateCallMethod ()
{
	CFunctionArg* pThisArg = GetDataPtrType (EDataPtrType_Normal)->GetSimpleFunctionArg ();

	rtl::CArrayT <CFunctionArg*> ArgArray = m_pTargetType->GetTargetType ()->GetArgArray ();
	ArgArray.Insert (0, pThisArg);
	size_t ArgCount = ArgArray.GetCount ();

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgArray);
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastCall_s", pType);

	char Buffer [256];
	rtl::CArrayT <CValue> ArgValueArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgValueArray.SetCount (ArgCount);

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, ArgCount);

	m_pModule->m_OperatorMgr.PrepareDataPtr (&ArgValueArray [0]);

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Call, EDataPtrType_Unsafe);

	CValue ReturnValue;
	m_pModule->m_LlvmIrBuilder.CreateCall (
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
CMulticastClassType::CreateCallMethod_u ()
{
	CFunctionArg* pThisArg = GetDataPtrType_c ()->GetSimpleFunctionArg ();

	rtl::CArrayT <CFunctionArg*> ArgArray = m_pTargetType->GetTargetType ()->GetArgArray ();
	ArgArray.Insert (0, pThisArg);
	size_t ArgCount = ArgArray.GetCount ();

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgArray);
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("MulticastCall_u", pType);

	char Buffer [256];
	rtl::CArrayT <CValue> ArgValueArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgValueArray.SetCount (ArgCount);

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, ArgCount);

	CFunction* pImplFunction = GetMethod (EMulticastMethod_Snapshot, EDataPtrType_Unsafe);

	rtl::CString s = jnc::GetLlvmTypeString (ArgValueArray [0].GetLlvmValue ()->getType ());

	CValue SnapshotValue;
	m_pModule->m_LlvmIrBuilder.CreateCall (
		pImplFunction,
		pImplFunction->GetType (),
		ArgValueArray [0],
		&SnapshotValue
		);

	CMcSnapshotClassType* pMcSnapshotType = m_pTargetType->GetMcSnapshotType ();

	ASSERT (SnapshotValue.GetType ()->Cmp (pMcSnapshotType) == 0);
	ArgValueArray [0] = SnapshotValue;

	pImplFunction = pMcSnapshotType->GetMethod (EMcSnapshotMethod_Call);

	CValue ReturnValue;
	m_pModule->m_LlvmIrBuilder.CreateCall (
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
CMulticastClassType::ConvertToSimpleMulticastPtr (CValue* pMulticastPtrValue)
{
	EFunctionPtrType PtrTypeKind = m_pTargetType->GetPtrTypeKind ();
	CFunctionType* pFuncitonType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	CFunctionPtrType* pFuncitonPtrType = pFuncitonType->GetFunctionPtrType (PtrTypeKind);
	CDataPtrType* pMulticastPtrType = pFuncitonPtrType->GetMulticastType ()->GetDataPtrType_c ();

	m_pModule->m_LlvmIrBuilder.CreateBitCast (*pMulticastPtrValue, pMulticastPtrType, pMulticastPtrValue);
}

void
CMulticastClassType::ConvertToSimpleFunctionPtr (CValue* pFunctionPtrValue)
{
	EFunctionPtrType PtrTypeKind = m_pTargetType->GetPtrTypeKind ();
	CFunctionType* pFuncitonType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	CFunctionPtrType* pFunctionPtrType = pFuncitonType->GetFunctionPtrType (PtrTypeKind);

	if (PtrTypeKind == EFunctionPtrType_Unsafe || PtrTypeKind == EFunctionPtrType_Thin)
	{
		m_pModule->m_LlvmIrBuilder.CreateBitCast (*pFunctionPtrValue, pFunctionPtrType, pFunctionPtrValue);	
		return;
	}

	CValue PfnValue;
	CValue ClosureValue;

	m_pModule->m_LlvmIrBuilder.CreateExtractValue (*pFunctionPtrValue, 0, NULL, &PfnValue);
	m_pModule->m_LlvmIrBuilder.CreateExtractValue (*pFunctionPtrValue, 1, NULL, &ClosureValue);
	m_pModule->m_LlvmIrBuilder.CreateClosureFunctionPtr (PfnValue, ClosureValue, pFunctionPtrType, pFunctionPtrValue);

	if (PtrTypeKind == EFunctionPtrType_Weak)
	{
		CValue StrengthenValue;
		m_pModule->m_LlvmIrBuilder.CreateExtractValue (*pFunctionPtrValue, 2, NULL, &StrengthenValue);
		m_pModule->m_LlvmIrBuilder.CreateInsertValue (*pFunctionPtrValue, StrengthenValue, 2, pFunctionPtrType, pFunctionPtrValue);
	}
}

CType*
COperatorMgr::GetMulticastMemberType (
	const CValue& OpValue,
	CMulticastClassType* pMulticastType,
	const char* pName
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pPtrType = (CDataPtrType*) OpValue.GetType ();
	
	rtl::CStringHashTableMapIteratorT <EMulticastMethod> Method = m_MulticastMethodMap.Find (pName);
	if (!Method)
	{
		err::SetFormatStringError ("'%s' is not a member of '%s'", pName, pMulticastType->GetTypeString ().cc ());
		return NULL;
	}

	CFunction* pFunction = pMulticastType->GetMethod (Method->m_Value, pPtrType->GetPtrTypeKind ());
	CFunctionPtrType* pFunctionPtrType = pFunction->GetType ()->GetFunctionPtrType (EFunctionPtrType_Thin);

	CType* pThisType = GetUnaryOperatorResultType (EUnOp_Addr, OpValue);
	if (!pThisType)
		return NULL;

	rtl::CBoxListT <CValue> ArgList;
	ArgList.InsertHead (pThisType);
	return GetClosureOperatorResultType (pFunctionPtrType, &ArgList);
}

bool
COperatorMgr::GetMulticastMember (
	const CValue& OpValue,
	CMulticastClassType* pMulticastType,
	const char* pName,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pPtrType = (CDataPtrType*) OpValue.GetType ();
	
	rtl::CStringHashTableMapIteratorT <EMulticastMethod> Method = m_MulticastMethodMap.Find (pName);
	if (!Method)
	{
		err::SetFormatStringError ("'%s' is not a member of '%s'", pName, pMulticastType->GetTypeString ().cc ());
		return false;
	}

	CFunction* pFunction = pMulticastType->GetMethod (Method->m_Value, pPtrType->GetPtrTypeKind ());
	
	CValue PtrValue;
	return 
		UnaryOperator (EUnOp_Addr, OpValue, &PtrValue) &&
		ClosureOperator (pFunction, PtrValue, pResultValue);
}

from COperatorMgr::CallOperator () 
{
	...

	if (OpValue.GetType ()->GetTypeKind () == EType_DataRef)
	{
		CDataPtrType* pPtrType = (CDataPtrType*) OpValue.GetType ();
		ASSERT (pPtrType->GetTargetType ()->GetTypeKind () == EType_Multicast);

		CMulticastClassType* pMulticastType = (CMulticastClassType*) pPtrType->GetTargetType ();
		CFunction* pCallMethod = pMulticastType->GetMethod (EMulticastMethod_Call, pPtrType->GetPtrTypeKind ());

		ref::CPtrT <CClosure> Closure = OpValue.GetClosure ();
		if (!Closure)
			Closure = OpValue.CreateClosure ();

		CValue PtrValue;
		UnaryOperator (EUnOp_Addr, OpValue, &PtrValue);
		Closure->GetArgList ()->InsertHead (PtrValue);

		OpValue.SetFunction (pCallMethod);
		OpValue.SetClosure (Closure);
	}
	...
}

*/

