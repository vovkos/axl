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

#define _AXL_SYS_CODEAUTHENTICATOR_H

#include "axl_sl_String.h"
#include "axl_sl_Array.h"

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

bool
verifyFileCodeAuthenticity(
	const sl::StringRef_w& fileName,
	const sl::StringRef_w& expectedProgramName,
	const sl::StringRef_w& expectedSubjectName,
	const sl::StringRef_w& expectedIssuerName,
	const sl::ArrayRef<char>& expectedSerialNumber
	);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CodeAuthenticator
{
protected:
	sl::String_w m_expectedProgramName;
	sl::String_w m_expectedSubjectName;
	sl::String_w m_expectedIssuerName;
	sl::Array<char> m_expectedSerialNumber;

public:
	CodeAuthenticator()
	{
	}

	CodeAuthenticator(const sl::StringRef& expectedIssuerName)
	{
		setup(expectedIssuerName);
	}

	CodeAuthenticator(
		const sl::StringRef& expectedProgramName,
		const sl::StringRef& expectedSubjectName,
		const sl::StringRef& expectedIssuerName,
		const sl::ArrayRef<char>& expectedSerialNumber
		)
	{
		setup(expectedProgramName, expectedSubjectName, expectedIssuerName, expectedSerialNumber);
	}

	void
	setup(const sl::StringRef& expectedIssuerName)
	{
		setup(sl::StringRef(), sl::StringRef(), expectedIssuerName, sl::ArrayRef<char>());
	}

	void
	setup(
		const sl::StringRef& expectedProgramName,
		const sl::StringRef& expectedSubjectName,
		const sl::StringRef& expectedIssuerName,
		const sl::ArrayRef<char>& expectedSerialNumber
		);

	bool
	verifyFile(const sl::StringRef& fileName)
	{
		return verifyFile(sl::String_w(fileName));
	}

	bool
	verifyFile(const sl::StringRef_w& fileName)
	{
		return verifyFileCodeAuthenticity(
			sl::String_w(fileName),
			m_expectedProgramName,
			m_expectedSubjectName,
			m_expectedIssuerName,
			m_expectedSerialNumber
			);
	}
};

#elif (_AXL_OS_LINUX)
#	error CodeAuthenticator for Linux is not implemented yet
#elif (_AXL_OS_DARWIN)
#	error CodeAuthenticator for Darwin is not implemented yet
#endif

//..............................................................................

} // namespace sys
} // namespace axl
