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

#define _AXL_SYS_WIN_ACCESSTOKEN_H

#include "axl_sys_win_Handle.h"
#include "axl_err_Error.h"
#include "axl_sl_Array.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class AccessToken: public Handle
{
public:
	bool
	openProcessToken(
		handle_t process,
		dword_t access = TOKEN_ALL_ACCESS
		);

	bool
	openProcessToken(dword_t access = TOKEN_ALL_ACCESS)
	{
		return openProcessToken(::GetCurrentProcess(), access);
	}

	bool
	openThreadToken(
		handle_t thread,
		dword_t access = TOKEN_ALL_ACCESS,
		bool openAsSelf = false
		);

	bool
	openThreadToken(
		dword_t access = TOKEN_ALL_ACCESS,
		bool openAsSelf = false
		)
	{
		return openThreadToken(::GetCurrentThread(), access);
	}

	bool
	getTokenInformation(
		TOKEN_INFORMATION_CLASS infoClass,
		void* p,
		dword_t size,
		dword_t* actualSize
		)
	{
		bool_t result = ::GetTokenInformation(m_h, infoClass, p, size, actualSize);
		return err::complete(result);
	}

	bool
	getTokenInformation(
		TOKEN_INFORMATION_CLASS infoClass,
		sl::Array<char>* buffer
		);

	sl::Array<char>
	getTokenInformation(TOKEN_INFORMATION_CLASS infoClass)
	{
		sl::Array<char> buffer;
		getTokenInformation(infoClass, &buffer);
		return buffer;
	}

	bool
	setTokenInformation(
		TOKEN_INFORMATION_CLASS infoClass,
		void* p,
		dword_t size
		)
	{
		bool_t result = ::SetTokenInformation(m_h, infoClass, p, size);
		return err::complete(result);
	}

	bool
	isMemberOf(PSID group);
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
