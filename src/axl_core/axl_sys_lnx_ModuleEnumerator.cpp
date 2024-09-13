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
#include "axl_sys_lnx_ModuleEnumerator.h"
#include "axl_io_MappedFile.h"

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

const char*
getModuleStateString(ModuleState state) {
	static const char* stringTable[] = {
		"Unknown",   // ModuleState_Unkown,
		"Live",      // ModuleState_Live,
		"Loading",   // ModuleState_Loading,
		"Unloading", // ModuleState_Unloading
	};

	return (size_t)state < countof(stringTable) ?
		stringTable[state] :
		stringTable[0];
}

inline
ModuleState
parseModuleState(const sl::StringRef& stateString) {
	return
		stateString.cmpIgnoreCase("Live") == 0 ? ModuleState_Live :
		stateString.cmpIgnoreCase("Loading") == 0 ? ModuleState_Loading :
		stateString.cmpIgnoreCase("Unloading") == 0 ? ModuleState_Unloading :
		ModuleState_Unknown;
}

template <typename Parser>
bool
parseProcModulesFile(Parser& parser) {
	io::File file;
	bool result = file.open("/proc/modules", io::FileFlag_ReadOnly);
	if (!result)
		return false;

	sl::Array<char> buffer;
	size_t size = io::readFile(&buffer, &file);
	if (size == -1)
		return false;

	const char* p = buffer.cp();
	const char* end = buffer.getEnd();
	const char* eol;

	for (; p < end; p = eol + 1) {
		eol = (char*)memchr(p, '\n', end - p);
		if (!eol)
			eol = end;

		char* next = (char*)memchr(p, ' ', eol - p);
		if (!next)
			continue;

		sl::StringRef name(p, next - p);
		if (!parser.checkModuleName(name))
			continue;

		p = next + 1;

		size_t size = strtoul(p, &next, 0);
		if (next == p || *next != ' ')
			continue;

		p = next + 1;
		size_t useCount = strtoul(p, &next, 0);
		if (next == p || *next != ' ')
			continue;

		p = next + 1;
		next = (char*)memchr(p, ' ', eol - p);
		if (!next)
			continue;

		p = next + 1;
		next = (char*)memchr(p, ' ', eol - p);
		if (!next)
			continue;

		ModuleState state = parseModuleState(sl::StringRef(p, next - p));
		if (!state)
			continue;

		p = next + 1;
		size_t offset = strtoul(p, &next, 0);
		if (next == p)
			continue;

		result = parser.processModule(name, size, useCount, state, offset);
		if (!result)
			break;
	}

	return true;
}

//..............................................................................

class EnumeratorModulesParser {
protected:
	sl::List<ModuleDesc>* m_moduleList;

public:
	EnumeratorModulesParser(sl::List<ModuleDesc>* moduleList) {
		m_moduleList = moduleList;
	}

	bool
	checkModuleName(const sl::StringRef& name) {
		return true;
	}

	bool
	processModule(
		const sl::StringRef& name,
		size_t size,
		size_t useCount,
		ModuleState state,
		size_t offset
	) {
		ModuleDesc* moduleDesc = new ModuleDesc;
		moduleDesc->m_name = name;
		moduleDesc->m_size = size;
		moduleDesc->m_useCount = useCount;
		moduleDesc->m_state = state;
		moduleDesc->m_offset = offset;
		m_moduleList->insertTail(moduleDesc);
		return true;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
enumerateModules(sl::List<ModuleDesc>* moduleList) {
	EnumeratorModulesParser parser(moduleList);
	bool result = parseProcModulesFile(parser);
	return result ? moduleList->getCount() : -1;
}

//..............................................................................

class FindModuleParser {
protected:
	sl::StringRef m_name;
	ModuleState m_state;
	ModuleDesc* m_moduleDesc;

public:
	FindModuleParser(
		const sl::StringRef& name,
		ModuleDesc* moduleDesc
	) {
		m_name = name;
		m_state = ModuleState_Unknown;
		m_moduleDesc = moduleDesc;
	}

	ModuleState
	getModuleState() const {
		return m_state;
	}

	bool
	checkModuleName(const sl::StringRef& name) {
		return name == m_name;
	}

	bool
	processModule(
		const sl::StringRef& name,
		size_t size,
		size_t useCount,
		ModuleState state,
		size_t offset
	) {
		ASSERT(name == m_name); // otherwise, we wouldn't be here

		if (m_moduleDesc) {
			m_moduleDesc->m_name = name;
			m_moduleDesc->m_size = size;
			m_moduleDesc->m_useCount = useCount;
			m_moduleDesc->m_state = state;
			m_moduleDesc->m_offset = offset;
		}

		m_state = state;
		return false; // don't continue
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
findModule(
	const sl::StringRef& name,
	ModuleDesc* moduleDesc
) {
	FindModuleParser parser(name, moduleDesc);
	bool result = parseProcModulesFile(parser);
	return parser.getModuleState() == ModuleState_Live;
}

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
