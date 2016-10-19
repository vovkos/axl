#include "pch.h"
#include "axl_sys_win_Process.h"
#include "axl_io_win_File.h"
#include "axl_sl_String.h"
#include "axl_sl_Array.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
Process::createProcess (
	const sl::StringRef_w& appName,
	const sl::StringRef_w& cmdLine,
	const SECURITY_ATTRIBUTES* processAttr,
	const SECURITY_ATTRIBUTES* threadAttr,
	bool inheritHandles,
	dword_t flags,
	const void* environment,
	const sl::StringRef_w& currentDir,
	const STARTUPINFO* startupInfo,
	handle_t* threadHandle
	)
{
	bool_t result;

	close ();

	sl::String_w mutableCmdLine = cmdLine;
	PROCESS_INFORMATION processInfo;

	result = ::CreateProcessW (
		appName.szn (),
		mutableCmdLine.getBuffer (),
		(SECURITY_ATTRIBUTES*) processAttr,
		(SECURITY_ATTRIBUTES*) threadAttr,
		inheritHandles,
		flags,
		(void*) environment,
		currentDir.szn (),
		(STARTUPINFO*) startupInfo,
		&processInfo
		);

	if (!result)
		return err::failWithLastSystemError ();

	if (threadHandle)
		*threadHandle = processInfo.hThread;
	else
		::CloseHandle (processInfo.hThread);

	m_h = processInfo.hProcess;
	return true;
}

//..............................................................................

bool
syncExec (
	const sl::StringRef_w& cmdLine,
	sl::Array <char>* output,
	dword_t* exitCode
	)
{
	bool_t result;

	SECURITY_ATTRIBUTES secAttr = { 0 };
	secAttr.nLength = sizeof (SECURITY_ATTRIBUTES);
	secAttr.bInheritHandle = true;

	io::win::File childStdOut;
	io::win::File childStdIn;
	io::win::File parentStdOut;
	io::win::File parentStdIn;

	result =
		::CreatePipe (parentStdOut.p (), childStdOut.p (), &secAttr, 0) &&
		::CreatePipe (childStdIn.p (), parentStdIn.p (), &secAttr, 0);

	if (!result)
		return err::failWithLastSystemError ();

	STARTUPINFO startupInfo = { 0 };
	startupInfo.cb = sizeof (STARTUPINFO);
	startupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	startupInfo.hStdOutput = childStdOut;
	startupInfo.hStdInput  = childStdIn;
	startupInfo.hStdError  = childStdOut;
	startupInfo.wShowWindow = SW_HIDE;

	Process process;
	result = process.createProcess (cmdLine, true, CREATE_NEW_CONSOLE, &startupInfo);
	if (!result)
		return false;

	childStdOut.close ();
	childStdIn.close ();

	output->clear ();

	for (;;)
	{
		char buffer [1024];

		size_t size = parentStdOut.read (buffer, countof (buffer));
		if (!size)
			break;

		if (size == -1)
		{
			if (err::getLastError ()->m_code == ERROR_BROKEN_PIPE)
				break; // pipe done - normal exit path

			return false;
		}

		output->append (buffer, size);
	}

	if (exitCode)
		process.getExitCode (exitCode);

	return true;
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
