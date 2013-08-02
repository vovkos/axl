#include "pch.h"
#include "axl_jnc_LlvmDiBuilder.h"
#include "axl_jnc_Module.h"

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

	m_pLlvmDiBuilder->createCompileUnit (
		llvm::dwarf::DW_LANG_C_plus_plus,
		m_pModule->GetFileName ().cc (),
		m_pModule->GetDirName ().cc (),
		"jnc-1.0.0",
		false, "", 1
		);	

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

llvm::DIType
CLlvmDiBuilder::CreateSubroutineType (CFunctionType* pFunctionType)
{
	rtl::CArrayT <CFunctionArg*> ArgArray = pFunctionType->GetArgArray ();
	size_t Count = ArgArray.GetCount ();

	char Buffer [256];
	rtl::CArrayT <llvm::Value*> ArgTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgTypeArray.SetCount (Count + 1);

	llvm::Value** ppDst = ArgTypeArray;
	*ppDst = pFunctionType->GetReturnType ()->GetLlvmDiType ();
	ppDst++;

	for (size_t i = 0; i < Count; i++, ppDst++)
		*ppDst = ArgArray [i]->GetType ()->GetLlvmDiType ();

	llvm::DIArray LlvmDiArray = m_pLlvmDiBuilder->getOrCreateArray (llvm::ArrayRef <llvm::Value*> (ArgTypeArray, Count + 1));
	return m_pLlvmDiBuilder->createSubroutineType (pFunctionType->GetModule ()->GetLlvmDiFile (), LlvmDiArray);
}

llvm::DIType
CLlvmDiBuilder::CreateStructType (CStructType* pStructType)
{
	rtl::CConstListT <CStructField> FieldList = pStructType->GetFieldList ();
	size_t Count = FieldList.GetCount ();
	
	char Buffer [256];
	rtl::CArrayT <llvm::Value*> FieldTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	FieldTypeArray.SetCount (Count);

	rtl::CIteratorT <CStructField> Field = FieldList.GetHead ();	
	for (size_t i = 0; Field; i++, Field++)
	{
		CStructField* pField = *Field;
		FieldTypeArray [i] = m_pLlvmDiBuilder->createMemberType (
			pField->GetModule ()->GetLlvmDiFile (),
			pField->GetName ().cc (),
			pField->GetModule ()->GetLlvmDiFile (),
			pField->GetItemDecl ()->GetPos ()->m_Line,
			pField->GetType ()->GetSize () * 8,
			pField->GetType ()->GetAlignFactor () * 8,
			pField->GetOffset () * 8,
			0, 
			pField->GetType ()->GetLlvmDiType ()
			);
	}

	llvm::DIArray LlvmDiArray = m_pLlvmDiBuilder->getOrCreateArray (llvm::ArrayRef <llvm::Value*> (FieldTypeArray, Count));
	
	return m_pLlvmDiBuilder->createStructType (
		pStructType->GetModule ()->GetLlvmDiFile (),
		pStructType->m_Tag.cc (),
		pStructType->GetModule ()->GetLlvmDiFile (),
		pStructType->GetItemDecl ()->GetPos ()->m_Line,
		pStructType->GetSize () * 8,
		pStructType->GetAlignFactor () * 8,
		0,
		LlvmDiArray, 
		1
		);
}

llvm::DIType
CLlvmDiBuilder::CreateUnionType (CUnionType* pUnionType)
{
	rtl::CConstListT <CStructField> FieldList = pUnionType->GetFieldList ();
	size_t Count = FieldList.GetCount ();
	
	char Buffer [256];
	rtl::CArrayT <llvm::Value*> FieldTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	FieldTypeArray.SetCount (Count);

	rtl::CIteratorT <CStructField> Field = FieldList.GetHead ();	
	for (size_t i = 0; Field; i++, Field++)
	{
		CStructField* pField = *Field;
		FieldTypeArray [i] = m_pLlvmDiBuilder->createMemberType (
			pField->GetModule ()->GetLlvmDiFile (),
			pField->GetName ().cc (),
			pField->GetModule ()->GetLlvmDiFile (),
			pField->GetItemDecl ()->GetPos ()->m_Line,
			pField->GetType ()->GetSize () * 8,
			pField->GetType ()->GetAlignFactor () * 8,
			pField->GetOffset () * 8,
			0, 
			pField->GetType ()->GetLlvmDiType ()
			);	
	}

	llvm::DIArray LlvmDiArray = m_pLlvmDiBuilder->getOrCreateArray (llvm::ArrayRef <llvm::Value*> (FieldTypeArray, Count));
	
	return m_pLlvmDiBuilder->createUnionType (
		pUnionType->GetModule ()->GetLlvmDiFile (),
		pUnionType->m_Tag.cc (),
		pUnionType->GetModule ()->GetLlvmDiFile (),
		pUnionType->GetItemDecl ()->GetPos ()->m_Line,
		pUnionType->GetSize () * 8,
		pUnionType->GetAlignFactor () * 8,
		0,
		LlvmDiArray
		);
}

llvm::DIType
CLlvmDiBuilder::CreateArrayType (CArrayType* pArrayType)
{
	char Buffer [256];
	rtl::CArrayT <llvm::Value*> DimArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	CArrayType* p = pArrayType;
	for (;;) 
	{
		CType* pElementType = p->GetElementType ();	
		size_t ElementCount = p->GetElementCount ();
		ASSERT (ElementCount);

		DimArray.Append (m_pLlvmDiBuilder->getOrCreateSubrange (0, ElementCount - 1));
		
		if (pElementType->GetTypeKind () != EType_Array)
			break;
		
		p = (CArrayType*) pElementType;		
	} 
		
	llvm::DIArray LlvmDiArray = m_pLlvmDiBuilder->getOrCreateArray (llvm::ArrayRef <llvm::Value*> (DimArray, DimArray.GetCount ()));
	
	return m_pLlvmDiBuilder->createArrayType (
		pArrayType->GetSize () * 8,
		pArrayType->GetAlignFactor () * 8,
		pArrayType->GetRootType ()->GetLlvmDiType (),
		LlvmDiArray		
		);
}

llvm::DIGlobalVariable
CLlvmDiBuilder::CreateGlobalVariable (CVariable* pVariable)
{
	return m_pLlvmDiBuilder->createGlobalVariable (
		pVariable->GetQualifiedName ().cc (),
		pVariable->GetModule ()->GetLlvmDiFile (),
		pVariable->GetItemDecl ()->GetPos ()->m_Line,
		pVariable->GetType ()->GetLlvmDiType (),
		true,
		pVariable->GetLlvmValue ()
		);
}

llvm::DIVariable
CLlvmDiBuilder::CreateLocalVariable (
	CVariable* pVariable,
	uint_t Tag
	)
{
	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	ASSERT (pScope);

	return m_pLlvmDiBuilder->createLocalVariable (
		Tag,
		pScope->GetLlvmDiScope (),
		pVariable->GetName ().cc (),
		pVariable->GetModule ()->GetLlvmDiFile (),
		pVariable->GetItemDecl ()->GetPos ()->m_Line,
		pVariable->GetType ()->GetLlvmDiType (),
		true,
		0, 0
		);
}

llvm::Instruction*
CLlvmDiBuilder::CreateDeclare (CVariable* pVariable)
{
	CBasicBlock* pBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	
	ASSERT (pBlock && pScope);

	llvm::Instruction* pLlvmInstruction = m_pLlvmDiBuilder->insertDeclare (
		pVariable->GetLlvmValue (),
		(llvm::DIVariable) pVariable->GetLlvmDiDescriptor (),
		pBlock->GetLlvmBlock ()
		);

	llvm::DebugLoc LlvmDebugLoc = llvm::DebugLoc::get (
		pVariable->GetItemDecl ()->GetPos ()->m_Line + 1, 
		pVariable->GetItemDecl ()->GetPos ()->m_Col + 1, 
		pScope->GetLlvmDiScope ()
		);
	
	pLlvmInstruction->setDebugLoc (LlvmDebugLoc);
	return pLlvmInstruction;
}

llvm::DISubprogram
CLlvmDiBuilder::CreateFunction (CFunction* pFunction)
{
	return m_pLlvmDiBuilder->createFunction (
		pFunction->GetModule ()->GetLlvmDiFile (),
		pFunction->m_Tag.cc (),      
		pFunction->m_Tag.cc (), // linkage name
		pFunction->GetModule ()->GetLlvmDiFile (),
		pFunction->GetItemDecl ()->GetPos ()->m_Line,
		pFunction->GetType ()->GetLlvmDiType (),
		true,
		true, 
		pFunction->GetItemDecl ()->GetPos ()->m_Line, // scope line
		llvm::DIDescriptor::FlagPrototyped,
		false,
		pFunction->GetLlvmFunction ()
		);
}

llvm::DILexicalBlock
CLlvmDiBuilder::CreateLexicalBlock (
	CScope* pParentScope,
	const CToken::CPos& Pos
	)
{
	return m_pLlvmDiBuilder->createLexicalBlock (
		pParentScope->GetLlvmDiScope (),
		pParentScope->GetModule ()->GetLlvmDiFile (),
		Pos.m_Line,
		Pos.m_Col
		);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
