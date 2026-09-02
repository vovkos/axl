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
#include "axl_db_MySql.h"

namespace axl {
namespace db {

//..............................................................................

bool
initMySqlLib(
	int argc,
	char** argv,
	char** groups
) {
	int result = ::mysql_server_init(argc, argv, groups);
	if (result != 0)
		return err::fail("mysql_server_init failed");

	class MySqlServerEnd {
	public:
		~MySqlServerEnd() {
			::mysql_server_end();
		}
	};

	sl::getSingleton<MySqlServerEnd>();
	return true;
}

//..............................................................................

size_t
MySql::escapeString(
	sl::String* string,
	const sl::StringRef& source
) {
	ASSERT(m_h);

	size_t length = source.getLength();
	if (!length) {
		string->clear();
		return 0;
	}

	if (string->cp() == source.cp()) {
		sl::String tmp;
		length = escapeString(&tmp, source);
		*string = std::move(tmp);
		return length;
	}

	char* p = string->createBuffer(length * 2); // worst case: every char is escaped
	if (!p)
		return -1;

	length = ::mysql_real_escape_string(m_h, p, source.cp(), (ulong_t)length);
	string->overrideLength(length);
	return length;
}

//..............................................................................

} // namespace db
} // namespace axl
