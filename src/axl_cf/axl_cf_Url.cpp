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
#include "axl_cf_Url.h"

namespace axl {
namespace cf {

//..............................................................................

bool
Url::create(
	CFStringRef string,
	CFURLRef baseUrl
	)
{
	CFURLRef url = ::CFURLCreateWithString(kCFAllocatorDefault, string, baseUrl);
	if (!url)
		return err::fail(err::SystemErrorCode_InsufficientResources);

	attach(url);
	return true;
}

bool
Url::create(
	CFStringRef filePath,
	CFURLPathStyle pathStyle,
	bool isDirectory,
	CFURLRef baseUrl
	)
{
	CFURLRef url = baseUrl ?
		::CFURLCreateWithFileSystemPathRelativeToBase(kCFAllocatorDefault, filePath, pathStyle, isDirectory, baseUrl) :
		::CFURLCreateWithFileSystemPath(kCFAllocatorDefault, filePath, pathStyle, isDirectory);

	if (!url)
		return err::fail(err::SystemErrorCode_InsufficientResources);

	attach(url);
	return true;
}

bool
Url::startAccessingSecurityScopedResource()
{
	ASSERT(m_p);
	Boolean result = ::CFURLStartAccessingSecurityScopedResource(m_p);
	return result ? true : err::fail(err::SystemErrorCode_Unsuccessful);
}

bool
Url::resolveBookmark(
	CFDataRef bookmark,
	uint_t options,
	CFURLRef relativeToUrl,
	bool* isStale
	)
{
	Boolean isStale_cf;
	CFErrorRef error;
	CFURLRef url = ::CFURLCreateByResolvingBookmarkData(kCFAllocatorDefault, bookmark, options, relativeToUrl, NULL, &isStale_cf, &error);
	if (!url)
	{
		cf::String description(::CFErrorCopyDescription(error), true);
		return err::fail(err::Error(description.getString()));
	}

	if (isStale)
		*isStale = isStale_cf != 0;

	attach(url);
	return true;
}

bool
Url::createBookmark(
	cf::Data* outData,
	uint_t options,
	CFURLRef relativeToUrl
	)
{
	ASSERT(m_p);

	CFErrorRef error;
	CFDataRef data = ::CFURLCreateBookmarkData(kCFAllocatorDefault, m_p, options, NULL, relativeToUrl, &error);
	if (!data)
	{
		cf::String description(::CFErrorCopyDescription(error), true);
		return err::fail(err::Error(description.getString()));
	}

	outData->copy(data);
	return true;
}

bool
Url::createBookmark(
	sl::Array<char>* outData,
	uint_t options,
	CFURLRef relativeToUrl
	)
{
	cf::Data data;
	bool result = createBookmark(&data, options, relativeToUrl);
	return result ? outData->copy((char*)data.getBytePtr(), data.getLength()) != 1 : false;
}

//..............................................................................

} // namespace cf
} // namespace axl
