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
#include "axl_cry_Dh.h"

namespace axl {
namespace cry {

//..............................................................................

bool
Dh::create()
{
	close();

	m_h = ::DH_new();
	return completeWithLastCryptoError(m_h != NULL);
}

bool
Dh::create1024x160()
{
	close();

	m_h = ::DH_get_1024_160();
	return completeWithLastCryptoError(m_h != NULL);
}

bool
Dh::create2048x224()
{
	close();

	m_h = ::DH_get_2048_224();
	return completeWithLastCryptoError(m_h != NULL);
}

bool
Dh::create2048x256()
{
	close();

	m_h = ::DH_get_2048_256();
	return completeWithLastCryptoError(m_h != NULL);
}

bool
Dh::readParameters(BIO* bio)
{
	close();

	bool result = ::PEM_read_bio_DHparams(bio, &m_h, 0, NULL) != NULL;
	return completeWithLastCryptoError(result);
}

//..............................................................................

} // namespace cry
} // namespace axl
