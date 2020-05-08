#include "pch.h"
#include "axl_err_Error.h"
#include "axl_spy_ImportTableMgr.h"

#if (_AXL_OS_POSIX)
#	include "axl_sys_psx_DynamicLib.h"
#	include <link.h>
#endif

namespace axl {
namespace spy {

//..............................................................................

#if (_AXL_OS_WIN)

size_t
ImportTableMgr::enumerateImports(void* module)
{
	if (!module)
		module = ::GetModuleHandle(NULL);

	char* moduleBase = (char*)module;

	IMAGE_DOS_HEADER* dosHdr = (IMAGE_DOS_HEADER*)moduleBase;
	if (dosHdr->e_magic != IMAGE_DOS_SIGNATURE)
	{
		err::setError("invalid module (bad DOS signature)");
		return -1;
	}

	IMAGE_NT_HEADERS* peHdr = (IMAGE_NT_HEADERS*)(moduleBase + dosHdr->e_lfanew);
	if (peHdr->Signature != IMAGE_NT_SIGNATURE)
	{
		err::setError("invalid module (bad PE signature)");
		return -1;
	}

	IMAGE_DATA_DIRECTORY* importDir = (IMAGE_DATA_DIRECTORY*)&peHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	IMAGE_IMPORT_DESCRIPTOR* p = (IMAGE_IMPORT_DESCRIPTOR*)(moduleBase + importDir->VirtualAddress);
	IMAGE_IMPORT_DESCRIPTOR* end = p + importDir->Size / sizeof(IMAGE_IMPORT_DESCRIPTOR);

	size_t count = 0;

	for (; p < end && p->Name; p++)
	{
        IMAGE_THUNK_DATA* nameThunk = (IMAGE_THUNK_DATA*)(moduleBase + p->OriginalFirstThunk);
        IMAGE_THUNK_DATA* addrThunk = (IMAGE_THUNK_DATA*)(moduleBase + p->FirstThunk);

		const char* moduleName = moduleBase + p->Name;
		printf("%s\n", moduleName);

		for (; addrThunk->u1.Function; nameThunk++, addrThunk++, count++)
		{
            if (IMAGE_SNAP_BY_ORDINAL(nameThunk->u1.Ordinal))
			{
                uint_t ordinal = IMAGE_ORDINAL(nameThunk->u1.Ordinal);
				printf("  @%d\n", ordinal);
            }
			else
			{
				IMAGE_IMPORT_BY_NAME* name = (IMAGE_IMPORT_BY_NAME*)(moduleBase + nameThunk->u1.AddressOfData);
				printf("  hint %04x: %s\n", name->Hint, name->Name);
			}
		}
	}

	return count;
}

#elif (_AXL_OS_LINUX)
#	if (_AXL_CPU_X86)
#	define ELF_R_TYPE  ELF32_R_TYPE
#	define ELF_R_SYM   ELF32_R_SYM
#	define ELF_ST_TYPE ELF32_ST_TYPE
#	define ELF_ST_BIND ELF32_ST_BIND

	typedef ElfW(Rel) ElfRel;

	enum Dt
	{
		Dt_Rel     = DT_REL,
		Dt_RelSz   = DT_RELSZ,
		Dt_RelEnt  = DT_RELENT,

		R_GlobDat  = R_386_GLOB_DAT,
		R_JumpSlot = R_386_JMP_SLOT,
	};
#	elif (_AXL_CPU_AMD64)
#	define ELF_R_TYPE  ELF64_R_TYPE
#	define ELF_R_SYM   ELF64_R_SYM
#	define ELF_ST_TYPE ELF64_ST_TYPE
#	define ELF_ST_BIND ELF64_ST_BIND

	typedef ElfW(Rela) ElfRel;

	enum
	{
		Dt_Rel     = DT_RELA,
		Dt_RelSz   = DT_RELASZ,
		Dt_RelEnt  = DT_RELAENT,

		R_GlobDat  = R_X86_64_GLOB_DAT,
		R_JumpSlot = R_X86_64_JUMP_SLOT,
	}
#	elif (_AXL_CPU_ARM32)
#	define ELF_R_TYPE  ELF32_R_TYPE
#	define ELF_R_SYM   ELF32_R_SYM
#	define ELF_ST_TYPE ELF32_ST_TYPE
#	define ELF_ST_BIND ELF32_ST_BIND

	typedef ElfW(Rel) ElfRel;

	enum
	{
		Dt_Rel     = DT_RELA,
		Dt_RelSz   = DT_RELASZ,
		Dt_RelEnt  = DT_RELAENT,

		R_GlobDat  = R_X86_64_GLOB_DAT,
		R_JumpSlot = R_X86_64_JUMP_SLOT,
	};
#	endif

void
printRelTable(
	ElfRel* rel,
	size_t count,
	int relType,
	char* moduleBase,
	ElfW(Sym)* symbolTable,
	char* stringTable,
	size_t stringTableSize
	)
{
	ElfRel* end = rel + count;

	for (; rel < end; rel++)
	{
		if (ELF_R_TYPE(rel->r_info) != relType)
			continue;

		size_t symIdx = ELF_R_SYM(rel->r_info);
		ElfW(Sym)* sym = symbolTable + symIdx;
		if (ELF_ST_TYPE(sym->st_info) != STT_FUNC)
			continue;

		if (sym->st_name >= stringTableSize)
		{
			printf("*** symbol out of string table\n");
			continue;
		}

		char* name = stringTable + sym->st_name;
		void** p = (void**)(moduleBase + rel->r_offset);
		printf("  %p %s\n", *p, name);
	}
}

size_t
ImportTableMgr::enumerateImports(void* module)
{
	bool result;

	sys::psx::DynamicLib exe;
	if (!module)
	{
		result = exe.open(NULL, RTLD_LAZY | RTLD_NOLOAD);
		if (!result)
			return -1;

		module = exe;
	}

	link_map* linkMap;

	sys::psx::DynamicLib lib;
	lib.attach(module);
	result = exe.getInfo(RTLD_DI_LINKMAP, &linkMap);
	lib.detach();

	if (!result)
		return -1;

	ElfW(Dyn)* dynTable[DT_NUM] = { 0 };
	ElfW(Dyn)* dyn = linkMap->l_ld;
	for (; dyn->d_tag; dyn++)
	{
		if (dyn->d_tag < countof(dynTable))
			dynTable[dyn->d_tag] = dyn;
	}

	// sanity checks

	if (!dynTable[DT_SYMTAB] ||
		!dynTable[DT_SYMENT] ||
		!dynTable[DT_STRTAB] ||
		!dynTable[DT_STRSZ] ||
		!dynTable[Dt_Rel] != !dynTable[Dt_RelSz] ||
		!dynTable[Dt_Rel] != !dynTable[Dt_RelEnt] ||
		!dynTable[DT_JMPREL] != !dynTable[DT_PLTRELSZ] ||
		dynTable[DT_SYMENT]->d_un.d_val != sizeof(ElfW(Sym)) ||
		dynTable[Dt_RelEnt]->d_un.d_val != sizeof(ElfRel))
	{
		err::setError("invalid ELF (missing or bad section(s))");
		return -1;
	}

	ElfW(Sym)* symbolTable = (ElfW(Sym)*)dynTable[DT_SYMTAB]->d_un.d_ptr;
	char* stringTable = (char*)dynTable[DT_STRTAB]->d_un.d_ptr;
	size_t stringTableSize = dynTable[DT_STRSZ]->d_un.d_val;

	size_t pltCount = dynTable[DT_PLTRELSZ] ? dynTable[DT_PLTRELSZ]->d_un.d_val / sizeof(ElfRel) : 0;
	size_t gotCount = dynTable[Dt_RelSz] ? dynTable[Dt_RelSz]->d_un.d_val / sizeof(ElfRel) : 0;

	if (pltCount)
	{
		printf("PLT:\n");

		printRelTable(
			(ElfRel*)dynTable[DT_JMPREL]->d_un.d_ptr,
			pltCount,
			R_JumpSlot,
			(char*)linkMap->l_addr,
			symbolTable,
			stringTable,
			stringTableSize
			);
	}

	if (gotCount)
	{
		printf("GOT:\n");

		printRelTable(
			(ElfRel*)dynTable[Dt_Rel]->d_un.d_ptr,
			gotCount,
			R_GlobDat,
			(char*)linkMap->l_addr,
			symbolTable,
			stringTable,
			stringTableSize
			);
	}

	return 0;
}

#elif (_AXL_OS_DARWIN)
#endif

//..............................................................................

} // namespace spy
} // namespace axl
