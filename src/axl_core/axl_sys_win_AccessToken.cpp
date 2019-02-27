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
#include "axl_sys_win_AccessToken.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool AccessToken::openProcessToken(
	handle_t process,
	dword_t access
	)
{
	close();

	bool_t result = ::OpenProcessToken(process, access, &m_h);
	return err::complete(result);
}

bool
AccessToken::openThreadToken(
	handle_t thread,
	dword_t access,
	bool openAsSelf
	)
{
	close();

	bool_t result = ::OpenThreadToken(thread, access, openAsSelf, &m_h);
	return err::complete(result);
}

bool
AccessToken::getTokenInformation(
	TOKEN_INFORMATION_CLASS infoClass,
	sl::Array<char>* buffer
	)
{
	dword_t size = 0;
	bool_t result = ::GetTokenInformation(m_h, infoClass, NULL, 0, &size);
	if (size == 0)
		return err::failWithLastSystemError();

	return
		buffer->setCount(size) &&
		getTokenInformation(infoClass, buffer->p(), size, &size);
}

bool
AccessToken::isMemberOf(PSID group)
{
	sl::Array<char> buffer;
	bool result = getTokenInformation(TokenGroups, &buffer);
	if (!result)
		return false;

	ASSERT(buffer.getCount() >= sizeof(TOKEN_GROUPS));

	const TOKEN_GROUPS* groups = (const TOKEN_GROUPS*) buffer.cp();
	for (size_t i = 0; i < groups->GroupCount; i++)
		if (::EqualSid(groups->Groups[i].Sid, group))
			return true;

	return false;
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
