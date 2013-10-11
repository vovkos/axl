// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_COLORIZER_H

#include "axl_log_BinLine.h"

namespace axl {
namespace log {

//.............................................................................

struct CColorizerTarget
{
	virtual
	void 
	SetAttr (
		uint64_t BeginOffset, 
		uint64_t EndOffset, 
		const gui::TTextAttr& Attr
		) = 0;
};

//.............................................................................

struct CColorizer
{
protected:
	size_t m_StateSize;

public:
	CColorizer ()
	{
		m_StateSize = 0;
	}

	size_t 
	GetStateSize ()
	{
		return m_StateSize;
	}

	virtual 
	void
	Colorize (
		CColorizerTarget* pTarget, 
		const void* p,
		size_t Size,
		bool IsFinal,
		void* pStateBuffer
		) = 0;

	void
	Finalize (
		CColorizerTarget* pTarget, 
		void* pStateBuffer
		)
	{
		Colorize (pTarget, NULL, 0, true, pStateBuffer);
		if (pStateBuffer)
			memset (pStateBuffer, 0, m_StateSize);
	}
};

//.............................................................................

} // namespace log {
} // namespace axl {
