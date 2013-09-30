// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_LINE_H

#include "axl_gui_Font.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace log {

class CCachePage;

//.............................................................................

enum ELineAttrFlag
{
	ELineAttrFlag_TileIcon  = 0x01,
	ELineAttrFlag_Delimiter = 0x02,
	ELineAttrFlag_Override  = 0x04,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TLineAttr: gui::TTextAttr
{
	size_t m_IconIdx;
	uint_t m_Flags;

	TLineAttr ()
	{
		m_IconIdx = -1;
		m_Flags = 0;
	}
};

//.............................................................................

enum ELine
{
	ELine_Undefined = 0,
	ELine_Text,
	ELine_BinHex,
	ELine_BinText,
	
	ELine__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ELineFlag
{
	ELineFlag_FirstLineOfPacket = 0x01,
	ELineFlag_LastLineOfPacket  = 0x02,
	ELineFlag_MergedBackward    = 0x04,
	ELineFlag_MergedForward     = 0x08,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TLine
{
	ELine m_LineKind;
	uint_t m_Flags;
	uint32_t m_FoldablePacketIdx;
	TLineAttr m_LineAttr;	
	uint_t m_PartCode;
	uint64_t m_FirstPacketOffset;
	uint64_t m_FirstTimestamp;
	uint64_t m_LastTimestamp;
	uint64_t m_PartIdx;
};

//.............................................................................

struct TLongestLineLength
{
	size_t m_TextLineLength;
	size_t m_BinHexLineSize;
	size_t m_BinTextLineSize;

	TLongestLineLength ()
	{
		Clear ();
	}

	void
	Clear ()
	{
		memset (this, 0, sizeof ( TLongestLineLength));
	}
};

//.............................................................................

class CLine
{
	friend class CCachePage;
	friend class CCacheMgr;
	friend class CLineRepresenterTarget;
	friend class CLineRepresentMgr;
	friend class CWidget;

protected:
	ELine m_LineKind;

	uint_t m_Flags;

	CCachePage* m_pPage;
	size_t m_LineIdx;
	size_t m_FoldablePacketIdx;

	TLineAttr m_LineAttr;

	uint_t m_PartCode; 

	uint64_t m_FirstPacketOffset;
	uint64_t m_FirstTimestamp;
	uint64_t m_LastTimestamp;
	uint64_t m_PartIdx;

public:
	intptr_t m_UserParam;

public:
	CLine ();

	virtual 
	~CLine ()
	{
	}

	ELine
	GetLineKind ()
	{
		return m_LineKind;
	}

	uint_t 
	GetFlags ()
	{
		return m_Flags;
	}

	CCachePage*
	GetCachePage ()
	{
		return m_pPage;
	}

	size_t
	GetLineIdx ()
	{
		return m_LineIdx;
	}

	size_t
	GetFoldablePacketIdx ()
	{
		return m_FoldablePacketIdx;
	}

	bool
	IsBin ()
	{
		return m_LineKind == ELine_BinHex || m_LineKind == ELine_BinText;
	}

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
	UpdateLongestLineLength (TLongestLineLength* pLength) = 0;
};

//.............................................................................

} // namespace log {
} // namespace axl {
