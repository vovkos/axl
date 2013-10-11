// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_HYPERLINKHANDLER_H

#include "axl_log_Line.h"

namespace axl {
namespace log {

//.............................................................................

struct CHyperlinkHandler
{
	virtual 
	bool 
	OnHyperlink (
		CLine* pLine, 
		const char* pHyperlink
		) = 0;
};

//.............................................................................

} // namespace log {
} // namespace axl {
