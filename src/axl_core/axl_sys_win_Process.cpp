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
#include "axl_sys_win_Process.h"
#include "axl_sys_win_NtDll.h"
#include "axl_sys_win_NtStatus.h"
#include "axl_io_win_File.h"
#include "axl_sl_String.h"
#include "axl_sl_Array.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
Process::create(
	const sl::StringRef_w& appName,
	const sl::StringRef_w& cmdLine,
	const SECURITY_ATTRIBUTES* processAttr,
	const SECURITY_ATTRIBUTES* threadAttr,
	bool inheritHandles,
	dword_t flags,
	const void* environment,
	const sl::StringRef_w& currentDir,
	const STARTUPINFOW* startupInfo,
	handle_t* threadHandle
) {
	bool_t result;

	close();

	sl::String_w mutableCmdLine = cmdLine;
	PROCESS_INFORMATION processInfo;

	result = ::CreateProcessW(
		appName.szn(),
		mutableCmdLine.getBuffer(),
		(SECURITY_ATTRIBUTES*)processAttr,
		(SECURITY_ATTRIBUTES*)threadAttr,
		inheritHandles,
		flags,
		(void*)environment,
		currentDir.szn(),
		(STARTUPINFOW*)startupInfo,
		&processInfo
	);

	if (!result)
		return err::failWithLastSystemError();

	if (threadHandle)
		*threadHandle = processInfo.hThread;
	else
		::CloseHandle(processInfo.hThread);

	m_h = processInfo.hProcess;
	return true;
}

//..............................................................................

bool
syncExec(
	const sl::StringRef_w& cmdLine,
	sl::Array<char>* output,
	dword_t* exitCode
) {
	bool_t result;

	SECURITY_ATTRIBUTES secAttr = { 0 };
	secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	secAttr.bInheritHandle = true;

	io::win::File childStdOut;
	io::win::File childStdIn;
	io::win::File parentStdOut;
	io::win::File parentStdIn;

	result =
		::CreatePipe(parentStdOut.p(), childStdOut.p(), &secAttr, 0) &&
		::CreatePipe(childStdIn.p(), parentStdIn.p(), &secAttr, 0);

	if (!result)
		return err::failWithLastSystemError();

	STARTUPINFOW startupInfo = { 0 };
	startupInfo.cb = sizeof(STARTUPINFO);
	startupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	startupInfo.hStdOutput = childStdOut;
	startupInfo.hStdInput = childStdIn;
	startupInfo.hStdError = childStdOut;
	startupInfo.wShowWindow = SW_HIDE;

	Process process;
	result = process.create(cmdLine, true, CREATE_NEW_CONSOLE, &startupInfo);
	if (!result)
		return false;

	childStdOut.close();
	childStdIn.close();

	output->clear();

	for (;;) {
		char buffer[1024];

		size_t size = parentStdOut.read(buffer, countof(buffer));
		if (!size)
			break;

		if (size == -1) {
			if (err::getLastError()->m_code == ERROR_BROKEN_PIPE)
				break; // pipe done - normal exit path

			return false;
		}

		output->append(buffer, size);
	}

	if (exitCode)
		process.getExitCode(exitCode);

	return true;
}

sl::StringRef_w
getProcessImageName(dword_t pid) {
	if (pid == 0)
		return L"System Idle Process";
	else if (pid == 4)
		return L"System";

	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (!hProcess) {
		if (::GetLastError() == ERROR_ACCESS_DENIED)
			hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);

		if (!hProcess) {
			err::setLastSystemError();
			return NULL;
		}
	}

	char stackBuffer[512];
	sl::Array<char> heapBuffer;

	ulong_t length;
	UNICODE_STRING* name = (UNICODE_STRING*)stackBuffer;

	NTSTATUS status = ntQueryInformationProcess(
		hProcess,
		ProcessImageFileName,
		name,
		sizeof(stackBuffer),
		&length
	);

	if (status == STATUS_INFO_LENGTH_MISMATCH) {
		bool result = heapBuffer.setCount(length);
		if (!result)
			return NULL;

		name = (UNICODE_STRING*)heapBuffer.p();

		status = ntQueryInformationProcess(
			hProcess,
			ProcessImageFileName,
			name,
			length,
			&length
		);
	}

	if (!NT_SUCCESS(status)) {
		err::setError(NtStatus(status));
		return NULL;
	}

	length = name->Length / sizeof(wchar_t);
	return sl::String_w(name->Buffer, length);
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
