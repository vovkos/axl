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

#define _AXL_CF_NUMBER_H

#include "axl_cf_Type.h"

namespace axl {
namespace cf {

//..............................................................................

class NumberRef: public TypeRefBase<CFNumberRef>
{
public:
	NumberRef()
	{
	}

	NumberRef(const NumberRef& src)
	{
		copy(src);
	}

	NumberRef(
		CFNumberRef p,
		bool isAttach = false
		)
	{
		isAttach ? copy(p) : attach(p);
	}

	NumberRef(char x)
	{
		m_p = NULL;
		create(kCFNumberCharType, &x);
	}

	NumberRef(short x)
	{
		m_p = NULL;
		create(kCFNumberShortType, &x);
	}

	NumberRef(int x)
	{
		m_p = NULL;
		create(kCFNumberIntType, &x);
	}

	NumberRef(long x)
	{
		m_p = NULL;
		create(kCFNumberLongType, &x);
	}

	NumberRef(long long x)
	{
		m_p = NULL;
		create(kCFNumberLongLongType, &x);
	}

	NumberRef(float x)
	{
		m_p = NULL;
		create(kCFNumberFloatType, &x);
	}

	NumberRef(double x)
	{
		m_p = NULL;
		create(kCFNumberDoubleType, &x);
	}

	NumberRef&
	operator = (const NumberRef& src)
	{
		copy(src);
		return *this;
	}

	NumberRef&
	operator = (CFNumberRef p)
	{
		copy(p);
		return *this;
	}

	NumberRef&
	operator = (char x)
	{
		create(kCFNumberCharType, &x);
		return *this;
	}

	NumberRef&
	operator = (short x)
	{
		create(kCFNumberShortType, &x);
		return *this;
	}

	NumberRef&
	operator = (int x)
	{
		create(kCFNumberIntType, &x);
		return *this;
	}

	NumberRef&
	operator = (long x)
	{
		create(kCFNumberLongType, &x);
		return *this;
	}

	NumberRef&
	operator = (long long x)
	{
		create(kCFNumberLongLongType, &x);
		return *this;
	}

	NumberRef&
	operator = (float x)
	{
		create(kCFNumberFloatType, &x);
		return *this;
	}

	NumberRef&
	operator = (double x)
	{
		create(kCFNumberDoubleType, &x);
		return *this;
	}

	bool
	create(
		CFNumberType type,
		const void* p
		);

	bool
	create(char x)
	{
		return create(kCFNumberCharType, &x);
	}

	bool
	create(short x)
	{
		return create(kCFNumberShortType, &x);
	}

	bool
	create(int x)
	{
		return create(kCFNumberIntType, &x);
	}

	bool
	create(long x)
	{
		return create(kCFNumberLongType, &x);
	}

	bool
	create(long long x)
	{
		return create(kCFNumberLongLongType, &x);
	}

	bool
	create(float x)
	{
		return create(kCFNumberFloatType, &x);
	}

	bool
	create(double x)
	{
		return create(kCFNumberDoubleType, &x);
	}

	bool
	isFloat() const
	{
		return ::CFNumberIsFloatType(m_p);
	}

	CFComparisonResult
	compare(CFNumberRef p2) const
	{
		return ::CFNumberCompare(m_p, p2, NULL);
	}
};

//..............................................................................

} // namespace cf
} // namespace axl
