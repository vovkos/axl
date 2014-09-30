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

class CCmdLineParserRoot
{
protected:
	static
	size_t
	extractArg (
		const char* p,
		const char* end,
		rtl::CString* arg
		);

	static
	bool
	parseArg (
		const char* p,
		rtl::CString* switchName,
		rtl::CString* value
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
	enum EFlag
	{
		EFlag_Forward = 0x01,
	};

protected:
	ESwitch m_valueSwitch;
	rtl::CString m_valueSwitchName;
	uint_t m_flags;

public:
	CCmdLineParserT ()
	{
		m_valueSwitch = (ESwitch) 0;
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

		rtl::CString arg;
		rtl::CString switchName;
		rtl::CString value;

		m_valueSwitch = (ESwitch) 0;

		for (int i = 0; p < end; i++)
		{
			size_t length = extractArg (p, end, &arg);
			if (length == -1)
				return false;

			if (arg.isEmpty ())
				break;

			result = (m_flags & EFlag_Forward) ?
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
		rtl::CString cmdLine (cmdLine, length);
		return parse (cmdLine, cmdLine.getLength ());
	}

	bool
	parse (
		int argc,
		const char* const* argv
		)
	{
		bool result;

		rtl::CString switchName;
		rtl::CString value;

		for (int i = 0; i < argc; i++)
		{
			const char* arg = argv [i];

			result = (m_flags & EFlag_Forward) ?
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

		rtl::CString switchName;
		rtl::CString value;

		for (int i = 0; i < argc; i++)
		{
			rtl::CString arg = argv [i];

			result = (m_flags & EFlag_Forward) ?
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
		if (!m_valueSwitch)
			return true;

		err::setFormatStringError ("missing value for switch '%s'", m_valueSwitchName.cc ());
		return false;
	}

	bool
	processArg (
		int i,
		const rtl::CString& switchName,
		const rtl::CString& value
		)
	{
		bool result;

		T* self = static_cast <T*> (this);

		if (switchName.isEmpty ())
		{
			if (!m_valueSwitch)
				return i == 0 ? self->onValue0 (value) : self->onValue (value);

			result = self->onSwitch (m_valueSwitch, value);
			if (!result)
				return false;

			m_valueSwitch = (ESwitch) 0;
			return true;
		}

		result = checkMissingValue ();
		if (!result)
			return false;

		ESwitch switch = TSwitchTable::findSwitch (switchName);
		if (!switch)
		{
			err::setFormatStringError ("unknown switch '%s'", switchName.cc ());
			return false;
		}

		if ((switch & ECmdLineSwitchFlag_HasValue) && value.isEmpty ())
		{
			m_valueSwitch = switch;
			m_valueSwitchName = switchName;
			return true;
		}

		return self->onSwitch (switch, value);
	}
};

//.............................................................................

struct TSwitchInfo: rtl::TListLink
{
	int m_switch;
	const char* m_nameTable [4]; // up to 4 alternative names
	const char* m_value;
	const char* m_description;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CString
getCmdLineHelpString (const CConstListT <TSwitchInfo>& switchInfoList);

//.............................................................................

#define AXL_RTL_BEGIN_CMD_LINE_SWITCH_TABLE(class, switch) \
class class \
{ \
public: \
	typedef switch ESwitch; \
	typedef axl::rtl::CHashTableMapT <char, ESwitch, axl::rtl::CHashIdT <char> > CCharMap; \
	typedef axl::rtl::CStringHashTableMapT <ESwitch> CStringMap; \
protected: \
	axl::rtl::CAuxListT <axl::rtl::TSwitchInfo> m_switchInfoList; \
	CCharMap m_codeMap; \
	CStringMap m_nameMap; \
protected: \
	ESwitch \
	findSwitchImpl (char code) \
	{ \
		CCharMap::CIterator it = m_codeMap.find (code); \
		return it ? it->m_value : (ESwitch) 0; \
	} \
	ESwitch \
	findSwitchImpl (const char* name) \
	{ \
		CStringMap::CIterator it = m_nameMap.find (name); \
		return it ? it->m_value : (ESwitch) 0; \
	} \
public: \
	static \
	class* \
	getSingleton () \
	{ \
		return axl::rtl::getSingleton <class> (); \
	} \
	static \
	const axl::rtl::CConstListT <axl::rtl::TSwitchInfo> \
	getSwitchInfoList () \
	{ \
		return getSingleton ()->m_switchInfoList; \
	} \
	static \
	ESwitch \
	findSwitch (const char* name) \
	{ \
		return name [1] ? \
			getSingleton ()->findSwitchImpl (name) : \
			getSingleton ()->findSwitchImpl (name [0]); \
	} \
	static \
	axl::rtl::CString \
	getHelpString () \
	{ \
		return axl::rtl::getCmdLineHelpString (getSingleton ()->m_switchInfoList); \
	} \
	class () \
	{

#define AXL_RTL_CMD_LINE_ADD_SWITCH_INFO(switch, name0, name1, name2, name3, value, description) \
		{ \
			static axl::rtl::TSwitchInfo switchInfo; \
			switchInfo.m_switch = switch; \
			switchInfo.m_nameTable [0] = name0; \
			switchInfo.m_nameTable [1] = name1; \
			switchInfo.m_nameTable [2] = name2; \
			switchInfo.m_nameTable [3] = name3; \
			switchInfo.m_value = value; \
			switchInfo.m_description = description; \
			m_switchInfoList.insertTail (&switchInfo); \
		}

#define AXL_RTL_CMD_LINE_MAP_SWITCH(switch, name) \
		ASSERT (name); \
		if (name [1]) \
			m_nameMap.visit (name)->m_value = switch; \
		else \
			m_codeMap.visit (name [0])->m_value = switch; \

#define AXL_RTL_CMD_LINE_SWITCH_GROUP(description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (0, NULL, NULL, NULL, NULL, NULL, description)

#define AXL_RTL_CMD_LINE_SWITCH_1(switch, name, value, description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (switch, name, NULL, NULL, NULL, value, description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switch, name)

#define AXL_RTL_CMD_LINE_SWITCH_2(switch, name0, name1, value, description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (switch, name0, name1, NULL, NULL, value, description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switch, name0) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switch, name1) \

#define AXL_RTL_CMD_LINE_SWITCH_3(switch, name0, name1, name2, value, description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (switch, name0, name1, name2, NULL, value, description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switch, name0) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switch, name1) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switch, name2) \

#define AXL_RTL_CMD_LINE_SWITCH_4(switch, name0, name1, name2, name3, value, description) \
		AXL_RTL_CMD_LINE_ADD_SWITCH_INFO (switch, name0, name1, name2, name3, value, description) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switch, name0) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switch, name1) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switch, name2) \
		AXL_RTL_CMD_LINE_MAP_SWITCH(switch, name3)

#define AXL_RTL_CMD_LINE_SWITCH AXL_RTL_CMD_LINE_SWITCH_1

#define AXL_RTL_END_CMD_LINE_SWITCH_TABLE() \
	} \
};

//.............................................................................

} // namespace rtl
} // namespace axl
