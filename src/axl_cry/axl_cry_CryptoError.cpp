#include "pch.h"
#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//.............................................................................

class LoadCryptoStrings
{
public:
	void
	operator () (int)
	{
		ERR_load_crypto_strings ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::String 
CryptoErrorProvider::getErrorDescription (const err::ErrorHdr* error)
{
	mt::callOnce (LoadCryptoStrings (), 0);
	return ERR_reason_error_string (error->m_code);
}

//.............................................................................

} // namespace cry
} // namespace axl
