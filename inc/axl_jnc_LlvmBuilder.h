// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"
#include "axl_jnc_BasicBlock.h"

namespace axl {
namespace jnc {

class CModule;
class CScope;

//.............................................................................

class CLlvmBuilder
{
protected:
	friend class CModule;
	CModule* m_pModule;

	llvm::IRBuilder <> m_LlvmBuilder;

public:
	CLlvmBuilder ();

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

	// memory access

	llvm::AllocaInst*
	CreateAlloca (
		CType* pType, 
		const tchar_t* pName
		)
	{
		return m_LlvmBuilder.CreateAlloca (pType->GetLlvmType (), 0, pName);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		size_t* pIndexArray,
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateGep (
		const CValue& Value,
		size_t Index,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		CValue IndexValue;
		IndexValue.SetConstSizeT (Index, EType_Int_p);
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
		size_t Index1,
		size_t Index2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		CValue IndexValue1;
		CValue IndexValue2;
		IndexValue1.SetConstSizeT (Index1, EType_Int_p);
		IndexValue2.SetConstSizeT (Index2, EType_Int_p);
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
		IndexValue1.SetConstSizeT (0, EType_Int_p);
		return CreateGep2 (Value, IndexValue1, IndexValue2, pResultType, pResultValue);
	}

	llvm::Value*
	CreateGep2 (
		const CValue& Value,
		size_t Index2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		CValue IndexValue1;
		CValue IndexValue2;
		IndexValue1.SetConstSizeT (0, EType_Int_p);
		IndexValue2.SetConstSizeT (Index2, EType_Int_p);
		return CreateGep2 (Value, IndexValue1, IndexValue2, pResultType, pResultValue);
	}

	llvm::Value*
	CreateExtractValue (
		const CValue& Value,
		size_t Index,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateExtractValue (Value.GetLlvmValue (), Index, "extract");
		pResultValue->SetLlvmRegister (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateExtractValue (
		const CValue& Value,
		size_t* pIndexArray,
		size_t IndexCount,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateExtractValue (Value.GetLlvmValue (), llvm::ArrayRef <size_t> (pIndexArray, IndexCount), "extract");
		pResultValue->SetLlvmRegister (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateInsertValue (
		const CValue& AggregateValue,
		const CValue& MemberValue,
		size_t Index,
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

		pResultValue->SetLlvmRegister (pInst, pResultType);
		return pInst;
	}

	llvm::Value*
	CreateInsertValue (
		const CValue& AggregateValue,
		const CValue& MemberValue,
		size_t* pIndexArray,
		size_t IndexCount,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateInsertValue (
			AggregateValue.GetLlvmValue (), 
			MemberValue.GetLlvmValue (),
			llvm::ArrayRef <size_t> (pIndexArray, IndexCount), 
			"insert"
			);

		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, pResultType);
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
		pResultValue->SetLlvmRegister (pInst, EType_Bool);
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
		pResultValue->SetLlvmRegister (pInst, EType_Bool);
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
		pResultValue->SetLlvmRegister (pInst, EType_Bool);
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
		pResultValue->SetLlvmRegister (pInst, EType_Bool);
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
		pResultValue->SetLlvmRegister (pInst, EType_Bool);
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
		pResultValue->SetLlvmRegister (pInst, EType_Bool);
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
		pResultValue->SetLlvmRegister (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateLe_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResuLeValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpSLE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "le_i");
		pResuLeValue->SetLlvmRegister (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateLe_u (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResuLeValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpULE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "le_u");
		pResuLeValue->SetLlvmRegister (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateLe_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResuLeValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateFCmpOLE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "le_f");
		pResuLeValue->SetLlvmRegister (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGt_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResuGtValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpSGT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "gt_i");
		pResuGtValue->SetLlvmRegister (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGt_u (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResuGtValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpUGT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "gt_u");
		pResuGtValue->SetLlvmRegister (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGt_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResuGtValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateFCmpOGT (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "gt_f");
		pResuGtValue->SetLlvmRegister (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGe_i (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResuGeValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpSGE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ge_i");
		pResuGeValue->SetLlvmRegister (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGe_u (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResuGeValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateICmpUGE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ge_u");
		pResuGeValue->SetLlvmRegister (pInst, EType_Bool);
		return pInst;
	}

	llvm::Value*
	CreateGe_f (
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResuGeValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateFCmpOGE (OpValue1.GetLlvmValue (), OpValue2.GetLlvmValue (), "ge_f");
		pResuGeValue->SetLlvmRegister (pInst, EType_Bool);
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
		llvm::Value* pInst = m_LlvmBuilder.CreateBitCast (OpValue.GetLlvmValue (), pType->GetLlvmType (), "cast");
		pResultValue->SetLlvmRegister (pInst, pType);
		return pInst;
	}

	llvm::Value*
	CreateIntToPtr (
		const CValue& OpValue,
		CPointerType* pType,
		CValue* pResultValue
		)
	{
		llvm::Value* pInst = m_LlvmBuilder.CreateIntToPtr (OpValue.GetLlvmValue (), pType->GetLlvmType (), "int2ptr");
		pResultValue->SetLlvmRegister (pInst, pType);
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
		pResultValue->SetLlvmRegister (pInst, pType);
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
		pResultValue->SetLlvmRegister (pInst, pType);
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
		pResultValue->SetLlvmRegister (pInst, pType);
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
		pResultValue->SetLlvmRegister (pInst, pType);
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
		pResultValue->SetLlvmRegister (pInst, pType);
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
		pResultValue->SetLlvmRegister (pInst, pType);
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
		pResultValue->SetLlvmRegister (pInst, pType);
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
		pResultValue->SetLlvmRegister (pInst, pType);
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
		pResultValue->SetLlvmRegister (pInst, pType);
		return pInst;
	}

	// calls

	llvm::CallInst*
	CreateCall (
		const CValue& CalleeValue,
		const CValue* pArgArray,
		size_t ArgCount,
		CType* pResultType,
		CValue* pResultValue
		);

	llvm::CallInst*
	CreateCall (
		const CValue& CalleeValue,
		const CValue& ArgValue,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		return CreateCall (CalleeValue, &ArgValue, 1, pResultType, pResultValue);
	}

	llvm::CallInst*
	CreateCall2 (
		const CValue& CalleeValue,
		const CValue& ArgValue1,
		const CValue& ArgValue2,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		CValue ArgArray [] =
		{
			ArgValue1,
			ArgValue2,
		};

		return CreateCall (CalleeValue, ArgArray, countof (ArgArray), pResultType, pResultValue);
	}

	llvm::CallInst*
	CreateCall3 (
		const CValue& CalleeValue,
		const CValue& ArgValue1,
		const CValue& ArgValue2,
		const CValue& ArgValue3,
		CType* pResultType,
		CValue* pResultValue
		)
	{
		CValue ArgArray [] =
		{
			ArgValue1,
			ArgValue2,
			ArgValue3,
		};

		return CreateCall (CalleeValue, ArgArray, countof (ArgArray), pResultType, pResultValue);
	}

	// safe pointer operations

	llvm::Value*
	CreateSafePtr (
		const CValue& PtrValue,
		CVariable* pVariable,
		CType* pResultType,
		CValue* pResultValue
		);

	bool
	CheckSafePtrRange (
		const CValue& SafePtrValue,
		size_t Size,
		ESafePtrError Error
		);

	bool
	CheckSafePtrScope (
		const CValue& SrcValue,
		CScope* pDstScope
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
