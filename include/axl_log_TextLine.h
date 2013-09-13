// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_TEXTLINE_H

#include "axl_log_Line.h"

namespace axl {
namespace log {

//.............................................................................

struct TTextLine
{
	size_t m_SourceLength;
	
	// followed by: 	
	// char m_Source [m_TextLength];
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CTextLine: public CLine
{
	friend class CPage;
	friend class CPageRepresenterTarget;
	friend class CWidget;

protected:
	gui::CHyperText m_HyperText;

public:
	CTextLine ()
	{
		m_LineKind = ELine_Text;
	}

	const gui::CHyperText*
	GetHyperText ()
	{
		return &m_HyperText;
	}

	virtual 
	size_t
	Load (
		const void* p,
		size_t Size
		);

	virtual 
	size_t 
	Save (rtl::CArrayT <uint8_t>* pBuffer);
};

//.............................................................................

} // namespace log {
} // namespace axl {
