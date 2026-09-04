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

// libmariadb only reports error strings (there is no client-side code-to-string
// table); the numeric code stays available via MySql::getErrno/MySqlStmt::getErrno

inline
size_t
setMySqlError(MYSQL* mysql) {
	const char* error = ::mysql_error(mysql);
	return *error ?
		err::setError(error) :
		err::setError("MySQL error #%d", ::mysql_errno(mysql));
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
	const char* error = ::mysql_stmt_error(stmt);
	return *error ?
		err::setError(error) :
		err::setError("MySQL error #%d", ::mysql_stmt_errno(stmt));
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
