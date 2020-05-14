#include "pch.h"
#include "axl_spy_ImportWriteProtection.h"
#include "axl_g_Module.h"
#include "axl_err_Error.h"

namespace axl {
namespace spy {

//..............................................................................

#if (_AXL_OS_WIN)

IMAGE_NT_HEADERS*
getPeHdr(HMODULE hModule);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
disableImportWriteProtection(
	void* module,
	ImportWriteProtectionBackup* backup
	)
{
	if (!module)
		module = ::GetModuleHandle(NULL);

	IMAGE_NT_HEADERS* peHdr = getPeHdr((HMODULE)module);
	if (!peHdr)
		return false;

	IMAGE_DATA_DIRECTORY* iatDir = (IMAGE_DATA_DIRECTORY*)&peHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT];
	size_t begin = (size_t)module + iatDir->VirtualAddress;
	size_t end = begin + iatDir->Size;

	// align just in case

	size_t pageSize = g::getModule()->getSystemInfo()->m_pageSize;
	begin &= ~(pageSize - 1);
	end = (end + pageSize - 1) & ~(pageSize - 1);

	backup->m_p = (char*)begin;
	backup->m_size = end - begin;

	bool_t result = ::VirtualProtect(
		backup->m_p,
		backup->m_size,
		PAGE_EXECUTE_READWRITE,
		&backup->m_oldProtection
		);

	return err::complete(result);
}

bool
disableImportWriteProtection(
	const ModuleIterator& moduleIterator,
	ImportWriteProtectionBackup* backup
	)
{
	return disableImportWriteProtection(moduleIterator.getModule(), backup);
}

bool
restoreImportWriteProtection(const ImportWriteProtectionBackup* backup)
{
	dword_t oldProtect;

	bool_t result = ::VirtualProtect(
		backup->m_p,
		backup->m_size,
		backup->m_oldProtection,
		&oldProtect
		);

	return err::complete(result);
}

#elif (_AXL_OS_LINUX)

bool
disableImportWriteProtection(
	void* module,
	ImportWriteProtectionBackup* backup
	)
{
	link_map* linkMap = (link_map*)module;
	size_t moduleBase = (size_t)linkMap->l_addr;
	ElfW(Ehdr)* ehdr = (ElfW(Ehdr)*)moduleBase;

	size_t p = moduleBase + ehdr->e_phoff;
	for (size_t i = 0; i < ehdr->e_phnum; i++, p += ehdr->e_phentsize)
	{
		ElfW(Phdr)* phdr = (ElfW(Phdr)*)p;
		if (phdr->p_type != PT_GNU_RELRO) // read-only-after-relocation (contains GOT)
			continue;

		size_t begin = moduleBase + phdr->p_vaddr;
		size_t end = begin + phdr->p_memsz;

		size_t pageSize = g::getModule()->getSystemInfo()->m_pageSize;
		begin &= ~(pageSize - 1);
		end = (end + pageSize - 1) & ~(pageSize - 1);

		backup->m_p = (char*)begin;
		backup->m_size = end - begin;
		backup->m_flags = phdr->p_flags;

		int result = ::mprotect(backup->m_p, backup->m_size, PROT_READ | PROT_WRITE | PROT_EXEC);
		return err::complete(result != -1);
	}

	// GOT not found, still OK

	backupBuffer->clear();
	return true;
}

bool
disableImportWriteProtection(
	const ModuleIterator& moduleIterator,
	ImportWriteProtectionBackup* backup
	)
{
	return disableImportWriteProtection(moduleIterator.getModule(), backup);
}

bool
restoreImportWriteProtection(const ImportWriteProtectionBackup* backup)
{
	int prot = 0;

	if (backup->m_flags & PF_R)
		prot |= PROT_READ;

	if (backup->m_flags & PF_W)
		prot |= PROT_WRITE;

	if (backup->m_flags & PF_X)
		prot |= PROT_EXEC;

	int result = ::mprotect(backup->m_p, backup->m_size, prot);
	return err::complete(result != -1);
}

#elif (_AXL_OS_DARWIN)

bool
disableImportWriteProtection(
	size_t imageIndex,
	ImportWriteProtectionBackup* backup
	)
{
	return true;
}

bool
disableImportWriteProtection(
	void* module,
	ImportWriteProtectionBackup* backup
	)
{
	return true;
}

bool
disableImportWriteProtection(
	const ModuleIterator& moduleIterator,
	ImportWriteProtectionBackup* backup
	)
{
	return disableImportWriteProtection(moduleIterator.getImageIndex(), backup);
}

bool
restoreImportWriteProtection(const ImportWriteProtectionBackup* backup)
{
	return true;
}

#endif

//..............................................................................

} // namespace spy
} // namespace axl
