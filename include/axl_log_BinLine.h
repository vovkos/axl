// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_BINLINE_H

#include "axl_log_Line.h"
#include "axl_log_BinDataConfig.h"
#include "axl_rtl_Array.h"
#include "axl_gui_TextAttrAnchorArray.h"

namespace axl {
namespace log {

//.............................................................................

struct TBinLinePart
{
	uint64_t m_Offset;
	uint64_t m_Timestamp;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TBinLineColorizedPart
{
	uint64_t m_BeginOffset;
	uint64_t m_EndOffset;
	gui::TTextAttr m_Attr;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TBinLine
{
	uint64_t m_BinOffset;
	TBinDataConfig m_BinDataConfig;
	size_t m_BinSize;
	size_t m_BinPartCount;
	size_t m_AttrAnchorCount;
	size_t m_RetroColorizedPartCount;

	// followed by: 	
	// uint8_t m_BinData [m_BinSize];
	// TBinLinePart m_BinPartArray [m_BinPartCount];
	// TTextAttrAnchor m_AttrArray [m_AttrAnchorCount];
	// TBinLineColorizerAttr m_RetroColorizedPartArray [m_RetroColorizedPartCount];
};

//.............................................................................

class CBinLine: public CLine 
{
	friend class CCachePage;
	friend class CCacheMgr;
	friend class CLineRepresenterTarget;
	friend class CWidget;

protected:
	uint64_t m_BinOffset;
	TBinDataConfig m_BinDataConfig;

	rtl::CArrayT <uchar_t> m_BinData;
	rtl::CArrayT <TBinLinePart> m_BinPartArray;
	gui::CTextAttrAnchorArray m_AttrArray;
	rtl::CArrayT <TBinLineColorizedPart> m_RetroColorizedPartArray; 

public:
	CBinLine ();

	uint64_t 
	GetBinOffset ()
	{
		return m_BinOffset;
	}

	int
	CmpBinOffset (uint64_t Offset)
	{
		return 
			Offset < m_BinOffset ? -1 :
			Offset > m_BinOffset + m_BinData.GetCount () ? 1 : 0;
	}

	bool
	DoesBinRangeOverlap (
		uint64_t BeginOffset,
		uint64_t EndOffset
		)
	{
		return 
			BeginOffset < m_BinOffset + m_BinData.GetCount () && 
			EndOffset > m_BinOffset;
	}

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

	bool	
	HasRetroColorizedParts ()
	{
		return !m_RetroColorizedPartArray.IsEmpty ();
	}

	void 
	AddRetroColorizedPart (
		uint64_t BeginOffset,
		uint64_t EndOffset,
		const gui::TTextAttr& Attr
		);

	void 
	ApplyRetroColorization (
		const TBinLineColorizedPart* pPartArray,
		size_t PartCount
		);

	virtual
	void
	Colorize (
		uint64_t BeginOffset,
		uint64_t EndOffset,
		const gui::TTextAttr& Attr
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

	virtual 
	void
	Clear ();
};

//.............................................................................

} // namespace log {
} // namespace axl {
