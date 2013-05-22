#include "pch.h"
#include "axl_io_win_Mapping.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

bool
CMapping::Create (
	HANDLE hFile,
	SECURITY_ATTRIBUTES* pSecAttr,
	uint_t PageProtection,
	uint64_t _MaxSize,
	const wchar_t* pName
	)
{
	Close ();

	ULARGE_INTEGER MaxSize;
	MaxSize.QuadPart = _MaxSize;

	m_h = ::CreateFileMappingW (
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
	uint_t Access,
	bool DoInheritHandle,
	const wchar_t* pName
	)
{
	Close ();

	m_h = ::OpenFileMappingW (Access, DoInheritHandle, pName);
	return err::Complete (m_h != NULL);
}

//.............................................................................

void*
CMappedView::View (
	HANDLE hMapping,
	uint_t Access,
	uint64_t _Offset,
	uint32_t Size
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
} // namespace io
} // namespace axl
