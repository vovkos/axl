// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

class CModule;
class CScope;

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
	llvm::DIFile m_LlvmDiFile;

public:
	CLlvmDiBuilder ();

	~CLlvmDiBuilder ()
	{
		Clear ();
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
	
	bool	
	CreateGlobalVariable (CVariable* pVariable);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
