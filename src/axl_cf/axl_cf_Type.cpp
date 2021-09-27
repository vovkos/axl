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
#include "axl_cf_Type.h"
#include "axl_cf_String.h"

namespace axl {
namespace cf {

//..............................................................................

sl::String
getStringFromCfString(CFStringRef cfString) {
	size_t length = ::CFStringGetLength(cfString);
	if (!length)
		return sl::String();

	CFRange range = { 0, length };
	CFIndex bufferLength = 0;
	::CFStringGetBytes(
		cfString,
		range,
		kCFStringEncodingUTF8,
		0,
		false,
		NULL,
		0,
		&bufferLength
	);

	if (!bufferLength)
		return sl::String();

	sl::String string;
	char* p = string.createBuffer(bufferLength);
	if (p)
		::CFStringGetBytes(
			cfString,
			range,
			kCFStringEncodingUTF8,
			0,
			false,
			(UInt8*)p,
			bufferLength,
			&bufferLength
		);

	return string;
}

sl::String
getTypeIdDescription(CFTypeID typeId) {
	CFStringRef cfString = ::CFCopyTypeIDDescription(typeId);
	return String(cfString, true).getString();
}

sl::String
cfTypeToString(
	CFTypeRef cfType,
	bool isVerbose
) {
	CFTypeID typeId = ::CFGetTypeID(cfType);
	if (typeId == getStringTypeId()) {
		CFStringRef cfString = (CFStringRef)cfType;
		return getStringFromCfString(cfString);
	} else if (typeId == getBooleanTypeId()) {
		CFBooleanRef cfBoolean = (CFBooleanRef)cfType;
		bool b = ::CFBooleanGetValue(cfBoolean);
		return b ? "true" : "false";
	} else if (typeId == getNumberTypeId()) {
		CFNumberRef cfNumber = (CFNumberRef)cfType;
		bool isFloat = ::CFNumberIsFloatType(cfNumber);
		if (isFloat) {
			double x = 0;
			::CFNumberGetValue(cfNumber, kCFNumberDoubleType, &x);
			return sl::formatString("%f", x);
		} else {
			long long x = 0;
			::CFNumberGetValue(cfNumber, kCFNumberLongLongType, &x);
			return sl::formatString("%lld", x);
		}

	} else if (isVerbose) {
		CFStringRef cfString = ::CFCopyDescription(cfType);
		return String(cfString, true).getString();
	} else {
		return getTypeIdDescription(typeId);
	}
}

//..............................................................................

} // namespace cf
} // namespace axl
