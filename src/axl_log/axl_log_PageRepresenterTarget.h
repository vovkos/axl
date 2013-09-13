// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_PAGEREPRESENTERTARGET_H

#include "axl_log_Representer.h"
#include "axl_log_MergeCriteria.h"
#include "axl_log_Page.h"

namespace axl {
namespace log {

//.............................................................................

class CPageRepresenterTarget: public IRepresenterTarget 
{
	friend class CServer;

public:
	AXL_OBJ_CLASS_0 (CPageRepresenterTarget, IRepresenterTarget);

protected:
	CPage* m_pPage;
	CLine* m_pPrevPageMergeLine;

	TMergeCriteria m_MergeCriteria;

	size_t m_VolatilePacketIdx;
	bool m_IsFirstLineOfPacket;
	
	uint64_t m_Timestamp;

	size_t m_PacketOffset;
	size_t m_BinOffset;
	size_t m_PartIdx;	
	size_t m_MergeId;
	
	size_t m_LongestTextLineLength;
	size_t m_LongestBinHexLineSize;
	size_t m_LongestBinTextLineSize;
	bool_t m_IsFull;
	size_t m_NextLineIdx;
	size_t m_EndLineIdx;

public:
	CPageRepresenterTarget ();

	~CPageRepresenterTarget ();

	virtual 
	void 
	AddPart (
		EPart PartKind,
		uint_t PartCode,
		uint_t MergeFlags,
		const void* p,
		size_t Size
		);

	virtual 
	void 
	Break ()
	{
		m_MergeCriteria.Break ();
	}

protected:
	bool
	CanAddLines ()
	{
		return m_NextLineIdx < m_EndLineIdx;
	}

	CLine* GetCurrentLine () 
	{
		return m_NextLineIdx ? 
			m_pPage->GetLineArray () [m_NextLineIdx - 1] : 
			m_pPrevPageMergeLine;
	}

	void 
	AddText (
		uint_t PartCode,
		uint_t MergeFlags,
		const char* pText,
		size_t Length,
		bool IsHyperText
		);
	
	void 
	AddBinText (
		uint_t PartCode,
		uint_t MergeFlags,
		const void* _p,
		size_t Size
		);

	void 
	AddBinHex (
		uint_t PartCode,
		uint_t MergeFlags,
		const void* _p,
		size_t Size
		);

	void
	AddLine (
		CLine* pLine,
		uint_t PartCode
		);

	CTextLine* 
	AddTextLine (uint_t PartCode);

	CBinTextLine* 
	AddBinTextLine (uint_t PartCode);

	CBinLine* 
	AddBinHexLine (uint_t PartCode);

	CLine* 
	CreatePrevPageMergeLine (
		const TMergeCriteria& MergeCriteria,
		size_t MergeTrailCol
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {
