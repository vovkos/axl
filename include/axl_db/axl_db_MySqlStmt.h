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

#define _AXL_DB_MYSQLSTMT_H

#include "axl_db_MySqlRes.h"

namespace axl {
namespace db {

//..............................................................................

enum MySqlStmtFetchResult {
	MySqlStmtFetchResult_NoData        = MYSQL_NO_DATA,
	MySqlStmtFetchResult_DataTruncated = MYSQL_DATA_TRUNCATED,
	MySqlStmtFetchResult_Success       = 0,
	MySqlStmtFetchResult_Error         = 1,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CloseMySqlStmt {
public:
	void
	operator () (MYSQL_STMT* h) {
		::mysql_stmt_close(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MySqlStmt: public sl::Handle<MYSQL_STMT*, CloseMySqlStmt> {
public:
	bool
	create(MYSQL* mysql);

	bool
	prepare(const sl::StringRef& query);

	bool
	setAttr(
		enum enum_stmt_attr_type attr,
		const void* value
	);

	bool
	getAttr(
		enum enum_stmt_attr_type attr,
		void* value
	);

	bool
	bindParams(MYSQL_BIND* bindTable);

	bool
	bindResult(MYSQL_BIND* bindTable);

	bool
	sendLongData(
		uint_t param,
		const void* p,
		size_t size
	);

	bool
	execute();

	MySqlStmtFetchResult
	fetch();

	bool
	fetchColumn(
		MYSQL_BIND* bind,
		uint_t column,
		ulong_t offset = 0
	);

	bool
	storeResult();

	bool
	freeResult();

	bool
	nextResult();

#if (_AXL_DB_MARIADB)
	bool
	moreResults() {
		ASSERT(m_h);
		return ::mysql_stmt_more_results(m_h) != 0;
	}
#endif

	bool
	reset();

	bool
	getResultMetaData(MySqlRes* res);

	void
	seekRow(uint64_t offset) {
		ASSERT(m_h);
		::mysql_stmt_data_seek(m_h, offset);
	}

	uint_t
	getParamCount() {
		ASSERT(m_h);
		return (uint_t)::mysql_stmt_param_count(m_h);
	}

	uint_t
	getFieldCount() {
		ASSERT(m_h);
		return ::mysql_stmt_field_count(m_h);
	}

	uint64_t
	getRowCount() {
		ASSERT(m_h);
		return ::mysql_stmt_num_rows(m_h);
	}

	uint64_t
	getAffectedRowCount() {
		ASSERT(m_h);
		return ::mysql_stmt_affected_rows(m_h);
	}

	uint64_t
	getInsertId() {
		ASSERT(m_h);
		return ::mysql_stmt_insert_id(m_h);
	}

#if (_AXL_DB_MARIADB)
	uint_t
	getWarningCount() {
		ASSERT(m_h);
		return ::mysql_stmt_warning_count(m_h);
	}
#endif

	uint_t
	getErrno() {
		ASSERT(m_h);
		return ::mysql_stmt_errno(m_h);
	}

	sl::StringRef
	getErrorString() {
		ASSERT(m_h);
		return ::mysql_stmt_error(m_h);
	}

	sl::StringRef
	getSqlState() {
		ASSERT(m_h);
		return ::mysql_stmt_sqlstate(m_h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
MySqlStmt::create(MYSQL* mysql) {
	close();
	m_h = ::mysql_stmt_init(mysql);
	return m_h ? true : failWithLastMySqlError(mysql);
}

inline
bool
MySqlStmt::prepare(const sl::StringRef& query) {
	ASSERT(m_h);
	int result = ::mysql_stmt_prepare(m_h, query.cp(), (ulong_t)query.getLength());
	return completeWithLastMySqlStmtError(m_h, result == 0);
}

inline
bool
MySqlStmt::setAttr(
	enum enum_stmt_attr_type attr,
	const void* value
) {
	ASSERT(m_h);
	int result = ::mysql_stmt_attr_set(m_h, attr, value);
	return completeWithLastMySqlStmtError(m_h, !result);
}

inline
bool
MySqlStmt::getAttr(
	enum enum_stmt_attr_type attr,
	void* value
) {
	ASSERT(m_h);
	int result = ::mysql_stmt_attr_get(m_h, attr, value);
	return completeWithLastMySqlStmtError(m_h, !result);
}

inline
bool
MySqlStmt::bindParams(MYSQL_BIND* bindTable) {
	ASSERT(m_h);
	int result = ::mysql_stmt_bind_param(m_h, bindTable);
	return completeWithLastMySqlStmtError(m_h, !result);
}

inline
bool
MySqlStmt::bindResult(MYSQL_BIND* bindTable) {
	ASSERT(m_h);
	int result = ::mysql_stmt_bind_result(m_h, bindTable);
	return completeWithLastMySqlStmtError(m_h, !result);
}

inline
bool
MySqlStmt::sendLongData(
	uint_t param,
	const void* p,
	size_t size
) {
	ASSERT(m_h);
	int result = ::mysql_stmt_send_long_data(m_h, param, (const char*)p, (ulong_t)size);
	return completeWithLastMySqlStmtError(m_h, !result);
}

inline
bool
MySqlStmt::execute() {
	ASSERT(m_h);
	int result = ::mysql_stmt_execute(m_h);
	return completeWithLastMySqlStmtError(m_h, result == 0);
}

inline
MySqlStmtFetchResult
MySqlStmt::fetch() {
	ASSERT(m_h);
	int result = ::mysql_stmt_fetch(m_h);
	if (result == 1) {
		setMySqlStmtError(m_h);
		return MySqlStmtFetchResult_Error;
	}

	return (MySqlStmtFetchResult)result;
}

inline
bool
MySqlStmt::fetchColumn(
	MYSQL_BIND* bind,
	uint_t column,
	ulong_t offset
) {
	ASSERT(m_h);
	int result = ::mysql_stmt_fetch_column(m_h, bind, column, offset);
	return completeWithLastMySqlStmtError(m_h, result == 0);
}

inline
bool
MySqlStmt::storeResult() {
	ASSERT(m_h);
	int result = ::mysql_stmt_store_result(m_h);
	return completeWithLastMySqlStmtError(m_h, result == 0);
}

inline
bool
MySqlStmt::freeResult() {
	ASSERT(m_h);
	int result = ::mysql_stmt_free_result(m_h);
	return completeWithLastMySqlStmtError(m_h, !result);
}

inline
bool
MySqlStmt::nextResult() {
	ASSERT(m_h);
	int result = ::mysql_stmt_next_result(m_h);
	return
		result == 0 ? true :
		result == -1 ? err::fail("no more result sets") :
		failWithLastMySqlStmtError(m_h);
}

inline
bool
MySqlStmt::reset() {
	ASSERT(m_h);
	int result = ::mysql_stmt_reset(m_h);
	return completeWithLastMySqlStmtError(m_h, !result);
}

inline
bool
MySqlStmt::getResultMetaData(MySqlRes* res) {
	ASSERT(m_h);
	MYSQL_RES* h = ::mysql_stmt_result_metadata(m_h);
	if (!h)
		return getErrno() ? failWithLastMySqlStmtError(m_h) : err::fail("the statement produces no result set");

	res->attach(h);
	return true;
}

//..............................................................................

} // namespace db
} // namespace axl
