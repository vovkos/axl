#include "stdafx.h"
#include "axl_g_Def.h"

namespace axl {
namespace rtl {

//.............................................................................

size_t
djb2 (
	const void* _p,
	size_t Size
	)
{
	uchar_t* p = (uchar_t*) _p;
	uchar_t* pEnd = p + Size;

	size_t Hash = 5381;

	for (; p < pEnd; p++)
        Hash = ((Hash << 5) + Hash) + *p; /* hash * 33 + c */

	return Hash;
}

//.............................................................................

} // namespace err
} // namespace axl
