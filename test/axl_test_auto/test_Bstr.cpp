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
#include "axl_sys_win_Bstr.h"

namespace {

//..............................................................................

void run() {
	sys::win::Bstr bstr;

	bstr = "hui";
	bstr = L"govno";
	bstr = sl::StringRef("i");
	bstr = sl::StringRef_w(L"muravei");
	bstr = sys::win::Bstr("muravei");
	bstr = sys::win::Bstr(L"govno");
	bstr = sys::win::Bstr(sl::StringRef("i"));
	bstr = sys::win::Bstr(sl::StringRef_w(L"hui"));
}

//..............................................................................

ADD_TEST_CASE("test_Bstr", run)

}
