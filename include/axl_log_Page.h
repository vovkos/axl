// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_PAGE_H

#include "axl_log_IndexFile.h"
#include "axl_log_TextLine.h"
#include "axl_log_BinTextLine.h"
#include "axl_log_BinHexLine.h"
#include "axl_rtl_HashTable.h"
#include "axl_log_MergeCriteria.h"

namespace axl {
namespace log {

//.............................................................................

enum EPageFlag
{
	EPageFlag_Representing = 0x01,
	EPageFlag_Represented  = 0x02,
	EPageFlag_Colorizing   = 0x04,
	EPageFlag_Colorized    = 0x08,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CPage: public rtl::TListLink
{
	friend class CServer;
	friend class CWidget;
	friend class CPageRepresenterTarget;

	friend class CColorizeMgr;

protected:
	uint_t m_Flags;

	rtl::CArrayT <CLine*> m_LineArray;
	rtl::CArrayT <TPageVolatilePacket> m_VolatilePacketArray;
	
	size_t m_PacketOffset;
	size_t m_IndexNodeOffset;
	TIndexLeaf m_IndexLeaf;
	rtl::CArrayT <TIndexVolatilePacket> m_IndexVolatilePacketArray;

	size_t m_LongestTextLineLength;
	size_t m_LongestBinHexLineSize;
	size_t m_LongestBinTextLineSize;

	// last colorize final state

	size_t m_ColorizedLineCount;
	size_t m_ColorizerLine;
	size_t m_ColorizerOffset;
	intptr_t m_ColorizerState;
	ref::CPtrT <void> m_ColorizerStateEx;

public:
	CPage ();

	~CPage ()
	{
		Clear ();
	}

	uint_t
	GetFlags ()
	{
		return m_Flags;
	}
	
	bool
	IsEmpty ()
	{
		return m_LineArray.IsEmpty ();
	}

	rtl::CArrayT <CLine*>
	GetLineArray ()
	{
		return m_LineArray;
	}

	rtl::CArrayT <TPageVolatilePacket>
	GetVolatilePacketArray ()
	{
		return m_VolatilePacketArray;
	}

	size_t
	GetVolatilePacketIdx (const TPageVolatilePacket* pPacket);

	void
	Clear ();

	CBinLine*
	FindFirstBinLine (size_t i);

	CBinLine*
	FindFirstBinLine (CLine* pLine)
	{
		return FindFirstBinLine (pLine->GetLineIdx ());
	}

	void
	AddLines (
		size_t LineIdx,
		intptr_t LineCount
		);

	void
	FreeLines (
		size_t LineIdx,
		size_t LineCount
		);
		
	void
	ClearColorization ();
	
	size_t 
	GetVolatilePacketAnchorLineIdx (TPageVolatilePacket* pVolatilePacket);

	bool 
	SaveLines (
		rtl::CArrayT <uchar_t>* pBuffer,
		size_t LineIdx = 0
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {
