// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_COLORIZER_H

#include "axl_log_BinLine.h"

namespace axl {
namespace log {

//.............................................................................

struct IColorizerTarget: public obj::IRoot
{
	// {05DCF2D1-3F09-453C-96A3-9F7B74BE9651}
	AXL_OBJ_INTERFACE (
		IColorizerTarget,
		0x05dcf2d1, 0x3f09, 0x453c, 0x96, 0xa3, 0x9f, 0x7b, 0x74, 0xbe, 0x96, 0x51
		)

	virtual
	void 
	SetAttr (
		uint64_t BeginOffset, 
		uint64_t EndOffset, 
		const gui::TTextAttr& Attr
		) = 0;
};

//.............................................................................

struct IColorizer: public obj::IRoot
{
	// {883849C1-3ED3-47D0-8D58-ED1FED54EF3F}
	AXL_OBJ_INTERFACE (
		IColorizer,
		0x883849c1, 0x3ed3, 0x47d0, 0x8d, 0x58, 0xed, 0x1f, 0xed, 0x54, 0xef, 0x3f
		)

protected:
	size_t m_StateSize;

public:
	IColorizer ()
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
		IColorizerTarget* pTarget, 
		const void* p,
		size_t Size,
		bool IsFinal,
		void* pStateBuffer
		) = 0;

	void
	Finalize (
		IColorizerTarget* pTarget, 
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
