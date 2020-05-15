#include "pch.h"
#include "axl_spy_ModuleEnumerator.h"
#include "axl_io_FilePathUtils.h"

namespace axl {
namespace spy {

//..............................................................................

#if (_AXL_OS_WIN)

ModuleIterator::ModuleIterator(const sl::ArrayRef<HMODULE>& moduleArray)
{
	m_moduleArray = moduleArray;
	m_index = 0;
}

ModuleIterator&
ModuleIterator::operator ++ ()
{
	if (m_index >= m_moduleArray.getCount())
		return *this;

	m_index++;
	m_moduleFileName.clear();
	return *this;
}

const sl::String&
ModuleIterator::prepareModuleFileName() const
{
	ASSERT(m_moduleFileName.isEmpty());

	if (m_index >= m_moduleArray.getCount())
		return m_moduleFileName;

	wchar_t fileName[1024];
	fileName[countof(fileName) - 1] = 0;
	::GetModuleFileNameW(m_moduleArray[m_index], fileName, countof(fileName) - 1);
	m_moduleFileName = fileName;
	return m_moduleFileName;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
enumerateModules(ModuleIterator* iterator)
{
	sl::Array<HMODULE> moduleArray;

	for (;;)
	{
		size_t currentCount = moduleArray.getCount();
		dword_t requiredSize;

		::EnumProcessModules(
			::GetCurrentProcess(),
			moduleArray.p(),
			currentCount * sizeof(HMODULE),
			&requiredSize
			);

		size_t requiredCount = requiredSize / sizeof(HMODULE);
		if (requiredCount <= currentCount)
			break;

		bool result = moduleArray.setCount(requiredCount);
		if (!result)
			return false;
	}

	*iterator = ModuleIterator(moduleArray);
	return true;
}

#elif (_AXL_OS_LINUX)

ModuleIterator&
ModuleIterator::operator ++ ()
{
	if (m_linkMap)
	{
		m_linkMap = m_linkMap->l_next;
		m_moduleFileName.clear();
	}

	return *this;
}

const sl::StringRef&
ModuleIterator::prepareModuleFileName() const
{
	if (!m_linkMap)
		return m_moduleFileName;

	m_moduleFileName = m_linkMap->l_name;

	if (m_moduleFileName.isEmpty())
		m_moduleFileName = io::getExeFilePath();

	return m_moduleFileName;
}

bool
enumerateModules(ModuleIterator* iterator)
{
	*iterator = ModuleIterator(_r_debug.r_map);
	return true;
}

#elif (_AXL_OS_DARWIN)

ModuleIterator::ModuleIterator(size_t count)
{
	m_count = count;
	m_index = 0;
}

ModuleIterator&
ModuleIterator::operator ++ ()
{
	if (m_index >= m_count)
		return *this;

	m_index++;
	m_moduleFileName.clear();
	return *this;
}

void*
ModuleIterator::prepareModule() const
{
	ASSERT(!m_module.isOpen());

	if (m_index < m_count)
		m_module.open(getModuleFileName());

	return m_module;
}

const sl::StringRef&
ModuleIterator::prepareModuleFileName() const
{
	ASSERT(m_moduleFileName.isEmpty());

	if (m_index < m_count)
		m_moduleFileName = ::_dyld_get_image_name(m_index);

	return m_moduleFileName;
}

bool
enumerateModules(ModuleIterator* iterator)
{
	*iterator = ModuleIterator(_dyld_image_count());
	return true;
}

#endif

//..............................................................................

ModuleIterator
ModuleIterator::operator ++ (int)
{
	ModuleIterator it = *this;
	operator ++ ();
	return it;
}

//..............................................................................

} // namespace spy
} // namespace axl
