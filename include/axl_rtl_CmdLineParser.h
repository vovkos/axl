// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_CMDLINEPARSER_H

#include "axl_rtl_List.h"
#include "axl_rtl_String.h"
#include "axl_rtl_StringHashTable.h"

namespace axl {
namespace rtl {

//.............................................................................

class CmdLineParserRoot
{
protected:
	static
	size_t
	extractArg (
		const char* p,
		const char* end,
		rtl::String* arg
		);

	static
	bool
	parseArg (
		const char* p,
		rtl::String* switchName,
		rtl::String* value
		);
};

//.............................................................................

enum CmdLineSwitchFlagKind
{
	CmdLineSwitchFlagKind_HasValue = 0x80000000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename SwitchTable
	>
class CmdLineParser: protected CmdLineParserRoot
{
public:
	typedef typename SwitchTable::SwitchKind SwitchKind;

protected:
	enum FlagKind
	{
		FlagKind_Forward = 0x01,
	};

protected:
	SwitchKind m_valueSwitchKind;
	rtl::String m_valueSwitchName;
	uint_t m_flags;

public:
	CmdLineParser ()
	{
		m_valueSwitchKind = (SwitchKind) 0;
		m_flags = 0;
	}

	bool
	parse (
		const char* cmdLine,
		size_t length = -1
		)
	{
		bool result;

		if (length == -1)
			length = strlen (cmdLine);

		const char* p = cmdLine;
		const char* end = p + length;

		rtl::String arg;
		rtl::String switchName;
		rtl::String value;

		m_valueSwitchKind = (SwitchKind) 0;

		for (int i = 0; p < end; i++)
		{
			size_t length = extractArg (p, end, &arg);
			if (length == -1)
				return false;

			if (arg.isEmpty ())
				break;

			result = (m_flags & FlagKind_Forward) ?
				static_cast <T*> (this)->onValue (arg) :
				parseArg (arg, &switchName, &value) &&
				processArg (i, switchName, value);

			if (!result)
				return false;

			p += length;
		}

		return
			checkMissingValue () &&
			static_cast <T*> (this)->finalize ();
	}

	bool
	parse (
		const wchar_t* cmdLine,
		size_t length = -1
		)
	{
		rtl::String cmdLine (cmdLine, length);
		return parse (cmdLine, cmdLine.getLength ());
	}

	bool
	parse (
		int argc,
		const char* const* argv
		)
	{
		bool result;

		rtl::String switchName;
		rtl::String value;

		for (int i = 0; i < argc; i++)
		{
			const char* arg = argv [i];

			result = (m_flags & FlagKind_Forward) ?
				static_cast <T*> (this)->onValue (arg) :
				parseArg (arg, &switchName, &value) &&
				processArg (i, switchName, value);

			if (!result)
				return false;
		}

		return static_cast <T*> (this)->finalize ();
	}

	bool
	parse (
		int argc,
		const wchar_t* const* argv
		)
	{
		bool result;

		rtl::String switchName;
		rtl::String value;

		for (int i = 0; i < argc; i++)
		{
			rtl::String arg = argv [i];

			result = (m_flags & FlagKind_Forward) ?
				static_cast <T*> (this)->onValue (arg) :
				parseArg (arg, &switchName, &value) &&
				processArg (i, switchName, value);

			if (!result)
				return false;
		}

		return static_cast <T*> (this)->finalize ();
	}

	// overridables

	bool
	onValue0 (const char* value)
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
	finalize ()
	{
		return true;
	}

protected:
	bool
	checkMissingValue ()
	{
		if (!m_valueSwitchKind)
			return true;

		err::setFormatStringError ("missing value for switch '%s'", m_valueSwitchName.cc ());
		return false;
	}

	bool
	processArg (
		int i,
		const rtl::String& switchName,
		const rtl::String& value
		)
	{
		bool result;

		T* self = static_cast <T*> (this);

		if (switchName.isEmpty ())
		{
			if (!m_valueSwitchKind)
				return i == 0 ? self->onValue0 (value) : self->onValue (value);

			result = self->onSwitch (m_valueSwitchKind, value);
			if (!result)
				return false;

			m_valueSwitchKind = (SwitchKind) 0;
			return true;
		}

		result = checkMissingValue ();
		if (!result)
			return false;

		SwitchKind switchKind = SwitchTable::findSwitch (switchName);
		if (!switchKind)
		{
			err::setFormatStringError ("unknown switch '%s'", switchName.cc ());
			return false;
		}

		if ((switchKind & CmdLineSwitchFlagKind_HasValue) && value.isEmpty ())
		{
			m_valueSwitchKind = switchKind;
			m_valueSwitchName = switchName;
			return true;
		}

		return self->onSwitch (switchKind, value);
	}
};

//.............................................................................

struct SwitchInfo: rtl::ListLink
{
	int m_switchKind;
	const char* m_nameTable [4]; // up to 4 alternative names
	const char* m_value;
	const char* m_description;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

String
getCmdLineHelpString (const ConstList <SwitchInfo>& switchInfoList);

//.............................................................................

#define AXL_RTL_BEGIN_CMD_LINE_SWITCH_TABLE(Class, SwitchKindEnum) \
class Class \
{ \
public: \
	typedef SwitchKindEnum SwitchKind; \
	typedef axl::rtl::HashTableMap <char, SwitchKind, axl::rtl::HashId <char> > CharMap; \
	typedef axl::rtl::StringHashTableMap <SwitchKind> StringMap; \
protected: \
	axl::rtl::AuxList <axl::rtl::SwitchInfo> m_switchInfoList; \
	CharMap m_codeMap; \
	StringMap m_nameMap; \
protected: \
	SwitchKind \
	findSwitchImpl (char code) \
	{ \
		CharMap::Iterator it = m_codeMap.find (code); \
		return it ? it->m_value : (SwitchKind) 0; \
	} \
	SwitchKind \
	findSwitchImpl (const char* name) \
	{ \
		StringMap::Iterator it = m_nameMap.find (name); \
		return it ? it->m_value : (SwitchKind) 0; \
	} \
public: \
	static \
	Class* \
	getSingleton () \
	{ \
		return axl::rtl::getSingleton <Class> (); \
	} \
	static \
	const axl::rtl::ConstList <axl::rtl::SwitchInfo> \
	getSwitchInfoList () \
	{ \
		return getSingleton ()->m_switchInfoList; \
	} \
	static \
	SwitchKind \
	findSwitch (const char* name) \
	{ \
		return name [1] ? \
			getSingleton ()->findSwitchImpl (name) : \
			getSingleton ()->findSwitchImpl (name [0]); \
	} \
	static \
	axl::rtl::String \
	getHelpString () \
	{ \
		return axl::rtl::getCmdLineHelpString (getSingleton ()->m_switchInfoList); \
	} \
	Class () \
	{

#define AXL_RTL_CMD_LINE_ADD_SWITCH_INFO(switchKind, name0, name1, name2, name3, value, description) \
		{ \
			static axl::rtl::SwitchInfo switchInfo; \
			switchInfo.m_switchKind = switchKind; \
			switchInfo.m_nameTable [0] = name0; \
			switchInfo.m_nameTable [1] = name1; \
			switchInfo.m_nameTable [2] = name2; \
			switchInfo.m_nameTable [3] = name3; \
			switchInfo.m_value = value; \
			switchInfo.m_description = description; \
			m_switchInfoList.insertTail (&switchInfo); \
		}

#define AXL_RTL_CMD_LINE_MAP_SWITCH(switchKind, name) \
		ASSERT (name); \
		if (name [1]) \
			m_nameMap.visit (name)->m_value = switchKind; \
		else \
			m_codeMap.visit (name [0])->m_value = switchKind; \

#define AXL_RTL_CMD_LINE_SWITCH_GROUP(description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (0, NULL, NULL, NULL, NULL, NULL, description)

#define AXL_RTL_CMD_LINE_SWITCH_1(switchKind, name, value, description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (switchKind, name, NULL, NULL, NULL, value, description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switchKind, name)

#define AXL_RTL_CMD_LINE_SWITCH_2(switchKind, name0, name1, value, description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (switchKind, name0, name1, NULL, NULL, value, description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switchKind, name0) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switchKind, name1) \

#define AXL_RTL_CMD_LINE_SWITCH_3(switchKind, name0, name1, name2, value, description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (switchKind, name0, name1, name2, NULL, value, description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switchKind, name0) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switchKind, name1) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switchKind, name2) \

#define AXL_RTL_CMD_LINE_SWITCH_4(switchKind, name0, name1, name2, name3, value, description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (switchKind, name0, name1, name2, name3, value, description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switchKind, name0) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switchKind, name1) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switchKind, name2) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switchKind, name3)

#define AXL_RTL_CMD_LINE_SWITCH AXL_RTL_CMD_LINE_SWITCH_1

#define AXL_RTL_END_CMD_LINE_SWITCH_TABLE() \
	} \
};

//.............................................................................

} // namespace rtl
} // namespace axl
