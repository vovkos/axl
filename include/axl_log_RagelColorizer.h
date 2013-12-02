// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_RAGELCOLORIZER_H

#include "axl_log_Colorizer.h"

namespace axl {
namespace log {

//.............................................................................

template <typename T>
class CRagelColorizerT: public log::CColorizer
{
protected:
	struct TState
	{
		uint64_t m_Offset;
		uint64_t m_TokenOffset;
		int32_t m_cs;
		int32_t m_act;
		int32_t m_IsValid;
	};

protected:
	CColorizerTarget* m_pTarget;
	char* m_pBlockBegin;
	uint64_t m_BlockOffset;

	// ragel interface variables

	char* p;
	char* pe;
	char* eof;
	char* ts;
	char* te;
	int act;
	int cs;

public:
	CRagelColorizerT ()
	{
		m_StateSize = sizeof (TState);
		Clear ();
	}

	void 
	Clear ()
	{
		m_pTarget = NULL;
		m_pBlockBegin = NULL;
		m_BlockOffset = 0;

		p = NULL;
		pe = NULL;
		eof = NULL;
		ts = NULL;
		te = NULL;
		act = 0;
		cs = 0;
	}

	virtual 
	void
	Colorize (
		CColorizerTarget* pTarget, 
		const void* pBlock,
		size_t Size,
		bool IsFinal,
		void* pStateBuffer
		)
	{
		Clear ();

		TState* pState = (TState*) pStateBuffer;

		if (!pState || !pState->m_IsValid)
		{
			static_cast <T*> (this)->Init ();
		}
		else
		{
			m_BlockOffset = pState->m_Offset;
			cs = pState->m_cs;
			act = pState->m_act;

			if (pState->m_TokenOffset != -1)
			{
				ASSERT (pState->m_TokenOffset <= pState->m_Offset);
				ts = (char*) pBlock - (size_t) (pState->m_Offset - pState->m_TokenOffset);
				te = (char*) pBlock;
			}
		}

		m_pTarget = pTarget;
		m_pBlockBegin = (char*) pBlock;

		p = (char*) pBlock;
		pe = p + Size;
		eof = IsFinal ? pe : NULL;

		static_cast <T*> (this)->Exec ();

		if (!IsFinal && pState)
		{
			pState->m_Offset = m_BlockOffset + Size;
			pState->m_TokenOffset = ts ? m_BlockOffset + ts - m_pBlockBegin : -1;
			pState->m_cs = cs;
			pState->m_act = act;
			pState->m_IsValid = true;
		}
	}
	
protected:
	// these are to be called from withing ragel scanner (*.rl)

	void
	SetAttr (
		const char* pTokenStart,
		const char* pTokenEnd,
		const gui::TTextAttr& Attr
		)
	{
		size_t BeginOffset = m_BlockOffset + pTokenStart - m_pBlockBegin;
		size_t EndOffset = m_BlockOffset + pTokenEnd - m_pBlockBegin;

		m_pTarget->SetAttr (BeginOffset, EndOffset, Attr);
	}
	
	// to be implemented in Colorizer instance:

	// void
	// Init ()
	// {
	// }

	// void
	// Exec ()
	// {
	// }
};

//.............................................................................

} // namespace log
} // namespace axl
