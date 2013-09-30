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
	friend class CCachePage;
	friend class CCacheMgr;
	friend class CLineRepresenterTarget;
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
		uint64_t BeginOffset,
		uint64_t EndOffset,
		const gui::TTextAttr& Attr
		);

	virtual 
	void
	UpdateLongestLineLength (TLongestLineLength* pLength);
};

//.............................................................................

} // namespace log {
} // namespace axl {
