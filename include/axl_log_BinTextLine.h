// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_BINTEXTLINE_H

#include "axl_log_BinLine.h"
#include "axl_rtl_String.h"

namespace axl {
namespace log {

//.............................................................................

// because of the tabs col<->offset mapping is not linear

struct TBinTextMapEntry
{
	size_t m_Col;
	size_t m_Offset;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TBinTextLine
{
	size_t m_BinTextLength;
	size_t m_BinTextMapEntryCount;

	// followed by: 	
	// char m_BinText [m_BinTextLength];
	// TBinTextMapEntry m_BinPartArray [m_BinTextMapEntryCount];
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinTextLine: public CBinLine 
{
	friend class CCachePage;
	friend class CCacheMgr;
	friend class CLineRepresenterTarget;
	friend class CWidget;

protected:
	rtl::CString m_BinText;
	rtl::CArrayT <TBinTextMapEntry> m_BinTextMap;

public:
	CBinTextLine ()
	{
		m_LineKind = ELine_BinText;
	}

	rtl::CString 
	GetBinText ()
	{
		return m_BinText;
	}

	size_t 
	FindOffsetByBinTextCol (size_t Col);

	size_t 
	FindBinTextColByOffset (size_t Offset);

	virtual
	size_t 
	GetBinLineSize ()
	{
		return GetBinDataConfig ()->m_BinTextLineSize;
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
	size_t
	Load (
		const void* p,
		size_t Size
		);

	virtual 
	size_t 
	Save (rtl::CArrayT <uint8_t>* pBuffer);

	virtual 
	void
	Clear ();

	virtual 
	void
	UpdateLongestLineLength (TLongestLineLength* pLength);
};

//.............................................................................

} // namespace log {
} // namespace axl {
