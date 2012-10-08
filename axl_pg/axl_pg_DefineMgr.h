// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

namespace axl {
namespace pg {

//.............................................................................

enum EDefine
{
	EDefine_String,
	EDefine_Integer
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CDefine: public rtl::TListLink
{
public:
	EDefine m_Kind;
	lex::CSrcPos m_SrcPos;
	rtl::CString m_Name;
	rtl::CString m_StringValue;
	int m_IntegerValue;

	CDefine ()
	{
		m_Kind = EDefine_String;
		m_IntegerValue = 0;
	}
};

//.............................................................................

class CDefineMgr
{
protected:
	rtl::CStdListT <CDefine> m_DefineList;
	rtl::CHashTableMapT <const tchar_t*, CDefine*, rtl::CHashString, rtl::CCmpString> m_DefineMap;

public:
	bool
	IsEmpty ()
	{
		return m_DefineList.IsEmpty ();
	}

	size_t
	GetCount ()
	{
		return m_DefineList.GetCount ();
	}

	void
	Clear ()
	{
		m_DefineList.Clear ();
		m_DefineMap.Clear ();
	}

	rtl::CIteratorT <CDefine>
	GetHead ()
	{
		return m_DefineList.GetHead ();
	}

	CDefine*
	GetDefine (const rtl::CString& Name);

	CDefine*
	FindDefine (const tchar_t* pName)
	{
		rtl::CHashTableMapIteratorT <const tchar_t*, CDefine*> It = m_DefineMap.Find (pName);
		return It ? It->m_Value : NULL;
	}
};

//.............................................................................

} // namespace pg {
} // namespace axl {
