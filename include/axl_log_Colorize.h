// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_COLORIZE_H

#include "axl_log_BinLine.h"

namespace axl {
namespace log {

//.............................................................................

class CColorizeTarget
{
protected:
	friend class CColorizeMgr;

	gui::CTextAttrAnchorArray m_AttrArray;
	size_t m_Metric;

public:
	intptr_t m_ColorizerState;
	ref::CPtrT <void> m_ColorizerStateEx;

public:
	CColorizeTarget ()
	{
		m_Metric = 0;
		m_ColorizerState = 0;
	}

	void 
	SetAttr (
		const gui::TTextAttr& Attr, 
		size_t Begin, 
		size_t End
		)
	{
		m_AttrArray.SetAttr (Attr, Begin, End, m_Metric);
	}

	void
	IncrementMetric ()
	{
		m_Metric++;
	}
};

//.............................................................................

struct TColorizeBlock
{
	uint_t m_PartCode;
	size_t m_PartIndex;
	uint64_t m_Timestamp;
	size_t m_Offset;
	const void* m_p;
	size_t m_Size;
	TBinLinePart* m_pPartArray;
	size_t m_PartCount;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct IColorizer: public obj::IRoot
{
	// {883849C1-3ED3-47D0-8D58-ED1FED54EF3F}
	AXL_OBJ_INTERFACE (
		IColorizer,
		0x883849c1, 0x3ed3, 0x47d0, 0x8d, 0x58, 0xed, 0x1f, 0xed, 0x54, 0xef, 0x3f
		)

	virtual 
	void 
	Colorize (
		CColorizeTarget* pTarget, 
		TColorizeBlock* pBlock, 
		bool SaveStateHint
		) = 0;
};

//.............................................................................

class CColorizerStack: 
	public IColorizer,
	public rtl::CArrayT <IColorizer*>
{
public:
	virtual 
	void
	Colorize (
		CColorizeTarget* pTarget, 
		TColorizeBlock* pBlock, 
		bool SaveStateHint
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {
