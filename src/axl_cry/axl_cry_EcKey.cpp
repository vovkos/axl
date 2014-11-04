#include "pch.h"
#include "axl_cry_EcKey.h"

namespace axl {
namespace cry {

//.............................................................................

void
EcKey::setFlags (uint_t flags)
{
	int prevFlags = getFlags ();

	addFlags (flags & ~prevFlags);
	removeFlags (prevFlags & ~flags);
}

bool
EcKey::create ()
{
	close ();

	m_h = EC_KEY_new ();
	return completeWithLastCryptoError (m_h != NULL);
}

bool
EcKey::create (uint_t curveId)
{
	close ();

	m_h = EC_KEY_new_by_curve_name (curveId);
	return completeWithLastCryptoError (m_h != NULL);
}

bool
EcKey::createCopy (const EC_KEY* src)
{
	close ();

	m_h = EC_KEY_dup (src);
	return completeWithLastCryptoError (m_h != NULL);
}

size_t
EcKey::signHash (
	void* signature,
	size_t signatureSize,
	const void* hash,
	size_t hashSize
	)
{
	ASSERT (signatureSize >= getSignatureSize ());
	
	uint_t resultSize = (uint_t) signatureSize;
	int result = ECDSA_sign (
		0, 
		(const uchar_t*) hash, 
		(int) hashSize, 
		(uchar_t*) signature, 
		&resultSize,
		m_h
		);

	if (!result)
	{
		setLastCryptoError ();
		return -1;
	}

	ASSERT (resultSize <= signatureSize);
	return resultSize;
}

//.............................................................................

} // namespace cry
} // namespace axl
