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

#include "axl_io_HidDb.h"
#include "axl_ini_Parser.h"

namespace axl {
namespace io {

//..............................................................................

class HidUsagePageDirIniParser: public ini::Parser<HidUsagePageDirIniParser> {
protected:
	enum Section {
		Section_Undefined,
		Section_UsagePage,
	};

	enum Key {
		Key_Undefined,
		Key_Id,
		Key_Name,
		Key_FileName,
		Key_Format,
	};

protected:
	AXL_SL_BEGIN_STRING_HASH_TABLE(SectionNameMap, Section)
		AXL_SL_HASH_TABLE_ENTRY("usage-page", Section_UsagePage)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(KeyNameMap, Key)
		AXL_SL_HASH_TABLE_ENTRY("id", Key_Id)
		AXL_SL_HASH_TABLE_ENTRY("name", Key_Name)
		AXL_SL_HASH_TABLE_ENTRY("file", Key_FileName)
		AXL_SL_HASH_TABLE_ENTRY("format", Key_Format)
	AXL_SL_END_HASH_TABLE()

protected:
	HidDb* m_db;
	HidUsagePage* m_page;

public:
	HidUsagePageDirIniParser(HidDb* db) {
		m_db = db;
		m_page = NULL;
	}

	~HidUsagePageDirIniParser() {
		delete m_page;
	}

	bool
	onSection(const sl::StringRef& sectionName);

	bool
	onKeyValue(
		const sl::StringRef& keyName,
		const sl::StringRef& value
	);

	bool
	finalize() {
		return m_page ? finalizePage() : true;
	}

protected:
	bool
	finalizePage();
};

//..............................................................................

class HidUsageIniParser: public ini::Parser<HidUsageIniParser> {
protected:
	enum Section {
		Section_Undefined,
		Section_Usages,
	};

protected:
	AXL_SL_BEGIN_STRING_HASH_TABLE(SectionNameMap, Section)
		AXL_SL_HASH_TABLE_ENTRY("usages", Section_Usages)
	AXL_SL_END_HASH_TABLE()

protected:
	HidUsagePage* m_page;
	Section m_section;

public:
	HidUsageIniParser(HidUsagePage* page) {
		m_page = page;
		m_section = Section_Undefined;
	}

	bool
	onSection(const sl::StringRef& sectionName) {
		m_section = SectionNameMap::findValue(sectionName, Section_Undefined);
		return true; // ignore unknown sections
	}

	bool
	onKeyValue(
		const sl::StringRef& keyName,
		const sl::StringRef& value
	);
};

//..............................................................................

} // namespace io
} // namespace axl
