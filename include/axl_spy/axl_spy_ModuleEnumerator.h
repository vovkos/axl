#pragma once

#include "axl_sl_Array.h"
#if (_AXL_OS_DARWIN)
#	include "axl_sys_psx_DynamicLib.h"
#endif

namespace axl {
namespace spy {

//..............................................................................

#if (_AXL_OS_WIN)

class ModuleIterator
{
protected:
	mutable sl::String m_moduleFileName;
	sl::ArrayRef<HMODULE> m_moduleArray;
	size_t m_index;

public:
    ModuleIterator()
	{
		m_index = -1;
	}

	ModuleIterator(const sl::ArrayRef<HMODULE>& moduleArray);

	operator bool () const
	{
		return m_index < m_moduleArray.getCount();
	}

    ModuleIterator&
    operator ++ ();

	ModuleIterator
	operator ++ (int);

	void*
	getModule() const
	{
		return m_index < m_moduleArray.getCount() ? m_moduleArray[m_index] : NULL;
	}

	const sl::String&
	getModuleFileName() const
	{
		return !m_moduleFileName.isEmpty() ? m_moduleFileName : prepareModuleFileName();
	}

protected:
	const sl::String&
	prepareModuleFileName() const;
};

#elif (_AXL_OS_LINUX)

class ModuleIterator
{
protected:
	mutable sl::StringRef m_moduleFileName;
	link_map* m_linkMap;

public:
	ModuleIterator()
	{
		m_linkMap = NULL;
	}

	ModuleIterator(link_map* linkMap)
	{
		m_linkMap = linkMap;
	}

	operator bool () const
	{
		return m_linkMap != NULL;
	}

	ModuleIterator&
	operator ++ ();

	ModuleIterator
	operator ++ (int);

	void*
	getModule() const
	{
		return m_linkMap;
	}

	const sl::StringRef&
	getModuleFileName() const
	{
		return !m_moduleFileName.isEmpty() ? m_moduleFileName : prepareModuleFileName();
	}

protected:
	const sl::StringRef&
	prepareModuleFileName() const;
};

#elif (_AXL_OS_DARWIN)

class ModuleIterator
{
protected:
	mutable sys::psx::DynamicLib m_module;
	mutable sl::StringRef m_moduleFileName;
	size_t m_count;
	size_t m_index;

public:
	ModuleIterator(size_t count = 0);

	operator bool ()
	{
		return m_index < m_count;
	}

	ModuleIterator&
	operator ++ ();

	ModuleIterator
	operator ++ (int);

	void*
	getModule() const
	{
		return m_module.isOpen() ? (void*)m_module : prepareModule();
	}

	const sl::StringRef&
	getModuleFileName() const
	{
		return !m_moduleFileName.isEmpty() ? m_moduleFileName : prepareModuleFileName();
	}

	size_t
	getImageIndex() const
	{
		return m_index;
	}

protected:
	void*
	prepareModule() const;

	const sl::StringRef&
	prepareModuleFileName() const;
};

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
