#pragma once

#include "axl_spy_ModuleEnumerator.h"

namespace axl {
namespace spy {

//..............................................................................

struct ImportWriteProtectionBackup {
	void* m_p;
	size_t m_size;

	union {
		uint_t m_flags;          // ElfW(Phdr)->p_flags
		dword_t m_oldProtection; // returned by VirtualProtect
	};
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
disableImportWriteProtection(
	void* module,
	ImportWriteProtectionBackup* backup
);

bool
disableImportWriteProtection(
	const ModuleIterator& moduleIterator,
	ImportWriteProtectionBackup* backup
);

bool
restoreImportWriteProtection(const ImportWriteProtectionBackup* backup);

//..............................................................................

} // namespace spy
} // namespace axl
