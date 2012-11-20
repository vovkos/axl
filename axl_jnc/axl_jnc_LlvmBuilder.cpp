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
			llvm::ArrayRef <llvm::Value*> (LlvmIndexArray, IndexCount)
			);

	pResultValue->SetLlvmRegister (pInst, pResultType);
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
			llvm::ArrayRef <llvm::Value*> (LlvmIndexArray, IndexCount)
			);

	pResultValue->SetLlvmRegister (pInst, pResultType);
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

	if (pResultType && pResultType->GetTypeKind () != EType_Void)
	{
		pInst = m_LlvmBuilder.CreateCall (
			CalleeValue.GetLlvmValue (), 
			llvm::ArrayRef <llvm::Value*> (LlvmArgArray, ArgCount),
			"call"
			);

		pResultValue->SetLlvmRegister (pInst, pResultType);
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
	CVariable* pVariable,
	CType* pResultType,
	CValue* pResultValue
	)
{
	CType* pType = pVariable->GetType ();
	CScope* pScope = pVariable->GetScope ();

	size_t ScopeLevel = pScope ? pScope->GetLevel () : 0;

	CType* pBytePtrType = m_pModule->m_TypeMgr.GetBytePtrType ();

	CValue ArgArray [4];
	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pBytePtrType, &ArgArray [0]);
	m_pModule->m_LlvmBuilder.CreateBitCast (pVariable, pBytePtrType, &ArgArray [1]);
	ArgArray [2].SetConstSizeT (pType->GetSize ());
	ArgArray [3].SetConstSizeT (ScopeLevel);

	CFunction* pCreateSafePtr = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CreateSafePtr);

	return CreateCall (
		pCreateSafePtr,
		ArgArray, countof (ArgArray), 
		pResultType,
		pResultValue
		);
}

bool
CLlvmBuilder::CheckSafePtrRange (
	const CValue& SafePtrValue,
	size_t Size,
	ESafePtrError Error
	)
{
	CValue SizeValue;
	SizeValue.SetConstSizeT (Size);

	CValue ErrorValue;
	ErrorValue.SetConstInt32 (Error, EType_Int);

	CFunction* pCheckSafePtrRange = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckSafePtrRange);

	CreateCall3 (
		pCheckSafePtrRange,
		SafePtrValue,
		SizeValue,
		ErrorValue,
		NULL,
		NULL
		);

	return true;
}

bool
CLlvmBuilder::CheckSafePtrScope (
	const CValue& SrcValue,
	CScope* pDstScope
	)
{
	size_t DstScopeLevel = pDstScope ? pDstScope->GetLevel () : 0;

	if (SrcValue.GetValueKind () == EValue_Variable)
	{
		CScope* pSrcScope = SrcValue.GetVariable ()->GetScope ();
		size_t SrcScopeLevel = pSrcScope ? pSrcScope->GetLevel () : 0;

		if (SrcScopeLevel > DstScopeLevel)
		{
			err::SetFormatStringError (_T("safe pointer/reference scope level mismatch"));
			return false;
		}

		return true;
	}

	CValue DstScopeLevelValue;
	DstScopeLevelValue.SetConstSizeT (DstScopeLevel);

	CFunction* pCheckFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckSafePtrScope);
	
	CValue FunctionValue;
	FunctionValue.SetFunction (pCheckFunction);

	m_pModule->m_LlvmBuilder.CreateCall2 (
		FunctionValue,
		SrcValue,
		DstScopeLevelValue,
		NULL,
		NULL
		);

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
