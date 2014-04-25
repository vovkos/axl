#include "pch.h"
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

uint16_t
crc16(
	const void* _p, 
	size_t Size
	)
{
	ushort_t* p = (ushort_t*) _p;
	uchar_t* pLast = (uchar_t*) _p + Size - 1;

	uintptr_t CheckSum = 0;

	for (; (uchar_t*) p < pLast; p++)
		CheckSum += *p;
  
	if (Size & 1)
		CheckSum += *pLast;

	CheckSum = (CheckSum >> 16) + (CheckSum & 0xffff);
	CheckSum += (CheckSum >>16);

	return (ushort_t) ~CheckSum;
}

//.............................................................................

} // namespace err
} // namespace axl
