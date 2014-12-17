#include "pch.h"
#include "axl_cry_EcPoint.h"

namespace axl {
namespace cry {

//.............................................................................

bool
EcPoint::create (EC_GROUP* group)
{
	close ();

	m_h = EC_POINT_new (group);
	return completeWithLastCryptoError (m_h != NULL);
}

bool
EcPoint::createCopy (
	EC_POINT* src,
	EC_GROUP* group
	)
{
	close ();

	m_h = EC_POINT_dup (src, group);
	return completeWithLastCryptoError (m_h != NULL);
}

bool
EcPoint::getData (
	rtl::Array <char>* data,
	EC_GROUP* group,
	point_conversion_form_t form,
	BN_CTX* ctx
	)
{
	int result = EC_POINT_point2oct (group, m_h, form, NULL, 0, ctx);
	if (result <= 0)
	{
		setLastCryptoError ();
		return false;
	}

	size_t size = result;
	data->setCount (result);

	result = EC_POINT_point2oct (group, m_h, form, (uchar_t*) data->a (), size, ctx);
	if (result <= 0)
	{
		setLastCryptoError ();
		return false;
	}

	return true;
}

bool
EcPoint::getHexString (
	rtl::String* string,
	EC_GROUP* group,
	point_conversion_form_t form,
	BN_CTX* ctx
	)
{
	char* p = EC_POINT_point2hex (group, m_h, form, ctx);
	*string = p;
	OPENSSL_free (p);
	return true;
}

//.............................................................................

} // namespace cry
} // namespace axl