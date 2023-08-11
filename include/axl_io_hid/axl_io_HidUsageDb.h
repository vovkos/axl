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

#define _AXL_IO_HIDUSAGEDB_H

#include "axl_io_HidPch.h"

namespace axl {
namespace io {

class HidUsageDb;
class HidUsagePageDirIniParser;
class HidUsageIniParser;

//..............................................................................

class HidUsagePage: public sl::ListLink {
	friend class HidUsagePageDirIniParser;
	friend class HidUsageIniParser;
	friend class HidUsageDb;

protected:
	const HidUsageDb* m_db;

	union {
		uint_t m_flags;
		struct {
			uint_t m_id             : 31;
			uint_t m_isNameMapReady : 1;
		};
	};

	sl::String m_name;
	sl::String m_fileName;
	sl::String m_format;

	sl::SimpleHashTable<uint_t, sl::String> m_usageNameMap;

public:
	HidUsagePage(const HidUsageDb* db) {
		m_db = db;
		m_flags = 0;
	}

	uint_t
	getId() const {
		return m_id;
	}

	const sl::String&
	getName() const {
		return m_name;
	}

	const sl::String&
	getFileName() const {
		return m_fileName;
	}

	const sl::String&
	getFormat() const {
		return m_format;
	}

	sl::String
	getUsageName(uint_t usage) const;

protected:
	bool
	load();
};

//..............................................................................

class HidUsageDb {
	friend class HidUsagePageDirIniParser;
	friend class HidUsageIniParser;

protected:
	sl::String m_dir;
	sl::List<HidUsagePage> m_usagePageList;
	sl::SimpleHashTable<uint_t, const HidUsagePage*> m_usagePageMap;

public:
	const sl::String&
	getDir() const {
		return m_dir;
	}

	void
	clear();

	bool
	load(const sl::StringRef& fileName);

	const HidUsagePage*
	getUsagePage(uint_t page) const;
};

//..............................................................................

} // namespace io
} // namespace axl
