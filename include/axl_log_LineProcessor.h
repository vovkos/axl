// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_LINEPROCESSOR_H

#include "axl_log_BinLine.h"

namespace axl {
namespace log {

//.............................................................................

struct CProcessLine
{
	virtual 
	void 
	Process (
		CLine* pLine,
		CLine* pPrevLine,
		size_t StartCol, // if StartCol == -1 then it is NOT the first line in range
		size_t EndCol    // if EndCol == -1 then it is NOT the last line in range
		) = 0;
};

//.............................................................................

struct CProcessBinBlock
{
	virtual 
	void 
	Process (
		const void* p,
		size_t Size
		) = 0;
};

//.............................................................................

} // namespace log {
} // namespace axl {
