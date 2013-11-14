// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_CMDLINEPARSER_H

#include "axl_rtl_List.h"
#include "axl_rtl_String.h"
#include "axl_rtl_StringHashTable.h"

namespace axl {
namespace rtl {

//.............................................................................

class CCmdLineParserRoot
{
protected:
	static
	size_t
	ExtractArg (
		const char* p,
		const char* pEnd,
		rtl::CString* pArg
		);

	static
	bool
	ParseArg (
		const char* p,
		rtl::CString* pSwitchName,
		rtl::CString* pValue
		);
};

//.............................................................................

enum ECmdLineSwitchFlag
{
	ECmdLineSwitchFlag_HasValue = 0x80000000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename TSwitchTable
	>
class CCmdLineParserT: protected CCmdLineParserRoot
{
public:
	typedef typename TSwitchTable::ESwitch ESwitch;

protected:
	ESwitch m_ValueSwitch;
	rtl::CString m_ValueSwitchName;

public:
	CCmdLineParserT ()
	{
		m_ValueSwitch = (ESwitch) 0;
	}

	bool
	Parse (
		const char* pCmdLine,
		size_t Length = -1
		)
	{
		bool Result;

		if (Length == -1)
			Length = strlen (pCmdLine);

		const char* p = pCmdLine;
		const char* pEnd = p + Length;

		rtl::CString Arg;
		rtl::CString SwitchName;
		rtl::CString Value;

		m_ValueSwitch = (ESwitch) 0;

		for (int i = 0; p < pEnd; i++)
		{
			size_t Length = ExtractArg (p, pEnd, &Arg);
			if (Length == -1)
				return false;

			if (Arg.IsEmpty ())
				break;

			Result =
				ParseArg (Arg, &SwitchName, &Value) &&
				ProcessArg (i, SwitchName, Value);

			if (!Result)
				return false;

			p += Length;
		}

		return
			CheckMissingValue () &&
			static_cast <T*> (this)->Finalize ();
	}

	bool
	Parse (
		const wchar_t* pCmdLine,
		size_t Length = -1
		)
	{
		rtl::CString CmdLine (pCmdLine, Length);
		return Parse (CmdLine, CmdLine.GetLength ());
	}

	bool
	Parse (
		int argc,
		const char* const* argv
		)
	{
		bool Result;

		rtl::CString SwitchName;
		rtl::CString Value;

		for (int i = 0; i < argc; i++)
		{
			Result =
				ParseArg (argv [i], &SwitchName, &Value) &&
				ProcessArg (i, SwitchName, Value);

			if (!Result)
				return false;
		}

		return static_cast <T*> (this)->Finalize ();
	}

	bool
	Parse (
		int argc,
		const wchar_t* const* argv
		)
	{
		bool Result;

		rtl::CString SwitchName;
		rtl::CString Value;

		for (int i = 0; i < argc; i++)
		{
			Result =
				ParseArg (rtl::CString (argv [i]), &SwitchName, &Value) &&
				ProcessArg (i, SwitchName, Value);

			if (!Result)
				return false;
		}

		return static_cast <T*> (this)->Finalize ();
	}

	// overridables

	bool
	OnValue0 (const char* pValue)
	{
		return true;
	}

	// bool
	// OnValue (const char* pValue);

	// bool
	// OnSwitch (
	//		ESwitch Switch,
	//		const rtl::CString& Value // -- or const char* pValue
	//		);

	bool
	Finalize ()
	{
		return true;
	}

protected:
	bool
	CheckMissingValue ()
	{
		if (!m_ValueSwitch)
			return true;

		err::SetFormatStringError ("missing value for switch '%s'", m_ValueSwitchName.cc ());
		return false;
	}

	bool
	ProcessArg (
		int i,
		const rtl::CString& SwitchName,
		const rtl::CString& Value
		)
	{
		bool Result;

		T* pThis = static_cast <T*> (this);

		if (SwitchName.IsEmpty ())
		{
			if (!m_ValueSwitch)
				return i == 0 ? pThis->OnValue0 (Value) : pThis->OnValue (Value);

			Result = pThis->OnSwitch (m_ValueSwitch, Value);
			if (!Result)
				return false;

			m_ValueSwitch = (ESwitch) 0;
			return true;
		}

		Result = CheckMissingValue ();
		if (!Result)
			return false;

		ESwitch Switch = TSwitchTable::FindSwitch (SwitchName);
		if (!Switch)
		{
			err::SetFormatStringError ("unknown switch '%s'", SwitchName.cc ());
			return false;
		}

		if ((Switch & ECmdLineSwitchFlag_HasValue) && Value.IsEmpty ())
		{
			m_ValueSwitch = Switch;
			m_ValueSwitchName = SwitchName;
			return true;
		}

		return pThis->OnSwitch (Switch, Value);
	}
};

//.............................................................................

struct TSwitchInfo: rtl::TListLink
{
	int m_Switch;
	const char* m_NameTable [4]; // up to 4 alternative names
	const char* m_pValue;
	const char* m_pDescription;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CString
GetCmdLineHelpString (const CConstListT <TSwitchInfo>& SwitchInfoList);

//.............................................................................

#define AXL_RTL_BEGIN_CMD_LINE_SWITCH_TABLE(Class, Switch) \
class Class \
{ \
public: \
	typedef Switch ESwitch; \
	typedef axl::rtl::CHashTableMapT <char, ESwitch, axl::rtl::CHashIdT <char> > CCharMap; \
	typedef axl::rtl::CStringHashTableMapT <ESwitch> CStringMap; \
protected: \
	axl::rtl::CAuxListT <axl::rtl::TSwitchInfo> m_SwitchInfoList; \
	CCharMap m_CodeMap; \
	CStringMap m_NameMap; \
protected: \
	ESwitch \
	FindSwitchImpl (char Code) \
	{ \
		CCharMap::CIterator It = m_CodeMap.Find (Code); \
		return It ? It->m_Value : (ESwitch) 0; \
	} \
	ESwitch \
	FindSwitchImpl (const char* pName) \
	{ \
		CStringMap::CIterator It = m_NameMap.Find (pName); \
		return It ? It->m_Value : (ESwitch) 0; \
	} \
public: \
	static \
	Class* \
	GetSingleton () \
	{ \
		return axl::rtl::GetSingleton <Class> (); \
	} \
	static \
	const axl::rtl::CConstListT <axl::rtl::TSwitchInfo> \
	GetSwitchInfoList () \
	{ \
		return GetSingleton ()->m_SwitchInfoList; \
	} \
	static \
	ESwitch \
	FindSwitch (const char* pName) \
	{ \
		return pName [1] ? \
			GetSingleton ()->FindSwitchImpl (pName) : \
			GetSingleton ()->FindSwitchImpl (pName [0]); \
	} \
	static \
	axl::rtl::CString \
	GetHelpString () \
	{ \
		return axl::rtl::GetCmdLineHelpString (GetSingleton ()->m_SwitchInfoList); \
	} \
	Class () \
	{

#define AXL_RTL_CMD_LINE_ADD_SWITCH_INFO(Switch, Name0, Name1, Name2, Name3, Value, Description) \
		{ \
			static axl::rtl::TSwitchInfo SwitchInfo; \
			SwitchInfo.m_Switch = Switch; \
			SwitchInfo.m_NameTable [0] = Name0; \
			SwitchInfo.m_NameTable [1] = Name1; \
			SwitchInfo.m_NameTable [2] = Name2; \
			SwitchInfo.m_NameTable [3] = Name3; \
			SwitchInfo.m_pValue = Value; \
			SwitchInfo.m_pDescription = Description; \
			m_SwitchInfoList.InsertTail (&SwitchInfo); \
		}

#define AXL_RTL_CMD_LINE_MAP_SWITCH(Switch, Name) \
		ASSERT (Name); \
		if (Name [1]) \
			m_NameMap.Goto (Name)->m_Value = Switch; \
		else \
			m_CodeMap.Goto (Name [0])->m_Value = Switch; \

#define AXL_RTL_CMD_LINE_SWITCH_GROUP(Description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (0, NULL, NULL, NULL, NULL, NULL, Description)

#define AXL_RTL_CMD_LINE_SWITCH_1(Switch, Name, Value, Description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (Switch, Name, NULL, NULL, NULL, Value, Description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(Switch, Name)

#define AXL_RTL_CMD_LINE_SWITCH_2(Switch, Name0, Name1, Value, Description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (Switch, Name0, Name1, NULL, NULL, Value, Description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(Switch, Name0) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(Switch, Name1) \

#define AXL_RTL_CMD_LINE_SWITCH_3(Switch, Name0, Name1, Name2, Value, Description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (Switch, Name0, Name1, Name2, NULL, Value, Description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(Switch, Name0) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(Switch, Name1) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(Switch, Name2) \

#define AXL_RTL_CMD_LINE_SWITCH_4(Switch, Name0, Name1, Name2, Name3, Value, Description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (Switch, Name0, Name1, Name2, Name3, Value, Description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(Switch, Name0) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(Switch, Name1) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(Switch, Name2) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(Switch, Name3)

#define AXL_RTL_CMD_LINE_SWITCH AXL_RTL_CMD_LINE_SWITCH_1

#define AXL_RTL_END_CMD_LINE_SWITCH_TABLE() \
	} \
};

//.............................................................................

} // namespace rtl
} // namespace axl
