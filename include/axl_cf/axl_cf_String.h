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

#define _AXL_CF_STRING_H

#include "axl_cf_Type.h"

namespace axl {
namespace cf {

//..............................................................................

class StringRef: public TypeRefBase<CFStringRef>
{
public:
	StringRef()
	{
	}

	StringRef(const StringRef& src)
	{
		copy(src);
	}

	StringRef(
		CFStringRef p,
		bool isAttach = false
		)
	{
		isAttach ? copy(p) : attach(p);
	}

	StringRef(
	   const char* p,
	   size_t length = -1
		)
	{
		create(p, length);
	}

	StringRef&
	operator = (const StringRef& src)
	{
		copy(src);
		return *this;
	}

	StringRef&
	operator = (CFStringRef p)
	{
		copy(p);
		return *this;
	}

	StringRef&
	operator = (const char* p)
	{
		create(p);
		return *this;
	}

	bool
	create(CFStringRef src);

	bool
	create(
			const char* p,
			size_t length = -1
			);

	size_t
	getLength() const
	{
		return ::CFStringGetLength(m_p);
	}

	CFComparisonResult
	compare(
		CFStringRef p2,
		CFStringCompareFlags flags
		) const
	{
		return ::CFStringCompare(m_p, p2, flags);
	}

	sl::String
	getString() const
	{
		return getStringFromCfString(m_p);
	}
};

//..............................................................................

} // namespace cf
} // namespace axl
