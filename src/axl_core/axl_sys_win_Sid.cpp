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
#include "axl_sys_win_Sid.h"
#include "axl_sys_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
Sid::create_va(
	SID_IDENTIFIER_AUTHORITY* identifierAuthority,
	size_t subAuthorityCount,
	axl_va_list va
) {
	size_t size = ::GetSidLengthRequired((uchar_t)subAuthorityCount);
	SID* sid = createBuffer(size, false);
	if (!sid)
		return false;

	bool_t result = ::InitializeSid(sid, identifierAuthority, (uchar_t)subAuthorityCount);
	if (!result)
		return err::failWithLastSystemError();

	dword_t* subAuthorityTable = ::GetSidSubAuthority(sid, 0);

	for (size_t i = 0; i < subAuthorityCount; i++)
		subAuthorityTable[i] = va.arg<dword_t> ();

	return true;
}

bool
Sid::parse(const sl::StringRef& string) {
	bool_t result;
	SID* sid;

	result = ::ConvertStringSidToSidA(string.sz(), (PSID*)&sid);
	if (!result)
		return err::failWithLastSystemError();

	result = copy(sid);
	::LocalFree(sid);
	return result != 0;
}

bool
Sid::parse(const sl::StringRef_w& string) {
	bool_t result;
	SID* sid;

	result = ::ConvertStringSidToSidW(string.sz(), (PSID*)&sid);
	if (!result)
		return err::failWithLastSystemError();

	result = copy(sid);
	::LocalFree(sid);
	return result != 0;
}

bool
Sid::getString(sl::String* string) const {
	if (!m_p) {
		string->clear();
		return true;
	}

	char* stringBuffer;
	bool_t result = ::ConvertSidToStringSidA(m_p, &stringBuffer);
	if (!result)
		return err::failWithLastSystemError();

	result = string->copy(stringBuffer);
	::LocalFree(stringBuffer);
	return result != 0;
}

bool
Sid::lookupAccountName(
	const sl::StringRef& systemName,
	const sl::StringRef& accountName,
	SID_NAME_USE* sidType
) {
	char sidBuffer[SECURITY_MAX_SID_SIZE];
	dword_t sidSize = sizeof(sidBuffer);
	char domainNameBuffer[MAX_PATH];
	dword_t domainNameLength = sizeof(domainNameBuffer);
	SID_NAME_USE dummySidType;

	bool_t result = ::LookupAccountNameA(
		systemName.szn(),
		accountName.szn(),
		(SID*)sidBuffer,
		&sidSize,
		domainNameBuffer,
		&domainNameLength,
		sidType ? sidType : &dummySidType
	);

	if (!result)
		return err::failWithLastSystemError();

	return copy((SID*)sidBuffer) != -1;
}

bool
Sid::lookupAccountName(
	const sl::StringRef_w& systemName,
	const sl::StringRef_w& accountName,
	SID_NAME_USE* sidType
) {
	char sidBuffer[SECURITY_MAX_SID_SIZE];
	dword_t sidSize = sizeof(sidBuffer);
	wchar_t domainNameBuffer[MAX_PATH];
	dword_t domainNameLength = sizeof(domainNameBuffer) / sizeof(wchar_t);
	SID_NAME_USE dummySidType;

	bool_t result = ::LookupAccountNameW(
		systemName.szn(),
		accountName.szn(),
		(SID*)sidBuffer,
		&sidSize,
		domainNameBuffer,
		&domainNameLength,
		sidType ? sidType : &dummySidType
	);

	if (!result)
		return err::failWithLastSystemError();

	return copy((SID*)sidBuffer) != -1;
}

bool
Sid::lookupAccountSid(
	const sl::StringRef& systemName,
	const SID* sid,
	sl::String* accountName,
	SID_NAME_USE* sidType
) {
	char accountNameBuffer[MAX_PATH];
	dword_t accountNameLength = sizeof(accountNameBuffer);
	char domainNameBuffer[MAX_PATH];
	dword_t domainNameLength = sizeof(domainNameBuffer);
	SID_NAME_USE dummySidType;

	if (!sid)
		return err::fail(err::SystemErrorCode_InvalidParameter);

	bool_t result = ::LookupAccountSidA(
		systemName.szn(),
		(SID*)sid,
		accountNameBuffer,
		&accountNameLength,
		domainNameBuffer,
		&domainNameLength,
		sidType ? sidType : &dummySidType
	);

	if (!result)
		return err::failWithLastSystemError();

	accountName->copy(accountNameBuffer, accountNameLength);
	return true;
}
//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
