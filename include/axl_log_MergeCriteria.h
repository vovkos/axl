// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_MERGECRITERIA_H

#include "axl_log_Line.h"
#include "axl_log_BinDataConfig.h"

namespace axl {
namespace log {

//.............................................................................

struct TMergeCriteria
{
	ELine m_LineKind;
	uint_t m_PartCode;
	uint64_t m_Timestamp;
	TBinDataConfig m_BinDataConfig;

	TMergeCriteria ()
	{
		Clear ();
	}

	bool
	IsEmpty () const
	{
		return m_LineKind == ELine_Undefined;
	}

	void
	Break ()
	{
		m_LineKind = ELine_Undefined;
	}

	void 
	Clear ();

	bool 
	ShouldMerge (const TMergeCriteria& Criteria2) const;
};

//.............................................................................

} // namespace log
} // namespace axl
