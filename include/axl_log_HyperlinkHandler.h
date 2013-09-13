// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_HYPERLINKHANDLER_H

#include "axl_log_Line.h"

namespace axl {
namespace log {

//.............................................................................

struct IHyperlinkHandler: public obj::IRoot
{
	// {3BB64985-06A6-4128-88D0-2BF6086D34B3}
	AXL_OBJ_INTERFACE (
		IHyperlinkHandler,
		0x3bb64985, 0x06a6, 0x4128, 0x88, 0xd0, 0x2b, 0xf6, 0x08, 0x6d, 0x34, 0xb3
		)

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
