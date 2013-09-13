// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_BINLINE_H

#include "axl_log_Line.h"
#include "axl_log_BinDataConfig.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace log {

//.............................................................................

struct TBinLinePart
{
	size_t m_Offset;
	uint64_t m_Timestamp;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TBinLine
{
	size_t m_BinOffset;
	TBinDataConfig m_BinDataConfig;
	size_t m_BinSize;
	size_t m_BinPartCount;
	size_t m_AttrAnchorCount;

	// followed by: 	
	// uint8_t m_BinData [m_BinSize];
	// TBinLinePart m_BinPartArray [m_BinPartCount];
	// TTextAttrAnchor m_OriginalAttrArray [m_AttrAnchorCount];
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBinLine: public CLine 
{
	friend class CPage;
	friend class CPageRepresenterTarget;
	friend class CWidget;

protected:
	size_t m_BinOffset;
	TBinDataConfig m_BinDataConfig;

	rtl::CArrayT <uchar_t> m_BinData;
	rtl::CArrayT <TBinLinePart> m_BinPartArray;

	gui::CTextAttrAnchorArray m_OriginalAttrArray; 
	gui::CTextAttrAnchorArray m_AttrArray;

public:
	CBinLine ()
	{
		m_BinOffset = 0;
	}

	size_t 
	GetBinOffset ()
	{
		return m_BinOffset;
	}

	int
	CmpBinOffset (size_t Offset); // full offset

	const TBinDataConfig*
	GetBinDataConfig ()
	{
		return &m_BinDataConfig;
	}

	virtual
	size_t 
	GetBinLineSize () = 0;

	rtl::CArrayT <uchar_t> 
	GetBinData ()
	{
		return m_BinData;
	}

	const TBinLinePart* // if NULL, then it's first part
	FindBinPart (size_t Offset); // line offset

	virtual
	size_t
	AddBinData (
		uint64_t Timestamp,
		const gui::TTextAttr& Attr,
		const void* p, 
		size_t Size
		) = 0;

	virtual
	void
	Colorize (
		const gui::TTextAttr& Attr,
		size_t OffsetStart,
		size_t OffsetEnd,
		size_t Metric
		) = 0;

	virtual 
	size_t
	Load (
		const void* p,
		size_t Size
		);

	virtual 
	size_t 
	Save (rtl::CArrayT <uint8_t>* pBuffer);
};

//.............................................................................

} // namespace log {
} // namespace axl {
