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

#pragma once

#define _AXL_SYS_MODULEENUMERATOR_H

#include "axl_sys_Lock.h"
#include "axl_sys_Event.h"

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

enum ModuleState {
	ModuleState_Unknown,
	ModuleState_Live,
	ModuleState_Loading,
	ModuleState_Unloading
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getModuleStateString(ModuleState state);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct ModuleDesc: sl::ListLink {
	sl::String m_name;
	size_t m_size;
	size_t m_useCount;
	size_t m_offset;
	ModuleState m_state;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
enumerateModules(sl::List<ModuleDesc>* moduleList);

bool
findModule(
	const sl::StringRef& name,
	ModuleDesc* moduleDesc = NULL
);

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
