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

class CCacheMgr;

//.............................................................................

struct TFoldablePacket
{ 
	uint64_t m_FoldFlags;
	uint64_t m_Offset;
	size_t m_LineCount;
};

//.............................................................................

enum EPageFlag
{
	EPageFlag_Representing = 0x01,
	EPageFlag_Represented  = 0x02,
	EPageFlag_Colorizing   = 0x04,
	EPageFlag_Colorized    = 0x08,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCachePage: public rtl::TListLink
{
	friend class CServerPeer;
	friend class CCacheMgr;
	friend class CWidget;
	friend class CLineRepresenterTarget;

	friend class CColorizeMgr;

protected:
	CCacheMgr* m_pCacheMgr;

	uint_t m_Flags;

	TIndexLeaf m_IndexLeaf;
	rtl::CArrayT <uint64_t> m_FoldFlagArray;
	rtl::CArrayT <TFoldablePacket> m_FoldablePacketArray;

	rtl::CArrayT <CLine*> m_LineArray;

public:
	CCachePage ();

	~CCachePage ()
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

	rtl::CArrayT <TFoldablePacket>
	GetFoldablePacketArray ()
	{
		return m_FoldablePacketArray;
	}

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
	LoadLines (
		size_t LineIdx,
		size_t LineCount,
		const void* pLineBuffer,
		size_t LineBufferSize,
		TLongestLineLength* pLength,
		bool* pIsRetroColorized
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {
