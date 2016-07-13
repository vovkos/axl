#include "pch.h"
#include "axl_sl_Hash.h"

namespace axl {
namespace sl {

//.............................................................................

size_t
djb2 (
	const void* _p,
	size_t size
	)
{
	uchar_t* p = (uchar_t*) _p;
	uchar_t* end = p + size;

	size_t hash = 5381;

	for (; p < end; p++)
		hash = ((hash << 5) + hash) + *p; /* hash * 33 + c */

	return hash;
}

uint16_t
checksum16 (
	const void* _p,
	size_t size
	)
{
	ushort_t* p = (ushort_t*) _p;
	uchar_t* last = (uchar_t*) _p + size - 1;

	uintptr_t sum = 0;

	for (; (uchar_t*) p < last; p++)
		sum += *p;

	if (size & 1)
		sum += *last;

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

	return (ushort_t) ~sum;
}

//.............................................................................

} // namespace err
} // namespace axl