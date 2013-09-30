// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_COLORIZEMGR_H

#include "axl_log_Protocol.h"
#include "axl_log_Representer.h"
#include "axl_log_Colorizer.h"
#include "axl_log_PacketFile.h"
#include "axl_log_MergeFile.h"

namespace axl {
namespace log {

//.............................................................................

class CColorizeMgr
{ 
protected:
	IRepresenter* m_pRepresenter;
	IColorizer* m_pColorizer;

	CPacketFile* m_pPacketFile;
	CMergeFile* m_pMergeFile;
	io::CMappedFile* m_pColorizerStateFile;

	rtl::CArrayT <uint8_t> m_ColorizerState;

	uint64_t m_PacketOffset;
	uint64_t m_MergePointIdx;
	TMergeCriteria m_MergeCriteria;

public:
	CColorizeMgr ();

	void
	Reset ();

	void
	Setup (
		IRepresenter* pRepresenter,
		IColorizer* pColorizer,
		CPacketFile* pPacketFile,
		CMergeFile* pMergeFile,
		io::CMappedFile* pColorizerStateFile
		);		

	// called from io thread

	void
	UpdateColorizerStateFile (); 
};

#if 0

//.............................................................................

class CColorizeMgr
{ 
	friend class CWidget;

protected:
	CMappedFile m_ColorFile;
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

#endif

//.............................................................................

} // namespace log {
} // namespace axl {
