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
		Size = (size_t) (pFile->GetSize () - Offset);

	g::TSystemInfo* pSystemInfo = g::GetModule ()->GetSystemInfo ();
	uint64_t ViewBegin = Offset - Offset % pSystemInfo->m_MappingAlignFactor;

	void* p;

#if (_AXL_ENV == AXL_ENV_WIN)
	uint_t Protection = (Flags & EFileFlag_ReadOnly) ? PAGE_READONLY : PAGE_READWRITE;
	uint_t Access = (Flags & EFileFlag_ReadOnly) ? FILE_MAP_READ : FILE_MAP_READ | FILE_MAP_WRITE;

	bool Result = m_Mapping.Create (pFile->m_File, NULL, Protection, Offset + Size);
	if (!Result)
		return NULL;

	p = m_View.View (m_Mapping, Access, Offset, Size);
	if (!p)
	{
		m_Mapping.Close ();
		return NULL;
	}
#elif (_AXL_ENV == AXL_ENV_POSIX)
	int Protection = (Flags & EFileFlag_ReadOnly) ? PROT_READ : PROT_READ | PROT_WRITE;

	p = m_Mapping.Map (NULL, Size, Protection, MAP_SHARED, pFile->m_File, Offset);
	if (!p)
		return NULL;
#endif

	m_p = (char*) p + Offset - ViewBegin;
	m_Size = Size;
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

	void* p;

#if (_AXL_ENV == AXL_ENV_WIN)
	uint_t Protection = (Flags & EFileFlag_ReadOnly) ? PAGE_READONLY : PAGE_READWRITE;
	uint_t Access = (Flags & EFileFlag_ReadOnly) ? FILE_MAP_READ : FILE_MAP_READ | FILE_MAP_WRITE;

	rtl::CString_utf16 Name = pName;

	bool Result = (Flags & EFileFlag_OpenExisting) ?
		m_Mapping.Open (Access, false, Name):
		m_Mapping.Create (INVALID_HANDLE_VALUE, NULL, Protection, Size, Name);

	if (!Result)
		return NULL;

	p = m_View.View (m_Mapping, Access, 0, Size);
	if (!p)
	{
		m_Mapping.Close ();
		return NULL;
	}
#elif (_AXL_ENV == AXL_ENV_POSIX)
	int ShmFlags = (Flags & EFileFlag_ReadOnly) ? O_RDONLY : O_RDWR;
	int Protection = (Flags & EFileFlag_ReadOnly) ? PROT_READ : PROT_READ | PROT_WRITE;

	if (!(Flags & EFileFlag_OpenExisting))
	{
		ShmFlags |= O_CREAT;
		m_SharedMemoryName = pName;
	}

	bool Result = m_SharedMemory.Open (pName, ShmFlags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (!Result)
		return NULL;

	p = m_Mapping.Map (NULL, Size, Protection, MAP_SHARED, m_SharedMemory, 0);
	if (!p)
	{
		m_SharedMemory.Close ();
		psx::CSharedMemory::Unlink (pName);
		return NULL;
	}

	if (!(Flags & EFileFlag_OpenExisting))
		m_SharedMemoryName = pName;
#endif

	m_p = p;
	m_Size = Size;
	return m_p;
}

void
CMapping::Close ()
{
	if (!IsOpen ())
		return;

#if (_AXL_ENV == AXL_ENV_WIN)
	m_Mapping.Close ();
	m_View.Close ();
#elif (_AXL_ENV == AXL_ENV_POSIX)
	m_SharedMemory.Close ();
	m_Mapping.Close ();

	if (!m_SharedMemoryName.IsEmpty())
	{
		psx::CSharedMemory::Unlink (m_SharedMemoryName);
		m_SharedMemoryName.Clear ();
	}
#endif

	m_p = NULL;
	m_Size = 0;
}

//.............................................................................

} // namespace axl {
} // namespace io {
