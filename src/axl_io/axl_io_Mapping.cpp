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
#include "axl_io_Mapping.h"

namespace axl {
namespace io {

//..............................................................................

void*
Mapping::open (
	File* file,
	uint64_t offset,
	size_t size,
	uint_t flags
	)
{
	close ();

	if (size == -1)
		size = (size_t) (file->getSize () - offset);

	const g::SystemInfo* systemInfo = g::getModule ()->getSystemInfo ();
	uint64_t viewBegin = offset - offset % systemInfo->m_mappingAlignFactor;
	uint64_t viewEnd = offset + size;

	void* p;

#if (_AXL_OS_WIN)
	uint_t protection = (flags & FileFlag_ReadOnly) ? PAGE_READONLY : PAGE_READWRITE;
	uint_t access = (flags & FileFlag_ReadOnly) ? FILE_MAP_READ : FILE_MAP_READ | FILE_MAP_WRITE;

	bool result = m_mapping.create (file->m_file, NULL, protection, viewEnd);
	if (!result)
		return NULL;

	p = m_view.view (m_mapping, access, viewBegin, (size_t) (viewEnd - viewBegin));
	if (!p)
	{
		m_mapping.close ();
		return NULL;
	}
#elif (_AXL_OS_POSIX)
	int protection = (flags & FileFlag_ReadOnly) ? PROT_READ : PROT_READ | PROT_WRITE;

	p = m_mapping.map (NULL, viewEnd - viewBegin, protection, MAP_SHARED, file->m_file, viewBegin);
	if (!p)
		return NULL;
#endif

	m_p = (char*) p + offset - viewBegin;
	m_size = size;
	return m_p;
}

void*
Mapping::open (
	const sl::StringRef& name,
	size_t size,
	uint_t flags
	)
{
	ASSERT (size != -1);

	close ();

	void* p;

#if (_AXL_OS_WIN)
	uint_t protection = (flags & FileFlag_ReadOnly) ? PAGE_READONLY : PAGE_READWRITE;
	uint_t access = (flags & FileFlag_ReadOnly) ? FILE_MAP_READ : FILE_MAP_READ | FILE_MAP_WRITE;

	sl::String_w name_w = name;

	bool result = (flags & FileFlag_OpenExisting) ?
		m_mapping.open (access, false, name_w):
		m_mapping.create (INVALID_HANDLE_VALUE, NULL, protection, size, name_w);

	if (!result)
		return NULL;

	p = m_view.view (m_mapping, access, 0, size);
	if (!p)
	{
		m_mapping.close ();
		return NULL;
	}
#elif (_AXL_OS_POSIX)
	int shmFlags = (flags & FileFlag_ReadOnly) ? O_RDONLY : O_RDWR;
	int protection = (flags & FileFlag_ReadOnly) ? PROT_READ : PROT_READ | PROT_WRITE;

	if (!(flags & FileFlag_OpenExisting))
		shmFlags |= O_CREAT;

	bool result = m_sharedMemory.open (name, shmFlags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (!result)
		return NULL;

	if (!(flags & FileFlag_OpenExisting) && m_sharedMemory.getSize () < size)
	{
		result = m_sharedMemory.setSize (size);
		if (!result)
		{
			if (!(flags & FileFlag_OpenExisting))
				psx::SharedMemory::unlink (name);

			return NULL;
		}
	}

	p = m_mapping.map (NULL, size, protection, MAP_SHARED, m_sharedMemory, 0);
	if (!p)
	{
		if (!(flags & FileFlag_OpenExisting))
			psx::SharedMemory::unlink (name);

		return NULL;
	}

	if (!(flags & FileFlag_OpenExisting))
		m_sharedMemoryName = name;

#endif

	m_p = p;
	m_size = size;
	return m_p;
}

void
Mapping::close ()
{
#if (_AXL_OS_WIN)
	m_mapping.close ();
	m_view.close ();
#elif (_AXL_OS_POSIX)
	m_sharedMemory.close ();
	m_mapping.close ();

	if (!m_sharedMemoryName.isEmpty())
	{
		psx::SharedMemory::unlink (m_sharedMemoryName);
		m_sharedMemoryName.clear ();
	}
#endif

	m_p = NULL;
	m_size = 0;
}

//..............................................................................

} // namespace io
} // namespace axl
