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
	wchar_t Dir [1024] = { 0 };
	GetCurrentDirectoryW (countof (Dir) - 1, Dir);
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
	char Buffer [256];
	rtl::CString_w FileName (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	FileName = pFileName;
	dword_t Attributes = GetFileAttributesW (FileName);
	return Attributes != INVALID_FILE_ATTRIBUTES;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	return access (pFileName, F_OK) != -1;
#endif
}

#if (_AXL_ENV == AXL_ENV_WIN)
inline
bool 
IsDir (const wchar_t* pFileName)
{
	dword_t Attributes = GetFileAttributesW (pFileName);
	return Attributes != INVALID_FILE_ATTRIBUTES && (Attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
#endif

bool
EnsureDirExists (const char* pFileName)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	char Buffer [256] = { 0 };
	rtl::CString_w FileName (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	FileName = pFileName;

	if (FileName.IsEmpty () || IsDir (FileName))
		return true;

	const wchar_t* p = FileName;
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

		if (!IsDir (FileName))
		{
			bool_t Result = CreateDirectoryW (FileName, NULL);
			if (!Result)
				return err::FailWithLastSystemError ();
		}

		*(wchar_t*) p2 = c; // restore

		p = p2 + 1;
		while (*p == '\\' || *p == '/') // skip separators
			p++;
	}

	return true;
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	error POSIX IsDir () is not yet implemented
#endif	
}

rtl::CString
GetFullFilePath (const char* pFileName)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	char Buffer [256];
	rtl::CString_w FileName (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	FileName = pFileName;

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
	char Buffer [256];
	rtl::CString_w FilePath (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	FilePath = pFilePath;

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
	char Buffer [256];
	rtl::CString_w FilePath (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	FilePath = pFilePath;

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
