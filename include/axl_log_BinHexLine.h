// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_BINHEXLINE_H

#include "axl_log_BinLine.h"

namespace axl {
namespace log {

//.............................................................................

class CBinHexLine: public CBinLine 
{
	friend class CPage;
	friend class CPageRepresenterTarget;
	friend class CWidget;

public:
	CBinHexLine ()
	{
		m_LineKind = ELine_BinHex;
	}

	virtual
	size_t 
	GetBinLineSize ()
	{
		return GetBinDataConfig ()->m_BinHexLineSize;
	}

	virtual
	size_t
	AddBinData (
		uint64_t Timestamp,
		const gui::TTextAttr& Attr,
		const void* p, 
		size_t Size
		);

	virtual
	void
	Colorize (
		const gui::TTextAttr& Attr,
		size_t OffsetStart,
		size_t OffsetEnd,
		size_t Metric
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {
