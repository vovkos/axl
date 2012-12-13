#include "stdafx.h"
#include "axl_jnc_LlvmBuilder.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CLlvmBuilder::CLlvmBuilder ():
	m_LlvmBuilder (llvm::getGlobalContext())
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);
}

llvm::SwitchInst*
CLlvmBuilder::CreateSwitch (
	const CValue& Value,
	CBasicBlock* pDefaultBlock,
	rtl::CHashTableMapIteratorT <intptr_t, CBasicBlock*> FirstCase,
	size_t CaseCount
	)
{
	CType* pType = Value.GetType ();
	ASSERT (pType->IsIntegerType ());

	llvm::SwitchInst* pInst = m_LlvmBuilder.CreateSwitch (
		Value.GetLlvmValue (), 
		pDefaultBlock->GetLlvmBlock (), 
		CaseCount
		);

	rtl::CHashTableMapIteratorT <intptr_t, CBasicBlock*> Case = FirstCase;
	for (; Case; Case++)
	{
		CValue ConstValue (Case->m_Key, pType);
		CBasicBlock* pBlock = Case->m_Value;

		pInst->addCase ((llvm::ConstantInt*) ConstValue.GetLlvmValue (), pBlock->GetLlvmBlock ());
	}

	return pInst;
}

llvm::PHINode*
CLlvmBuilder::CreatePhi (
	const CValue* pValueArray,
	CBasicBlock** pBlockArray,
	size_t Count,
	CValue* pResultValue
	)
{
	if (pValueArray->IsEmpty ())
	{
		pResultValue->SetVoid ();
		return NULL;
	}

	llvm::PHINode* pPhiNode = m_LlvmBuilder.CreatePHI (pValueArray->GetType ()->GetLlvmType (), Count, "phi");

	for (size_t i = 0; i < Count; i++)
		pPhiNode->addIncoming (pValueArray [i].GetLlvmValue (), pBlockArray [i]->GetLlvmBlock ());

	pResultValue->SetLlvmValue (pPhiNode, pValueArray->GetType ());
	return pPhiNode;
}

llvm::PHINode*
CLlvmBuilder::CreatePhi (
	const CValue& Value1,
	CBasicBlock* pBlock1,
	const CValue& Value2,
	CBasicBlock* pBlock2,
	CValue* pResultValue
	)
{
	if (Value1.IsEmpty ())
	{
		pResultValue->SetVoid ();
		return NULL;
	}

	llvm::PHINode* pPhiNode = m_LlvmBuilder.CreatePHI (Value1.GetLlvmValue ()->getType (), 2,  "phi");
	pPhiNode->addIncoming (Value1.GetLlvmValue (), pBlock1->GetLlvmBlock ());
	pPhiNode->addIncoming (Value2.GetLlvmValue (), pBlock2->GetLlvmBlock ());
	pResultValue->SetLlvmValue (pPhiNode, Value1.GetType ());
	return pPhiNode;
}

llvm::Value*
CLlvmBuilder::CreateGep (
	const CValue& Value,
	const CValue* pIndexArray,
	size_t IndexCount,
	CType* pResultType,
	CValue* pResultValue
	)
{
	char Buffer [256];
	rtl::CArrayT <llvm::Value*> LlvmIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	LlvmIndexArray.SetCount (IndexCount);

	for (size_t i = 0; i < IndexCount; i++)
		LlvmIndexArray [i] = pIndexArray [i].GetLlvmValue ();

	llvm::Value* pInst;
	pInst = m_LlvmBuilder.CreateGEP (
			Value.GetLlvmValue (), 
			llvm::ArrayRef <llvm::Value*> (LlvmIndexArray, IndexCount),
			"gep"
			);

	pResultValue->SetLlvmValue (pInst, pResultType);
	return pInst;
}

llvm::Value*
CLlvmBuilder::CreateGep (
	const CValue& Value,
	size_t* pIndexArray,
	size_t IndexCount,
	CType* pResultType,
	CValue* pResultValue
	)
{
	char Buffer [256];
	rtl::CArrayT <llvm::Value*> LlvmIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	LlvmIndexArray.SetCount (IndexCount);

	for (size_t i = 0; i < IndexCount; i++)
	{
		CValue IndexValue;
		IndexValue.SetConstSizeT (pIndexArray [i]);
		LlvmIndexArray [i] = IndexValue.GetLlvmValue ();
	}

	llvm::Value* pInst;
	pInst = m_LlvmBuilder.CreateGEP (
			Value.GetLlvmValue (), 
			llvm::ArrayRef <llvm::Value*> (LlvmIndexArray, IndexCount),
			"gep"
			);

	pResultValue->SetLlvmValue (pInst, pResultType);
	return pInst;
}

llvm::CallInst*
CLlvmBuilder::CreateCall (
	const CValue& CalleeValue,
	ECallConv CallingConvention,
	const CValue* pArgArray,
	size_t ArgCount,
	CType* pResultType, 
	CValue* pResultValue
	)
{
	char Buffer [256];
	rtl::CArrayT <llvm::Value*> LlvmArgArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	LlvmArgArray.SetCount (ArgCount);

	for (size_t i = 0; i < ArgCount; i++)
		LlvmArgArray [i] = pArgArray [i].GetLlvmValue ();

	llvm::CallInst* pInst;

	if (pResultType && pResultType->GetTypeKind () != EType_Void)
	{
		pInst = m_LlvmBuilder.CreateCall (
			CalleeValue.GetLlvmValue (), 
			llvm::ArrayRef <llvm::Value*> (LlvmArgArray, ArgCount),
			"call"
			);

		pResultValue->SetLlvmValue (pInst, pResultType);
	}
	else
	{
		pInst = m_LlvmBuilder.CreateCall (
			CalleeValue.GetLlvmValue (), 
			llvm::ArrayRef <llvm::Value*> (LlvmArgArray, ArgCount)
			);

		if (pResultValue)
			pResultValue->SetVoid ();
	}

	pInst->setCallingConv (GetLlvmCallingConvention (CallingConvention));
	return pInst;
}

llvm::Value*
CLlvmBuilder::CreateSafePtr (
	const CValue& PtrValue,
	const CValue& RegionBeginValue,
	size_t Size,
	const CValue& ScopeLevelValue,
	CPointerType* pResultType,
	CValue* pResultValue
	)
{
	CValue ValidatorValue;
	CreateSafePtrValidator (
		RegionBeginValue,
		Size,
		ScopeLevelValue,
		&ValidatorValue
		);
	
	CValue SafePtrValue = pResultType->GetUndefValue ();
	CreateInsertValue (SafePtrValue, PtrValue, 0, NULL, &SafePtrValue);
	return CreateInsertValue (SafePtrValue, ValidatorValue, 1, pResultType, pResultValue);
}

llvm::Value*
CLlvmBuilder::ModifySafePtr (
	const CValue& SrcSafePtrValue,
	const CValue& PtrValue,
	CPointerType* pResultType,
	CValue* pResultValue
	)
{
	CValue ValidatorValue;
	CreateExtractValue (SrcSafePtrValue, 0, NULL, &ValidatorValue);
	
	CValue SafePtrValue = pResultType->GetUndefValue ();
	CreateInsertValue (SafePtrValue, PtrValue, 0, NULL, &SafePtrValue);
	return CreateInsertValue (SafePtrValue, ValidatorValue, 1, pResultType, pResultValue);
}

llvm::Value*
CLlvmBuilder::CreateSafePtrValidator (
	const CValue& RawRegionBeginValue,
	size_t Size,
	const CValue& ScopeLevelValue,
	CValue* pResultValue
	)
{
	CValue SizeValue (Size, EType_SizeT);

	CValue RegionBeginValue;
	CreateBitCast (RawRegionBeginValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &RegionBeginValue);
	
	CFunction* pCreateValidator = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CreateSafePtrValidator);

	return CreateCall3 (
		pCreateValidator,
		pCreateValidator->GetType (),
		RegionBeginValue, 
		SizeValue,
		ScopeLevelValue,
		pResultValue
		);
}

bool
CLlvmBuilder::CheckSafePtrRange (
	const CValue& RawPtrValue,
	size_t Size,
	const CValue& ValidatorValue,
	ERuntimeError Error
	)
{
	CValue SizeValue (Size, EType_SizeT);
	CValue ErrorValue (Error, EType_Int);

	CValue PtrValue;
	CreateBitCast (RawPtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CFunction* pCheckSafePtrRange = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckSafePtrRange);

	CreateCall4 (
		pCheckSafePtrRange,
		pCheckSafePtrRange->GetType (),
		PtrValue,
		SizeValue,
		ValidatorValue,
		ErrorValue,
		NULL
		);

	return true;
}

bool
CLlvmBuilder::CheckSafePtrScope (
	const CValue& Value,
	CScope* pDstScope
	)
{
	size_t DstScopeLevel = pDstScope ? pDstScope->GetLevel () : 0;

	if (Value.GetValueKind () == EValue_Variable)
	{
		CScope* pSrcScope = Value.GetVariable ()->GetScope ();
		size_t SrcScopeLevel = pSrcScope ? pSrcScope->GetLevel () : 0;

		if (SrcScopeLevel > DstScopeLevel)
		{
			err::SetFormatStringError (_T("safe pointer/reference scope level mismatch"));
			return false;
		}

		return true;
	}

	CValue DstScopeLevelValue (DstScopeLevel, EType_SizeT);
	CValue SrcScopeLevelValue;

	size_t ScopeLevelIndexArray [2] = { 1, 2 };
	CreateExtractValue (Value, ScopeLevelIndexArray, 2, NULL, &SrcScopeLevelValue);

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

llvm::Value*
CLlvmBuilder::CreateInterface (
	const CValue& PtrValue,
	const CValue& ScopeLevelValue,
	CClassType* pResultType,
	CValue* pResultValue
	)
{
	CValue InterfaceValue = pResultType->GetUndefValue ();
	CreateInsertValue (InterfaceValue, PtrValue, 0, NULL, &InterfaceValue);
	return CreateInsertValue (InterfaceValue, ScopeLevelValue, 1, pResultType, pResultValue);
}

bool
CLlvmBuilder::CheckNullPtr (
	const CValue& Value,
	ERuntimeError Error
	)
{
	CValue PtrValue;
	CreateExtractValue (Value, 0, NULL, &PtrValue);
	CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckNullPtr);	
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pCheckFunction, 
		pCheckFunction->GetType (), 
		PtrValue, 
		CValue (Error, EType_Int),
		NULL
		);

	return true;
}

bool
CLlvmBuilder::CheckInterfaceScope (
	const CValue& Value,
	CScope* pDstScope
	)
{
	size_t DstScopeLevel = pDstScope ? pDstScope->GetLevel () : 0;
	CValue DstScopeLevelValue (DstScopeLevel, EType_SizeT);
	CValue SrcScopeLevelValue;

	CreateExtractValue (Value, 1, NULL, &SrcScopeLevelValue);

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

bool
CLlvmBuilder::DynamicCastInterface (
	const CValue& Value,
	CClassType* pResultType,
	CValue* pResultValue
	)
{
	CValue PtrValue;
	CValue ScopeLevelValue;

	CreateExtractValue (Value, 0, NULL, &PtrValue);
	CreateExtractValue (Value, 1, NULL, &ScopeLevelValue);
	
	CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CValue TypeValue (&pResultType, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));

	CFunction* pDynamicCastInterface = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_DynamicCastInterface);
	
	m_pModule->m_LlvmBuilder.CreateCall2 (
		pDynamicCastInterface,
		pDynamicCastInterface->GetType (),
		PtrValue,
		TypeValue,
		&PtrValue
		);

	CreateBitCast (PtrValue, pResultType->GetInterfaceStructType()->GetPointerType (EType_Pointer_u), &PtrValue);

	CreateInterface (PtrValue, ScopeLevelValue, pResultType, pResultValue);
	return true;
}

bool
CLlvmBuilder::InitializeObject (
	const CValue& Value,
	CClassType* pType,
	int Flags
	)
{
	CValue FlagsValue (Flags, EType_Int);

	CFunction* pInializeObject = pType->GetInitializer ();
	if (!pInializeObject)
		return false;

	m_pModule->m_LlvmBuilder.CreateCall2 (
		pInializeObject,
		pInializeObject->GetType (),
		Value,
		FlagsValue,
		NULL
		);

	return true;
}

bool
CLlvmBuilder::CreateFunctionPointer (
	const CValue& PtrValue,
	ECallConv CallingConvention,
	const CValue& InterfaceValue,
	CFunctionPointerType* pResultType,
	CValue* pResultValue
	)
{
	CValue CallConvValue (CallingConvention, EType_Int_p);

	CValue FunctionPtrValue = pResultType->GetUndefValue ();
	CreateInsertValue (FunctionPtrValue, PtrValue, 0, NULL, &FunctionPtrValue);
	CreateInsertValue (FunctionPtrValue, CallConvValue, 1, NULL, &FunctionPtrValue);
	CreateInsertValue (FunctionPtrValue, InterfaceValue, 2, pResultType, pResultValue);
	return true;
}

bool
CLlvmBuilder::CheckFunctionPointerScope (
	const CValue& Value,
	CScope* pDstScope
	)
{
	size_t DstScopeLevel = pDstScope ? pDstScope->GetLevel () : 0;
	CValue DstScopeLevelValue (DstScopeLevel, EType_SizeT);
	CValue SrcScopeLevelValue;

	size_t ScopeLevelIndexArray [2] = { 2, 1 };
	CreateExtractValue (Value, ScopeLevelIndexArray, 2, NULL, &SrcScopeLevelValue);
	
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

bool
CLlvmBuilder::CreatePropertyPointer (
	const CValue& PtrValue,
	const CValue& InterfaceValue,
	CPropertyPointerType* pResultType,
	CValue* pResultValue
	)
{
	ASSERT (PtrValue.GetType ()->GetTypeKind () == EType_Property);
	CPropertyType* pPropertyType = (CPropertyType*) PtrValue.GetType ();

	ASSERT (pPropertyType->CmpAccessorTypes (pResultType->GetPropertyType ()) == 0);

	CValue VTablePtrValue;
	if (PtrValue.GetValueKind () == EValue_Property)
	{
		bool Result = pPropertyType->GetVTablePtrValue (&VTablePtrValue);
		if (!Result)
			return false;
	}
	else
	{
		VTablePtrValue = PtrValue;
	}
	
	CStructType* pVTableType = pResultType->GetPropertyType ()->GetVTableStructType ();
	CreateBitCast (VTablePtrValue, pVTableType->GetPointerType (EType_Pointer_u), &VTablePtrValue);

	CValue PropertyPtrValue = pResultType->GetUndefValue ();
	CreateInsertValue (PropertyPtrValue, VTablePtrValue, 0, NULL, &PropertyPtrValue);
	CreateInsertValue (PropertyPtrValue, InterfaceValue, 1, NULL, &PropertyPtrValue);

	pResultValue->SetLlvmValue (
		PropertyPtrValue.GetLlvmValue (),
		pResultType,
		PtrValue.GetValueKind () == EValue_Property ? EValue_Property : EValue_LlvmRegister
		);

	return true;
}

bool
CLlvmBuilder::CheckPropertyPointerScope (
	const CValue& Value,
	CScope* pDstScope
	)
{
	if (Value.GetValueKind () == EValue_Property) 
		return true;

	size_t DstScopeLevel = pDstScope ? pDstScope->GetLevel () : 0;
	CValue DstScopeLevelValue (DstScopeLevel, EType_SizeT);
	CValue SrcScopeLevelValue;

	size_t ScopeLevelIndexArray [2] = { 1, 1 };
	CreateExtractValue (Value, ScopeLevelIndexArray, 2, NULL, &SrcScopeLevelValue);

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

//.............................................................................

} // namespace axl {
} // namespace jnc {
