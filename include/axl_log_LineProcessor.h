// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_LINEPROCESSOR_H

#include "axl_log_BinLine.h"

namespace axl {
namespace log {

//.............................................................................

struct IProcessLine: public obj::IRoot
{
	// {1484DEE4-2F12-4145-99B6-33853C734B3A}
	AXL_OBJ_INTERFACE (
		IProcessLine,
		0x1484dee4, 0x2f12, 0x4145, 0x99, 0xb6, 0x33, 0x85, 0x3c, 0x73, 0x4b, 0x3a
		)

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

struct IProcessBinBlock: public obj::IRoot
{
	// {E652545A-AA16-4A8E-953F-55D63A80A18F}
	AXL_OBJ_INTERFACE (
		IProcessBinBlock,
		0xe652545a, 0xaa16, 0x4a8e, 0x95, 0x3f, 0x55, 0xd6, 0x3a, 0x80, 0xa1, 0x8f
		)

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
