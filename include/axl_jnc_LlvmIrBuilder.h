// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"
#include "axl_jnc_BasicBlock.h"
#include "axl_jnc_DataPtrType.h"
#include "axl_jnc_FunctionType.h"
#include "axl_jnc_PropertyType.h"
#include "axl_jnc_Variable.h"

namespace axl {
namespace jnc {

class CModule;
class CScope;

//.............................................................................

class CLlvmIrBuilder
{
	friend class CModule;

protected:
	CModule* m_pModule;

	uint_t m_CommentMdKind;
	uint_t m_EmptyLineMdKind;
	llvm::IRBuilder <> m_LlvmIrBuilder;

public:
	CLlvmIrBuilder ();

	uint_t
	GetCommentMdKind ()
	{
		return m_CommentMdKind;
	}

	bool
	CreateComment (
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat); 
		return CreateComment_va (pFormat, va);
	}

	bool
	CreateComment_va (
		const char* pFormat,
		axl_va_list va
		);

	bool
	CreateComment_0 (const char* pText);

	bool
	CreateEmptyLine ()
	{
		return CreateComment_0 (NULL);
	}

	llvm::DebugLoc
	GetCurrentDebugLoc ()
	{
		return m_LlvmIrBuilder.getCurrentDebugLocation ();
	}

	void
	SetCurrentDebugLoc (const llvm::DebugLoc& LlvmDebugLoc)
	{
		m_LlvmIrBuilder.SetCurrentDebugLocation (LlvmDebugLoc);
	}

	void
	ClearCurrentDebugLoc ()
	{
		m_LlvmIrBuilder.SetCurrentDebugLocation (llvm::DebugLoc ());
	}

	void
	SetInstDebugLoc (llvm::Instruction* pLlvmInst)
	{
		m_LlvmIrBuilder.SetInstDebugLocation (pLlvmInst);
	}

	// branches

	llvm::Instruction*
	GetInsertPoint ()
	{
		return m_LlvmIrBuilder.GetInsertPoint ();
	}

	void
	SetInsertPoint (CBasicBlock* pBlock);

	void
	SetInsertPoint (llvm::Instruction* pLlvmInst)
	{
		m_LlvmIrBuilder.SetInsertPoint (pLlvmInst);
	}

	llvm::UnreachableInst*
	CreateUnreachable ()
	{
		return m_LlvmIrBuilder.CreateUnreachable ();
	}

	llvm::BranchInst*
	CreateBr (CBasicBlock* pBlock)
	{
		return m_LlvmIrBuilder.CreateBr (pBlock->GetLlvmBlock ());
	}

	llvm::BranchInst*
	CreateCondBr (
		const CValue& Value,
		CBasicBlock* pTrueBlock,
		CBasicBlock* pFalseBlock
		)
	{
		return m_LlvmIrBuilder.CreateCondBr (
			Value.GetLlvmValue (),
			pTrueBlock->GetLlvmBlock (),
			pFalseBlock->GetLlvmBlock ()
			);
	}

	llvm::IndirectBrInst*
	CreateIndirectBr (
		const CValue& Value,
		CBasicBlock** ppBlockArray,
		size_t BlockCount
		);

	llvm::SwitchInst*
	CreateSwitch (
		const CValue& Value,
		CBasicBlock* pDefaultBlock,
		rtl::CHashTableMapIteratorT <intptr_t, CBasicBlock*> FirstCase,
		size_t CaseCount
		);

	llvm::SwitchInst*
	CreateSwitch (
		const CValue& Value,
		CBasicBlock* pDefaultBlock,
		intptr_t* pConstArray,
		CBasicBlock** pBlockArray,
		size_t CaseCount
		);

	llvm::ReturnInst*
	CreateRet ()
	{
		return m_LlvmIrBuilder.CreateRetVoid ();
	}

	llvm::ReturnInst*
	CreateRet (const CValue& Value)
	{
		return m_LlvmIrBuilder.CreateRet (Value.GetLlvmValue ());
	}

	llvm::PHINode*
	CreatePhi (
		const CValue* pValueArray,
		CBasicBlock** pBlockArray,
		size_t Count,
		CValue* pResultValue
		);

	llvm::PHINode*
	CreatePhi (
		const CValue& Value1,
		CBasicBlock* pBlock1,
		const CValue& Value2,
		CBasicBlock* pBlock2,
		CValue* pResultValue
		);

	// memory access

	llvm::AllocaInst*
	CreateAlloca (
		CType* pType,
		const char* pName,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::AllocaInst* pInst = m_LlvmIrBuilder.CreateAlloca (pType->GetLlvmType (), 0, pName);
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::LoadInst*
	CreateLoad (
		const CValue& Value,
		CType* pResultType,
		CValue* pResultValue,
		bool IsVolatile = false
		)
	{
		llvm::LoadInst* pInst = m_LlvmIrBuilder.CreateLoad (Value.GetLlvmValue (), IsVolatile, "loa");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::StoreInst*
	CreateStore (
		const CValue& SrcValue,
		const CValue& DstValue,
		bool IsVolatile = false
		)
	{
		return m_LlvmIrBuilder.CreateStore (SrcValue.GetLlvmValue (), DstValue.GetLlvmValue (), IsVolatile);
	}

	// member access

	llvm::Value*
	CreateGep (
		const CValue& Value,
		const CValue* pIndexArray,
		size_t IndexCount,
		CType* pResultType,
		CValue* pResultValue
		);


	llvm::Value*
	CreateGep (
		const CValue& Value,
		const int32_t* pIndexArray,
		size_t IndexCount,
		CType* pResultType,
		CValue* pResultValue
		);

	llvm::Value*
	CreateGep (
		const CValue& Value,
		const CValue& IndexValue,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateGEP (Value.GetLlvmValue (), IndexValue.GetLlvmValue (), "gep");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateGep (
		const CValue& Value,
		int32_t Index,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		CValue IndexValue;
		IndexValue.SetConstInt32 (Index, EType_Int32);
		return CreateGep (Value, IndexValue, pResultType, pResultValue);
	}

	llvm::Value*
	CreateGep2 (
		const CValue& Value,
		const CValue& IndexValue1,
		const CValue& IndexValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		CValue IndexArray [] =
		{
			IndexValue1,
			IndexValue2,
		};

		return CreateGep (Value, IndexArray, countof (IndexArray), pResultType, pResultValue);
	}

	llvm::Value*
	CreateGep2 (
		const CValue& Value,
		int32_t Index1,
		int32_t Index2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		CValue IndexValue1;
		CValue IndexValue2;
		IndexValue1.SetConstInt32 (Index1, EType_Int32);
		IndexValue2.SetConstInt32 (Index2, EType_Int32);
		return CreateGep2 (Value, IndexValue1, IndexValue2, pResultType, pResultValue);
	}

	llvm::Value*
	CreateGep2 (
		const CValue& Value,
		const CValue& IndexValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		CValue IndexValue1;
		IndexValue1.SetConstInt32 (0, EType_Int32);
		return CreateGep2 (Value, IndexValue1, IndexValue2, pResultType, pResultValue);
	}

	llvm::Value*
	CreateGep2 (
		const CValue& Value,
		int32_t Index2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		CValue IndexValue1;
		CValue IndexValue2;
		IndexValue1.SetConstInt32 (0, EType_Int32);
		IndexValue2.SetConstInt32 (Index2, EType_Int32);
		return CreateGep2 (Value, IndexValue1, IndexValue2, pResultType, pResultValue);
	}

	llvm::Value*
	CreateExtractValue (
		const CValue& Value,
		int32_t Index,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateExtractValue (Value.GetLlvmValue (), Index, "extract");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateExtractValue (
		const CValue& Value,
		const int32_t* pIndexArray,
		size_t IndexCount,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateExtractValue (
			Value.GetLlvmValue (),
			llvm::ArrayRef <uint_t> ((uint_t*) pIndexArray, IndexCount),
			"extract"
			);
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateInsertValue (
		const CValue& AggregateValue,
		const CValue& MemberValue,
		int32_t Index,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateInsertValue (
			AggregateValue.GetLlvmValue (),
			MemberValue.GetLlvmValue (),
			Index,
			"insert"
			);

		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateInsertValue (
		const CValue& AggregateValue,
		const CValue& MemberValue,
		const int32_t* pIndexArray,
		size_t IndexCount,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateInsertValue (
			AggregateValue.GetLlvmValue (),
			MemberValue.GetLlvmValue (),
			llvm::ArrayRef <uint_t> ((uint_t*) pIndexArray, IndexCount),
			"insert"
			);

		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	// unary

	llvm::Value*
	CreateNeg_i (
		const CValue& OpValue,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateNeg (OpValue.GetLlvmValue (), "neg_i");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateNeg_f (
		const CValue& OpValue,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFNeg (OpValue.GetLlvmValue (), "neg_f");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateNot (
		const CValue& OpValue,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateNot (OpValue.GetLlvmValue (), "not");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	// binary

	llvm::Value*
	CreateAdd_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateAdd (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "add_i");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateAdd_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFAdd (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "add_f");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateSub_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateSub (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "sub_i");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateSub_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFSub (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "sub_f");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateMul_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateMul (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "mul_i");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateMul_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFMul (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "mul_f");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateDiv_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateSDiv (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "div_i");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateDiv_u (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateUDiv (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "div_u");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateDiv_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFDiv (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "div_f");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateMod_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateSRem (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "mod_i");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateMod_u (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateURem (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "mod_u");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateShl (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateShl (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "shl");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateShr (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateLShr (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "shr");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateAnd (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateAnd (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "and");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateOr (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateOr (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "or");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateXor (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateXor (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "xor");
		pResultValue->SetLlvmValue (pInst, pResultType);
		return pInst;
	}

	// relational

	llvm::Value*
	CreateEq_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateICmpEQ (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "eq_i");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateEq_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFCmpOEQ (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "eq_f");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateNe_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateICmpNE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ne_i");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateNe_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFCmpONE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ne_f");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateLt_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateICmpSLT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "lt_i");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateLt_u (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateICmpULT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "lt_u");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateLt_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFCmpOLT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "lt_f");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateLe_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateICmpSLE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "le_i");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateLe_u (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateICmpULE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "le_u");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateLe_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFCmpOLE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "le_f");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGt_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateICmpSGT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "gt_i");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGt_u (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateICmpUGT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "gt_u");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGt_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFCmpOGT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "gt_f");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGe_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateICmpSGE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ge_i");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGe_u (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateICmpUGE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ge_u");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGe_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFCmpOGE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ge_f");
		pResultValue->SetLlvmValue (pInst, EType_Bool);
		return pInst;
	}

	llvm::AtomicCmpXchgInst*
	CreateCmpXchg (
		const CValue& PtrValue,
		const CValue& CmpValue,
		const CValue& NewValue,
		llvm::AtomicOrdering OrderingKind,
		llvm::SynchronizationScope SyncKind,
		CValue* pResultValue
		)
	{
		llvm::AtomicCmpXchgInst* pInst = m_LlvmIrBuilder.CreateAtomicCmpXchg (
			PtrValue.GetLlvmValue (),
			CmpValue.GetLlvmValue (),
			NewValue.GetLlvmValue (),
			OrderingKind,
			SyncKind
			);

		pResultValue->SetLlvmValue (pInst, NewValue.GetType ());
		return pInst;
	}

	llvm::AtomicRMWInst*
	CreateRmw (
		llvm::AtomicRMWInst::BinOp OpKind,
		const CValue& PtrValue,
		const CValue& NewValue,
		llvm::AtomicOrdering OrderingKind,
		llvm::SynchronizationScope SyncKind,
		CValue* pResultValue
		)
	{
		llvm::AtomicRMWInst* pInst = m_LlvmIrBuilder.CreateAtomicRMW (
			OpKind,
			PtrValue.GetLlvmValue (),
			NewValue.GetLlvmValue (),
			llvm::Acquire,
			llvm::CrossThread
			);

		pResultValue->SetLlvmValue (pInst, NewValue.GetType ());
		return pInst;
	}

	// casts

	llvm::Value*
	CreateBitCast (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateBitCast (OpValue.GetLlvmValue (), pType->GetLlvmType (), "bitcast");
		pResultValue->SetLlvmValue (pInst, pType);
		return pInst;
	}

	llvm::Value*
	CreateIntToPtr (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateIntToPtr (OpValue.GetLlvmValue (), pType->GetLlvmType (), "int2ptr");
		pResultValue->SetLlvmValue (pInst, pType);
		return pInst;
	}

	llvm::Value*
	CreatePtrToInt (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreatePtrToInt (OpValue.GetLlvmValue (), pType->GetLlvmType (), "ptr2int");
		pResultValue->SetLlvmValue (pInst, pType);
		return pInst;
	}

	llvm::Value*
	CreateTrunc_i (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateTrunc (OpValue.GetLlvmValue (), pType->GetLlvmType (), "trunc_i");
		pResultValue->SetLlvmValue (pInst, pType);
		return pInst;
	}

	llvm::Value*
	CreateTrunc_f (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFPTrunc (OpValue.GetLlvmValue (), pType->GetLlvmType (), "trunc_f");
		pResultValue->SetLlvmValue (pInst, pType);
		return pInst;
	}

	llvm::Value*
	CreateExt_i (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateSExt (OpValue.GetLlvmValue (), pType->GetLlvmType (), "ext_i");
		pResultValue->SetLlvmValue (pInst, pType);
		return pInst;
	}

	llvm::Value*
	CreateExt_u (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateZExt (OpValue.GetLlvmValue (), pType->GetLlvmType (), "ext_u");
		pResultValue->SetLlvmValue (pInst, pType);
		return pInst;
	}

	llvm::Value*
	CreateExt_f (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFPExt (OpValue.GetLlvmValue (), pType->GetLlvmType (), "ext_f");
		pResultValue->SetLlvmValue (pInst, pType);
		return pInst;
	}

	llvm::Value*
	CreateIntToFp (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateSIToFP (OpValue.GetLlvmValue (), pType->GetLlvmType (), "i2f");
		pResultValue->SetLlvmValue (pInst, pType);
		return pInst;
	}

	llvm::Value*
	CreateIntToFp_u (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateUIToFP (OpValue.GetLlvmValue (), pType->GetLlvmType (), "u2f");
		pResultValue->SetLlvmValue (pInst, pType);
		return pInst;
	}

	llvm::Value*
	CreateFpToInt (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmIrBuilder.CreateFPToSI (OpValue.GetLlvmValue (), pType->GetLlvmType (), "f2i");
		pResultValue->SetLlvmValue (pInst, pType);
		return pInst;
	}

	// calls

	llvm::CallInst*
	CreateCall (
		const CValue& CalleeValue,
		ECallConv CallConv,
		const CValue* pArgArray,
		size_t ArgCount,
		CType* pResultType,
		CValue* pResultValue
		);

	llvm::CallInst*
	CreateCall (
		const CValue& CalleeValue,
		CFunctionType* pFunctionType,
		const CValue* pArgArray,
		size_t ArgCount,
		CValue* pResultValue
		)
	{
		return CreateCall (
			CalleeValue,
			pFunctionType->GetCallConv (),
			pArgArray,
			ArgCount,
			pFunctionType->GetReturnType (),
			pResultValue
			);
	}

	llvm::CallInst*
	CreateCall (
		const CValue& CalleeValue,
		CFunctionType* pFunctionType,
		const CValue& ArgValue,
		CValue* pResultValue
		)
	{
		return CreateCall (CalleeValue, pFunctionType, &ArgValue, 1, pResultValue);
	}

	llvm::CallInst*
	CreateCall (
		const CValue& CalleeValue,
		CFunctionType* pFunctionType,
		CValue* pResultValue
		)
	{
		return CreateCall (CalleeValue, pFunctionType, NULL, 0, pResultValue);
	}

	llvm::CallInst*
	CreateCall2 (
		const CValue& CalleeValue,
		CFunctionType* pFunctionType,
		const CValue& ArgValue1,
		const CValue& ArgValue2,
		CValue* pResultValue
		)
	{
		CValue ArgArray [] =
		{
			ArgValue1,
			ArgValue2,
		};

		return CreateCall (CalleeValue, pFunctionType, ArgArray, countof (ArgArray), pResultValue);
	}

	llvm::CallInst*
	CreateCall3 (
		const CValue& CalleeValue,
		CFunctionType* pFunctionType,
		const CValue& ArgValue1,
		const CValue& ArgValue2,
		const CValue& ArgValue3,
		CValue* pResultValue
		)
	{
		CValue ArgArray [] =
		{
			ArgValue1,
			ArgValue2,
			ArgValue3,
		};

		return CreateCall (CalleeValue, pFunctionType, ArgArray, countof (ArgArray), pResultValue);
	}

	llvm::CallInst*
	CreateCall4 (
		const CValue& CalleeValue,
		CFunctionType* pFunctionType,
		const CValue& ArgValue1,
		const CValue& ArgValue2,
		const CValue& ArgValue3,
		const CValue& ArgValue4,
		CValue* pResultValue
		)
	{
		CValue ArgArray [] =
		{
			ArgValue1,
			ArgValue2,
			ArgValue3,
			ArgValue4,
		};

		return CreateCall (CalleeValue, pFunctionType, ArgArray, countof (ArgArray), pResultValue);
	}

	// function & property pointer operations

	bool
	CreateClosureFunctionPtr (
		const CValue& PfnValue,
		const CValue& ClosureValue,
		CFunctionPtrType* pResultType,
		CValue* pResultValue
		);

	bool
	CreateClosurePropertyPtr (
		const CValue& ThinPtrValue,
		const CValue& ClosureValue,
		CPropertyPtrType* pResultType,
		CValue* pResultValue
		);

	// runtime error

	bool
	RuntimeError (const CValue& ErrorValue);

	bool
	RuntimeError (ERuntimeError Error)
	{
		return RuntimeError (CValue (Error, EType_Int));
	}
};

//.............................................................................

class CLlvmScopeComment
{
protected:
	CLlvmIrBuilder* m_pLlvmIrBuilder;

public:
	CLlvmScopeComment (
		CLlvmIrBuilder* pLlvmIrBuilder,
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat);
		pLlvmIrBuilder->CreateComment_va (pFormat, va);
		m_pLlvmIrBuilder = pLlvmIrBuilder;
	}

	~CLlvmScopeComment ()
	{
		m_pLlvmIrBuilder->CreateEmptyLine ();
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
