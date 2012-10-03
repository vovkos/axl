// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_UPDATEPEAK_H

namespace axl {
namespace rtl {

//.............................................................................

inline
void
InterlockedUpdatePeak (
	volatile long* pPeak,
	long Current
	)
{
	for (;;)
	{
		long Old = *pPeak;
		
		if (Current <= Old)
			break;

		if (::InterlockedCompareExchange (pPeak, Current, Old) == Old)
			break;
	}
}

//.............................................................................

} // namespace rtl
} // namespace axl
