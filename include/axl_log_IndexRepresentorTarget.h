// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_REPRESENT_H

#include "axl_log_Represent.h"

namespace axl {
namespace log {

//.............................................................................

struct TIndexRepresentorTargetData
{
	size_t m_LineCount;
	size_t m_Col;
	size_t m_BinOffset;
	size_t m_PartIndex;
	size_t m_MergeId;

	TMergeCriteria m_MergeCriteria;

	TIndexRepresentorTargetData ();
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CIndexRepresentorTarget: 
	public IRepresentorTarget,
	public TIndexRepresentorTargetData
{
	friend class CIndexMgr;

public:
	AXL_OBJ_CLASS_0 (CIndexRepresentorTarget, IRepresentorTarget);
	
public:
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
	OverrideLineAttr ()
	{
		// nothing to do here
	}

	virtual 
	void 
	Break ()
	{
		m_MergeCriteria.Break ();
	}

protected:
	void 
	AddText (
		const char* pText,
		size_t Length
		);

	void 
	AddBinText (
		const TBinDataConfig& BinDataConfig,
		const void* _p,
		size_t Size
		);

	void 
	AddBinHex (
		const TBinDataConfig& BinDataConfig,
		size_t Size
		);
};

//.............................................................................

} // namespace log 
} // namespace axl
