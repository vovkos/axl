#include "stdafx.h"
#include "axl_jnc_LlvmBuilder.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CLlvmBuilder::CLlvmBuilder ():
	m_LlvmBuilder (llvm::getGlobalContext ())
{
	m_CommentMdKind = llvm::getGlobalContext().getMDKindID ("jnc.comment");
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);
}

bool
CLlvmBuilder::CreateCommentV (
	const char* pFormat,
	va_list va
	)
{
	if (m_LlvmBuilder.GetInsertBlock ()->getInstList ().empty ())
		return false;

	char Buffer [256];
	rtl::CStringA String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.FormatV (pFormat, va);

	llvm::Instruction* pInst = &m_LlvmBuilder.GetInsertBlock ()->getInstList ().back ();
	llvm::MDString* pMdString = llvm::MDString::get (llvm::getGlobalContext (), String);	
	llvm::MDNode* pMdNode = llvm::MDNode::get (llvm::getGlobalContext (), llvm::ArrayRef <llvm::Value*> ((llvm::Value**) &pMdString, 1));
	pInst->setMetadata (m_CommentMdKind, pMdNode);
	return true;
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
		rtl::CString s = GetLlvmTypeString (CalleeValue.GetLlvmValue ()->getType ());

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
	
	CreateComment ("create safe pointer");

	CValue SafePtrValue = pResultType->GetUndefValue ();
	CreateInsertValue (SafePtrValue, PtrValue, 0, NULL, &SafePtrValue);
	return CreateInsertValue (SafePtrValue, ValidatorValue, 1, pResultType, pResultValue);
}

llvm::Value*
CLlvmBuilder::CreateSafePtr (
	const CValue& PtrValue,
	const CValue& RegionBeginValue,
	size_t Size,
	CScope* pScope,
	CPointerType* pResultType,
	CValue* pResultValue
	)
{
	CValue ScopeLevelValue = m_pModule->m_OperatorMgr.CalcScopeLevelValue (pScope);

	return CreateSafePtr (
		PtrValue,
		RegionBeginValue,
		Size,
		ScopeLevelValue,
		pResultType,
		pResultValue
		);
}

llvm::Value*
CLlvmBuilder::ModifySafePtr (
	const CValue& SrcSafePtrValue,
	const CValue& PtrValue,
	CPointerType* pResultType,
	CValue* pResultValue
	)
{
	CreateComment ("modify safe pointer");

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
	CreateComment ("create safe pointer validator");

	CValue SizeValue (Size, EType_SizeT);

	CValue RegionBeginValue;
	CreateBitCast (RawRegionBeginValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &RegionBeginValue);

	CValue RegionEndValue;
	CreateGep (RegionBeginValue, SizeValue, NULL, &RegionEndValue);
	
	CType* pType = m_pModule->m_TypeMgr.GetStdType (EStdType_SafePtrValidator);

	CValue ValidatorValue = pType->GetUndefValue ();
	m_pModule->m_LlvmBuilder.CreateInsertValue (ValidatorValue, RegionBeginValue, 0, NULL, &ValidatorValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (ValidatorValue, RegionEndValue, 1, NULL, &ValidatorValue);	
	return m_pModule->m_LlvmBuilder.CreateInsertValue (ValidatorValue, ScopeLevelValue, 2, pType, pResultValue);
}

llvm::Value*
CLlvmBuilder::CreateSafePtrValidator (
	const CValue& RegionBeginValue,
	size_t Size,
	CScope* pScope,
	CValue* pResultValue
	)
{
	CValue ScopeLevelValue = m_pModule->m_OperatorMgr.CalcScopeLevelValue (pScope);

	return CreateSafePtrValidator (
		RegionBeginValue,
		Size,
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
	CreateComment ("check safe pointer range");

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
CLlvmBuilder::GetDstScopeLevel (
	const CValue& DstValue,
	CValue* pDstScopeLevelValue
	)
{
	if (DstValue.GetValueKind () == EValue_Variable)
	{
		CScope* pScope = DstValue.GetVariable ()->GetScope ();
		m_pModule->m_OperatorMgr.CalcScopeLevelValue (pScope, pDstScopeLevelValue);
	}
	else if (DstValue.GetType ()->GetTypeKind () == EType_Reference)
	{
		size_t ScopeLevelIndexArray [] = 
		{
			1, // TSafePtrValidator m_Validator
			2, // size_t m_ScopeLvel
		};

		CreateExtractValue (DstValue, ScopeLevelIndexArray, countof (ScopeLevelIndexArray), m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT), pDstScopeLevelValue);
	}
	else
	{
		pDstScopeLevelValue->SetConstSizeT (0);
	}

	return true;
}

bool
CLlvmBuilder::CheckSafePtrScopeLevel (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CValue SrcScopeLevelValue;

	if (SrcValue.GetValueKind () == EValue_Variable)
	{
		CScope* pSrcScope = SrcValue.GetVariable ()->GetScope ();
		size_t SrcScopeLevel = pSrcScope ? pSrcScope->GetLevel () : 0;

		if (DstValue.GetValueKind () == EValue_Variable)  // can check at compile time
		{
			CScope* pDstScope = DstValue.GetVariable ()->GetScope ();

			size_t DstScopeLevel = pDstScope ? pDstScope->GetLevel () : 0;

			if (SrcScopeLevel > DstScopeLevel)
			{
				err::SetFormatStringError (_T("safe pointer/reference scope level mismatch"));
				return false;
			}

			return true;
		}

		SrcScopeLevelValue.SetConstSizeT (SrcScopeLevel);
	}
	else
	{
		size_t ScopeLevelIndexArray [] = 
		{
				1, // TSafePtrValidator m_Validator
				2, // size_t m_ScopeLevel
		};

		CreateExtractValue (SrcValue, ScopeLevelIndexArray, countof (ScopeLevelIndexArray), NULL, &SrcScopeLevelValue);
	}

	CValue DstScopeLevelValue;
	GetDstScopeLevel (DstValue, &DstScopeLevelValue);

	CreateComment ("check safe pointer scope level");

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckScopeLevel);
	
	CreateCall2 (
		pCheckFunction,
		pCheckFunction->GetType (),
		SrcScopeLevelValue,
		DstScopeLevelValue,
		NULL
		);

	return true;
}

bool
CLlvmBuilder::CheckNullPtr (const CValue& Value)
{
	CreateComment ("check null pointer");

	CValue PtrValue;
	ERuntimeError Error;

	CType* pType = Value.GetType ();
	EType TypeKind = pType->GetTypeKind ();

	switch (TypeKind)
	{
	case EType_Interface:
	case EType_Class:
		PtrValue = Value;
		Error = ERuntimeError_NullInterface;
		break;

	case EType_FunctionPointer:
		CreateExtractValue (Value, 0, NULL, &PtrValue);
		Error = ERuntimeError_NullFunction;
		break;

	case EType_PropertyPointer:
		CreateExtractValue (Value, 0, NULL, &PtrValue);
		Error = ERuntimeError_NullProperty;
		break;
		
	default:
		ASSERT (false);
	}

	CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckNullPtr);	
	CreateCall2 (
		pCheckFunction, 
		pCheckFunction->GetType (), 
		PtrValue, 
		CValue (Error, EType_Int),
		NULL
		);

	return true;
}

bool
CLlvmBuilder::CheckInterfaceScopeLevel (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CValue DstScopeLevelValue;
	GetDstScopeLevel (DstValue, &DstScopeLevelValue);

	CreateComment ("check interface scope level");

	CValue AbstractInterfaceValue;
	CreateBitCast (SrcValue, m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr), &AbstractInterfaceValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckInterfaceScopeLevel);

	CreateCall2 (
		pCheckFunction,
		pCheckFunction->GetType (),
		AbstractInterfaceValue,
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
	CreateComment ("dynamic cast interface");

	CValue PtrValue;
	CreateBitCast (Value, m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr), &PtrValue);

	CValue TypeValue (&pResultType, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));

	CFunction* pDynamicCastInterface = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_DynamicCastInterface);
	
	CreateCall2 (
		pDynamicCastInterface,
		pDynamicCastInterface->GetType (),
		PtrValue,
		TypeValue,
		&PtrValue
		);

	CreateBitCast (PtrValue, pResultType, &PtrValue);
	return true;
}

bool
CLlvmBuilder::InitializeObject (
	const CValue& Value,
	CClassType* pType,
	CScope* pScope
	)
{
	CreateComment ("initialize object");

	CFunction* pInializeObject = pType->GetInitializer ();
	if (!pInializeObject)
		return false;

	CreateCall2 (
		pInializeObject,
		pInializeObject->GetType (),
		Value,
		m_pModule->m_OperatorMgr.CalcScopeLevelValue (pScope),
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
	CreateComment ("create function pointer");

	CValue CallConvValue (CallingConvention, EType_Int_p);

	CValue FunctionPtrValue = pResultType->GetUndefValue ();
	CreateInsertValue (FunctionPtrValue, PtrValue, 0, NULL, &FunctionPtrValue);
	CreateInsertValue (FunctionPtrValue, CallConvValue, 1, NULL, &FunctionPtrValue);
	CreateInsertValue (FunctionPtrValue, InterfaceValue, 2, pResultType, pResultValue);
	return true;
}

bool
CLlvmBuilder::CheckFunctionPointerScopeLevel (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CValue DstScopeLevelValue;
	GetDstScopeLevel (DstValue, &DstScopeLevelValue);

	CreateComment ("check funtion pointer scope");

	CValue InterfaceValue;
	CreateExtractValue (SrcValue, 1, NULL, &InterfaceValue);
	
	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckInterfaceScopeLevel);
	
	CreateCall2 (
		pCheckFunction,
		pCheckFunction->GetType (),
		InterfaceValue,
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
	CreateComment ("create property pointer");

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
CLlvmBuilder::CheckPropertyPointerScopeLevel (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	if (SrcValue.GetValueKind () == EValue_Property) 
		return true;

	CValue DstScopeLevelValue;
	GetDstScopeLevel (DstValue, &DstScopeLevelValue);

	CreateComment ("check property pointer scope");

	CValue InterfaceValue;
	CreateExtractValue (SrcValue, 1, NULL, &InterfaceValue);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckInterfaceScopeLevel);
	
	CreateCall2 (
		pCheckFunction,
		pCheckFunction->GetType (),
		InterfaceValue,
		DstScopeLevelValue,
		NULL
		);

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
