#include "pch.h"
#include "axl_g_Def.h"

namespace axl {
namespace rtl {

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
crc16 (
	const void* _p,
	size_t size
	)
{
	ushort_t* p = (ushort_t*) _p;
	uchar_t* last = (uchar_t*) _p + size - 1;

	uintptr_t checkSum = 0;

	for (; (uchar_t*) p < last; p++)
		checkSum += *p;

	if (size & 1)
		checkSum += *last;

	checkSum = (checkSum >> 16) + (checkSum & 0xffff);
	checkSum += (checkSum >>16);

	return (ushort_t) ~checkSum;
}

//.............................................................................

} // namespace err
} // namespace axl
