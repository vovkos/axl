//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_SL_CMDLINEPARSER_H

#include "axl_sl_List.h"
#include "axl_sl_String.h"
#include "axl_sl_StringHashTable.h"

namespace axl {
namespace sl {

//..............................................................................

struct SwitchInfo: sl::ListLink
{
	int m_switchKind;

	// all strings below are static literals

	const char* m_nameTable[4]; // up to 4 alternative names
	const char* m_value;
	const char* m_description;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

String
getCmdLineHelpString(const ConstList<SwitchInfo>& switchInfoList);

//..............................................................................

class CmdLineParserRoot
{
protected:
	enum ArgKind
	{
		ArgKind_Value,
		ArgKind_CharSwitch,
		ArgKind_StringSwitch,
	};

	enum Flag
	{
		Flag_Forward = 0x01,
	};

protected:
	int m_argIdx;
	int m_valueSwitchKind;
	sl::String m_valueSwitchName;
	uint_t m_flags;

protected:
	CmdLineParserRoot()
	{
		m_argIdx = 0;
		m_valueSwitchKind = 0;
		m_flags = 0;
	}

	static
	ArgKind
	getArgKind(const sl::StringRef& arg)
	{
		return
			arg.getLength() < 2 || arg[0] != '-' ? ArgKind_Value :
			arg[1] != '-' ? ArgKind_CharSwitch :
			ArgKind_StringSwitch;
	}

	static
	size_t
	extractArg(
		const sl::StringRef& cmdLine,
		sl::String* arg
		);

	static
	bool
	parseSwitch(
		ArgKind argKind,
		const sl::StringRef& arg,
		sl::String* switchName,
		sl::String* value
		);
};

//..............................................................................

template <
	typename T,
	typename SwitchTable
	>
class CmdLineParser: protected CmdLineParserRoot
{
public:
	typedef typename SwitchTable::SwitchKind SwitchKind;

public:
	bool
	parse(const sl::StringRef& cmdLine0)
	{
		bool result;

		sl::StringRef cmdLine(cmdLine0.cp(), cmdLine0.getLength());

		sl::String arg;
		sl::String switchName;
		sl::String value;

		m_valueSwitchKind = 0;

		for (m_argIdx = 0; !cmdLine.isEmpty(); m_argIdx++)
		{
			size_t length = extractArg(cmdLine, &arg);
			if (length == -1)
				return false;

			if (arg.isEmpty())
				break;

			result = processArg(arg);
			if (!result)
				return false;

			cmdLine.offset(length);
		}

		return
			checkMissingValue() &&
			static_cast<T*> (this)->finalize();
	}

	bool
	parse(
		int argc,
		const char* const* argv
		)
	{
		bool result;

		for (int i = 0; i < argc; i++)
		{
			m_argIdx = i;
			result = processArg(argv[i]);
			if (!result)
				return false;
		}

		return
			checkMissingValue() &&
			static_cast<T*> (this)->finalize();
	}

	bool
	parse(
		int argc,
		const wchar_t* const* argv
		)
	{
		bool result;

		for (int i = 0; i < argc; i++)
		{
			m_argIdx = i;
			result = processArg(sl::String(argv[i]));
			if (!result)
				return false;
		}

		return
			checkMissingValue() &&
			static_cast<T*> (this)->finalize();
	}

	// overridables

	bool
	onValue0(const sl::StringRef& value)
	{
		return true;
	}

	bool
	onValue(const sl::StringRef& value)
	{
		return true;
	}

	bool
	onSwitch(
		SwitchKind switchKind,
		const sl::StringRef& value
		)
	{
		return true;
	}

	bool
	finalize()
	{
		return true;
	}

protected:
	bool
	checkMissingValue()
	{
		if (!m_valueSwitchKind)
			return true;

		err::setFormatStringError("missing value for switch '%s'", m_valueSwitchName.sz());
		return false;
	}

	bool
	processArg(const sl::StringRef& arg)
	{
		bool result;

		T* self = static_cast<T*> (this);

		if (m_flags & Flag_Forward)
			return self->onValue(arg);

		sl::String switchName;
		sl::String value;

		ArgKind argKind = getArgKind(arg);
		switch (argKind)
		{
		case ArgKind_Value:
			return processValue(arg);

		case ArgKind_StringSwitch:
			result = parseSwitch(argKind, arg.getSubString(2), &switchName, &value);
			if (!result)
				return false;

			const SwitchInfo* switchInfo = SwitchTable::findSwitch(switchName);
			if (!switchInfo)
			{
				err::setFormatStringError("unknown switch --%s", switchName.sz());
				return false;
			}

			return processSwitch(switchInfo, switchName, value);
		}

		ASSERT(argKind == ArgKind_CharSwitch);

		size_t length = arg.getLength();
		for (size_t i = 1; i < length; i++)
		{
			const SwitchInfo* switchInfo = SwitchTable::findSwitch(arg[i]);
			if (!switchInfo)
			{
				err::setFormatStringError("unknown switch -%c", (uchar_t) arg [i]);
				return false;
			}

			if (switchInfo->m_value)
			{
				return
					parseSwitch(argKind, arg.getSubString(i), &switchName, &value) &&
					processSwitch(switchInfo, switchName, value);
			}

			result = self->onSwitch((SwitchKind)switchInfo->m_switchKind, NULL);
		}

		return true;
	}

	bool
	processValue(const sl::StringRef& value)
	{
		T* self = static_cast<T*> (this);

		if (!m_valueSwitchKind)
			return m_argIdx == 0 ? self->onValue0(value) : self->onValue(value);

		bool result = self->onSwitch((SwitchKind)m_valueSwitchKind, value);
		if (!result)
			return false;

		m_valueSwitchKind = 0;
		return true;
	}

	bool
	processSwitch(
		const SwitchInfo* switchInfo,
		const sl::StringRef& switchName,
		const sl::StringRef& value
		)
	{
		bool result;

		T* self = static_cast<T*> (this);

		result = checkMissingValue();
		if (!result)
			return false;

		if (switchInfo->m_value && value.isEmpty())
		{
			m_valueSwitchKind = switchInfo->m_switchKind;
			m_valueSwitchName = switchName;
			return true;
		}

		return self->onSwitch((SwitchKind)switchInfo->m_switchKind, value);
	}
};

//..............................................................................

#define AXL_SL_BEGIN_CMD_LINE_SWITCH_TABLE(Class, SwitchKind0) \
class Class \
{ \
public: \
	typedef SwitchKind0 SwitchKind; \
	typedef axl::sl::SwitchInfo SwitchInfo; \
	typedef axl::sl::SimpleHashTable<char, SwitchInfo*> CharMap; \
	typedef axl::sl::StringHashTable<SwitchInfo*> StringMap; \
protected: \
	axl::sl::AuxList<axl::sl::SwitchInfo> m_switchInfoList; \
	CharMap m_charMap; \
	StringMap m_nameMap; \
public: \
	static \
	Class* \
	getSingleton() \
	{ \
		return axl::sl::getSingleton<Class> (); \
	} \
	static \
	axl::sl::ConstList<axl::sl::SwitchInfo> \
	getSwitchInfoList() \
	{ \
		return getSingleton()->m_switchInfoList; \
	} \
	static \
	const SwitchInfo* \
	findSwitch(char c) \
	{ \
		CharMap::Iterator it = getSingleton()->m_charMap.find(c); \
		return it ? it->m_value : NULL; \
	} \
	static \
	const SwitchInfo* \
	findSwitch(const sl::StringRef& name) \
	{ \
		StringMap::Iterator it = getSingleton()->m_nameMap.find(name); \
		return it ? it->m_value : NULL; \
	} \
	static \
	axl::sl::String \
	getHelpString() \
	{ \
		return axl::sl::getCmdLineHelpString(getSingleton()->m_switchInfoList); \
	} \
	Class() \
	{ \
		SwitchInfo* lastSwitchInfo = NULL;

#define AXL_SL_CMD_LINE_ADD_SWITCH_INFO(switchKind, name0, name1, name2, name3, value, description) \
		{ \
			static axl::sl::SwitchInfo switchInfo; \
			switchInfo.m_switchKind = switchKind; \
			switchInfo.m_nameTable[0] = name0; \
			switchInfo.m_nameTable[1] = name1; \
			switchInfo.m_nameTable[2] = name2; \
			switchInfo.m_nameTable[3] = name3; \
			switchInfo.m_value = value; \
			switchInfo.m_description = description; \
			m_switchInfoList.insertTail(&switchInfo); \
			lastSwitchInfo = &switchInfo; \
		}

#define AXL_SL_CMD_LINE_MAP_SWITCH(name) \
		ASSERT(name); \
		if (name[1]) \
			m_nameMap[name] = lastSwitchInfo; \
		else \
			m_charMap[name[0]] = lastSwitchInfo; \

#define AXL_SL_CMD_LINE_SWITCH_GROUP(description) \
		AXL_SL_CMD_LINE_ADD_SWITCH_INFO(0, NULL, NULL, NULL, NULL, NULL, description)

#define AXL_SL_CMD_LINE_SWITCH_1(switchKind, name, value, description) \
		AXL_SL_CMD_LINE_ADD_SWITCH_INFO(switchKind, name, NULL, NULL, NULL, value, description) \
		AXL_SL_CMD_LINE_MAP_SWITCH(name)

#define AXL_SL_CMD_LINE_SWITCH_2(switchKind, name0, name1, value, description) \
		AXL_SL_CMD_LINE_ADD_SWITCH_INFO(switchKind, name0, name1, NULL, NULL, value, description) \
		AXL_SL_CMD_LINE_MAP_SWITCH(name0) \
		AXL_SL_CMD_LINE_MAP_SWITCH(name1) \

#define AXL_SL_CMD_LINE_SWITCH_3(switchKind, name0, name1, name2, value, description) \
		AXL_SL_CMD_LINE_ADD_SWITCH_INFO(switchKind, name0, name1, name2, NULL, value, description) \
		AXL_SL_CMD_LINE_MAP_SWITCH(name0) \
		AXL_SL_CMD_LINE_MAP_SWITCH(name1) \
		AXL_SL_CMD_LINE_MAP_SWITCH(name2) \

#define AXL_SL_CMD_LINE_SWITCH_4(switchKind, name0, name1, name2, name3, value, description) \
		AXL_SL_CMD_LINE_ADD_SWITCH_INFO(switchKind, name0, name1, name2, name3, value, description) \
		AXL_SL_CMD_LINE_MAP_SWITCH(name0) \
		AXL_SL_CMD_LINE_MAP_SWITCH(name1) \
		AXL_SL_CMD_LINE_MAP_SWITCH(name2) \
		AXL_SL_CMD_LINE_MAP_SWITCH(name3)

#define AXL_SL_CMD_LINE_SWITCH AXL_SL_CMD_LINE_SWITCH_1

#define AXL_SL_END_CMD_LINE_SWITCH_TABLE() \
	} \
};

//..............................................................................

} // namespace sl
} // namespace axl
