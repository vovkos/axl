#pragma once

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

class ElfParser {
protected:
	size_t m_fileSize;
	const ElfW(Ehdr)* m_elfHdr;
	const ElfW(Shdr)* m_shdrTable;
	const ElfW(Phdr)* m_phdrTable;
	const char* m_stringTable;
	size_t m_stringTableSize;

public:
	bool
	isOpen() {
		return m_elfHdr != NULL;
	}

	void
	close();

	bool
	open(
		const void* p,
		size_t size
	);

	size_t
	getFileSize() {
		ASSERT(isOpen());
		return m_fileSize;
	}

	const ElfW(Ehdr)*
	getElfHdr() {
		ASSERT(isOpen());
		return m_elfHdr;
	}

	const ElfW(Shdr)*
	getSectionHdrTable() {
		ASSERT(isOpen());
		return m_shdrTable;
	}

	const ElfW(Phdr)*
	getProgramHdrTable() {
		ASSERT(isOpen());
		return m_phdrTable;
	}

	const char*
	getString(size_t offset) {
		ASSERT(isOpen());
		return offset < m_stringTableSize ? m_stringTable + offset : NULL;
	}
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
