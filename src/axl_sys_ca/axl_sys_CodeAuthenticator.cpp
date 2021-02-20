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
#include "axl_sys_CodeAuthenticator.h"

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
	)
{
	sl::String_w programName;
	sl::String_w subjectName;
	sl::String_w issuerName;
	sl::Array<char> serialNumber;

	bool result = win::verifyAuthenticodeSignature(
		fileName,
		!expectedProgramName.isEmpty() ? &programName : NULL,
		!expectedSubjectName.isEmpty() ? &subjectName : NULL,
		!expectedIssuerName.isEmpty() ? &issuerName : NULL,
		!expectedSerialNumber.isEmpty() ? &serialNumber : NULL,
		NULL
		);

	if (!result)
		return false;

	if (!expectedProgramName.isEmpty() && expectedProgramName != programName)
		return err::fail("program name mismatch");

	if (!expectedSubjectName.isEmpty() && expectedSubjectName != subjectName)
		return err::fail("Authenticode signer mismatch");

	if (!expectedIssuerName.isEmpty() && expectedIssuerName != issuerName)
		return err::fail("Authenticode CA name mismatch");

	if (!expectedSerialNumber.isEmpty() &&
		(expectedSerialNumber.getCount() != serialNumber.getCount() ||
		memcmp(expectedSerialNumber, serialNumber, serialNumber.getCount()) != 0
		))
		return err::fail("Authenticode serial number mismatch");

	return true;
}

//..............................................................................

void
CodeAuthenticator::setup(
	const sl::StringRef& expectedProgramName,
	const sl::StringRef& expectedSubjectName,
	const sl::StringRef& expectedIssuerName,
	const sl::ArrayRef<char>& expectedSerialNumber
	)
{
	m_expectedProgramName = expectedProgramName;
	m_expectedSubjectName = expectedSubjectName;
	m_expectedIssuerName = expectedIssuerName;
	m_expectedSerialNumber = expectedSerialNumber;
}

#elif (_AXL_OS_LINUX)

#	error CodeAuthenticator for Linux is not implemented yet

#elif (_AXL_OS_DARWIN)

#	error CodeAuthenticator for Darwin is not implemented yet

#endif

//..............................................................................

} // namespace sys
} // namespace axl
