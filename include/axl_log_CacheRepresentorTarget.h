// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_CACHEREPRESENTORTARGET_H

#include "axl_log_Represent.h"
#include "axl_log_MergeCriteria.h"
#include "axl_log_CachePage.h"

namespace axl {
namespace log {

//.............................................................................

class CCacheRepresentorTarget: public IRepresentorTarget 
{
public:
	AXL_OBJ_CLASS_0 (CCacheRepresentorTarget, IRepresentorTarget);

protected:
	friend class CCacheMgr;

	CCachePage* m_pPage;
	TCacheVolatilePacket* m_pVolatilePacket;
	
	TMergeCriteria m_MergeCriteria;
	
	CLine* m_pPrevPageMergeLine;
	bool_t m_IsFirstLineOfPacket;
	
	uint64_t m_Timestamp;

	size_t m_PacketOffset;
	size_t m_BinOffset;
	size_t m_PartIndex;	
	size_t m_MergeId;
	
	size_t m_LongestTextLine;
	size_t m_LongestBinHexLine;
	size_t m_LongestBinTextLine;
	bool_t m_IsFull;
	size_t m_Line;
	size_t m_EndLine;

public:
	CCacheRepresentorTarget ();

	~CCacheRepresentorTarget ();

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
		return m_Line < m_EndLine;
	}

	CLine* GetCurrentLine () 
	{
		return m_Line ? m_pPage->GetLine (m_Line - 1) : m_pPrevPageMergeLine;
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
