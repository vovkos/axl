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

#define _AXL_DB_MYSQL_H

#include "axl_db_MySqlRes.h"

namespace axl {
namespace db {

//..............................................................................

// call initMySqlLib once from the main thread

bool
initMySqlLib(
	int argc = 0,
	char** argv = NULL,
	char** groups = NULL
);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// every worker thread which talks to the server must be registered with libmariadb
// (mysql_init/real_connect do it implicitly, endMySqlThread must be explicit)

inline
bool
initMySqlThread() {
	int result = ::mysql_thread_init();
	return !result ? true : err::fail("mysql_thread_init failed");
}

inline
void
endMySqlThread() {
	::mysql_thread_end();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class AutoMySqlThread {
protected:
	bool m_isInitialized;

public:
	AutoMySqlThread() {
		m_isInitialized = initMySqlThread();
	}

	~AutoMySqlThread() {
		if (m_isInitialized)
			endMySqlThread();
	}
};

//..............................................................................


class FreeMySql {
public:
	void
	operator () (MYSQL* h) {
		::mysql_close(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MySql: public sl::Handle<MYSQL*, FreeMySql> {
public:
	bool
	create();

	bool
	connect(
		const sl::StringRef& host,
		const sl::StringRef& user,
		const sl::StringRef& password,
		const sl::StringRef& db,
		uint_t port = 0,
		const sl::StringRef& unixSocket = sl::StringRef(),
		ulong_t flags = 0
	);

	bool
	setOption(
		enum mysql_option option,
		const void* value
	);

	bool
	getOption(
		enum mysql_option option,
		void* value
	);

	bool
	setSsl(
		const sl::StringRef& key,
		const sl::StringRef& cert,
		const sl::StringRef& ca,
		const sl::StringRef& caPath,
		const sl::StringRef& cipherList
	);

	bool
	selectDb(const sl::StringRef& db);

	bool
	setCharSet(const sl::StringRef& name);

	bool
	ping();

	bool
	query(const sl::StringRef& query);

	bool
	sendQuery(const sl::StringRef& query);

	bool
	readQueryResult();

	bool
	storeResult(MySqlRes* res);

	bool
	useResult(MySqlRes* res);

	bool
	nextResult();

	bool
	hasMoreResults() {
		ASSERT(m_h);
		return ::mysql_more_results(m_h) != 0;
	}

	bool
	setAutoCommit(bool isAutoCommit);

	bool
	commit();

	bool
	rollback();

	size_t
	escapeString(
		sl::String* string,
		const sl::StringRef& source
	);

	sl::String
	escapeString(const sl::StringRef& source);

	uint64_t
	getAffectedRowCount() {
		ASSERT(m_h);
		return ::mysql_affected_rows(m_h);
	}

	uint64_t
	getInsertId() {
		ASSERT(m_h);
		return ::mysql_insert_id(m_h);
	}

	uint_t
	getFieldCount() {
		ASSERT(m_h);
		return ::mysql_field_count(m_h);
	}

	uint_t
	getWarningCount() {
		ASSERT(m_h);
		return ::mysql_warning_count(m_h);
	}

	uint_t
	getErrno() {
		ASSERT(m_h);
		return ::mysql_errno(m_h);
	}

	sl::StringRef
	getErrorString() {
		ASSERT(m_h);
		return ::mysql_error(m_h);
	}

	sl::StringRef
	getSqlState() {
		ASSERT(m_h);
		return ::mysql_sqlstate(m_h);
	}

	sl::StringRef
	getInfo() {
		ASSERT(m_h);
		return ::mysql_info(m_h);
	}

	sl::StringRef
	getHostInfo() {
		ASSERT(m_h);
		return ::mysql_get_host_info(m_h);
	}

	sl::StringRef
	getServerInfo() {
		ASSERT(m_h);
		return ::mysql_get_server_info(m_h);
	}

	ulong_t
	getServerVersion() {
		ASSERT(m_h);
		return ::mysql_get_server_version(m_h);
	}

	sl::StringRef
	getCharSetName() {
		ASSERT(m_h);
		return ::mysql_character_set_name(m_h);
	}

	sl::StringRef
	getSslCipher() {
		ASSERT(m_h);
		return ::mysql_get_ssl_cipher(m_h);
	}

#if (_AXL_DB_MARIADB)
	// for non-blocking IO (MYSQL_OPT_NONBLOCK); pair with getTimeoutValueMs()

	my_socket
	getSocket() {
		ASSERT(m_h);
		return ::mysql_get_socket(m_h);
	}

	uint_t
	getTimeoutValueMs() {
		ASSERT(m_h);
		return ::mysql_get_timeout_value_ms(m_h);
	}
#endif
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
MySql::create() {
	close();
	m_h = ::mysql_init(NULL);
	return m_h ? true : err::fail(err::SystemErrorCode_InsufficientResources);
}

inline
bool
MySql::connect(
	const sl::StringRef& host,
	const sl::StringRef& user,
	const sl::StringRef& password,
	const sl::StringRef& db,
	uint_t port,
	const sl::StringRef& unixSocket,
	ulong_t flags
) {
	ASSERT(m_h);

	// NULL selects libmariadb defaults, so we use szn()

	MYSQL* mysql = ::mysql_real_connect(
		m_h,
		host.szn(),
		user.szn(),
		password.szn(),
		db.szn(),
		port,
		unixSocket.szn(),
		flags
	);

	return completeWithLastMySqlError(m_h, mysql != NULL);
}

inline
bool
MySql::setOption(
	enum mysql_option option,
	const void* value
) {
	ASSERT(m_h);
	int result = ::mysql_options(m_h, option, value);
	return completeWithLastMySqlError(m_h, result == 0);
}

inline
bool
MySql::getOption(
	enum mysql_option option,
	void* value
) {
	ASSERT(m_h);
	int result = ::mysql_get_option(m_h, option, value);
	return completeWithLastMySqlError(m_h, result == 0);
}

inline
bool
MySql::setSsl(
	const sl::StringRef& key,
	const sl::StringRef& cert,
	const sl::StringRef& ca,
	const sl::StringRef& caPath,
	const sl::StringRef& cipherList
) {
	ASSERT(m_h);

	int result = ::mysql_ssl_set(
		m_h,
		key.szn(),
		cert.szn(),
		ca.szn(),
		caPath.szn(),
		cipherList.szn()
	);

	return completeWithLastMySqlError(m_h, result == 0);
}

inline
bool
MySql::selectDb(const sl::StringRef& db) {
	ASSERT(m_h);
	int result = ::mysql_select_db(m_h, db.sz());
	return completeWithLastMySqlError(m_h, result == 0);
}

inline
bool
MySql::setCharSet(const sl::StringRef& name) {
	ASSERT(m_h);
	int result = ::mysql_set_character_set(m_h, name.sz());
	return completeWithLastMySqlError(m_h, result == 0);
}

inline
bool
MySql::ping() {
	ASSERT(m_h);
	int result = ::mysql_ping(m_h);
	return completeWithLastMySqlError(m_h, result == 0);
}

inline
bool
MySql::query(const sl::StringRef& query) {
	ASSERT(m_h);
	int result = ::mysql_real_query(m_h, query.cp(), (ulong_t)query.getLength());
	return completeWithLastMySqlError(m_h, result == 0);
}

inline
bool
MySql::sendQuery(const sl::StringRef& query) {
	ASSERT(m_h);
	int result = ::mysql_send_query(m_h, query.cp(), (ulong_t)query.getLength());
	return completeWithLastMySqlError(m_h, result == 0);
}

inline
bool
MySql::readQueryResult() {
	ASSERT(m_h);
	int result = ::mysql_read_query_result(m_h);
	return completeWithLastMySqlError(m_h, !result);
}

inline
bool
MySql::storeResult(MySqlRes* res) {
	ASSERT(m_h);
	MYSQL_RES* h = ::mysql_store_result(m_h);
	if (!h)
		return getErrno() ? failWithLastMySqlError(m_h) : err::fail("the query returned no result set");

	res->attach(h);
	return true;
}

inline
bool
MySql::useResult(MySqlRes* res) {
	ASSERT(m_h);
	MYSQL_RES* h = ::mysql_use_result(m_h);
	if (!h)
		return getErrno() ? failWithLastMySqlError(m_h) : err::fail("the query returned no result set");

	res->attach(h);
	return true;
}

inline
bool
MySql::nextResult() {
	ASSERT(m_h);
	int result = ::mysql_next_result(m_h);
	return
		result == 0 ? true :
		result == -1 ? err::fail("no more result sets") :
		failWithLastMySqlError(m_h);
}

inline
bool
MySql::setAutoCommit(bool isAutoCommit) {
	ASSERT(m_h);
	int result = ::mysql_autocommit(m_h, isAutoCommit);
	return completeWithLastMySqlError(m_h, !result);
}

inline
bool
MySql::commit() {
	ASSERT(m_h);
	int result = ::mysql_commit(m_h);
	return completeWithLastMySqlError(m_h, !result);
}

inline
bool
MySql::rollback() {
	ASSERT(m_h);
	int result = ::mysql_rollback(m_h);
	return completeWithLastMySqlError(m_h, !result);
}

inline
sl::String
MySql::escapeString(const sl::StringRef& source) {
	sl::String string;
	escapeString(&string, source);
	return string;
}

//..............................................................................

} // namespace db
} // namespace axl
