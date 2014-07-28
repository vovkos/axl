#include "pch.h"
#include "axl_io_Mapping.h"
#include "axl_g_Module.h"
#include "axl_rtl_String.h"

namespace axl {
namespace io {

//.............................................................................

void*
CMapping::Open (
	CFile* pFile,
	uint64_t Offset,
	size_t Size,
	uint_t Flags
	)
{
	Close ();

	if (Size == -1)
		Size = (size_t) pFile->GetSize ();

	g::TSystemInfo* pSystemInfo = g::GetModule ()->GetSystemInfo ();
	uint64_t ViewBegin = Offset - Offset % pSystemInfo->m_MappingAlignFactor;

#if (_AXL_ENV == AXL_ENV_WIN)
	uint_t Protection = (Flags & EFileFlag_ReadOnly) ? PAGE_READONLY : PAGE_READWRITE;
	uint_t Access = (Flags & EFileFlag_ReadOnly) ? FILE_MAP_READ : FILE_MAP_READ | FILE_MAP_WRITE;
	
	bool Result = m_Mapping.Create (pFile->m_File, NULL, Protection, Offset + Size);
	if (!Result)
		return NULL;

	void* p = m_View.View (m_Mapping, Access, Offset, Size);
	if (!p)
	{
		m_Mapping.Close ();
		return NULL;
	}

	m_p = (char*) p + Offset - ViewBegin;

#elif (_AXL_ENV == AXL_ENV_POSIX)
#	error POSIX CMapping not implemented yet
#endif

	return m_p;
}

void*
CMapping::Open (
	const char* pName,
	size_t Size,
	uint_t Flags
	)
{
	ASSERT (Size != 0 && Size != -1);

	Close ();

#if (_AXL_ENV == AXL_ENV_WIN)
	uint_t Protection = (Flags & EFileFlag_ReadOnly) ? PAGE_READONLY : PAGE_READWRITE;
	uint_t Access = (Flags & EFileFlag_ReadOnly) ? FILE_MAP_READ : FILE_MAP_READ | FILE_MAP_WRITE;
	
	rtl::CString_utf16 Name = pName;

	bool Result = (Flags & EFileFlag_OpenExisting) ?		
		m_Mapping.Open (Access, false, Name):
		m_Mapping.Create (INVALID_HANDLE_VALUE, NULL, Protection, Size, Name);

	if (!Result)
		return NULL;

	m_p = m_View.View (m_Mapping, Access, 0, Size);
	if (!m_p)
	{
		m_Mapping.Close ();
		return NULL;
	}

#elif (_AXL_ENV == AXL_ENV_POSIX)
#	error POSIX CMapping not implemented yet
#endif

	return m_p;
}

void
CMapping::Close ()
{
#if (_AXL_ENV == AXL_ENV_WIN)
	m_Mapping.Close ();
	m_View.Close ();
#elif (_AXL_ENV == AXL_ENV_POSIX)
	m_Mapping.Close ();
	m_MappingName.Close ();
#endif

	m_p = NULL;
}

//.............................................................................

} // namespace axl {
} // namespace io {
