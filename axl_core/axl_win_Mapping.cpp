#include "stdafx.h"
#include "axl_win_Mapping.h"
#include "axl_err_Error.h"
#include "axl_win_CriticalSection.h"

namespace axl {
namespace win {

//.............................................................................

bool
CMapping::Create (
	HANDLE hFile,
	SECURITY_ATTRIBUTES* pSecAttr,
	ulong_t PageProtection,
	uint64_t _MaxSize,
	const tchar_t* pName
	)
{
	Close ();

	ULARGE_INTEGER MaxSize;
	MaxSize.QuadPart = _MaxSize;

	m_h = ::CreateFileMapping (
		hFile, 
		pSecAttr, 
		PageProtection, 
		MaxSize.HighPart, 
		MaxSize.LowPart,
		pName
		);

	return err::Complete (m_h != NULL);
}

bool
CMapping::Open (
	ulong_t Access,
	bool DoInheritHandle,
	const tchar_t* pName
	)
{
	Close ();

	m_h = ::OpenFileMapping (Access, DoInheritHandle, pName);
	return err::Complete (m_h != NULL);
}

//.............................................................................

void*
CMappedView::View (
	HANDLE hMapping,
	ulong_t Access,
	uint64_t _Offset,
	ulong_t Size
	)
{
	Close ();

	ULARGE_INTEGER Offset;
	Offset.QuadPart = _Offset;

	m_h = ::MapViewOfFile (hMapping, Access, Offset.HighPart, Offset.LowPart, Size);
	if (!m_h)
	{
		err::FailWithLastSystemError ();
		return NULL;
	}

	return m_h;
}

//.............................................................................

} // namespace win
} // namespace axl
