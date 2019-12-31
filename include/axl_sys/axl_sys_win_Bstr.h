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

#define _AXL_SYS_WIN_BSTR_H

#include "axl_sl_Handle.h"
#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class FreeBstr
{
public:
	void
	operator () (BSTR h)
	{
		::SysFreeString(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Bstr: public sl::Handle<BSTR, FreeBstr>
{
public:
	Bstr()
	{
	}

	Bstr(const Bstr& src)
	{
		copy(src.m_h);
	}

	Bstr(const sl::StringRef& string)
	{
		copy(string);
	}

	Bstr(const sl::StringRef_w& string)
	{
		copy(string);
	}

	Bstr&
	operator = (const Bstr& src)
	{
		copy(src.m_h);
		return *this;
	}

	Bstr&
	operator = (const sl::StringRef& string)
	{
		copy(string);
		return *this;
	}

	Bstr&
	operator = (const sl::StringRef_w& string)
	{
		copy(string);
		return *this;
	}

	size_t
	getLength() const
	{
		return ::SysStringLen(m_h);
	}

	size_t
	getByteLength() const
	{
		return ::SysStringByteLen(m_h);
	}

	bool
	copy(
		BSTR p,
		size_t length
		);

	bool
	copy(const sl::StringRef& string)
	{
		return copy(sl::String_w(string));
	}

	bool
	copy(const sl::StringRef_w& string)
	{
		return copy((BSTR)string.cp(), string.getLength());
	}
};

//..............................................................................

// common pattern is to use this func in COM object implementations
// so it's natural to return HRESULT

template <typename T>
HRESULT
createBstrFromString(
	BSTR* p,
	const sl::StringRefBase<T>& string
	)
{
	ASSERT(p);

	Bstr bstr;
	bool result = bstr.copy(string);
	if (!result)
		return E_OUTOFMEMORY;

	*p = bstr.detach();
	return S_OK;
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
