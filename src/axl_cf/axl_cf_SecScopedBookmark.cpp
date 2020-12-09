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
#include "axl_cf_SecScopedBookmark.h"

namespace axl {
namespace cf {

//..............................................................................

void
SecScopedBookmark::close()
{
	stopAccess();
	m_filePath.clear();
	m_isDirectory = false;
	m_dataCache.clear();
}

void
SecScopedBookmark::create(
	const sl::StringRef& filePath,
	bool isDirectory
	)
{
	if (m_filePath == filePath && m_isDirectory == isDirectory)
		return;

	stopAccess();
	m_filePath = filePath;
	m_isDirectory = isDirectory;
	m_dataCache.clear();
}

void
SecScopedBookmark::copy(const SecScopedBookmark& src)
{
	if (this == &src || m_filePath == src.m_filePath && m_isDirectory == src.m_isDirectory)
		return;

	stopAccess();
	m_filePath = src.m_filePath;
	m_isDirectory = src.m_isDirectory;
	m_dataCache = src.m_dataCache;

	if (!src.m_accessUrl)
		return;

	bool result = ((Url*)&src.m_accessUrl)->startAccessingSecurityScopedResource();
	if (result)
		m_accessUrl = src.m_accessUrl;
	else
		AXL_TRACE("WARNING: can't access an already-accessible resource: %s", err::getLastErrorDescription().sz());
}

#if (_AXL_CPP_HAS_RVALUE_REF)

void
SecScopedBookmark::move(SecScopedBookmark&& src)
{
	stopAccess();

	m_filePath = std::move(src.m_filePath);
	m_accessUrl = std::move(src.m_accessUrl);
	m_dataCache = std::move(src.m_dataCache);
	m_isDirectory = std::move(src.m_isDirectory);
}

#endif

bool
SecScopedBookmark::load(
	const void* p,
	size_t size
	)
{
	close();

	Url url;
	bool result = url.resolveBookmark(p, size, kCFURLBookmarkResolutionWithoutUIMask | kCFURLBookmarkResolutionWithSecurityScope);
	if (!result)
		return false;

	m_filePath = url.getFileSystemPath();
	m_isDirectory = url.isDirectory();

	stopAccess();

	result = url.startAccessingSecurityScopedResource(); // may fail for already-accessible locations (not an error really)
	if (result)
		m_accessUrl = std::move(url);

	return true;
}

bool
SecScopedBookmark::save(sl::Array<char>* buffer) const
{
	if (!m_dataCache.isEmpty())
	{
		*buffer = m_dataCache;
		return true;
	}

	Url url;

	bool result =
		url.create(m_filePath, kCFURLPOSIXPathStyle, m_isDirectory) &&
		url.createBookmark(&m_dataCache, kCFURLBookmarkCreationWithSecurityScope);

	if (!result)
		return false;

	*buffer = m_dataCache;
	return true;
}

void
SecScopedBookmark::stopAccess()
{
	if (!m_accessUrl)
		return;

	m_accessUrl.stopAccessingSecurityScopedResource();
	m_accessUrl.clear();
}

//..............................................................................

} // namespace cf
} // namespace axl
