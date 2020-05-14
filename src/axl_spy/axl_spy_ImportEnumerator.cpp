#include "pch.h"
#include "axl_spy_ImportEnumerator.h"
#include "axl_g_Module.h"

#if (_AXL_OS_LINUX)
#	include "axl_sys_psx_DynamicLib.h"
#endif

namespace axl {
namespace spy {

//..............................................................................

#if (_AXL_OS_WIN)
#	define _AXL_SPY_USE_RAW_SIZE 1
#	if (_AXL_DEBUG)
#		define _AXL_SPY_TRACE_NON_CODE_IMPORT    1
#		define _AXL_SPY_ANALYZE_NON_CODE_ADDRESS 0
#	endif

IMAGE_NT_HEADERS*
getPeHdr(HMODULE hModule)
{
	char* moduleBase = (char*)hModule;

	IMAGE_DOS_HEADER* dosHdr = (IMAGE_DOS_HEADER*)moduleBase;
	if (dosHdr->e_magic != IMAGE_DOS_SIGNATURE)
	{
		err::setError("invalid module (bad DOS signature)");
		return NULL;
	}

	IMAGE_NT_HEADERS* peHdr = (IMAGE_NT_HEADERS*)(moduleBase + dosHdr->e_lfanew);
	if (peHdr->Signature != IMAGE_NT_SIGNATURE)
	{
		err::setError("invalid module (bad PE signature)");
		return NULL;
	}

	return peHdr;
}

//..............................................................................

bool
PeCodeMap::isCode(size_t address)
{
	ModuleCodeMap* map = getModuleCodeMap(address);
	if (!map)
		return false;

	if (map->m_codeSection.isInside(address))
		return true;

	size_t count = map->m_auxCodeSectionArray.getCount();
	for (size_t i = 0; i < count; i++)
		if (map->m_auxCodeSectionArray[i].isInside(address))
			return true;

	return false;
}

#if (_AXL_SPY_ANALYZE_NON_CODE_ADDRESS)

void
PeCodeMap::analyzeNonCodeAddress(size_t address)
{
	ModuleCodeMap* map = getModuleCodeMap(address);
	if (!map)
	{
		printf("  *** parent module not found for %p\n", address);
		return;
	}

	HMODULE hModule = (HMODULE)map->m_moduleBase;

	wchar_t baseName[256];
	wchar_t fileName[256];
	::GetModuleBaseNameW(::GetCurrentProcess(), hModule, baseName, sizeof(baseName));
	::GetModuleFileNameW(hModule, fileName, sizeof(fileName));

	size_t rva = address - map->m_moduleBase;

	printf(
		"  address:     %p\n"
		"  RVA:         %p\n"
		"  module:\n"
		"    baseName:  %S\n"
		"    fileName:  %S\n"
		"    begin:     %p\n"
		"    end:       %p\n"
		"    size:      %zd\n"
		"  sections:\n",

		address,
		rva,
		baseName,
		fileName,
		map->m_moduleBase,
		map->m_moduleEnd,
		map->m_moduleEnd - map->m_moduleBase
		);

	IMAGE_NT_HEADERS* peHdr = getPeHdr(hModule);
	if (!peHdr)
	{
		printf("  *** can't get PE header: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	IMAGE_SECTION_HEADER* p = IMAGE_FIRST_SECTION(peHdr);
	IMAGE_SECTION_HEADER* end = p + peHdr->FileHeader.NumberOfSections;

	size_t moduleBase = (size_t)hModule;
	for (; p < end; p++)
	{
		size_t sectionEnd = p->VirtualAddress + p->Misc.VirtualSize;

		printf(
			"    %c%8s: %08zx .. %08zx\n",
			rva >= p->VirtualAddress && rva < sectionEnd ? '>' : ' ',
			p->Name,
			p->VirtualAddress,
			sectionEnd
			);
	}
}

#endif

PeCodeMap::ModuleCodeMap*
PeCodeMap::getModuleCodeMap(size_t address)
{
	sl::MapIterator<size_t, ModuleCodeMap> it = m_moduleMap.find(address, sl::BinTreeFindRelOp_Lt);
	if (it && address < it->m_value.m_moduleEnd)
		return &it->m_value;

	HMODULE hModule;
	MODULEINFO moduleInfo;

	bool_t result =
		::GetModuleHandleExW(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
			(LPCWSTR)address,
			&hModule
			) &&
		::GetModuleInformation(
			::GetCurrentProcess(),
			hModule,
			&moduleInfo,
			sizeof(moduleInfo)
			);

	if (!result)
	{
		err::setLastSystemError();
		return NULL;
	}

	ASSERT(moduleInfo.lpBaseOfDll == hModule);

	IMAGE_NT_HEADERS* peHdr = getPeHdr(hModule);
	if (!peHdr)
		return false;

	size_t moduleBase = (size_t)hModule;
	it = m_moduleMap.visit(moduleBase);

	ModuleCodeMap* map = &it->m_value;
	map->m_moduleBase = moduleBase;
	map->m_moduleEnd = moduleBase + moduleInfo.SizeOfImage;
	map->m_codeSection.m_begin = 0;
	map->m_codeSection.m_end = 0;

	IMAGE_SECTION_HEADER* p = IMAGE_FIRST_SECTION(peHdr);
	IMAGE_SECTION_HEADER* end = p + peHdr->FileHeader.NumberOfSections;

	for (; p < end; p++)
	{
		if (!(p->Characteristics & IMAGE_SCN_CNT_CODE))
			continue;

		AddressRange range;
		range.m_begin = moduleBase + p->VirtualAddress;
#if (_AXL_SPY_USE_RAW_SIZE) // normally, shouldn't matter
		range.m_end = range.m_begin + p->SizeOfRawData;
#else
		range.m_end = range.m_begin + p->Misc.VirtualSize;
#endif

		if (!map->m_codeSection.m_begin)
			map->m_codeSection = range;
		else
			map->m_auxCodeSectionArray.append(range);
	}

	return map;
}

//..............................................................................

ImportIterator::ImportIterator()
{
	m_ordinal = -1;
    m_importDescIdx = -1;
	m_nameThunk = NULL;
	m_addrThunk = NULL;
}

ImportIterator::ImportIterator(PeImportEnumeration* enumeration)
{
	m_ordinal = -1;
	m_importDescIdx = -1;
	m_nameThunk = NULL;
	m_addrThunk = NULL;

	if (!enumeration)
		return;

	m_enumeration = enumeration;
	m_importDescIdx = 0;
	openImportDesc();
}

ImportIterator&
ImportIterator::operator ++ ()
{
	if (!m_enumeration || m_importDescIdx >= m_enumeration->m_importDescCount)
	{
		reset();
		return *this;
	}

	ASSERT(m_nameThunk && m_addrThunk);
	m_nameThunk++;
	m_addrThunk++;

	bool result = readThunk();
	if (!result)
	{
		m_importDescIdx++;
		openImportDesc();
	}

	return *this;
}

ImportIterator
ImportIterator::operator ++ (int)
{
	ImportIterator it = *this;
	operator ++ ();
	return it;
}

bool
ImportIterator::openImportDesc()
{
	ASSERT(m_enumeration);

	bool result;

	for (; m_importDescIdx < m_enumeration->m_importDescCount; m_importDescIdx++)
	{
		IMAGE_IMPORT_DESCRIPTOR* p = &m_enumeration->m_importDir[m_importDescIdx];
		if (!p->Name) // NULL-terminator
			break;

		m_nameThunk = (IMAGE_THUNK_DATA*)(m_enumeration->m_moduleBase + p->OriginalFirstThunk);
		m_addrThunk = (IMAGE_THUNK_DATA*)(m_enumeration->m_moduleBase + p->FirstThunk);

		result = readThunk();
		if (result)
		{
			m_exportModuleName = m_enumeration->m_moduleBase + p->Name;
			return true;
		}
	}

	reset();
	return false;
}

bool
ImportIterator::readThunk()
{
	ASSERT(m_enumeration && m_nameThunk && m_addrThunk);

	for (; m_addrThunk->u1.Function; m_nameThunk++, m_addrThunk++)
	{
		if (IMAGE_SNAP_BY_ORDINAL(m_nameThunk->u1.Ordinal))
		{
			m_ordinal = IMAGE_ORDINAL(m_nameThunk->u1.Ordinal);
			m_name.clear();
		}
		else
		{
			IMAGE_IMPORT_BY_NAME* name = (IMAGE_IMPORT_BY_NAME*)(m_enumeration->m_moduleBase + m_nameThunk->u1.AddressOfData);
			m_ordinal = -1;
			m_hint = name->Hint;
			m_name = name->Name;
		}

		if (!isCode(m_addrThunk->u1.Function))
		{
#if (_AXL_SPY_TRACE_NON_CODE_IMPORT)
			if (m_ordinal != -1)
				printf("  *** Non-code: %s@%d\n", m_exportModuleName.sz(), m_ordinal);
			else
				printf("  *** Non-code: %s:%s\n", m_exportModuleName.sz(), m_name.sz());
#	if (_AXL_SPY_ANALYZE_NON_CODE_ADDRESS)
			m_codeMap->analyzeNonCodeAddress(m_addrThunk->u1.Function);
#	endif
#endif
			continue;
		}

		m_slot = (void**)&m_addrThunk->u1.Function; // not RVA!
		return true;
	}

	return false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
enumerateImports(
	ImportIterator* iterator,
    void* module
	)
{
	if (!module)
		module = ::GetModuleHandle(NULL);

	IMAGE_NT_HEADERS* peHdr = getPeHdr((HMODULE)module);
	if (!peHdr)
		return false;

	char* moduleBase = (char*)module;
	IMAGE_DATA_DIRECTORY* importDir = (IMAGE_DATA_DIRECTORY*)&peHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

	ref::Ptr<PeImportEnumeration> enumeration = AXL_REF_NEW(PeImportEnumeration);
	enumeration->m_moduleBase = moduleBase;
	enumeration->m_importDir = (IMAGE_IMPORT_DESCRIPTOR*)(moduleBase + importDir->VirtualAddress);
	enumeration->m_importDescCount = importDir->Size / sizeof(IMAGE_IMPORT_DESCRIPTOR);

	*iterator = ImportIterator(enumeration);
	return true;
}

#elif (_AXL_OS_LINUX)

ImportIterator::ImportIterator(ElfImportEnumeration* enumeration)
{
	if (!enumeration)
	{
		m_index = -1;
		return;
	}

	m_enumeration = enumeration;
	m_index = 0;
	readRel();
}

ImportIterator&
ImportIterator::operator ++ ()
{
	if (!m_enumeration ||
		m_index >= m_enumeration->m_pltRelCount + m_enumeration->m_gotRelCount)
	{
		reset();
		return *this;
	}

	m_index++;
	readRel();
	return *this;
}

ImportIterator
ImportIterator::operator ++ (int)
{
	ImportIterator it = *this;
	operator ++ ();
	return it;
}

bool
ImportIterator::readRel()
{
	ASSERT(m_enumeration);

	int relType;
	ElfRel* rel;
	ElfRel* end;

	if (m_index < m_enumeration->m_pltRelCount)
	{
		relType = R_JUMP_SLOT;
		rel = m_enumeration->m_pltRelTable;
		end = rel + m_enumeration->m_pltRelCount;
		rel += m_index;
	}
	else
	{
		relType = R_GLOB_DAT;
		rel = m_enumeration->m_gotRelTable;
		end = rel + m_enumeration->m_gotRelCount;
		rel += m_index - m_enumeration->m_pltRelCount;
	}

	for (; rel < end; rel++, m_index++)
	{
		if (ELF_R_TYPE(rel->r_info) != relType)
			continue;

		size_t symIdx = ELF_R_SYM(rel->r_info);
		ElfW(Sym)* sym = m_enumeration->m_symbolTable + symIdx;
		if (ELF_ST_TYPE(sym->st_info) != STT_FUNC)
			continue;

		if (sym->st_name >= m_enumeration->m_stringTableSize)
		{
			AXL_TRACE("WARNING: ImportIterator::readRel: symbol out of string table\n");
			continue;
		}

		m_name = m_enumeration->m_stringTable + sym->st_name;
		m_slot = (void**)(m_enumeration->m_moduleBase + rel->r_offset);
		return true;
	}

	if (relType == R_JUMP_SLOT)
		return readRel(); // retry with GOT

	reset();
	return false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
enumerateImports(
	ImportIterator* iterator,
	void* module
)
{
	link_map* linkMap = module ? (link_map*)module : _r_debug.r_map;

	ElfW(Dyn)* dynTable[DT_NUM] = { 0 };
	ElfW(Dyn)* dyn = linkMap->l_ld;
	for (; dyn->d_tag; dyn++)
		if (dyn->d_tag < countof(dynTable))
			dynTable[dyn->d_tag] = dyn;

	// sanity check

	if (!dynTable[DT_SYMTAB] ||
		!dynTable[DT_SYMENT] ||
		!dynTable[DT_STRTAB] ||
		!dynTable[DT_STRSZ] ||
		!dynTable[DT_GOT_REL] != !dynTable[DT_GOT_RELSZ] ||
		!dynTable[DT_GOT_REL] != !dynTable[DT_GOT_RELENT] ||
		!dynTable[DT_JMPREL] != !dynTable[DT_PLTRELSZ] ||
		dynTable[DT_SYMENT]->d_un.d_val != sizeof(ElfW(Sym)) ||
		dynTable[DT_GOT_RELENT] && dynTable[DT_GOT_RELENT]->d_un.d_val != sizeof(ElfRel))
	{
		err::setError("invalid ELF (missing or bad section(s))");
		return false;
	}

	size_t pltRelCount = dynTable[DT_PLTRELSZ] ? dynTable[DT_PLTRELSZ]->d_un.d_val / sizeof(ElfRel) : 0;
	size_t gotRelCount = dynTable[DT_GOT_RELSZ] ? dynTable[DT_GOT_RELSZ]->d_un.d_val / sizeof(ElfRel) : 0;

	if (!pltRelCount && !gotRelCount)
	{
		*iterator = ImportIterator();
		return true;
	}

	ref::Ptr<ElfImportEnumeration> enumeration = AXL_REF_NEW(ElfImportEnumeration);
	enumeration->m_moduleBase = (char*)linkMap->l_addr;
	enumeration->m_symbolTable = (ElfW(Sym)*)dynTable[DT_SYMTAB]->d_un.d_ptr;
	enumeration->m_stringTable = (char*)dynTable[DT_STRTAB]->d_un.d_ptr;
	enumeration->m_stringTableSize = dynTable[DT_STRSZ]->d_un.d_val;
	enumeration->m_pltRelTable = dynTable[DT_JMPREL] ? (ElfRel*)dynTable[DT_JMPREL]->d_un.d_ptr : NULL;
	enumeration->m_pltRelCount = pltRelCount;
	enumeration->m_gotRelTable = dynTable[DT_GOT_REL] ? (ElfRel*)dynTable[DT_GOT_REL]->d_un.d_ptr : NULL;
	enumeration->m_gotRelCount = gotRelCount;

	*iterator = ImportIterator(enumeration);
	return true;
}

#elif (_AXL_OS_DARWIN)

uint64_t uleb128(const uint8_t** p)
{
	uint64_t r = 0;
	int s = 0;

	do
	{
		r |= (uint64_t)(**p & 0x7f) << s;
		s += 7;
	} while (*(*p)++ >= 0x80);

	return r;
}

int64_t sleb128(const uint8_t** p)
{
	int64_t r = 0;
	int s = 0;
	for (;;)
	{
		uint8_t b = *(*p)++;
		if (b < 0x80)
		{
			if (b & 0x40)
				r -= (0x80 - b) << s;
			else
				r |= (b & 0x3f) << s;

			break;
		}

		r |= (b & 0x7f) << s;
		s += 7;
	}

	return r;
}

size_t
ImportTableMgr::enumerateImports(void* module)
{
	uint8_t* slide = (uint8_t*)_dyld_get_image_vmaddr_slide(0);
	mach_header_64* machHdr = (mach_header_64*)_dyld_get_image_header(0);
	load_command* cmd = (load_command*)(machHdr + 1);

	sl::Array<segment_command_64*> segmentCmdArray;
	size_t linkEditSegmentIdx = -1;
	size_t lazyBindOffset = -1;
	size_t lazyBindSize = -1;

	for (uint_t i = 0; i < machHdr->ncmds; i++)
	{
		printf("load_command: 0x%x\n", cmd->cmd);

		switch (cmd->cmd)
		{
			segment_command_64* segmentCmd;
			dyld_info_command* dyldInfoCmd;

		case LC_SEGMENT_64:
			segmentCmd = (segment_command_64*)cmd;

			if (strcmp(segmentCmd->segname, "__LINKEDIT") == 0)
				linkEditSegmentIdx = segmentCmdArray.getCount();

			segmentCmdArray.append(segmentCmd);
			break;

		case LC_DYLD_INFO_ONLY:
			dyldInfoCmd = (dyld_info_command*)cmd;
			lazyBindOffset = dyldInfoCmd->lazy_bind_off;
			lazyBindSize = dyldInfoCmd->lazy_bind_size;
			break;
		}

		cmd = (load_command*)((char*)cmd + cmd->cmdsize);
	}

	if (linkEditSegmentIdx == -1 ||
		lazyBindOffset == -1 ||
		lazyBindSize == -1)
	{
		err::setError("invalid MACH-O file");
		return -1;
	}

	segment_command_64* linkEditSegmentCmd = segmentCmdArray[linkEditSegmentIdx];

	const uint8_t* p = slide + linkEditSegmentCmd->vmaddr - linkEditSegmentCmd->fileoff + lazyBindOffset;
	const uint8_t* end = p + lazyBindSize;

	while (p < end)
	{
		uint8_t op = *p & BIND_OPCODE_MASK;
		uint8_t imm = *p & BIND_IMMEDIATE_MASK;
		p++;

		switch (op)
		{
		case BIND_OPCODE_DONE:
		case BIND_OPCODE_DO_BIND:
		case BIND_OPCODE_SET_TYPE_IMM:
		case BIND_OPCODE_SET_DYLIB_ORDINAL_IMM:
		case BIND_OPCODE_SET_DYLIB_SPECIAL_IMM:
		case BIND_OPCODE_DO_BIND_ADD_ADDR_IMM_SCALED:
			break;

		case BIND_OPCODE_SET_SYMBOL_TRAILING_FLAGS_IMM:
			printf("  %s\n", p);
			p += strlen((char*)p) + 1;
			break;

		case BIND_OPCODE_SET_ADDEND_SLEB:
			sleb128(&p);
			break;

		case BIND_OPCODE_SET_DYLIB_ORDINAL_ULEB:
		case BIND_OPCODE_SET_SEGMENT_AND_OFFSET_ULEB:
		case BIND_OPCODE_ADD_ADDR_ULEB:
		case BIND_OPCODE_DO_BIND_ADD_ADDR_ULEB:
			uleb128(&p);
			break;

		case BIND_OPCODE_DO_BIND_ULEB_TIMES_SKIPPING_ULEB:
			uleb128(&p);
			uleb128(&p);
			break;

		default:
			printf("*** unexpected bind op: 0x%02x\n", op);
		}
	}

	return -1;
}

#endif

//..............................................................................

} // namespace spy
} // namespace axl
