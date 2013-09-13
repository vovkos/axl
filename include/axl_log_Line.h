// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_LINE_H

#include "axl_gui_HyperText.h"

namespace axl {
namespace log {

class CPage;

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

enum ELineDelimiter
{
	ELineDelimiter_None = 0,
	ELineDelimiter_Light,
	ELineDelimiter_Normal,
	ELineDelimiter_Bold,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ELineAttrFlag
{
	ELineAttrFlag_TileIcon = 0x01,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TLineAttr: gui::TTextAttr
{
	uint_t m_Flags;
	size_t m_Icon;

	ELineDelimiter m_UpperDelimiter;
	ELineDelimiter m_LowerDelimiter;

	TLineAttr ();
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TPageVolatilePacket
{ 
	size_t m_Offset;
	uint_t m_Code;
	uint64_t m_Timestamp;
	size_t m_DataSize; 
	size_t m_VolatileIdx;
	uint_t m_VolatileFlags;
	size_t m_FirstLineIdx;
	size_t m_LineCount;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TLine
{
	ELine m_LineKind;
	TLineAttr m_LineAttr;
	TPageVolatilePacket m_VolatilePacket;

	bool_t m_IsFirstLineOfPacket;
	size_t m_FirstPacketOffset;
	uint64_t m_FirstTimestamp;
	uint64_t m_LastTimestamp;

	uint_t m_PartCode; 
	size_t m_PartIdx;
	size_t m_MergeId; // all the lines in the same merged block have the same merge id
};

//.............................................................................

class CLine
{
	friend class CPage;
	friend class CPageRepresenterTarget;
	friend class CWidget;

protected:
	ELine m_LineKind;

	CPage* m_pPage;
	size_t m_LineIdx;
	size_t m_VolatilePacketIdx;

	TLineAttr m_LineAttr;

	bool m_IsFirstLineOfPacket;
	size_t m_FirstPacketOffset;
	uint64_t m_FirstTimestamp;
	uint64_t m_LastTimestamp;

	uint_t m_PartCode; 
	size_t m_PartIdx;
	size_t m_MergeId; // all the lines in the same merged block have the same merge id

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

	CPage*
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
	GetVolatilePacketIdx ()
	{
		return m_VolatilePacketIdx;
	}

	bool
	IsBin ()
	{
		return m_LineKind == ELine_BinHex || m_LineKind == ELine_BinText;
	}

	bool
	IsMerged (CLine* pNextLine)
	{
		return m_MergeId != -1 && m_MergeId == pNextLine->m_MergeId;
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
};

//.............................................................................

} // namespace log {
} // namespace axl {
