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

#pragma once

#include "axl_err_ErrorMgr.h"

namespace axl {
namespace sys {

//..............................................................................

// {D33DAEEE-CD49-4A4C-9FEB-9026D0F09D75}
AXL_SL_DEFINE_GUID(g_exceptionErrorGuid, 0xd33daeee, 0xcd49, 0x4a4c, 0x9f, 0xeb, 0x90, 0x26, 0xd0, 0xf0, 0x9d, 0x75);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ExceptionErrorCode {
	ExceptionErrorCode_WinException32,
	ExceptionErrorCode_WinException64,
	ExceptionErrorCode_LinuxSignal32,
	ExceptionErrorCode_LinuxSignal64,
	ExceptionErrorCode_DarwinSignal32,
	ExceptionErrorCode_DarwinSignal64,

#if (_AXL_OS_LINUX)
	ExceptionErrorCode_PosixSignal32 = ExceptionErrorCode_LinuxSignal32,
	ExceptionErrorCode_PosixSignal64 = ExceptionErrorCode_LinuxSignal64,
#elif (_AXL_OS_DARWIN)
	ExceptionErrorCode_PosixSignal32 = ExceptionErrorCode_DarwinSignal32,
	ExceptionErrorCode_PosixSignal64 = ExceptionErrorCode_DarwinSignal64,
#endif

#if (_AXL_OS_WIN)
#	if (AXL_PTR_SIZE == 4)
	ExceptionErrorCode_WinException = ExceptionErrorCode_WinException32,
#	elif (AXL_PTR_SIZE == 8)
	ExceptionErrorCode_WinException = ExceptionErrorCode_WinException64,
#	endif
#elif (_AXL_OS_POSIX)
#	if (AXL_PTR_SIZE == 4)
	ExceptionErrorCode_PosixSignal  = ExceptionErrorCode_PosixSignal32,
#	elif (AXL_PTR_SIZE == 8)
	ExceptionErrorCode_PosixSignal  = ExceptionErrorCode_PosixSignal64,
#	endif
#endif
};

//..............................................................................

template <typename T>
struct WinExceptionInfoBase {
	uint32_t m_ntstatus;
	uint32_t m_paramCount;
	T m_codeAddress;
	T m_paramTable[3]; // EXCEPTION_MAXIMUM_PARAMETERS is 15, but no more than 3 is ever used
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef WinExceptionInfoBase<uintptr_t> WinExceptionInfo;
typedef WinExceptionInfoBase<uint32_t> WinExceptionInfo32;
typedef WinExceptionInfoBase<uint64_t> WinExceptionInfo64;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
struct PosixSignalInfoBase {
	uint32_t m_signal;
	uint32_t m_code;
	T m_codeAddress;
	T m_faultAddress;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef PosixSignalInfoBase<uintptr_t> PosixSignalInfo;
typedef PosixSignalInfoBase<uint32_t> PosixSignalInfo32;
typedef PosixSignalInfoBase<uint64_t> PosixSignalInfo64;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (_AXL_OS_WIN)
size_t
setWinExceptionError(
	uint32_t ntstatus,
	uintptr_t codeAddress,
	const uintptr_t* paramTable = NULL,
	size_t paramCount = 0
);
#elif (_AXL_OS_POSIX)
size_t
setPosixSignalError(
	uint32_t signal,
	uint32_t code,
	uintptr_t codeAddress,
	uintptr_t faultAddress
);
#endif

//..............................................................................

class ExceptionErrorProvider: public err::ErrorProvider {
public:
	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
registerExceptionErrorProvider() {
	err::getErrorMgr()->registerProvider(
		g_exceptionErrorGuid,
		sl::getSimpleSingleton<ExceptionErrorProvider> ()
	);
}

//..............................................................................

} // namespace sys
} // namespace axl
