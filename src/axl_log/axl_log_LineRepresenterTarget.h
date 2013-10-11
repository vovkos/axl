// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_LINEREPRESENTERTARGET_H

#include "axl_log_Representer.h"
#include "axl_log_MergeCriteria.h"
#include "axl_log_LineRepresentMgr.h"

namespace axl {
namespace log {

//.............................................................................

class CLineRepresenterTarget: public CRepresenterTarget 
{
	friend class CLineRepresentMgr;

protected:
	CColorizer* m_pColorizer;

	TLinePool* m_pLinePool;
	rtl::CArrayT <uchar_t>* m_pLineBuffer;
	rtl::CArrayT <uchar_t>* m_pPageBuffer;
	
	CLine* m_pCurrentLine;
	void* m_pColorizerState;

	TMergeCriteria m_MergeCriteria;

	size_t m_FoldablePacketIdx;

	bool m_IsCurrentLineSaved;
	bool m_IsFirstLineOfPacket;
	bool m_IsFull;

	intptr_t m_LineIdx;
	size_t m_LineCount;

	uint64_t m_Timestamp;
	uint64_t m_PacketOffset;
	uint64_t m_BinOffset;
	uint64_t m_PartIdx;
	
public:
	CLineRepresenterTarget ();

	void
	StartPacket (
		const TPacket* pPacket,
		uint64_t PacketOffset
		);

	virtual 
	void 
	AddPart (
		EPart PartKind,
		uint_t PartCode,
		const void* p,
		size_t Size
		);

	virtual 
	void 
	Break ()
	{
		m_MergeCriteria.Break ();
	}

	void
	SaveCurrentLine ();

protected:
	bool
	CanAddLines ()
	{
		return m_LineCount == -1 || m_LineIdx < (intptr_t) m_LineCount;
	}

	void 
	AddText (
		bool ShouldMerge,
		uint_t PartCode,
		const char* pText,
		size_t Length,
		bool IsHyperText
		);
	
	void 
	AddBinText (
		bool ShouldMerge,
		uint_t PartCode,
		const void* _p,
		size_t Size
		);

	void 
	AddBinHex (
		bool ShouldMerge,
		uint_t PartCode,
		const void* _p,
		size_t Size
		);

	bool
	AddLine (
		bool ShouldMerge,
		CLine* pLine,
		uint_t PartCode
		);

	CTextLine* 
	AddTextLine (
		bool ShouldMerge,
		uint_t PartCode
		);

	CBinTextLine* 
	AddBinTextLine (
		bool ShouldMerge,
		uint_t PartCode
		);

	CBinLine* 
	AddBinHexLine (
		bool ShouldMerge,
		uint_t PartCode
		);

	CLine* 
	CreatePrevPageMergeLine (
		ELine MergeLineKind,
		size_t MergeTrailCol
		);

	void
	ColorizeLine (
		CBinLine* pLine,
		const void* p,
		size_t Size
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {
