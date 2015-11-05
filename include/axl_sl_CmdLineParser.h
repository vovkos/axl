// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_CMDLINEPARSER_H

#include "axl_sl_List.h"
#include "axl_sl_String.h"
#include "axl_sl_StringHashTable.h"

namespace axl {
namespace sl {

//.............................................................................

class CmdLineParserRoot
{
protected:
	static
	size_t
	extractArg (
		const char* p,
		const char* end,
		sl::String* arg
		);

	static
	bool
	parseArg (
		const char* p,
		sl::String* switchName,
		sl::String* value
		);
};

//.............................................................................

enum CmdLineSwitchFlag
{
	CmdLineSwitchFlag_HasValue = 0x80000000,
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
	enum Flag
	{
		Flag_Forward = 0x01,
	};

protected:
	SwitchKind m_valueSwitchKind;
	sl::String m_valueSwitchName;
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

		sl::String arg;
		sl::String switchName;
		sl::String value;

		m_valueSwitchKind = (SwitchKind) 0;

		for (int i = 0; p < end; i++)
		{
			size_t length = extractArg (p, end, &arg);
			if (length == -1)
				return false;

			if (arg.isEmpty ())
				break;

			result = (m_flags & Flag_Forward) ?
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
		sl::String cmdLine_utf8 (cmdLine, length);
		return parse (cmdLine_utf8, cmdLine_utf8.getLength ());
	}

	bool
	parse (
		int argc,
		const char* const* argv
		)
	{
		bool result;

		sl::String switchName;
		sl::String value;

		for (int i = 0; i < argc; i++)
		{
			const char* arg = argv [i];

			result = (m_flags & Flag_Forward) ?
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

		sl::String switchName;
		sl::String switchValue;

		for (int i = 0; i < argc; i++)
		{
			sl::String arg = argv [i];

			result = (m_flags & Flag_Forward) ?
				static_cast <T*> (this)->onValue (arg) :
				parseArg (arg, &switchName, &switchValue) &&
				processArg (i, switchName, switchValue);

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
	//		const sl::CString& Value // -- or const char* pValue
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
		const sl::String& switchName,
		const sl::String& value
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

		if ((switchKind & CmdLineSwitchFlag_HasValue) && value.isEmpty ())
		{
			m_valueSwitchKind = switchKind;
			m_valueSwitchName = switchName;
			return true;
		}

		return self->onSwitch (switchKind, value);
	}
};

//.............................................................................

struct SwitchInfo: sl::ListLink
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

#define AXL_SL_BEGIN_CMD_LINE_SWITCH_TABLE(Class, SwitchKind_0) \
class Class \
{ \
public: \
	typedef SwitchKind_0 SwitchKind; \
	typedef axl::sl::HashTableMap <char, SwitchKind, axl::sl::HashId <char> > CharMap; \
	typedef axl::sl::StringHashTableMap <SwitchKind> StringMap; \
protected: \
	axl::sl::AuxList <axl::sl::SwitchInfo> m_switchInfoList; \
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
		return axl::mt::getSingleton <Class> (); \
	} \
	static \
	const axl::sl::ConstList <axl::sl::SwitchInfo> \
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
	axl::sl::String \
	getHelpString () \
	{ \
		return axl::sl::getCmdLineHelpString (getSingleton ()->m_switchInfoList); \
	} \
	Class () \
	{

#define AXL_SL_CMD_LINE_ADD_SWITCH_INFO(switchKind, name0, name1, name2, name3, value, description) \
		{ \
			static axl::sl::SwitchInfo switchInfo; \
			switchInfo.m_switchKind = switchKind; \
			switchInfo.m_nameTable [0] = name0; \
			switchInfo.m_nameTable [1] = name1; \
			switchInfo.m_nameTable [2] = name2; \
			switchInfo.m_nameTable [3] = name3; \
			switchInfo.m_value = value; \
			switchInfo.m_description = description; \
			m_switchInfoList.insertTail (&switchInfo); \
		}

#define AXL_SL_CMD_LINE_MAP_SWITCH(switchKind, name) \
		ASSERT (name); \
		if (name [1]) \
			m_nameMap.visit (name)->m_value = switchKind; \
		else \
			m_codeMap.visit (name [0])->m_value = switchKind; \

#define AXL_SL_CMD_LINE_SWITCH_GROUP(description) \
		AXL_SL_CMD_LINE_ADD_SWITCH_INFO (0, NULL, NULL, NULL, NULL, NULL, description)

#define AXL_SL_CMD_LINE_SWITCH_1(switchKind, name, value, description) \
		AXL_SL_CMD_LINE_ADD_SWITCH_INFO (switchKind, name, NULL, NULL, NULL, value, description) \
		AXL_SL_CMD_LINE_MAP_SWITCH(switchKind, name)

#define AXL_SL_CMD_LINE_SWITCH_2(switchKind, name0, name1, value, description) \
		AXL_SL_CMD_LINE_ADD_SWITCH_INFO (switchKind, name0, name1, NULL, NULL, value, description) \
		AXL_SL_CMD_LINE_MAP_SWITCH(switchKind, name0) \
		AXL_SL_CMD_LINE_MAP_SWITCH(switchKind, name1) \

#define AXL_SL_CMD_LINE_SWITCH_3(switchKind, name0, name1, name2, value, description) \
		AXL_SL_CMD_LINE_ADD_SWITCH_INFO (switchKind, name0, name1, name2, NULL, value, description) \
		AXL_SL_CMD_LINE_MAP_SWITCH(switchKind, name0) \
		AXL_SL_CMD_LINE_MAP_SWITCH(switchKind, name1) \
		AXL_SL_CMD_LINE_MAP_SWITCH(switchKind, name2) \

#define AXL_SL_CMD_LINE_SWITCH_4(switchKind, name0, name1, name2, name3, value, description) \
		AXL_SL_CMD_LINE_ADD_SWITCH_INFO (switchKind, name0, name1, name2, name3, value, description) \
		AXL_SL_CMD_LINE_MAP_SWITCH(switchKind, name0) \
		AXL_SL_CMD_LINE_MAP_SWITCH(switchKind, name1) \
		AXL_SL_CMD_LINE_MAP_SWITCH(switchKind, name2) \
		AXL_SL_CMD_LINE_MAP_SWITCH(switchKind, name3)

#define AXL_SL_CMD_LINE_SWITCH AXL_SL_CMD_LINE_SWITCH_1

#define AXL_SL_END_CMD_LINE_SWITCH_TABLE() \
	} \
};

//.............................................................................

} // namespace sl
} // namespace axl
