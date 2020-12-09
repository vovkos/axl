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

#define _AXL_CF_SECSCOPEDBOOKMARK_H

#include "axl_sl_List.h"
#include "axl_sl_String.h"
#include "axl_sl_StringHashTable.h"
#include "axl_cf_Url.h"

namespace axl {
namespace cf {

//..............................................................................

class SecScopedBookmark
{
protected:
	sl::String m_filePath;
	mutable sl::Array<char> m_dataCache;
	Url m_accessUrl;
	bool m_isDirectory;

public:
	SecScopedBookmark()
	{
		m_isDirectory = false;
	}

	SecScopedBookmark(const SecScopedBookmark& src)
	{
		operator = (src);
	}

	SecScopedBookmark(SecScopedBookmark& src)
	{
		operator = (src);
	}

	~SecScopedBookmark()
	{
		close();
	}

	SecScopedBookmark&
	operator = (const SecScopedBookmark& src)
	{
		copy(src);
		return *this;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	SecScopedBookmark&
	operator = (SecScopedBookmark&& src)
	{
		move(std::move(src));
		return *this;
	}
#endif

	bool
	isEmpty() const
	{
		return m_filePath.isEmpty();
	}

	const sl::String&
	getFilePath() const
	{
		return m_filePath;
	}

	bool
	isDirectory() const
	{
		return m_isDirectory;
	}

	void
	close();

	void
	create(
		const sl::StringRef& filePath,
		bool isDirectory
		);

	void
	copy(const SecScopedBookmark& src);

#if (_AXL_CPP_HAS_RVALUE_REF)
	void
	move(SecScopedBookmark&& src);
#endif

	bool
	load(
		const void* p,
		size_t size
		);

	bool
	save(sl::Array<char>* buffer) const;

	sl::Array<char>
	save() const
	{
		sl::Array<char> buffer;
		save(&buffer);
		return buffer;
	}

	void
	stopAccess();
};

//..............................................................................

} // namespace cf
} // namespace axl
