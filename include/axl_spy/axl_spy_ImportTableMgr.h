#pragma once

#include "axl_g_Pch.h"

namespace axl {
namespace spy {

//..............................................................................

class ImportTableMgr
{
public:
	size_t
	enumerateImports(HMODULE hModule = NULL);
};

//..............................................................................

} // namespace spy
} // namespace axl
