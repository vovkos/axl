// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_BINLINE_H

#include "axl_log_Line.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace log {

//.............................................................................

struct TBinLinePart
{
	size_t m_Offset;
	uint64_t m_Timestamp;

	TBinLinePart (
		size_t Offset = 0,
		uint64_t Timestamp = 0
		)
	{
		m_Offset = Offset;
		m_Timestamp = Timestamp;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinLine: public CLine 
{
protected:
	friend class CColorizeMgr;
	friend class CCachePage;
	friend class CCacheRepresentorTarget;
	friend class CWidget;

	size_t m_Offset;
	rtl::CArrayT <uchar_t> m_BinData;
	rtl::CArrayT <TBinLinePart> m_PartArray;
	
	TBinDataConfig m_BinDataConfig;

	gui::CTextAttrAnchorArray m_RepresentorAttrArray; 
	gui::CTextAttrAnchorArray m_AttrArray;

public:
	CBinLine ()
	{
		m_LineKind = ELine_BinHex;
		m_Offset = 0;
	}

	size_t 
	GetSize ()
	{
		return m_LineKind == ELine_BinText ? 
			m_BinDataConfig.m_BinTextLineSize :
			m_BinDataConfig.m_BinHexLineSize;
	}

	rtl::CArrayT <uchar_t> 
	GetBinData ()
	{
		return m_BinData;
	}

	const TBinDataConfig*
	GetBinDataConfig ()
	{
		return &m_BinDataConfig;
	}

	void 
	AddData (
		uint64_t Timestamp,
		const gui::TTextAttr& Attr,
		const void* p, 
		size_t Size
		);

	void
	Colorize (
		const gui::TTextAttr& Attr,
		size_t OffsetStart,
		size_t OffsetEnd,
		size_t Metric
		);

	int
	CmpOffset (size_t Offset); // full offset

	TBinLinePart* // if NULL, then it's first part
	FindPart (size_t Offset); // full offset

	size_t
	GetBinHexLineOffset (
		size_t Col,
		size_t* pHexCol
		);

	bool
	GetBinLineOffset (
		size_t Col,
		size_t* pOffset,
		size_t* pLineOffset,
		size_t* pHexCol,
		size_t* pMergeId
		);
};

//.............................................................................

// because of the tabs col<->offset mapping is not linear

struct TLogBinTextMapEntry
{
	size_t m_Col;
	size_t m_Offset;

	TLogBinTextMapEntry (
		size_t Col = 0,
		size_t Offset = 0
		)
	{
		m_Col = Col;
		m_Offset = Offset;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinTextLine: public CBinLine 
{
protected:
	friend class CCachePage;
	friend class CCacheRepresentorTarget;
	friend class CWidget;

	rtl::CString m_BinText;
	rtl::CArrayT <TLogBinTextMapEntry> m_BinTextMap;

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
	AddData (
		uint64_t Timestamp,
		const gui::TTextAttr& Attr,
		const void* p, 
		size_t Size
		);

	size_t 
	FindOffsetByCol (size_t Col);

	size_t 
	FindColByOffset (size_t Offset);

	size_t
	GetBinTextLineOffset (size_t Col);
};

//.............................................................................

} // namespace log {
} // namespace axl {
