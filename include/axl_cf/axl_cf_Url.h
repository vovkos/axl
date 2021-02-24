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

#define _AXL_CF_URL_H

#include "axl_sl_Array.h"
#include "axl_cf_String.h"
#include "axl_cf_Data.h"

namespace axl {
namespace cf {

//..............................................................................

class Url: public TypeBase<CFURLRef>
{
public:
	Url()
	{
	}

	Url(const Url& src):
		TypeBase<CFURLRef>(src)
	{
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	Url(Url&& src):
		TypeBase<CFURLRef>(std::move(src))
	{
	}
#endif

	Url(
		CFURLRef p,
		bool isAttach = false
		):
		TypeBase<CFURLRef>(p, isAttach)
	{
	}

	Url&
	operator = (const Url& src)
	{
		copy(src);
		return *this;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	Url&
	operator = (Url&& src)
	{
		move(std::move(src));
		return *this;
	}
#endif

	Url&
	operator = (CFURLRef p)
	{
		copy(p);
		return *this;
	}

	CFURLRef
	getBaseUrl()
	{
		ASSERT(m_p);
		return ::CFURLGetBaseURL(m_p);
	}

	bool
	isDirectory()
	{
		ASSERT(m_p);
		return ::CFURLHasDirectoryPath(m_p);
	}

	sl::String
	getString()
	{
		ASSERT(m_p);
		return cf::String(::CFURLGetString(m_p), false).getString();
	}

	sl::String
	getPath()
	{
		ASSERT(m_p);
		return cf::String(::CFURLCopyPath(m_p), true).getString();
	}

	sl::String
	getFileSystemPath(CFURLPathStyle pathStyle = kCFURLPOSIXPathStyle)
	{
		ASSERT(m_p);
		return cf::String(::CFURLCopyFileSystemPath(m_p, pathStyle), true).getString();
	}

	bool
	create(
		CFStringRef string,
		CFURLRef baseUrl = NULL
		);

	bool
	create(
		const sl::StringRef& string,
		CFURLRef baseUrl = NULL
		)
	{
		return create(cf::String(string), baseUrl);
	}

	bool
	create(
		CFStringRef filePath,
		CFURLPathStyle pathStyle,
		bool isDirectory,
		CFURLRef baseUrl = NULL
		);

	bool
	create(
		const sl::StringRef& filePath,
		CFURLPathStyle pathStyle,
		bool isDirectory,
		CFURLRef baseUrl = NULL
		)
	{
		return create(cf::String(filePath), pathStyle, isDirectory, baseUrl);
	}

	bool
	startAccessingSecurityScopedResource();

	void
	stopAccessingSecurityScopedResource()
	{
		ASSERT(m_p);
		::CFURLStopAccessingSecurityScopedResource(m_p);
	}

	bool
	resolveBookmark(
		CFDataRef bookmark,
		uint_t options,
		CFURLRef relativeToUrl = NULL,
		bool* isStale = NULL
		);

	bool
	resolveBookmark(
		const void* p,
		size_t size,
		uint_t options,
		CFURLRef relativeToUrl = NULL,
		bool* isStale = NULL
		)
	{
		return resolveBookmark(cf::Data(p, size), options, relativeToUrl, isStale);
	}

	bool
	createBookmark(
		cf::Data* data,
		uint_t options,
		CFURLRef relativeToUrl = NULL
		);

	bool
	createBookmark(
		sl::Array<char>* data,
		uint_t options,
		CFURLRef relativeToUrl = NULL
		);
};

//..............................................................................

} // namespace cf
} // namespace axl
