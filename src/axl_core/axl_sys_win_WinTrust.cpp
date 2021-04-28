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
#include "axl_sys_win_WinTrust.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
verifyTrustFile(const sl::StringRef& fileName)
{
	char buffer[256];
	sl::String_w fileName_w(rc::BufKind_Stack, buffer, sizeof(buffer));
	return verifyTrustFile(fileName_w);
}

bool
verifyTrustFile(const sl::StringRef_w& fileName)
{
	WINTRUST_FILE_INFO fileInfo = { 0 };
	fileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
	fileInfo.pcwszFilePath = fileName.sz();

	WINTRUST_DATA winTrustData = { 0 };
	winTrustData.cbStruct = sizeof(winTrustData);
	winTrustData.dwUIChoice = WTD_UI_NONE;
	winTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
	winTrustData.dwUnionChoice = WTD_CHOICE_FILE;
	winTrustData.dwStateAction = WTD_STATEACTION_VERIFY;
	winTrustData.pFile = &fileInfo;

	GUID actionGuid = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	long status = ::WinVerifyTrust((HWND)INVALID_HANDLE_VALUE, &actionGuid, &winTrustData);
	bool result = status == ERROR_SUCCESS;
	if (!result)
		err::setError(status == TRUST_E_NOSIGNATURE ? err::getLastSystemErrorCode() : status);

	winTrustData.dwStateAction = WTD_STATEACTION_CLOSE;
	::WinVerifyTrust(NULL, &actionGuid, &winTrustData);
	return result;
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
