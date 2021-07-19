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
#include "axl_sys_ExceptionError.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_NtStatus.h"
#endif

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

size_t
setWinExceptionError(
	uint32_t ntstatus,
	uintptr_t codeAddress,
	const uintptr_t* paramTable,
	size_t paramCount
	)
{
	enum
	{
		Size = sizeof(err::ErrorHdr) + sizeof(WinExceptionInfo)
	};

	err::Error error;
	err::ErrorHdr* hdr = error.createBuffer(Size);
	WinExceptionInfo* exceptionInfo = (WinExceptionInfo*)(hdr + 1);

	if (paramCount > countof(exceptionInfo->m_paramTable))
		paramCount = countof(exceptionInfo->m_paramTable);

	hdr->m_guid = g_exceptionErrorGuid;
	hdr->m_code = ExceptionErrorCode_WinException;
	hdr->m_size = Size;

	exceptionInfo->m_ntstatus = ntstatus;
	exceptionInfo->m_codeAddress = codeAddress;
	exceptionInfo->m_paramCount = paramCount;
	memcpy(exceptionInfo->m_paramTable, paramTable, paramCount * sizeof(uintptr_t));

	return err::setError(error);
}

#elif (_AXL_OS_POSIX)

size_t
setPosixSignalError(
	uint32_t signal,
	uint32_t code,
	uintptr_t codeAddress,
	uintptr_t faultAddress
	)
{
	enum
	{
		Size = sizeof(err::ErrorHdr) + sizeof(PosixSignalInfo)
	};

	err::Error error;
	err::ErrorHdr* hdr = error.createBuffer(Size);
	PosixSignalInfo* signalInfo = (PosixSignalInfo*)(hdr + 1);

	hdr->m_guid = g_exceptionErrorGuid;
	hdr->m_code = ExceptionErrorCode_PosixSignal;
	hdr->m_size = Size;

	signalInfo->m_signal = signal;
	signalInfo->m_code = code;
	signalInfo->m_codeAddress = codeAddress;
	signalInfo->m_faultAddress = faultAddress;

	return err::setError(error);
}

#endif

//..............................................................................

template <typename T>
sl::StringRef
getWinExceptionDescription(const T* exceptionInfo)
{
	const char* pointerFormat = sizeof(exceptionInfo->m_codeAddress) == 8 ? "0x%016llx" : "0x%08x";

	sl::String string;

#if (_AXL_OS_WIN)
	string = sys::win::NtStatusProvider::getErrorDescription(exceptionInfo->m_ntstatus);
	string.trimRight();
	if (string.isSuffix('.'))
		string.chop(1);
#else
	string.format("NTSTATUS-%08x", exceptionInfo->m_ntstatus);
#endif

	string += " by ";
	string.appendFormat(pointerFormat, exceptionInfo->m_codeAddress);

	// STATUS_ACCESS_VIOLATION or STATUS_IN_PAGE_ERROR

	if (exceptionInfo->m_ntstatus == 0xc0000005 ||
		exceptionInfo->m_ntstatus == 0xc0000006)
	{
		uintptr_t op = exceptionInfo->m_paramTable[0];
		uintptr_t faultAddress = exceptionInfo->m_paramTable[1];

		string += " (";

		switch (op)
		{
		case 0:
			string += "read at ";
			break;

		case 1:
			string += "write at ";
			break;

		case 8:
			string += "execute at ";
			break;

		default:
			string.appendFormat("op-%d at ", op);
		}

		if (!faultAddress)
		{
			string += "null)";
		}
		else
		{
			string.appendFormat(pointerFormat, faultAddress);
			string += ")";
		}
	}

	return string;
}

sl::StringRef
getLinuxSignalName(int signal)
{
	const char* nameTable[32] =
	{
		"SIGNULL",   // 0
		"SIGHUP",    // 1
		"SIGINT",    // 2
		"SIGQUIT",   // 3
		"SIGILL",    // 4
		"SIGTRAP",   // 5
		"SIGABRT",   // 6
		"SIGBUS",    // 7
		"SIGFPE",    // 8
		"SIGKILL",   // 9
		"SIGUSR1",   // 10
		"SIGSEGV",   // 11
		"SIGUSR2",   // 12
		"SIGPIPE",   // 13
		"SIGALRM",   // 14
		"SIGTERM",   // 15
		"SIGSTKFLT", // 16
		"SIGCHLD",   // 17
		"SIGCONT",   // 18
		"SIGSTOP",   // 19
		"SIGTSTP",   // 20
		"SIGTTIN",   // 21
		"SIGTTOU",   // 22
		"SIGURG",    // 23
		"SIGXCPU",   // 24
		"SIGXFSZ",   // 25
		"SIGVTALRM", // 26
		"SIGPROF",   // 27
		"SIGWINCH",  // 28
		"SIGIO",     // 29
		"SIGPWR",    // 30
		"SIGSYS",    // 31
	};

	if ((size_t)signal < countof(nameTable))
		return nameTable[signal];

	return sl::formatString("SIG%d", signal);
}

sl::StringRef
getDarwinSignalName(int signal)
{
	const char* nameTable[32] =
	{
		"SIGNULL",   // 0
		"SIGHUP",    // 1
		"SIGINT",    // 2
		"SIGQUIT",   // 3
		"SIGILL",    // 4
		"SIGTRAP",   // 5
		"SIGABRT",   // 6
		"SIGEMT",    // 7
		"SIGFPE",    // 8
		"SIGKILL",   // 9
		"SIGBUS",    // 10
		"SIGSEGV",   // 11
		"SIGSYS",    // 12
		"SIGPIPE",   // 13
		"SIGALRM",   // 14
		"SIGTERM",   // 15
		"SIGURG",    // 16
		"SIGSTOP",   // 17
		"SIGTSTP",   // 18
		"SIGCONT",   // 19
		"SIGCHLD",   // 20
		"SIGTTIN",   // 21
		"SIGTTOU",   // 22
		"SIGIO",     // 23
		"SIGXCPU",   // 24
		"SIGXFSZ",   // 25
		"SIGVTALRM", // 26
		"SIGPROF",   // 27
		"SIGWINCH",  // 28
		"SIGINFO",   // 29
		"SIGUSR1",   // 30
		"SIGUSR2",   // 31
	};

	if ((size_t)signal < countof(nameTable))
		return nameTable[signal];

	return sl::formatString("SIG%d", signal);
}

template <typename T>
sl::StringRef
getPosixSignalDescription(
	const T* signalInfo,
	const sl::StringRef& signalName,
	bool hasFaultAddress
	)
{
	const char* pointerFormat = sizeof(signalInfo->m_codeAddress) == 8 ? "0x%016llx" : "0x%08x";

	sl::String string = signalName;
	string += " by ";
	string.appendFormat(pointerFormat, signalInfo->m_codeAddress);

	if (hasFaultAddress)
		if (!signalInfo->m_faultAddress)
		{
			string += " (null pointer access)";
		}
		else
		{
			string += " (access ";
			string.appendFormat(pointerFormat, signalInfo->m_faultAddress);
			string += ")";
		}

	return string;
}

template <typename T>
sl::StringRef
getLinuxSignalDescription(const T* signalInfo)
{
	return getPosixSignalDescription(
		signalInfo,
		getLinuxSignalName(signalInfo->m_signal),
		signalInfo->m_signal == 7 || signalInfo->m_signal == 11 // SIGSEGV or SIGBUS
		);
}

template <typename T>
sl::StringRef
getDarwinSignalDescription(const T* signalInfo)
{
	return getPosixSignalDescription(
		signalInfo,
		getDarwinSignalName(signalInfo->m_signal),
		signalInfo->m_signal == 10 || signalInfo->m_signal == 11 // SIGSEGV or SIGBUS
		);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::StringRef
ExceptionErrorProvider::getErrorDescription(const err::ErrorRef& error)
{
	switch (error->m_code)
	{
	case ExceptionErrorCode_WinException32:
		return getWinExceptionDescription((const WinExceptionInfo32*)(error.cp() + 1));

	case ExceptionErrorCode_WinException64:
		return getWinExceptionDescription((const WinExceptionInfo64*)(error.cp() + 1));

	case ExceptionErrorCode_LinuxSignal32:
		return getLinuxSignalDescription((const PosixSignalInfo32*)(error.cp() + 1));

	case ExceptionErrorCode_LinuxSignal64:
		return getLinuxSignalDescription((const PosixSignalInfo64*)(error.cp() + 1));

	case ExceptionErrorCode_DarwinSignal32:
		return getDarwinSignalDescription((const PosixSignalInfo32*)(error.cp() + 1));

	case ExceptionErrorCode_DarwinSignal64:
		return getDarwinSignalDescription((const PosixSignalInfo64*)(error.cp() + 1));

	default:
		return "unknown exception";
	}
}

//..............................................................................

} // namespace sys
} // namespace axl
