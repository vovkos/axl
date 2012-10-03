#include "stdafx.h"
#include "axl_io_FilePathUtils.h"
#include "axl_win_File.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

rtl::CString
GetFullFilePath (const tchar_t* pFileName)
{
	size_t Length = GetFullPathName (pFileName, 0, NULL, NULL);
	if (!Length)
	{
		err::SetLastSystemError ();
		return rtl::CString ();
	}

	rtl::CString FilePath;
	tchar_t* p = FilePath.GetBuffer (Length);
	GetFullPathName (pFileName, Length, p, NULL);
	return FilePath;
}

rtl::CString
GetDirName (const tchar_t* pFilePath)
{
	char Drive [1024];
	char DirName [1024];

	_splitpath (pFilePath, Drive, DirName, NULL, NULL);
	rtl::CString String = Drive;
	String.Append (DirName);
	return String;
}

rtl::CString
GetFileName (const tchar_t* pFilePath)
{
	char FileName [1024];
	char Extension [1024];

	_splitpath (pFilePath, NULL, NULL, FileName, Extension);
	rtl::CString String = FileName;
	String.Append (Extension);
	return String;
}

rtl::CString
GetExtension (const tchar_t* pFilePath)
{
	char Extenstion [1024];

	_splitpath (pFilePath, NULL, NULL, NULL, Extenstion);
	return Extenstion;
}

rtl::CString
ConcatFilePath (
	rtl::CString* pFilePath,
	const tchar_t* pFileName
	)
{
	if (pFilePath->IsEmpty ())
	{
		*pFilePath = pFileName;
		return *pFilePath;
	}

	tchar_t Last = pFilePath->GetBack ();
	if (Last != _T('\\') && Last != _T('/'))
		pFilePath->Append (_T('\\'));

	pFilePath->Append (pFileName);
	return *pFilePath;
}

rtl::CString 
FindFilePath (
	const tchar_t* pFileName,
	const tchar_t* pFirstDirName,
	const rtl::CBoxListT <rtl::CString>* pDirNameList,
	bool DoFindInCurrentDir
	)
{
	rtl::CString FilePath;

	if (pFirstDirName)
	{
		FilePath = ConcatFilePath (pFirstDirName, pFileName);
		if (win::CFile::DoesFileExist (FilePath))
			return GetFullFilePath (FilePath);
	}

	if (DoFindInCurrentDir)
		if (win::CFile::DoesFileExist (pFileName))
			return GetFullFilePath (pFileName);

	if (pDirNameList)
	{
		rtl::CBoxIteratorT <rtl::CString> DirName = pDirNameList->GetHead ();
		for (; DirName; DirName++)
		{
			FilePath.ForceCopy (*DirName);
			ConcatFilePath (&FilePath, pFileName);
			if (win::CFile::DoesFileExist (FilePath))
				return GetFullFilePath (FilePath);
		}
	}

	return rtl::CString ();
}

//.............................................................................

} // namespace io
} // namespace axl

