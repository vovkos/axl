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
#include "axl_io_FilePathUtils.h"
#include "axl_err_Error.h"
#include "axl_enc_Utf.h"

#if (_AXL_OS_WIN)
#	include "axl_io_File.h"
#	include "axl_sys_win_NtDll.h"
#	include "axl_sys_win_NtStatus.h"
#endif

//..............................................................................

#if (_AXL_OS_DARWIN)
char*
get_current_dir_name ()
{
	size_t size = 128;

	for (;;)
	{
		char* buffer = (char*) ::malloc (size);
		char* result = ::getcwd (buffer, size);
		if (result == buffer)
			return buffer;

		::free (buffer);

		if (errno != ERANGE)
			return NULL;

		size *= 2;
	}
}
#endif

namespace axl {
namespace io {

//..............................................................................

sl::String
getTempDir ()
{
#if (_AXL_OS_WIN)
	wchar_t dir [1024] = { 0 };
	::GetTempPathW (countof (dir) - 1, dir);
	return dir;
#elif (_AXL_OS_POSIX)
	return "/tmp";
#endif
}

sl::String
getCurrentDir ()
{
#if (_AXL_OS_WIN)
	wchar_t dir [1024] = { 0 };
	::GetCurrentDirectoryW (countof (dir) - 1, dir);
	return dir;
#elif (_AXL_OS_POSIX)
	char* p = ::get_current_dir_name ();
	sl::String dir = p;
	::free (p);
	return dir;
#endif
}

bool
setCurrentDir (const sl::StringRef& dir)
{
#if (_AXL_OS_WIN)
	bool_t result = ::SetCurrentDirectoryW (dir.s2 ().sz ());
	return err::complete (result);
#elif (_AXL_OS_POSIX)
	int result = ::chdir (dir.sz ());
	return err::complete (result == 0);
#endif
}

sl::String
getExeFilePath ()
{
	char buffer [1024] = { 0 };

#if (_AXL_OS_WIN)
	::GetModuleFileNameA (::GetModuleHandle (NULL), buffer, countof (buffer) - 1);
#elif (_AXL_OS_POSIX)
#	if (_AXL_OS_LINUX)
	::readlink ("/proc/self/exe", buffer, countof (buffer) - 1);
#	elif (_AXL_OS_DARWIN)
	uint32_t size = sizeof (buffer);
	_NSGetExecutablePath (buffer, &size);
#	else
#		error unsupported POSIX flavor
#	endif
#endif

	return buffer;
}

sl::String
getExeDir ()
{
	return io::getDir (io::getExeFilePath ());
}

bool
doesFileExist (const sl::StringRef& fileName)
{
#if (_AXL_OS_WIN)
	char buffer [256];
	sl::String_w fileName_w (ref::BufKind_Stack, buffer, sizeof (buffer));
	fileName_w = fileName;
	dword_t attributes = ::GetFileAttributesW (fileName_w);
	return attributes != INVALID_FILE_ATTRIBUTES;
#elif (_AXL_OS_POSIX)
	return ::access (fileName.sz (), F_OK) != -1;
#endif
}

#if (_AXL_OS_WIN)
bool
isDir (const sl::StringRef_w& fileName)
{
	dword_t attributes = ::GetFileAttributesW (fileName.sz ());
	return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool
isDir (const sl::StringRef& fileName)
{
	char buffer [256];
	sl::String_w fileName_w (ref::BufKind_Stack, buffer, sizeof (buffer));
	return isDir (fileName_w);
}

bool
ensureDirExists (const sl::StringRef& fileName)
{
	char buffer [256] = { 0 };
	sl::String_w fileName_w (ref::BufKind_Stack, buffer, sizeof (buffer));
	fileName_w = fileName;

	if (fileName_w.isEmpty () || isDir (fileName_w))
		return true;

	const wchar_t* p = fileName_w;
	if (p [1] == ':')
		p += 2;

	while (*p == '\\' || *p == '/') // skip root
		p++;

	while (*p)
	{
		const wchar_t* p2 = p + 1;
		while (*p2 && *p2 != '\\' && *p2 != '/')
			p2++;

		wchar_t c = *p2; // save
		*(wchar_t*) p2 = 0;

		if (!isDir (fileName_w))
		{
			bool_t result = ::CreateDirectoryW (fileName_w, NULL);
			if (!result)
				return err::failWithLastSystemError ();
		}

		*(wchar_t*) p2 = c; // restore

		p = p2 + 1;
		while (*p == '\\' || *p == '/') // skip separators
			p++;
	}

	return true;
}

#elif (_AXL_OS_POSIX)

bool
isDir (const sl::StringRef& fileName)
{
	struct stat st;

	int result = ::stat (fileName.sz (), &st);
	return result == 0 && S_ISDIR (st.st_mode);
}

bool
ensureDirExists (const sl::StringRef& fileName0)
{
	sl::String fileName = fileName0;

	if (isDir (fileName))
		return true;

	const char* p = fileName.p (); // ensure exclusive buffer (we're going to modify it)
	while (*p == '/') // skip root
		p++;

	while (*p)
	{
		const char* p2 = p + 1;
		while (*p2 && *p2 != '/')
			p2++;

		char c = *p2; // save
		*(char*) p2 = 0;

		if (!isDir (fileName))
		{
			int result = ::mkdir (fileName, S_IRWXU);
			if (result != 0)
				return err::failWithLastSystemError ();
		}

		*(char*) p2 = c; // restore

		p = p2 + 1;
		while (*p == '/') // skip separators
			p++;
	}

	return true;
}

#endif

sl::String
getFullFilePath (const sl::StringRef& fileName)
{
#if (_AXL_OS_WIN)
	char buffer [256];
	sl::String_w fileName_w (ref::BufKind_Stack, buffer, sizeof (buffer));
	fileName_w = fileName;

	size_t bufferLength = ::GetFullPathNameW (fileName_w, 0, NULL, NULL);
	if (!bufferLength)
		return err::failWithLastSystemError (NULL);

	sl::String_w filePath;

	for (;;)
	{
		wchar_t* p = filePath.createBuffer (bufferLength);
		if (!p)
			return NULL;

		size_t actualLength = ::GetFullPathNameW (fileName_w, bufferLength, p, NULL);
		ASSERT (actualLength != bufferLength); // on success, it should be bufferLength - 1

		if (!actualLength)
			return err::failWithLastSystemError (NULL);

		if (actualLength <= bufferLength)
			return sl::String (filePath, actualLength);

		bufferLength = actualLength; // try again with a bigger buffer
	}

#elif (_AXL_OS_POSIX)
	char fullPath [PATH_MAX] = { 0 };
	::realpath (fileName.sz (), fullPath);
	return fullPath;
#endif
}

sl::String
getDir (const sl::StringRef& filePath)
{
#if (_AXL_OS_WIN)
	char buffer [256];
	sl::String_w filePath_w (ref::BufKind_Stack, buffer, sizeof (buffer));
	filePath_w = filePath;

	wchar_t drive [4] = { 0 };
	wchar_t dir [1024] = { 0 };

	_wsplitpath_s (
		filePath_w,
		drive, countof (drive) - 1,
		dir, countof (dir) - 1,
		NULL, 0,
		NULL, 0
		);

	sl::String string = drive;
	string.append (dir);
	return string;

#elif (_AXL_OS_POSIX)
	const char* p0 = filePath.sz ();
	const char* p = strrchr (p0, '/');
	return p ? sl::String (p0, p - p0) : sl::String ();
#endif
}

sl::String
getFileName (const sl::StringRef& filePath)
{
#if (_AXL_OS_WIN)
	char buffer [256];
	sl::String_w filePath_w (ref::BufKind_Stack, buffer, sizeof (buffer));
	filePath_w = filePath;

	wchar_t fileName [1024] = { 0 };
	wchar_t extension [1024] = { 0 };

	_wsplitpath_s (
		filePath_w,
		NULL, 0,
		NULL, 0,
		fileName, countof (fileName) - 1,
		extension, countof (extension) - 1
		);

	sl::String string = fileName;
	string.append (extension);
	return string;
#elif (_AXL_OS_POSIX)
	const char* p = strrchr (filePath.sz (), '/');
	return p ? sl::String (p + 1) : filePath;
#endif
}

sl::String
getExtension (const sl::StringRef& filePath)
{
	size_t i = filePath.find ('.');
	return i != -1 ? filePath.getSubString (i) : NULL;
}

sl::String
concatFilePath (
	sl::String* filePath,
	const sl::StringRef& fileName
	)
{
	if (filePath->isEmpty ())
	{
		*filePath = fileName;
		return *filePath;
	}

	char last = (*filePath) [filePath->getLength () - 1];

#if (_AXL_OS_WIN)
	if (last != '\\' && last != '/')
		filePath->append ('\\');
#elif (_AXL_OS_POSIX)
	if (last != '/')
		filePath->append ('/');
#endif

	filePath->append (fileName);
	return *filePath;
}

sl::String
findFilePath (
	const sl::StringRef& fileName,
	const sl::StringRef& firstDir,
	const sl::BoxList <sl::String>* dirList,
	bool doFindInCurrentDir
	)
{
	sl::String filePath;

	if (!firstDir.isEmpty ())
	{
		filePath = concatFilePath (firstDir, fileName);
		if (doesFileExist (filePath))
			return getFullFilePath (filePath);
	}

	if (doFindInCurrentDir)
		if (doesFileExist (fileName))
			return getFullFilePath (fileName);

	if (dirList)
	{
		sl::BoxIterator <sl::String> dir = dirList->getHead ();
		for (; dir; dir++)
		{
			filePath.forceCopy (*dir);
			concatFilePath (&filePath, fileName);
			if (doesFileExist (filePath))
				return getFullFilePath (filePath);
		}
	}

	return sl::String ();
}

#if (_AXL_OS_WIN)

bool
getSymbolicLinkTarget (
	sl::String* targetName,
	const sl::StringRef& linkName
	)
{
	using namespace axl::sys::win;

	NTSTATUS status;

	sl::String_w linkName_w = linkName;

	UNICODE_STRING uniLinkName;
	uniLinkName.Buffer = linkName_w.p ();
	uniLinkName.Length = linkName_w.getLength () * sizeof (WCHAR);
	uniLinkName.MaximumLength = linkName_w.getHdr ()->m_bufferSize;

	OBJECT_ATTRIBUTES oa = { 0 };
	oa.Length = sizeof (oa);
	oa.RootDirectory = NULL;
	oa.ObjectName = &uniLinkName;

	io::win::File link;
	status = ntOpenSymbolicLinkObject (
		link.p (),
		GENERIC_READ,
		&oa
		);

	if (status < 0)
	{
		err::setError (sys::win::NtStatus (status));
		return false;
	}

	UNICODE_STRING uniTarget = { 0 };
	ULONG bufferSize = 0;

	status = ntQuerySymbolicLinkObject (link, &uniTarget, &bufferSize);
	if (status != STATUS_BUFFER_TOO_SMALL)
	{
		err::setError (sys::win::NtStatus (status));
		return false;
	}

	sl::String_w targetName_w;
	wchar_t* p = targetName_w.createBuffer (bufferSize / sizeof (WCHAR));

	uniTarget.Buffer = p;
	uniTarget.Length = 0;
	uniTarget.MaximumLength = (USHORT) bufferSize;

	status = ntQuerySymbolicLinkObject (link, &uniTarget, &bufferSize);
	if (status < 0)
	{
		err::setError (sys::win::NtStatus (status));
		return false;
	}

	ASSERT (uniTarget.Length <= uniTarget.MaximumLength);
	targetName_w.setReducedLength (uniTarget.Length / sizeof (WCHAR));
	*targetName = targetName_w;
	return true;
}

#elif (_AXL_OS_POSIX)

bool
getSymbolicLinkTarget (
	sl::String* targetName,
	const sl::StringRef& linkName
	)
{
	struct stat linkStat = { 0 };

	int result = ::lstat (linkName.sz (), &linkStat);
	if (result == -1)
		return err::failWithLastSystemError ();

	if (!S_ISLNK (linkStat.st_mode))
		return err::fail (EINVAL);

	size_t bufferLength = linkStat.st_size ? linkStat.st_size : PATH_MAX;

	char* p = targetName->createBuffer (bufferLength);
	if (!p)
		return false;

	result = readlink (linkName.sz (), p, bufferLength);
	if (result == -1)
		return err::failWithLastSystemError ();

	ASSERT (result <= bufferLength);
	targetName->setReducedLength (result);
	return true;
}

#endif

//..............................................................................

} // namespace io
} // namespace axl
