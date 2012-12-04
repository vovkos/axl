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

llvm::PHINode*
CLlvmBuilder::CreatePhi (
	const CValue* pValueArray,
	CBasicBlock** pBlockArray,
	size_t Count
	)
{
	llvm::PHINode* pPhiNode = m_LlvmBuilder.CreatePHI (pValueArray->GetType ()->GetLlvmType (), Count, "phi");

	for (size_t i = 0; i < Count; i++)
		pPhiNode->addIncoming (pValueArray [i].GetLlvmValue (), pBlockArray [i]->GetLlvmBlock ());

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

	rtl::CString s = GetLlvmTypeString (CalleeValue.GetLlvmValue ()->getType ());

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
		RegionBeginValue, 
		SizeValue,
		ScopeLevelValue,
		pCreateValidator->GetType ()->GetReturnType (),
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
		PtrValue,
		SizeValue,
		ValidatorValue,
		ErrorValue,
		NULL,
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

	CValue PtrValue;
	CValue ValidatorValue;
	CValue DstScopeLevelValue (DstScopeLevel, EType_SizeT);

	CreateExtractValue (Value, 0, NULL, &PtrValue);
	CreateExtractValue (Value, 1, NULL, &ValidatorValue);
	CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckSafePtrScope);
	
	m_pModule->m_LlvmBuilder.CreateCall3 (
		pCheckFunction,
		PtrValue,
		ValidatorValue,
		DstScopeLevelValue,
		NULL,
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
CLlvmBuilder::CheckInterfaceNull (const CValue& Value)
{
	CValue PtrValue;
	CreateExtractValue (Value, 0, NULL, &PtrValue);
	CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckInterfaceNull);	
	m_pModule->m_LlvmBuilder.CreateCall (pCheckFunction, PtrValue, NULL, NULL);
	return true;
}

bool
CLlvmBuilder::CheckInterfaceScope (
	const CValue& Value,
	CScope* pDstScope
	)
{
	CValue PtrValue;
	CValue ScopeLevelValue;

	CreateExtractValue (Value, 0, NULL, &PtrValue);
	CreateExtractValue (Value, 1, NULL, &ScopeLevelValue);
	CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	size_t DstScopeLevel = pDstScope ? pDstScope->GetLevel () : 0;
	CValue DstScopeLevelValue (DstScopeLevel, EType_SizeT);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckInterfaceScope);
	
	m_pModule->m_LlvmBuilder.CreateCall3 (
		pCheckFunction,
		PtrValue,
		ScopeLevelValue,
		DstScopeLevelValue,
		NULL,
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
		PtrValue,
		TypeValue,
		pDynamicCastInterface->GetType ()->GetReturnType (),
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
		Value,
		FlagsValue,
		NULL,
		NULL
		);

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
