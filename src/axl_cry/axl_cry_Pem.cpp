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
#include "axl_cry_Pem.h"
#include "axl_cry_Bio.h"
#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//..............................................................................

bool
encodePem(
	sl::String* buffer,
	const sl::StringRef& name,
	const sl::StringRef& header,
	const void* p,
	size_t size
) {
	cry::Bio bio;
	bio.createMem();

	int result = ::PEM_write_bio(bio, name.sz(), header.sz(), (uchar_t*)p, size);
	if (!result)
		return failWithLastCryptoError();

	BUF_MEM* mem = bio.getBufMem();
	return !buffer || buffer->copy((char*)mem->data, mem->length) != -1;
}

bool
decodePem(
	sl::String* name,
	sl::String* header,
	sl::Array<char>* buffer,
	const sl::StringRef& pem
) {
	cry::Bio bio;
	bio.createMemBuf(pem.cp(), pem.getLength());

	char* nameSz;
	char* headerSz;
	uchar_t* p;
	long size;

	int result = ::PEM_read_bio(bio, &nameSz, &headerSz, &p, &size);

	if (!result)
		return failWithLastCryptoError();

	result =
		(!name || name->copy(nameSz) != -1) &&
		(!header || header->copy(headerSz) != -1) &&
		buffer->copy((char*)p, size) != -1;

	::OPENSSL_free(nameSz);
	::OPENSSL_free(headerSz);
	::OPENSSL_free(p);

	return result != 0;
}

bool
decodePem(
	const sl::StringRef& expectedName,
	sl::String* header,
	sl::Array<char>* buffer,
	const sl::StringRef& pem
) {
	sl::String name;
	bool result = decodePem(&name, header, buffer, pem);
	if (!result)
		return false;

	if (name != expectedName)
		return err::fail("unexpected PEM type");

	return true;
}

//..............................................................................

} // namespace cry
} // namespace axl
