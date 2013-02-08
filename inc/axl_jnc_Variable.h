// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Scope.h"

namespace axl {
namespace jnc {

class CScope;

//.............................................................................

enum EVariable
{
	EVariable_Undefined,
	EVariable_Global,
	EVariable_Local,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetVariableKindString (EVariable VariableKind);

//.............................................................................

class CVariable: public CNamedModuleItem
{
protected:
	friend class CVariableMgr;

	EVariable m_VariableKind;
	CType* m_pType;
	CScope* m_pScope;
	llvm::Value* m_pLlvmValue;

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

	size_t
	GetScopeLevel ()
	{
		return m_pScope ? m_pScope->GetLevel () : 0;
	}

	llvm::Value* 
	GetLlvmValue ()
	{
		return m_pLlvmValue;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
