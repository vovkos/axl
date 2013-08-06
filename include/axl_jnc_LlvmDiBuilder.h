// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

class CModule;
class CScope;
class CFunctionType;
class CStructType;
class CUnionType;
class CArrayType;
class CVariable;
class CFunction;

//.............................................................................

enum
{
	DW_LANG_Jancy = llvm::dwarf::DW_LANG_lo_user + 1,
};

//.............................................................................

class CLlvmDiBuilder
{
	friend class CModule;

protected:
	CModule* m_pModule;
	llvm::DIBuilder* m_pLlvmDiBuilder;

public:
	CLlvmDiBuilder ();

	~CLlvmDiBuilder ()
	{
		Clear ();
	}

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	llvm::DIBuilder*
	GetLlvmDiBuilder ()
	{
		return m_pLlvmDiBuilder;
	}

	bool
	Create ();

	void
	Clear ();

	void
	Finalize ()
	{
		m_pLlvmDiBuilder->finalize ();
	}

	llvm::DIFile
	CreateFile (
		const char* pFileName,
		const char* pDirName
		)
	{
		return m_pLlvmDiBuilder->createFile (pFileName, pDirName);
	}

	llvm::DIType
	CreateBasicType (
		const char* pName,
		size_t Size,
		size_t AlignFactor,
		uint_t Code
		)
	{
		return m_pLlvmDiBuilder->createBasicType (pName, Size * 8, AlignFactor * 8, Code);
	}

	llvm::DIType
	CreateSubroutineType (CFunctionType* pFunctionType);

	llvm::DIType
	CreateStructType (CStructType* pStructType);

	llvm::DIType
	CreateUnionType (CUnionType* pUnionType);

	llvm::DIType
	CreateArrayType (CArrayType* pArrayType);

	llvm::DIType
	CreatePointerType (CType* pType);

	llvm::DIGlobalVariable
	CreateGlobalVariable (CVariable* pVariable);

	llvm::DIVariable
	CreateLocalVariable (
		CVariable* pVariable,
		uint_t Tag = llvm::dwarf::DW_TAG_auto_variable
		);

	llvm::DISubprogram
	CreateFunction (CFunction* pFunction);

	llvm::DILexicalBlock
	CreateLexicalBlock (
		CScope* pParentScope,
		const CToken::CPos& Pos
		);

	llvm::Instruction*
	CreateDeclare (CVariable* pVariable);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
