//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_sys_win_NtDll.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

void
initNtDllFunctions()
{
	HMODULE hNtDll = ::GetModuleHandleW(L"ntdll.dll");
	ASSERT(hNtDll);

	ntQueryDirectoryFile = (NtQueryDirectoryFileFunc*)::GetProcAddress(hNtDll, "NtQueryDirectoryFile");
	ntOpenDirectoryObject = (NtOpenDirectoryObjectFunc*)::GetProcAddress(hNtDll, "NtOpenDirectoryObject");
	ntQueryDirectoryObject = (NtQueryDirectoryObjectFunc*)::GetProcAddress(hNtDll, "NtQueryDirectoryObject");
	ntOpenSymbolicLinkObject = (NtOpenSymbolicLinkObjectFunc*)::GetProcAddress(hNtDll, "NtOpenSymbolicLinkObject");
	ntQuerySymbolicLinkObject = (NtQuerySymbolicLinkObjectFunc*)::GetProcAddress(hNtDll, "NtQuerySymbolicLinkObject");
	ntQueryInformationFile = (NtQueryInformationFileFunc*)::GetProcAddress(hNtDll, "NtQueryInformationFile");
	ntQueryInformationProcess = (NtQueryInformationProcessFunc*)::GetProcAddress(hNtDll, "NtQueryInformationProcess");
	ntQueryObject = (NtQueryObjectFunc*)::GetProcAddress(hNtDll, "NtQueryObject");

	ASSERT(ntQueryDirectoryFile);
	ASSERT(ntOpenDirectoryObject);
	ASSERT(ntQueryDirectoryObject);
	ASSERT(ntOpenSymbolicLinkObject);
	ASSERT(ntQuerySymbolicLinkObject);
	ASSERT(ntQueryInformationFile);
	ASSERT(ntQueryInformationProcess);
	ASSERT(ntQueryObject);
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
