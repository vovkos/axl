#include "pch.h"
#include "axl_jnc_Runtime.h"
#include "axl_jnc_StdLib.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CRuntime::CRuntime ()
{
	m_pModule = NULL;
	m_pLlvmExecutionEngine = NULL;
	m_TlsSlot = -1;
	m_TlsSize = 0;
}

bool
CRuntime::Create (CModule* pModule)
{
	Clear ();

	llvm::EngineBuilder EngineBuilder (pModule->GetLlvmModule ());	
	
	std::string errorString;
	EngineBuilder.setErrorStr (&errorString);
	EngineBuilder.setUseMCJIT(true);

	m_pLlvmExecutionEngine = EngineBuilder.create ();
	if (!m_pLlvmExecutionEngine)
	{
		err::SetFormatStringError ("cannot create execution engine: %s\n", errorString.c_str());
		return false;
	}

	m_pModule = pModule;
	return true;
}

void
CRuntime::Clear ()
{
	if (m_TlsSlot != -1)
		GetTlsMgr ()->DestroySlot (m_TlsSlot);

	if (m_pLlvmExecutionEngine)
		delete m_pLlvmExecutionEngine;

	m_pLlvmExecutionEngine = NULL;
	m_TlsSlot = -1;
	m_TlsSize = 0;
}

//.............................................................................

} // namespace axl 
} // namespace jnc 
