#include "pch.h"
#include "axl_err_Error.h"
#include "axl_spy_ImportTableMgr.h"

namespace axl {
namespace spy {

//..............................................................................

size_t
ImportTableMgr::enumerateImports(HMODULE hModule)
{
	if (!hModule)
		hModule = ::GetModuleHandle(NULL);

	char* moduleBase = (char*)hModule;

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

	for (; p < end && p->Name; p++)
	{
        IMAGE_THUNK_DATA* nameThunk = (IMAGE_THUNK_DATA*)(moduleBase + p->OriginalFirstThunk);
        IMAGE_THUNK_DATA* addrThunk = (IMAGE_THUNK_DATA*)(moduleBase + p->FirstThunk);

		const char* moduleName = moduleBase + p->Name;
		printf("%s\n", moduleName);

		for (; addrThunk->u1.Function; nameThunk++, addrThunk++)
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

    return 0;
}

//..............................................................................

} // namespace spy
} // namespace axl
