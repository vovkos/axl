#pragma once

#include "axl_sl_String.h"
#include "axl_sl_Array.h"

#if (_AXL_OS_WIN)
#	include "axl_sl_RbTree.h"
#elif (_AXL_OS_LINUX)
#   include "axl_sys_psx_DynamicLib.h"
#	include <link.h>

#	if (_AXL_CPU_X86)
#	    define ELF_R_TYPE    ELF32_R_TYPE
#	    define ELF_R_SYM     ELF32_R_SYM
#	    define ELF_ST_TYPE   ELF32_ST_TYPE
#	    define ELF_ST_BIND   ELF32_ST_BIND

#       define DT_GOT_REL    DT_REL
#       define DT_GOT_RELSZ  DT_RELSZ
#       define DT_GOT_RELENT DT_RELENT

#		define R_GLOB_DAT     R_386_GLOB_DAT
#		define R_JUMP_SLOT    R_386_JMP_SLOT

typedef ElfW(Rel) ElfRel;

#	elif (_AXL_CPU_AMD64)
#	    define ELF_R_TYPE    ELF64_R_TYPE
#	    define ELF_R_SYM     ELF64_R_SYM
#	    define ELF_ST_TYPE   ELF64_ST_TYPE
#	    define ELF_ST_BIND   ELF64_ST_BIND

#       define DT_GOT_REL    DT_RELA
#       define DT_GOT_RELSZ  DT_RELASZ
#       define DT_GOT_RELENT DT_RELAENT

#       define R_GLOB_DAT    R_X86_64_GLOB_DAT
#       define R_JUMP_SLOT   R_X86_64_JUMP_SLOT

typedef ElfW(Rela) ElfRel;

#	elif (_AXL_CPU_ARM32)
#	    define ELF_R_TYPE    ELF32_R_TYPE
#	    define ELF_R_SYM     ELF32_R_SYM
#	    define ELF_ST_TYPE   ELF32_ST_TYPE
#	    define ELF_ST_BIND   ELF32_ST_BIND

#       define DT_GOT_REL    DT_REL
#       define DT_GOT_RELSZ  DT_RELSZ
#       define DT_GOT_RELENT DT_RELENT

#       define R_GLOB_DAT    R_ARM_GLOB_DAT
#       define R_JUMP_SLOT   R_ARM_JUMP_SLOT

	typedef ElfW(Rel) ElfRel;

#	endif
#endif

namespace axl {
namespace spy {

//..............................................................................

class ImportIteratorBase
{
protected:
    sl::StringRef m_name;
    void** m_slot;

public:
	ImportIteratorBase()
	{
		m_slot = NULL;
	}

	operator bool ()
	{
		return m_slot != NULL;
	}

    const sl::StringRef&
    getName() const
    {
        return m_name;
    }

    void**
    getSlot() const
    {
        return m_slot;
    }

protected:
	void
	reset()
	{
    	m_name.clear();
    	m_slot = NULL;
	}
};

#if (_AXL_OS_WIN)

//..............................................................................

class PeCodeMap: public ref::RefCount
{
protected:
	struct AddressRange
	{
		size_t m_begin;
		size_t m_end;

		bool
		isInside(size_t address)
		{
			return address >= m_begin && address < m_end;
		}
	};

	struct ModuleCodeMap
	{
		size_t m_moduleBase;
		size_t m_moduleEnd;
		AddressRange m_codeSection;
		sl::Array<AddressRange> m_auxCodeSectionArray;
	};

protected:
	sl::RbTree<size_t, ModuleCodeMap> m_moduleMap;

public:
	bool
	isCode(size_t address);

	void
	analyzeNonCodeAddress(size_t address);

protected:
	ModuleCodeMap*
	getModuleCodeMap(size_t address);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct PeImportEnumeration: ref::RefCount
{
    char* m_moduleBase;
    IMAGE_IMPORT_DESCRIPTOR* m_importDir;
	size_t m_importDescCount;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ImportIterator: public ImportIteratorBase
{
protected:
	sl::StringRef m_exportModuleName;
	uint_t m_ordinal;
	uint_t m_hint;

	ref::Ptr<PeImportEnumeration> m_enumeration;
	ref::Ptr<PeCodeMap> m_codeMap;
	size_t m_importDescIdx;
	IMAGE_THUNK_DATA* m_nameThunk;
    IMAGE_THUNK_DATA* m_addrThunk;

public:
    ImportIterator();
	ImportIterator(PeImportEnumeration* enumeration);

    ImportIterator&
    operator ++ ();

	ImportIterator
	operator ++ (int);

	const sl::StringRef&
	getModuleName()
	{
		return m_exportModuleName;
	}

	uint_t
	getOrdinal()
	{
		return m_ordinal;
	}

	uint_t
	getHint()
	{
		return m_hint;
	}

protected:
	bool
	openImportDesc();

	bool
	isCode(size_t address)
	{
		return (m_codeMap ? m_codeMap : m_codeMap = AXL_REF_NEW(PeCodeMap))->isCode(address);
	}

	bool
	readThunk();
};

#elif (_AXL_OS_LINUX)

//..............................................................................

struct ElfImportEnumeration: ref::RefCount
{
    char* m_moduleBase;
    ElfW(Sym)* m_symbolTable;
    char* m_stringTable;
    size_t m_stringTableSize;
    ElfRel* m_pltRelTable;
    size_t m_pltRelCount;
    ElfRel* m_gotRelTable;
    size_t m_gotRelCount;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ImportIterator: public ImportIteratorBase
{
protected:
    ref::Ptr<ElfImportEnumeration> m_enumeration;
    size_t m_index;

public:
    ImportIterator()
	{
    	m_index = -1;
	}

	ImportIterator(ElfImportEnumeration* enumeration);

    ImportIterator&
    operator ++ ();

	ImportIterator
	operator ++ (int);

protected:
	bool
	readRel();
};

#elif (_AXL_OS_DARWIN)
#endif

//..............................................................................

bool
enumerateImports(
	ImportIterator* iterator,
    void* module = NULL
	);

inline
ImportIterator
enumerateImports(void* module = NULL)
{
	ImportIterator iterator;
	enumerateImports(&iterator, module);
	return iterator;
}

//..............................................................................

} // namespace spy
} // namespace axl
