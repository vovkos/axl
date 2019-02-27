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

#define _AXL_SYS_WIN_SID_H

#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

AXL_SELECT_ANY SID_IDENTIFIER_AUTHORITY g_nullSidAuthority      = SECURITY_NULL_SID_AUTHORITY;
AXL_SELECT_ANY SID_IDENTIFIER_AUTHORITY g_worldSidAuthority     = SECURITY_WORLD_SID_AUTHORITY;
AXL_SELECT_ANY SID_IDENTIFIER_AUTHORITY g_localSidAuthority     = SECURITY_LOCAL_SID_AUTHORITY;
AXL_SELECT_ANY SID_IDENTIFIER_AUTHORITY g_creatorSidAuthority   = SECURITY_CREATOR_SID_AUTHORITY;
AXL_SELECT_ANY SID_IDENTIFIER_AUTHORITY g_nonUniqueSidAuthority = SECURITY_NON_UNIQUE_AUTHORITY;
AXL_SELECT_ANY SID_IDENTIFIER_AUTHORITY g_resMgrSidAuthority    = SECURITY_RESOURCE_MANAGER_AUTHORITY;
AXL_SELECT_ANY SID_IDENTIFIER_AUTHORITY g_ntSidAuthority        = SECURITY_NT_AUTHORITY;

AXL_SELECT_ANY SID g_worldSid        = { SID_REVISION, 1, SECURITY_WORLD_SID_AUTHORITY, SECURITY_WORLD_RID };
AXL_SELECT_ANY SID g_localSid        = { SID_REVISION, 1, SECURITY_LOCAL_SID_AUTHORITY, SECURITY_LOCAL_RID };
AXL_SELECT_ANY SID g_creatorOwnerSid = { SID_REVISION, 1, SECURITY_CREATOR_SID_AUTHORITY, SECURITY_CREATOR_OWNER_RID };
AXL_SELECT_ANY SID g_creatorGroupSid = { SID_REVISION, 1, SECURITY_CREATOR_SID_AUTHORITY, SECURITY_CREATOR_GROUP_RID };

AXL_SELECT_ANY SID g_dialupSid         = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_DIALUP_RID };
AXL_SELECT_ANY SID g_networkSid        = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_NETWORK_RID };
AXL_SELECT_ANY SID g_batchSid          = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_BATCH_RID };
AXL_SELECT_ANY SID g_interactiveSid    = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_INTERACTIVE_RID };
AXL_SELECT_ANY SID g_serviceSid        = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_SERVICE_RID };
AXL_SELECT_ANY SID g_anonymousSid      = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_ANONYMOUS_LOGON_RID };
AXL_SELECT_ANY SID g_proxySid          = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_PROXY_RID };
AXL_SELECT_ANY SID g_enterpriseSid     = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_ENTERPRISE_CONTROLLERS_RID };
AXL_SELECT_ANY SID g_selfSid           = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_PRINCIPAL_SELF_RID };
AXL_SELECT_ANY SID g_authenticatedSid  = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_AUTHENTICATED_USER_RID };
AXL_SELECT_ANY SID g_terminalServerSid = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_TERMINAL_SERVER_RID };
AXL_SELECT_ANY SID g_systemSid         = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_LOCAL_SYSTEM_RID };
AXL_SELECT_ANY SID g_localServiceSid   = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_LOCAL_SERVICE_RID };
AXL_SELECT_ANY SID g_networkServiceSid = { SID_REVISION, 1, SECURITY_NT_AUTHORITY, SECURITY_NETWORK_SERVICE_RID };

//..............................................................................

class SizeOfSid
{
public:
	size_t
	operator () (const SID* sid)
	{
		return ::GetLengthSid((PSID)sid);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Sid: public ref::Buf<SID, SizeOfSid>
{
public:
	Sid()
	{
	}

	Sid(const SID* sid)
	{
		copy(sid);
	}

	Sid(
		SID_IDENTIFIER_AUTHORITY* identifierAuthority,
		size_t subAuthorityCount,
		...
		)
	{
		AXL_VA_DECL(va, subAuthorityCount);
		create_va(identifierAuthority, subAuthorityCount, va);
	}

	Sid&
	operator = (const Sid& src)
	{
		copy(src);
		return *this;
	}

	Sid&
	operator = (const SID* sid)
	{
		copy(sid);
		return *this;
	}

	bool
	create(
		SID_IDENTIFIER_AUTHORITY* identifierAuthority,
		size_t subAuthorityCount,
		...
		)
	{
		AXL_VA_DECL(va, subAuthorityCount);
		return create_va(identifierAuthority, subAuthorityCount, va);
	}

	bool
	create_va(
		SID_IDENTIFIER_AUTHORITY* identifierAuthority,
		size_t subAuthorityCount,
		axl_va_list va
		);

	bool
	parse(const sl::StringRef& string);

	bool
	parse(const sl::StringRef_w& string);

	bool
	getString(sl::String* string) const;

	sl::String
	getString() const
	{
		sl::String string;
		getString(&string);
		return string;
	}

	bool
	lookupAccountName(
		const sl::StringRef& systemName,
		const sl::StringRef& accountName,
		SID_NAME_USE* sidType = NULL
		);

	bool
	lookupAccountName(
		const sl::StringRef_w& systemName,
		const sl::StringRef_w& accountName,
		SID_NAME_USE* sidType = NULL
		);

	bool
	lookupAccountName(
		const sl::StringRef& accountName,
		SID_NAME_USE* sidType = NULL
		)
	{
		return lookupAccountName(NULL, accountName, sidType);
	}

	bool
	lookupAccountName(
		const sl::StringRef_w& accountName,
		SID_NAME_USE* sidType = NULL
		)
	{
		return lookupAccountName(NULL, accountName, sidType);
	}

	bool
	lookupAccountSid(
		const sl::StringRef& systemName,
		sl::String* accountName,
		SID_NAME_USE* sidType = NULL
		)
	{
		return lookupAccountSid(systemName, m_p, accountName, sidType);
	}

	bool
	lookupAccountSid(
		sl::String* accountName,
		SID_NAME_USE* sidType = NULL
		)
	{
		return lookupAccountSid(NULL, m_p, accountName, sidType);
	}

	sl::String
	lookupAccountSid(
		const sl::StringRef& systemName,
		SID_NAME_USE* sidType = NULL
		)
	{
		sl::String accountName;
		lookupAccountSid(systemName, m_p, &accountName, sidType);
		return accountName;
	}

	sl::String
	lookupAccountSid(SID_NAME_USE* sidType = NULL)
	{
		sl::String accountName;
		lookupAccountSid(NULL, m_p, &accountName, sidType);
		return accountName;
	}

	static
	bool
	lookupAccountSid(
		const sl::StringRef& systemName,
		const SID* sid,
		sl::String* accountName,
		SID_NAME_USE* sidType = NULL
		);

	static
	bool
	lookupAccountSid(
		const SID* sid,
		sl::String* accountName,
		SID_NAME_USE* sidType = NULL
		)
	{
		return lookupAccountSid(NULL, sid, accountName, sidType);
	}

	static
	sl::String
	lookupAccountSid(
		const sl::StringRef& systemName,
		const SID* sid,
		SID_NAME_USE* sidType = NULL
		)
	{
		sl::String accountName;
		lookupAccountSid(systemName, sid, &accountName, sidType);
		return accountName;
	}

	static
	sl::String
	lookupAccountSid(
		const SID* sid,
		SID_NAME_USE* sidType = NULL
		)
	{
		return lookupAccountSid(NULL, sid, sidType);
	}
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
