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

#define _AXL_DB_MYSQLRES_H

#include "axl_db_MySqlError.h"

namespace axl {
namespace db {

//..............................................................................

class FreeMySqlRes {
public:
	void
	operator () (MYSQL_RES* h) {
		::mysql_free_result(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class MySqlRes: public sl::Handle<MYSQL_RES*, FreeMySqlRes> {
public:
	MySqlRes() {}

	MySqlRes(MYSQL_RES* h):
		sl::Handle<MYSQL_RES*, FreeMySqlRes>(h) {}

	uint64_t
	getRowCount() {
		ASSERT(m_h);
		return ::mysql_num_rows(m_h);
	}

	uint_t
	getFieldCount() {
		ASSERT(m_h);
		return ::mysql_num_fields(m_h);
	}

	const MYSQL_FIELD*
	getFields() {
		ASSERT(m_h);
		return ::mysql_fetch_fields(m_h);
	}

	const MYSQL_FIELD*
	getField(uint_t i) {
		ASSERT(m_h && i < getFieldCount());
		return ::mysql_fetch_field_direct(m_h, i);
	}

	MYSQL_ROW
	fetchRow() {
		ASSERT(m_h);
		return ::mysql_fetch_row(m_h);
	}

	// only valid for the row returned by the most recent fetchRow()

	const ulong_t*
	fetchLengths() {
		ASSERT(m_h);
		return ::mysql_fetch_lengths(m_h);
	}

	void
	seekRow(uint64_t offset) {
		ASSERT(m_h);
		::mysql_data_seek(m_h, offset);
	}

	void
	seekField(uint_t offset) {
		ASSERT(m_h);
		::mysql_field_seek(m_h, (MYSQL_FIELD_OFFSET)offset);
	}
};

//..............................................................................

} // namespace db
} // namespace axl
