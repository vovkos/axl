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

class CLlvmBuilder
{
	friend class CModule;

protected:
	CModule* m_pModule;

	llvm::IRBuilder <> m_LlvmBuilder;
	uint_t m_CommentMdKind;

public:
	CLlvmBuilder ();

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

	// branches

	void
	SetInsertPoint (CBasicBlock* pBlock)
	{
		m_LlvmBuilder.SetInsertPoint (pBlock->GetLlvmBlock ());
	}

	llvm::UnreachableInst*
	CreateUnreachable ()
	{
		return m_LlvmBuilder.CreateUnreachable ();
	}

	llvm::BranchInst*
	CreateBr (CBasicBlock* pBlock)
	{
		return m_LlvmBuilder.CreateBr (pBlock->GetLlvmBlock ());
	}

	llvm::BranchInst*
	CreateCondBr (
		const CValue& Value,
		CBasicBlock* pTrueBlock,
		CBasicBlock* pFalseBlock
		)
	{
		return m_LlvmBuilder.CreateCondBr (
			Value.GetLlvmValue (), 
			pTrueBlock->GetLlvmBlock (),
			pFalseBlock->GetLlvmBlock ()
			);
	}

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
		return m_LlvmBuilder.CreateRetVoid ();
	}

	llvm::ReturnInst*
	CreateRet (const CValue& Value)
	{
		return m_LlvmBuilder.CreateRet (Value.GetLlvmValue ());
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
		llvm::AllocaInst* pInst = m_LlvmBuilder.CreateAlloca (pType->GetLlvmType (), 0, pName);
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
		llvm::LoadInst* pInst = m_LlvmBuilder.CreateLoad (Value.GetLlvmValue (), IsVolatile, "loa");
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
		return m_LlvmBuilder.CreateStore (SrcValue.GetLlvmValue (), DstValue.GetLlvmValue (), IsVolatile);
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
		llvm::Value* pInst = m_LlvmBuilder.CreateGEP (Value.GetLlvmValue (), IndexValue.GetLlvmValue (), "gep");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateExtractValue (Value.GetLlvmValue (), Index, "extract");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateExtractValue (
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
		llvm::Value* pInst = m_LlvmBuilder.CreateInsertValue (
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
		llvm::Value* pInst = m_LlvmBuilder.CreateInsertValue (
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
		llvm::Value* pInst = m_LlvmBuilder.CreateNeg (OpValue.GetLlvmValue (), "neg_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFNeg (OpValue.GetLlvmValue (), "neg_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateNot (OpValue.GetLlvmValue (), "not");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateAdd (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "add_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFAdd (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "add_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateSub (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "sub_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFSub (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "sub_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateMul (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "mul_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFMul (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "mul_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateSDiv (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "div_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateUDiv (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "div_u");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFDiv (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "div_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateSRem (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "mod_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateURem (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "mod_u");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateShl (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "shl");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateLShr (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "shr");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateAnd (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "and");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateOr (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "or");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateXor (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "xor");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpEQ (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "eq_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFCmpOEQ (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "eq_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpNE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ne_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFCmpONE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ne_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpSLT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "lt_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpULT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "lt_u");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFCmpOLT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "lt_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpSLE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "le_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpULE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "le_u");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFCmpOLE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "le_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpSGT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "gt_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpUGT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "gt_u");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFCmpOGT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "gt_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpSGE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ge_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpUGE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ge_u");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFCmpOGE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ge_f");
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
		llvm::AtomicCmpXchgInst* pInst = m_LlvmBuilder.CreateAtomicCmpXchg (
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
		llvm::AtomicRMWInst* pInst = m_LlvmBuilder.CreateAtomicRMW (
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
		llvm::Value* pInst = m_LlvmBuilder.CreateBitCast (OpValue.GetLlvmValue (), pType->GetLlvmType (), "bitcast");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateIntToPtr (OpValue.GetLlvmValue (), pType->GetLlvmType (), "int2ptr");
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
		llvm::Value* pInst = m_LlvmBuilder.CreatePtrToInt (OpValue.GetLlvmValue (), pType->GetLlvmType (), "ptr2int");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateTrunc (OpValue.GetLlvmValue (), pType->GetLlvmType (), "trunc_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFPTrunc (OpValue.GetLlvmValue (), pType->GetLlvmType (), "trunc_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateSExt (OpValue.GetLlvmValue (), pType->GetLlvmType (), "ext_i");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateZExt (OpValue.GetLlvmValue (), pType->GetLlvmType (), "ext_u");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFPExt (OpValue.GetLlvmValue (), pType->GetLlvmType (), "ext_f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateSIToFP (OpValue.GetLlvmValue (), pType->GetLlvmType (), "i2f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateUIToFP (OpValue.GetLlvmValue (), pType->GetLlvmType (), "u2f");
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
		llvm::Value* pInst = m_LlvmBuilder.CreateFPToSI (OpValue.GetLlvmValue (), pType->GetLlvmType (), "f2i");
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

} // namespace jnc {
} // namespace axl {
