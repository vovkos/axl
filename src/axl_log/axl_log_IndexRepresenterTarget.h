// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_INDEXREPRESENTERTARGET_H

#include "axl_log_Representer.h"
#include "axl_log_MergeCriteria.h"
#include "axl_log_PacketFile.h"

namespace axl {
namespace log {
	
//.............................................................................

class CIndexRepresenterTarget: public IRepresenterTarget
{
	friend class CIndexMgr;

protected:
	size_t m_LineCount;
	size_t m_Col;
	uint64_t m_PartIdx;
	uint64_t m_FirstPartIdx;
	uint64_t m_BinOffset;

	TMergeCriteria m_MergeCriteria;

	bool m_IsFirstPartMerged;

public:
	CIndexRepresenterTarget ();

	void
	StartPacket (const TPacket* pPacket);

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

protected:
	void 
	AddText (
		const char* pText,
		size_t Length
		);

	void 
	AddBinText (
		const void* p,
		size_t Size
		);

	void 
	AddBinHex (size_t Size);
};

//.............................................................................

} // namespace log 
} // namespace axl
