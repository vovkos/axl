// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_COLORIZEMGR_H

#include "axl_log_Colorize.h"
#include "axl_log_CachePage.h"

namespace axl {
namespace log {

//.............................................................................

class CColorizeMgr
{ 
	friend class CWidget;

protected:
	IColorizer* m_pColorizer;
	CCacheMgr* m_pCacheMgr;

	rtl::CArrayT <uchar_t> m_DataBuffer;	
	rtl::CArrayT <TBinLinePart> m_PartBuffer;	
	size_t m_CrossoverLimit;
	bool m_IsInvalidateRequired;

public:
	CColorizeMgr ();

	IColorizer* 
	GetColorizer ()
	{
		return m_pColorizer;
	}

	void
	SetColorizer (IColorizer* pColorizer);

	bool 
	EnsureColorized (CBinLine* pLine)
	{
		return ColorizePage (pLine->GetCachePage ());
	}

	bool
	ColorizePage (CCachePage* pPage);

	bool
	ReColorizeLineRange (
		CCachePage* pPage,
		size_t FirstLine,
		size_t LineCount
		);

protected:
	CLine*
	ColorizeBlock (
		CCachePage* pPage,
		CBinLine* pFirstLine,
		size_t LineOffset
		);

	CLine*
	Backtrack (CLine* pLine);

	void
	CollectLine (
		TColorizeBlock* pBlock,
		CBinLine* pLine
		);

	void
	CollectFirstLine (
		TColorizeBlock* pBlock,
		CBinLine* pLine,
		size_t LineOffset
		);

	CBinLine*
	CollectBlock (
		TColorizeBlock* pBlock,
		CCachePage* pPage,
		CBinLine* pLine,
		size_t LineOffset
		);

	void
	FillLineAttr (
		const gui::TTextAttr& Attr,
		size_t Metric,
		CBinLine* pAnchorLine,
		size_t LineCount
		);

	CBinLine*
	FindLine (
		CCachePage* pPage,
		CBinLine* pLine,
		size_t Offset
		);

	void
	ApplyColorization (
		CCachePage* pPage,
		CBinLine* pFirstLine,
		CBinLine* pLastLine,
		const gui::CTextAttrAnchorArray* pAttrArray
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {
