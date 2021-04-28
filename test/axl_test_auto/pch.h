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

//..............................................................................

// axl

#include "axl_sl_Array.h"
#include "axl_sl_HashTable.h"
#include "axl_sl_BoxList.h"
#include "axl_sl_Packer.h"
#include "axl_sl_Unpacker.h"
#include "axl_sl_HandleTable.h"
#include "axl_sl_Packetizer.h"
#include "axl_sl_StringHashTable.h"
#include "axl_sl_ForEach.h"
#include "axl_sl_CmdLineParser.h"
#include "axl_sl_BoyerMooreFind.h"
#include "axl_rc_WeakPtr.h"
#include "axl_rc_Child.h"
#include "axl_io_File.h"
#include "axl_io_MappedFile.h"
#include "axl_lex_RagelLexer.h"

#include "axl_ini_Parser.h"
#include "axl_enc_Base32Encoding.h"
#include "axl_enc_Base32Encoding_nj.h"
#include "axl_sys_Time.h"

using namespace axl;

//..............................................................................

#include <new>
#include <typeinfo>

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#if (_AXL_OS_WIN)
#	include <tchar.h>
#	include <winsock2.h>
#	include <ws2ipdef.h>
#	include <commctrl.h>

#	pragma comment(lib, "ws2_32.lib")
#	pragma comment(lib, "crypt32.lib")
#	pragma comment(lib, "comctl32.lib")
#endif

//..............................................................................
