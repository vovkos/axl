#pragma once

#include "axl_g_Pch.h"

namespace axl {
namespace spy {

//..............................................................................

class ImportTableMgr
{
public:
	size_t
	enumerateImports(void* module = NULL);
};

//..............................................................................

} // namespace spy
} // namespace axl
