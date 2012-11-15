// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

class CScope;

//.............................................................................

enum EVariable
{
	EVariable_Global,
	EVariable_Local,
	EVariable_Arg,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
class CVariable:
	public CModuleItem,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CVariableMgr;

	EVariable m_VariableKind;
	CType* m_pType;
	CScope* m_pScope;

	union
	{
		llvm::Value* m_pLlvmValue;
		llvm::GlobalVariable* m_pLlvmGlobalVariable;
		llvm::AllocaInst* m_pLlvmAlloca;
	};

public:
	CVariable ();

	~CVariable ();

	EVariable
	GetVariableKind ()
	{
		return m_VariableKind;
	}

	CType* 
	GetType ()
	{
		return m_pType;
	}

	CScope*
	GetScope ()
	{
		return m_pScope;
	}

	llvm::Value* 
	GetLlvmValue ()
	{
		return m_pLlvmValue;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
