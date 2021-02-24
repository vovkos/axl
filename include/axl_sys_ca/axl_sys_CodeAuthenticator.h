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

#include "axl_sys_CaPch.h"

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

namespace lnx {

//..............................................................................

class ElfHashGenerator
{
protected:
	sl::String m_signatureSectionName;

protected:
	bool
	generateHash(
		const void* elfBase,
		size_t elfSize,
		uchar_t hash[SHA_DIGEST_LENGTH],
		mem::Block* signatureSection = NULL
	);
};

//..............................................................................

} // namespace lnx

class CodeAuthenticator: lnx::ElfHashGenerator
{
protected:
	cry::Rsa m_publicKey;

public:
	CodeAuthenticator()
	{
	}

	CodeAuthenticator(
		sl::StringRef& signatureSectionName,
		const sl::StringRef& publicKeyPem
		)
	{
		setup(signatureSectionName, publicKeyPem);
	}

	bool
	setup(
		const sl::StringRef& signatureSectionName,
		const sl::StringRef& publicKeyPem
		);

	bool
	verifyFile(const sl::StringRef& fileName);
};

namespace lnx {

//..............................................................................

class ElfSignatureGenerator: public ElfHashGenerator
{
protected:
	cry::Rsa m_privateKey;

public:
	ElfSignatureGenerator()
	{
	}

	ElfSignatureGenerator(
		sl::StringRef& signatureSectionName,
		const sl::StringRef& privateKeyPem
		)
	{
		setup(signatureSectionName, privateKeyPem);
	}

	bool
	setup(
		const sl::StringRef& signatureSectionName,
		const sl::StringRef& privateKeyPem
		);

	size_t
	getSignatureSize()
	{
		return m_privateKey.getSize();
	}

	bool
	generateSignature(
		const sl::StringRef& fileName,
		sl::Array<char>* signature
		);
};

//..............................................................................

} // namespace lnx

#elif (_AXL_OS_DARWIN)

class CodeAuthenticator
{
protected:
	cf::String m_expectedProgramId;
	cf::String m_expectedTeamId;

public:
	CodeAuthenticator()
	{
	}

	CodeAuthenticator(const sl::StringRef& expectedIssuerName)
	{
		setup(expectedIssuerName);
	}

	CodeAuthenticator(
		const sl::StringRef& expectedProgramId,
		const sl::StringRef& expectedTeamId
		)
	{
		setup(expectedProgramId, expectedTeamId);
	}

	void
	setup(const sl::StringRef& expectedTeamId)
	{
		setup(sl::StringRef(), expectedTeamId);
	}

	void
	setup(
		const sl::StringRef& expectedProgramId,
		const sl::StringRef& expectedTeamId
		);

	bool
	verifyFile(const sl::StringRef& fileName);
};

#endif

//..............................................................................

} // namespace sys
} // namespace axl
