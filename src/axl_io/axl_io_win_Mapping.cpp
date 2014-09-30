#include "pch.h"
#include "axl_io_win_Mapping.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

bool
Mapping::create (
	HANDLE hFile,
	SECURITY_ATTRIBUTES* secAttr,
	uint_t pageProtection,
	uint64_t _MaxSize,
	const wchar_t* name
	)
{
	close ();

	ULARGE_INTEGER maxSize;
	maxSize.QuadPart = _MaxSize;

	m_h = ::CreateFileMappingW (
		hFile, 
		secAttr, 
		pageProtection, 
		maxSize.HighPart, 
		maxSize.LowPart,
		name
		);

	return err::complete (m_h != NULL);
}

bool
Mapping::open (
	uint_t access,
	bool doInheritHandle,
	const wchar_t* name
	)
{
	close ();

	m_h = ::OpenFileMappingW (access, doInheritHandle, name);
	return err::complete (m_h != NULL);
}

//.............................................................................

void*
MappedView::view (
	HANDLE hMapping,
	uint_t access,
	uint64_t _Offset,
	uint32_t size
	)
{
	close ();

	ULARGE_INTEGER offset;
	offset.QuadPart = _Offset;

	m_h = ::MapViewOfFile (hMapping, access, offset.HighPart, offset.LowPart, size);
	if (!m_h)
	{
		err::failWithLastSystemError ();
		return NULL;
	}

	return m_h;
}

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl
