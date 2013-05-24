#include "pch.h"
#include "axl_jnc_Disassembler.h"
#include "axl_mem_Block.h"

namespace axl {
namespace jnc {

//.............................................................................

static 
int 
ByteReader (
	uint8_t* pByte, 
	uint64_t Offset, 
	void* pContext
	)
{
	mem::TBlock* pBlock = (mem::TBlock*) pContext;

	if (Offset >= pBlock->m_Size)
		return -1;

	*pByte = *((uchar_t*) pBlock->m_p + Offset);
	return 0;
}

//.............................................................................

CDisassembler::CDisassembler ()
{
	m_pDisassembler = llvm::EDDisassembler::getDisassembler (
		llvm::Triple::x86, 
		llvm::EDDisassembler::kEDAssemblySyntaxX86ATT 
		// llvm::EDDisassembler::kEDAssemblySyntaxX86Intel -- does not work!
		);
}

bool 
CDisassembler::Disassemble (
	const void* pCode,
	size_t Size,
	rtl::CString* pString
	)
{
	if (!m_pDisassembler)
		return false;

	mem::TBlock Block ((void*) pCode, Size);
	
	size_t Address = 0;
	while (Address < Size) 
	{
		llvm::EDInst* pInst = m_pDisassembler->createInst (ByteReader, Address, &Block);
		if (!pInst)
			break;

		size_t TokenCount = pInst->numTokens ();
		if (TokenCount == -1)
			return false;

		pString->AppendFormat ("%08x  ", Address);

		for (size_t i = 0; i < TokenCount; i++)
		{
			llvm::EDToken* pToken;
			pInst->getToken (pToken, i);

			const char* pTokenString;
			pToken->getString (pTokenString);

			pString->Append (pTokenString);
		}

		pString->Append ("\r\n");

		Address += (size_t) pInst->byteSize ();
		delete pInst;
	}

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
