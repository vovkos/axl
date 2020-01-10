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
#include "axl_cry_Asn1.h"
#include "axl_cry_Bio.h"

namespace axl {
namespace cry {

//..............................................................................

sl::String
getAsn1TimeString(const ASN1_TIME* time)
{
	Bio bio;
	bio.createMem();
	ASN1_TIME_print(bio, time);

	BUF_MEM* mem = bio.getBufMem();
	return sl::String(mem->data, mem->length);
}

sl::String
getAsn1StringString(const ASN1_STRING* string)
{
	Bio bio;
	bio.createMem();
	ASN1_STRING_print(bio, string);

	BUF_MEM* mem = bio.getBufMem();
	return sl::String(mem->data, mem->length);
}

sl::String
getAsn1ObjectString(const ASN1_OBJECT* object)
{
	char buffer[256];
	OBJ_obj2txt(buffer, sizeof(buffer), object, 0);
	return buffer;
}

//..............................................................................

} // namespace cry
} // namespace axl
