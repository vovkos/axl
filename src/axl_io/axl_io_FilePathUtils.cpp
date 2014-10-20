#include "pch.h"
#include "axl_io_FilePathUtils.h"
#include "axl_err_Error.h"
#include "axl_rtl_Utf.h"

namespace axl {
namespace io {

//.............................................................................

rtl::String
getCurrentDir ()
{
#if (_AXL_ENV == AXL_ENV_WIN)
	wchar_t dir [1024] = { 0 };
	::GetCurrentDirectoryW (countof (dir) - 1, dir);
	return dir;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	char* p = get_current_dir_name ();
	rtl::String dir = p;
	free (p);
	return dir;
#endif
}

rtl::String
getExeFilePath ()
{
	char buffer [1024];
	buffer [countof (buffer) - 1] = 0;
	
#if (_AXL_ENV == AXL_ENV_WIN)
	::GetModuleFileNameA (::GetModuleHandle (NULL), buffer, countof (buffer) - 1);
#elif (_AXL_ENV == AXL_ENV_POSIX)
	readlink ("/proc/self/exe", buffer, countof (buffer) - 1);
#endif

	return buffer;
}

rtl::String
getExeDir ()
{
	return io::getDir (io::getExeFilePath ());
}

bool
doesFileExist (const char* fileName)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	char buffer [256];
	rtl::String_w fileName_w (ref::BufKind_Stack, buffer, sizeof (buffer));
	fileName_w = fileName;
	dword_t attributes = ::GetFileAttributesW (fileName_w);
	return attributes != INVALID_FILE_ATTRIBUTES;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	return access (fileName, F_OK) != -1;
#endif
}

#if (_AXL_ENV == AXL_ENV_WIN)
inline
bool
isDir (const wchar_t* fileName)
{
	dword_t attributes = ::GetFileAttributesW (fileName);
	return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool
ensureDirExists (const char* fileName)
{
	char buffer [256] = { 0 };
	rtl::String_w fileName_w (ref::BufKind_Stack, buffer, sizeof (buffer));
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

#elif (_AXL_ENV == AXL_ENV_POSIX)

inline
bool
isDir (const char* fileName)
{
	struct stat st;

	int result = stat (fileName, &st);
	return result == 0 && S_ISDIR (st.st_mode);
}

bool
ensureDirExists (const char* fileName)
{
	if (isDir (fileName))
		return true;

	const char* p = fileName;
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
			int result = mkdir (fileName, S_IRWXU);
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

rtl::String
getFullFilePath (const char* fileName)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	char buffer [256];
	rtl::String_w fileName_w (ref::BufKind_Stack, buffer, sizeof (buffer));
	fileName_w = fileName;

	size_t length = ::GetFullPathNameW (fileName_w, 0, NULL, NULL);
	if (!length)
		return err::failWithLastSystemError (NULL);

	rtl::String_w filePath;
	wchar_t* p = filePath.getBuffer (length);
	::GetFullPathNameW (fileName_w, length, p, NULL);
	return filePath;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	char fullPath [PATH_MAX] = { 0 };
	realpath (fileName, fullPath);
	return fullPath;
#endif
}

rtl::String
getDir (const char* filePath)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	char buffer [256];
	rtl::String_w filePath_w (ref::BufKind_Stack, buffer, sizeof (buffer));
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

	rtl::String string = drive;
	string.append (dir);
	return string;

#elif (_AXL_ENV == AXL_ENV_POSIX)
	rtl::String string = filePath;
	dirname (string.getBuffer ());
	string.updateLength ();
	return string;
#endif
}

rtl::String
getFileName (const char* filePath)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	char buffer [256];
	rtl::String_w filePath_w (ref::BufKind_Stack, buffer, sizeof (buffer));
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

	rtl::String string = fileName;
	string.append (extension);
	return string;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	const char *p = strrchr (filePath, '/');
	return p ? p + 1 : filePath;
#endif
}

rtl::String
getExtension (const char* filePath)
{
	const char *p = strchr (filePath, '.');
	return p ? p + 1 : NULL;
}

rtl::String
concatFilePath (
	rtl::String* filePath,
	const char* fileName
	)
{
	if (filePath->isEmpty ())
	{
		*filePath = fileName;
		return *filePath;
	}

	char last = (*filePath) [filePath->getLength () - 1];

#if (_AXL_ENV == AXL_ENV_WIN)
	if (last != '\\' && last != '/')
		filePath->append ('\\');
#elif (_AXL_ENV == AXL_ENV_POSIX)
	if (last != '/')
		filePath->append ('/');
#endif

	filePath->append (fileName);
	return *filePath;
}

rtl::String
findFilePath (
	const char* fileName,
	const char* firstDir,
	const rtl::BoxList <rtl::String>* dirList,
	bool doFindInCurrentDir
	)
{
	rtl::String filePath;

	if (firstDir)
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
		rtl::BoxIterator <rtl::String> dir = dirList->getHead ();
		for (; dir; dir++)
		{
			filePath.forceCopy (*dir);
			concatFilePath (&filePath, fileName);
			if (doesFileExist (filePath))
				return getFullFilePath (filePath);
		}
	}

	return rtl::String ();
}

//.............................................................................

} // namespace io
} // namespace axl
