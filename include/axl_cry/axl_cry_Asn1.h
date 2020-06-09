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

#pragma once

#define _AXL_CRY_ASN1STRING_H

#include "axl_cry_Pch.h"

namespace axl {
namespace cry {

//..............................................................................

sl::String
getAsn1TimeString(const ASN1_TIME* time);

sl::String
getAsn1StringString(const ASN1_STRING* string);

sl::String
getAsn1ObjectString(const ASN1_OBJECT* object);

//..............................................................................

} // namespace cry
} // namespace axl
