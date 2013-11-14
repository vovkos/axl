#include "pch.h"
#include "axl_io_FilePathUtils.h"
#include "axl_err_Error.h"
#include "axl_rtl_Utf.h"

namespace axl {
namespace io {

//.............................................................................

rtl::CString
GetCurrentDir ()
{
#if (_AXL_ENV == AXL_ENV_WIN)
	char Dir [PATH_MAX] = { 0 };
	GetCurrentDirectory (Dir, sizeof (Dir) - 1);
	return Dir;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	char* p = get_current_dir_name ();
	rtl::CString Dir = p;
	free (p);
	return Dir;
#endif
}

bool
DoesFileExist (const char* pFileName)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	return GetFileAttributesW (rtl::CString_w (pFileName)) != INVALID_FILE_ATTRIBUTES;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	return access (pFileName, F_OK) != -1;
#endif
}

rtl::CString
GetFullFilePath (const char* pFileName)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	rtl::CString_w FileName = pFileName;

	size_t Length = GetFullPathNameW (FileName, 0, NULL, NULL);
	if (!Length)
		return err::FailWithLastSystemError (NULL);

	rtl::CString_w FilePath;
	wchar_t* p = FilePath.GetBuffer (Length);
	GetFullPathNameW (FileName, Length, p, NULL);
	return FilePath;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	char FullPath [PATH_MAX] = { 0 };
	realpath (pFileName, FullPath);
	return FullPath;
#endif
}

rtl::CString
GetDir (const char* pFilePath)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	rtl::CString_w FilePath = pFilePath;

	wchar_t Drive [4] = { 0 };
	wchar_t Dir [1024] = { 0 };

	_wsplitpath_s (
		FilePath,
		Drive, countof (Drive) - 1,
		Dir, countof (Dir) - 1,
		NULL, 0,
		NULL, 0
		);

	rtl::CString String = Drive;
	String.Append (Dir);
	return String;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	rtl::CString String = pFilePath;
	dirname (String.GetBuffer ());
	return String;
#endif
}

rtl::CString
GetFileName (const char* pFilePath)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	rtl::CString_w FilePath = pFilePath;

	wchar_t FileName [1024] = { 0 };
	wchar_t Extension [1024] = { 0 };

	_wsplitpath_s (
		FilePath,
		NULL, 0,
		NULL, 0,
		FileName, countof (FileName) - 1,
		Extension, countof (Extension) - 1
		);

	rtl::CString String = FileName;
	String.Append (Extension);
	return String;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	const char *p = strrchr (pFilePath, '/');
	return p ? p + 1 : pFilePath;
#endif
}

rtl::CString
GetExtension (const char* pFilePath)
{
	const char *p = strchr (pFilePath, '.');
	return p ? p + 1 : NULL;
}

rtl::CString
ConcatFilePath (
	rtl::CString* pFilePath,
	const char* pFileName
	)
{
	if (pFilePath->IsEmpty ())
	{
		*pFilePath = pFileName;
		return *pFilePath;
	}

	char Last = (*pFilePath) [pFilePath->GetLength () - 1];

#if (_AXL_ENV == AXL_ENV_WIN)
	if (Last != '\\' && Last != '/')
		pFilePath->Append ('\\');
#elif (_AXL_ENV == AXL_ENV_POSIX)
	if (Last != '/')
		pFilePath->Append ('/');
#endif

	pFilePath->Append (pFileName);
	return *pFilePath;
}

rtl::CString
FindFilePath (
	const char* pFileName,
	const char* pFirstDir,
	const rtl::CBoxListT <rtl::CString>* pDirList,
	bool DoFindInCurrentDir
	)
{
	rtl::CString FilePath;

	if (pFirstDir)
	{
		FilePath = ConcatFilePath (pFirstDir, pFileName);
		if (DoesFileExist (FilePath))
			return GetFullFilePath (FilePath);
	}

	if (DoFindInCurrentDir)
		if (DoesFileExist (pFileName))
			return GetFullFilePath (pFileName);

	if (pDirList)
	{
		rtl::CBoxIteratorT <rtl::CString> Dir = pDirList->GetHead ();
		for (; Dir; Dir++)
		{
			FilePath.ForceCopy (*Dir);
			ConcatFilePath (&FilePath, pFileName);
			if (DoesFileExist (FilePath))
				return GetFullFilePath (FilePath);
		}
	}

	return rtl::CString ();
}

//.............................................................................

} // namespace io
} // namespace axl
