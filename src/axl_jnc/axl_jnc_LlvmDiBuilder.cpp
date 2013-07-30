#include "pch.h"
#include "axl_jnc_LlvmDiBuilder.h"
#include "axl_jnc_Module.h"

#include "axl_io_FilePathUtils.h"

namespace axl {
namespace jnc {

//.............................................................................

CLlvmDiBuilder::CLlvmDiBuilder ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_pLlvmDiBuilder = NULL;
}

bool
CLlvmDiBuilder::Create ()
{
	Clear ();

	llvm::Module* pLlvmModule = m_pModule->GetLlvmModule ();
	ASSERT (pLlvmModule);

	m_pLlvmDiBuilder = new llvm::DIBuilder (*pLlvmModule);

	rtl::CString FilePath = m_pModule->GetFilePath ();
	rtl::CString FileName = io::GetFileName (FilePath);
	rtl::CString DirName = io::GetDirName  (FilePath);

	m_pLlvmDiBuilder->createCompileUnit (llvm::dwarf::DW_LANG_C99, FileName.cc (), DirName.cc (), "jncc-1.0.0", false, "", 1);
	
	m_LlvmDiFile = m_pLlvmDiBuilder->createFile (FileName.cc (), DirName.cc ());	
	
	return true;
}

void
CLlvmDiBuilder::Clear ()
{
	if (!m_pLlvmDiBuilder)
		return;
	
	delete m_pLlvmDiBuilder;
	m_pLlvmDiBuilder = NULL;
}

bool	
CLlvmDiBuilder::CreateGlobalVariable (CVariable* pVariable)
{
	unsigned Encoding = llvm::dwarf::DW_ATE_signed;
	llvm::StringRef BTName = "long int";

	llvm::DIType LlvmDiType = m_pLlvmDiBuilder->createBasicType (BTName, 4, 4, Encoding);

	m_pLlvmDiBuilder->createGlobalVariable (
		pVariable->GetQualifiedName ().cc (),
		m_LlvmDiFile,
		5,
		LlvmDiType,
		true,
		pVariable->GetLlvmValue ()
		);

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
