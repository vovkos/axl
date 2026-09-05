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

#include "axl_sl_Handle.h"
#include "axl_sl_String.h"
#include "axl_sl_Array.h"
#include "axl_err_ErrorMgr.h"

#include <mysql.h>

// MariaDB Connector/C is a superset of the Oracle client -- it also carries the
// non-blocking IO calls and the per-statement warning/result-set queries

#ifdef MARIADB_BASE_VERSION
#	define _AXL_DB_MARIADB 1
#endif

// errmsg.h (CR_*) and mysqld_error.h (ER_*) are deliberately NOT included:
// on Windows the CR_* macros collide with cfgmgr32.h CONFIGRET codes.
// Include them locally where you compare against specific error codes.
