#include "pch.h"
#include "axl_sys_lnx_ElfParser.h"

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

void
ElfParser::close() {
	m_fileSize = 0;
	m_elfHdr = NULL;
	m_shdrTable = NULL;
	m_phdrTable = NULL;
	m_stringTable = NULL;
	m_stringTableSize = 0;
}

bool
ElfParser::open(
	const void* p0,
	size_t size
) {
	close();

	char* p = (char*)p0;
	ElfW(Ehdr)* elfHdr = (ElfW(Ehdr)*)p;

	if (size < sizeof(ElfW(Ehdr)) ||
		elfHdr->e_ident[EI_MAG0] != ELFMAG0 ||
		elfHdr->e_ident[EI_MAG1] != ELFMAG1 ||
		elfHdr->e_ident[EI_MAG2] != ELFMAG2 ||
		elfHdr->e_ident[EI_MAG3] != ELFMAG3 ||
		elfHdr->e_ehsize < sizeof(ElfW(Ehdr)) ||
		elfHdr->e_ehsize > size ||
		elfHdr->e_phentsize != sizeof(ElfW(Phdr)) ||
		elfHdr->e_shentsize != sizeof(ElfW(Shdr)) ||
		elfHdr->e_phoff > size ||
		elfHdr->e_phoff + elfHdr->e_phnum * elfHdr->e_phentsize > size ||
		elfHdr->e_shoff > size ||
		elfHdr->e_shoff + elfHdr->e_shnum * elfHdr->e_shentsize > size ||
		elfHdr->e_shstrndx == SHN_UNDEF ||
		elfHdr->e_shstrndx >= elfHdr->e_shnum
	)
		return err::fail("invalid ELF-file");

	ElfW(Phdr)* phdrTable = (ElfW(Phdr)*)(p + elfHdr->e_phoff);
	ElfW(Shdr)* shdrTable = (ElfW(Shdr)*)(p + elfHdr->e_shoff);
	ElfW(Shdr)* stringShdr = &shdrTable[elfHdr->e_shstrndx];

	ElfW(Shdr)* shdr = shdrTable;
	ElfW(Shdr)* shdrEnd = shdr + elfHdr->e_shnum;
	for (; shdr < shdrEnd; shdr++)
		if (size < shdr->sh_offset ||
			size < shdr->sh_offset + shdr->sh_size ||
			stringShdr->sh_size <= shdr->sh_name)
			return err::fail("invalid section in ELF-file");

	m_fileSize = size;
	m_elfHdr = elfHdr;
	m_shdrTable = shdrTable;
	m_phdrTable = phdrTable;
	m_stringTable = p + stringShdr->sh_offset;
	m_stringTableSize = stringShdr->sh_size;
	return true;
}

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
