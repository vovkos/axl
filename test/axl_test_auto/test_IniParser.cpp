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

#include "pch.h"
#include "test.h"

namespace {

//..............................................................................

class MyParser: public ini::Parser<MyParser> {
protected:
	enum SectionKind {
		SectionKind_Undefined,
		SectionKind_Session,
		SectionKind_Scripts,
	};

	enum KeyKind {
		KeyKind_Undefined,
		KeyKind_Name,
		KeyKind_Description,
		KeyKind_Guid,
		KeyKind_Icon,
		KeyKind_LogRepresenterClass,
		KeyKind_SessionClass,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(SectionNameMap, SectionKind)
		AXL_SL_HASH_TABLE_ENTRY("session", SectionKind_Session)
		AXL_SL_HASH_TABLE_ENTRY("scripts", SectionKind_Scripts)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(KeyNameMap, KeyKind)
		AXL_SL_HASH_TABLE_ENTRY("name", KeyKind_Name)
		AXL_SL_HASH_TABLE_ENTRY("description", KeyKind_Description)
		AXL_SL_HASH_TABLE_ENTRY("guid", KeyKind_Guid)
		AXL_SL_HASH_TABLE_ENTRY("icon", KeyKind_Icon)
		AXL_SL_HASH_TABLE_ENTRY("session-class", KeyKind_SessionClass)
		AXL_SL_HASH_TABLE_ENTRY("log-representer-class", KeyKind_LogRepresenterClass)
	AXL_SL_END_HASH_TABLE()

public:
	bool
	onSection(const char* name) {
		SectionNameMap::Iterator it = SectionNameMap::find(name);
		SectionKind section = it ? it->m_value : SectionKind_Undefined;
		printf("OnSection '%s'\n", name);
		return true;
	}

	bool
	onKeyValue(
		const char* name,
		const char* value
	) {
		KeyNameMap::Iterator it = KeyNameMap::find(name);
		KeyKind key = it ? it->m_value : KeyKind_Undefined;
		printf("OnKeyValue '%s' = '%s'\n", name, value);
		return true;
	}
};

void
run() {
	bool result;

	const char* filePath = "D:/Prj/Ninja/ioninja/scripts/nj_ssn/nj_ssn_Serial/nj_ssn_Serial.njini";

	io::MappedFile file;
	result = file.open(filePath, io::FileFlag_ReadOnly);
	if (!result) {
		printf("error opening file: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	const char* p = (const char*) file.view();
	uint64_t size = file.getSize();

	MyParser parser;
	result = parser.parse(filePath, sl::StringRef(p, (size_t)size));
	if (!result) {
		printf("error parsing file: %s\n", err::getLastErrorDescription().sz());
		return;
	}
}

//..............................................................................

ADD_TEST_CASE("test_IniParser", run)

}
