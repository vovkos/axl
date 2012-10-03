// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_pg_Config.h"

namespace axl {
namespace pg {

//.............................................................................

class CTarget: public rtl::TListLink
{
public:
	rtl::CString m_FileName;
	rtl::CString m_FrameFileName;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CGenerator
{
protected:
	st::CStringTemplate m_StringTemplate;
	rtl::CString m_Buffer;

public:
	const CConfig* m_pConfig;

public:
	CGenerator ()
	{
		m_pConfig = NULL;
	}

	void
	Prepare (class CModule* pModule);
	
	bool
	Generate (
		const tchar_t* pFileName,
		const tchar_t* pFrameFileName
		);

	bool
	Generate (CTarget* pTarget)
	{
		return Generate (pTarget->m_FileName, pTarget->m_FrameFileName);
	}

	bool
	GenerateList (rtl::CIteratorT <CTarget> Target);
};

//.............................................................................

} // namespace pg {
} // namespace axl {
