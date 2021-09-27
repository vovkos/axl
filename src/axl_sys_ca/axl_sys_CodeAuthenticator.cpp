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
#if (_AXL_OS_LINUX)
#	include "axl_sys_lnx_ElfParser.h"
#endif

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

void
CodeAuthenticator::setup(
	const sl::StringRef& expectedProgramName,
	const sl::StringRef& expectedSubjectName,
	const sl::StringRef& expectedIssuerName,
	const sl::ArrayRef<char>& expectedSerialNumber
) {
	m_expectedProgramName = expectedProgramName;
	m_expectedSubjectName = expectedSubjectName;
	m_expectedIssuerName = expectedIssuerName;
	m_expectedSerialNumber = expectedSerialNumber;
}

bool
CodeAuthenticator::verifyFile(const sl::StringRef& fileName) {
	sl::String_w programName;
	sl::String_w subjectName;
	sl::String_w issuerName;
	sl::Array<char> serialNumber;

	bool result = win::verifyAuthenticodeSignature(
		sl::String_w(fileName),
		!m_expectedProgramName.isEmpty() ? &programName : NULL,
		!m_expectedSubjectName.isEmpty() ? &subjectName : NULL,
		!m_expectedIssuerName.isEmpty() ? &issuerName : NULL,
		!m_expectedSerialNumber.isEmpty() ? &serialNumber : NULL,
		NULL
	);

	if (!result)
		return false;

	if (!m_expectedProgramName.isEmpty() && m_expectedProgramName != programName)
		return err::fail("program name mismatch");

	if (!m_expectedSubjectName.isEmpty() && m_expectedSubjectName != subjectName)
		return err::fail("Authenticode signer mismatch");

	if (!m_expectedIssuerName.isEmpty() && m_expectedIssuerName != issuerName)
		return err::fail("Authenticode CA name mismatch");

	if (!m_expectedSerialNumber.isEmpty() &&
		(m_expectedSerialNumber.getCount() != serialNumber.getCount() ||
		memcmp(m_expectedSerialNumber, serialNumber, serialNumber.getCount()) != 0
	))
		return err::fail("Authenticode serial number mismatch");

	return true;
}

#elif (_AXL_OS_LINUX)

namespace lnx {

//..............................................................................

bool
ElfHashGenerator::generateHash(
	const void* elfBase,
	size_t elfSize,
	uchar_t hash[SHA_DIGEST_LENGTH],
	mem::Block* signatureSection
) {
	lnx::ElfParser elf;

	bool result = elf.open(elfBase, elfSize);
	if (!result)
		return false;

	char* p = (char*)elfBase;
	SHA_CTX shaCtx;
	SHA1_Init(&shaCtx);

	const ElfW(Shdr)* shdr = elf.getSectionHdrTable();
	const ElfW(Shdr)* shdrEnd = shdr + elf.getElfHdr()->e_shnum;
	for (; shdr < shdrEnd; shdr++) {
		const char* name = elf.getString(shdr->sh_name);
		if (m_signatureSectionName != name)
			SHA1_Update(&shaCtx, p + shdr->sh_offset, shdr->sh_size);
		else if (signatureSection)
			signatureSection->setup(p + shdr->sh_offset, shdr->sh_size);
	}

	SHA1_Final(hash, &shaCtx);
	return true;
}

//..............................................................................

} // namespace lnx

bool
CodeAuthenticator::setup(
	const sl::StringRef& signatureSectionName,
	const sl::StringRef& publicKeyPem
) {
	m_signatureSectionName = signatureSectionName;
	return m_publicKey.readPublicKey(publicKeyPem);
}

bool
CodeAuthenticator::verifyFile(const sl::StringRef& fileName) {
	io::SimpleMappedFile file;
	uchar_t hash[SHA_DIGEST_LENGTH];
	mem::Block signatureSection;

	bool result =
		file.open(fileName, io::FileFlag_ReadOnly) &&
		generateHash(file.p(), file.getMappingSize(), hash, &signatureSection);

	if (!result)
		return false;

	if (!signatureSection.m_p)
		return err::fail("ELF-file signature not found");

	result = m_publicKey.verifyHash(
		NID_sha1,
		hash,
		sizeof(hash),
		signatureSection.m_p,
		signatureSection.m_size
	);

	if (!result)
		return err::fail("ELF-file signature mismatch");

	return true;
}

namespace lnx {

//..............................................................................

bool
ElfSignatureGenerator::setup(
	const sl::StringRef& signatureSectionName,
	const sl::StringRef& privateKeyPem
) {
	m_signatureSectionName = signatureSectionName;
	return m_privateKey.readPrivateKey(privateKeyPem);
}

bool
ElfSignatureGenerator::generateSignature(
	const sl::StringRef& fileName,
	sl::Array<char>* signature
) {
	io::SimpleMappedFile file;
	uchar_t hash[SHA_DIGEST_LENGTH];

	return
		file.open(fileName, io::FileFlag_ReadOnly) &&
		generateHash(file.p(), file.getMappingSize(), hash) &&
		m_privateKey.signHash(NID_sha1, signature, hash, sizeof(hash));
}

//..............................................................................

} // namespace lnx

#elif (_AXL_OS_DARWIN)

void
CodeAuthenticator::setup(
	const sl::StringRef& expectedProgramId,
	const sl::StringRef& expectedTeamId
) {
	if (!expectedProgramId.isEmpty())
		m_expectedProgramId = expectedProgramId;
	else
		m_expectedProgramId.clear();

	if (!expectedTeamId.isEmpty())
		m_expectedTeamId = expectedTeamId;
	else
		m_expectedTeamId.clear();
}

bool
CodeAuthenticator::verifyFile(const sl::StringRef& fileName) {
	sec::StaticCode code;
	cf::Dictionary dictionary;

	bool result =
		code.createWithPath(fileName, false) &&
		code.copySigningInformation(kSecCSSigningInformation, dictionary.p());

	if (!result)
		return false;

	CFStringRef programId = (CFStringRef)dictionary.getValue(kSecCodeInfoIdentifier);
	if (!programId)
		return err::fail("code is not signed");

	if (!m_expectedProgramId.isNull() && m_expectedProgramId.compare(programId))
		return err::fail("program identifier mismatch");

	CFStringRef teamId = (CFStringRef)dictionary.getValue(kSecCodeInfoTeamIdentifier);
	if (!m_expectedTeamId.isNull() && m_expectedTeamId.compare(teamId))
		return err::fail("team identifier mismatch");

	return true;
}

#endif

//..............................................................................

} // namespace sys
} // namespace axl
