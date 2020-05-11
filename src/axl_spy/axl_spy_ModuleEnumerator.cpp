#include "pch.h"
#include "axl_err_Error.h"
#include "axl_spy_ModuleEnumerator.h"

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

ModuleIterator
ModuleIterator::operator ++ (int)
{
	ModuleIterator it = *this;
	operator ++ ();
	return it;
}

const sl::String&
ModuleIterator::prepareModuleFileName()
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
#elif (_AXL_OS_DARWIN)
#endif

//..............................................................................

} // namespace spy
} // namespace axl
