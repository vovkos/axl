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

#define _AXL_DB_MYSQLERROR_H

#include "axl_db_MySqlPch.h"

namespace axl {
namespace db {

//..............................................................................

// {F8E8B8D2-0303-4ECF-BD0E-4130616457C9}
AXL_SL_DEFINE_GUID(
	g_mySqlErrorGuid,
	0xf8e8b8d2, 0x0303, 0x4ecf, 0xbd, 0xe, 0x41, 0x30, 0x61, 0x64, 0x57, 0xc9
);

//..............................................................................

class MySqlErrorProvider: public err::ErrorProvider {
public:
	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
registerMySqlErrorProvider() {
	err::getErrorMgr()->registerProvider(
		g_mySqlErrorGuid,
		sl::getSimpleSingleton<MySqlErrorProvider>()
	);
}

//..............................................................................

// libmariadb only reports error strings (there is no client-side code-to-string
// table), hence, the description is carried inside the error payload

class MySqlError: public err::Error {
public:
	MySqlError() {}

	MySqlError(
		uint_t code,
		const sl::StringRef& description
	) {
		create(code, description);
	}

	size_t
	create(
		uint_t code,
		const sl::StringRef& description
	) {
		return createStringError(g_mySqlErrorGuid, code, description);
	}
};

//..............................................................................

inline
size_t
setMySqlError(MYSQL* mysql) {
	return err::setError(MySqlError(::mysql_errno(mysql), ::mysql_error(mysql)));
}

inline
bool
failWithLastMySqlError(MYSQL* mysql) {
	setMySqlError(mysql);
	return false;
}

inline
bool
completeWithLastMySqlError(
	MYSQL* mysql,
	bool result
) {
	if (!result)
		setMySqlError(mysql);

	return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
size_t
setMySqlStmtError(MYSQL_STMT* stmt) {
	return err::setError(MySqlError(::mysql_stmt_errno(stmt), ::mysql_stmt_error(stmt)));
}

inline
bool
failWithLastMySqlStmtError(MYSQL_STMT* stmt) {
	setMySqlStmtError(stmt);
	return false;
}

inline
bool
completeWithLastMySqlStmtError(
	MYSQL_STMT* stmt,
	bool result
) {
	if (!result)
		setMySqlStmtError(stmt);

	return result;
}

//..............................................................................

} // namespace db
} // namespace axl
