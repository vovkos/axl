// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_pg_DefineMgr.h"

namespace axl {
namespace pg {

//.............................................................................

enum EConfigFlag
{
	EConfigFlag_Verbose  = 0x01,
	EConfigFlag_NoPpLine = 0x02
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CConfig
{
public:
	int m_Flags;

	rtl::CString  m_OutputDir;
	rtl::CBoxListT <rtl::CString> m_ImportDirList;
	rtl::CBoxListT <rtl::CString> m_FrameDirList;

	CConfig ()
	{
		m_Flags = 0;
	}
};

//.............................................................................

} // namespace pg {
} // namespace axl {
