#pragma once

#include "axl_sl_Array.h"

namespace axl {
namespace spy {

//..............................................................................

#if (_AXL_OS_WIN)

class ModuleIterator
{
protected:
	size_t m_index;
	sl::ArrayRef<HMODULE> m_moduleArray;
	sl::String m_moduleFileName;

public:
    ModuleIterator()
	{
		m_index = -1;
	}

	ModuleIterator(const sl::ArrayRef<HMODULE>& moduleArray);

	operator bool ()
	{
		return m_index < m_moduleArray.getCount();
	}

    ModuleIterator&
    operator ++ ();

	ModuleIterator
	operator ++ (int);

	void*
	getModule()
	{
		return m_index < m_moduleArray.getCount() ? m_moduleArray[m_index] : NULL;
	}

	const sl::String&
	getModuleFileName()
	{
		return !m_moduleFileName.isEmpty() ? m_moduleFileName : prepareModuleFileName();
	}

protected:
	const sl::String&
	prepareModuleFileName();
};

#elif (_AXL_OS_LINUX)

class ModuleIterator
{
};

#elif (_AXL_OS_DARWIN)
#endif

bool
enumerateModules(ModuleIterator* iterator);

inline
ModuleIterator
enumerateModules()
{
	ModuleIterator iterator;
	enumerateModules(&iterator);
	return iterator;
}

//..............................................................................

} // namespace spy
} // namespace axl
