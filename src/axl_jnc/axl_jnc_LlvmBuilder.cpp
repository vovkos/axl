#include "pch.h"
#include "axl_jnc_LlvmBuilder.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CLlvmBuilder::CLlvmBuilder ():
	m_LlvmBuilder (llvm::getGlobalContext ())
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_CommentMdKind = llvm::getGlobalContext().getMDKindID ("jnc.comment");
}

bool
CLlvmBuilder::CreateComment_va (
	const char* pFormat,
	axl_va_list va
	)
{
	char Buffer [256];
	rtl::CString String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.Format_va (pFormat, va);

	return CreateComment_0 (String);
}

bool
CLlvmBuilder::CreateComment_0 (const char* pText)
{
	CBasicBlock* pBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	llvm::BasicBlock* pLlvmBlock = pBlock->GetLlvmBlock ();

	if (pLlvmBlock->getInstList ().empty ())
	{
		pBlock->m_LeadingComment = pText;
		return true;
	}

	llvm::Instruction* pInst = &pLlvmBlock->getInstList ().back ();
	llvm::MDString* pMdString = llvm::MDString::get (llvm::getGlobalContext (), pText);
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
	ASSERT (pType->GetTypeKindFlags () & ETypeKindFlag_Integer);

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

void
CLlvmBuilder::SetInsertPoint (CBasicBlock* pBlock)
{
	if (!(pBlock->GetFlags () & EBasicBlockFlag_Entry) || !pBlock->HasTerminator ())
		m_LlvmBuilder.SetInsertPoint (pBlock->GetLlvmBlock ());
	else
		m_LlvmBuilder.SetInsertPoint (pBlock->GetLlvmBlock ()->getTerminator ());
}

llvm::SwitchInst*
CLlvmBuilder::CreateSwitch (
	const CValue& Value,
	CBasicBlock* pDefaultBlock,
	intptr_t* pConstArray,
	CBasicBlock** pBlockArray,
	size_t CaseCount
	)
{
	CType* pType = Value.GetType ();
	ASSERT (pType->GetTypeKindFlags () & ETypeKindFlag_Integer);

	llvm::SwitchInst* pInst = m_LlvmBuilder.CreateSwitch (
		Value.GetLlvmValue (), 
		pDefaultBlock->GetLlvmBlock (), 
		CaseCount
		);

	for (size_t i = 0; i < CaseCount; i++)
	{
		CValue ConstValue (pConstArray [i], pType);
		CBasicBlock* pBlock = pBlockArray [i];

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
	const int32_t* pIndexArray,
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
		IndexValue.SetConstInt32 (pIndexArray [i], EType_Int32_u);
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
	ECallConv CallConv,
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

	pInst->setCallingConv (GetLlvmCallConv (CallConv));
	return pInst;
}

bool
CLlvmBuilder::CreateClosureFunctionPtr (
	const CValue& RawPfnValue,
	const CValue& RawIfaceValue,
	CFunctionPtrType* pResultType,
	CValue* pResultValue
	)
{
	CLlvmScopeComment Comment (this, "create closure function pointer");

	CValue PfnValue;
	CValue IfaceValue;
	CValue FunctionPtrValue = pResultType->GetUndefValue ();

	CFunctionType* pStdObjectMemberMethodType = pResultType->GetTargetType ()->GetStdObjectMemberMethodType ();

	CreateBitCast (RawPfnValue, pStdObjectMemberMethodType->GetFunctionPtrType (EFunctionPtrType_Thin), &PfnValue);
	CreateBitCast (RawIfaceValue, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr), &IfaceValue);

	CreateInsertValue (FunctionPtrValue, PfnValue, 0, NULL, &FunctionPtrValue);
	CreateInsertValue (FunctionPtrValue, IfaceValue, 1, pResultType, pResultValue);
	return true;
}

bool
CLlvmBuilder::CreateClosurePropertyPtr (
	const CValue& RawPfnValue,
	const CValue& RawIfaceValue,
	CPropertyPtrType* pResultType,
	CValue* pResultValue
	)
{
	CLlvmScopeComment Comment (this, "create closure property pointer");

	CValue PfnValue;
	CValue IfaceValue;
	CValue PropertyPtrValue = pResultType->GetUndefValue ();

	CPropertyType* pStdObjectMemberPropertyType = pResultType->GetTargetType ()->GetStdObjectMemberPropertyType ();

	CreateBitCast (RawPfnValue, pStdObjectMemberPropertyType->GetPropertyPtrType (EPropertyPtrType_Thin), &PfnValue);
	CreateBitCast (RawIfaceValue, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr), &IfaceValue);
	
	CreateInsertValue (PropertyPtrValue, PfnValue, 0, NULL, &PropertyPtrValue);
	CreateInsertValue (PropertyPtrValue, IfaceValue, 1, pResultType, pResultValue);
	return true;
}

bool
CLlvmBuilder::RuntimeError (const CValue& ErrorValue)
{
	CFunction* pOnRuntimeError = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_OnRuntimeError);

	// TODO: calc real code address

	CValue CodeAddrValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	CreateCall2 (
		pOnRuntimeError, 
		pOnRuntimeError->GetType (),
		ErrorValue, 
		CodeAddrValue,
		NULL
		);

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
