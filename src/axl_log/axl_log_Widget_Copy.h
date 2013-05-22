// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_COPY_H

#include "axl_log_Widget.h"

namespace axl {
namespace log {

//.............................................................................

class CProcessCopy: public IProcessLine
{
public:
	AXL_OBJ_CLASS_0 (CProcessCopy, IProcessLine)

public:
	rtl::CString* m_pString;

public:
	CProcessCopy ()
	{
		m_pString = NULL;
	}

	virtual 
	void 
	Process (
		CLine* pLine,
		CLine* pPrevLine,
		size_t StartCol,
		size_t EndCol
		);

protected:
	void 
	TextCopy (
		CTextLine* pLine, 
		size_t FromCol, 
		size_t ToCol
		);

	void 
	BinHexAsciiCopy (
		CBinLine* pLine,
		size_t FromCol, 
		size_t ToCol,
		bool HexOnly
		);

	void 
	BinTextCopy (
		CBinTextLine* pLine, 
		size_t FromCol, 
		size_t ToCol
		);
};

//.............................................................................

class CProcessBinHexCopy: public CProcessCopy
{
public:
	AXL_OBJ_CLASS_0 (CProcessBinHexCopy, IProcessLine)

public:
	virtual 
	void 
	Process (
		CLine* pLine,
		CLine* pPrevLine,
		size_t StartCol,
		size_t EndCol
		);
};

//.............................................................................

class CProcessBinHexAsciiCopy: public CProcessCopy
{
public:
	AXL_OBJ_CLASS_0 (CProcessBinHexAsciiCopy, IProcessLine)

public:
	virtual 
	void 
	Process (
		CLine* pLine,
		CLine* pPrevLine,
		size_t StartCol,
		size_t EndCol
		);
};

//.............................................................................

class CProcessBinCopy: public IProcessBinBlock
{
public:
	AXL_OBJ_CLASS_0 (CProcessBinCopy, IProcessBinBlock)

public:
	rtl::CString* m_pString;

public:
	CProcessBinCopy ()
	{
		m_pString = NULL;
	}

	virtual 
	void
	Process (
		const void* p,
		size_t Size
		)
	{
		m_pString->Append ((char*) p, Size / sizeof (char));
	}
};

//.............................................................................

} // namespace log {
} // namespace axl {
