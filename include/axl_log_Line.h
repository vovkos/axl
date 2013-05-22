// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_LINE_H

#include "axl_log_BinDataConfig.h"
#include "axl_gui_HyperText.h"

namespace axl {
namespace log {

class CCachePage;
class CWidget;
struct TCacheVolatilePacket;

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

//.............................................................................

class CLine
{
protected:
	friend class CColorizeMgr;
	friend class CCacheMgr;
	friend class CCachePage;
	friend class CCacheRepresentorTarget;
	friend class CWidget;

	ELine m_LineKind;

	CCachePage* m_pPage;
	size_t m_PageIndex;

	TCacheVolatilePacket* m_pVolatilePacket;

	TLineAttr m_LineAttr;

	bool_t m_IsFirstLineOfPacket;
	size_t m_FirstPacketOffset;
	uint64_t m_FirstTimestamp;
	uint64_t m_LastTimestamp;

	uint_t m_PartCode; 
	size_t m_PartIndex;
	size_t m_MergeId; // all the lines in the same merged block have the same merge id

public:
	intptr_t m_UserParam;

public:
	CLine ();

	virtual ~CLine ()
	{
	}

	ELine
	GetLineKind ()
	{
		return m_LineKind;
	}

	CCachePage*
	GetCachePage ()
	{
		return m_pPage;
	}

	CWidget*
	GetWidget ();

	size_t
	GetCachePageIndex ()
	{
		return m_PageIndex;
	}

	bool
	IsBin ()
	{
		return m_LineKind == ELine_BinHex || m_LineKind == ELine_BinText;
	}

	bool
	IsMerged (CLine* pNextLine)
	{
		return 
			m_pVolatilePacket == pNextLine->m_pVolatilePacket &&
			m_MergeId == pNextLine->m_MergeId;
	}
};

//.............................................................................

class CTextLine: public CLine
{
protected:
	friend class CCacheRepresentorTarget;
	friend class CWidget;

	gui::CHyperText m_HyperText;

public:
	CTextLine ()
	{
		m_LineKind = ELine_Text;
	}

	const gui::CHyperText*
	GetHyperText ()
	{
		return &m_HyperText;
	}
};

//.............................................................................

} // namespace log {
} // namespace axl {
