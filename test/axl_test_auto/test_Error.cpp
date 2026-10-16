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
#include "axl_err_Errno.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_WinError.h"
#	include "axl_sys_win_CfgMgrError.h"
#endif

// {6D3B1C24-9A0E-4E7B-9C21-4F5A2E8D6B10}
// keep out of the anonymous namespace -- AXL_SELECT_ANY can't be internal linkage
AXL_SL_DEFINE_GUID(
	g_testErrorGuid,
	0x6d3b1c24, 0x9a0e, 0x4e7b, 0x9c, 0x21, 0x4f, 0x5a, 0x2e, 0x8d, 0x6b, 0x10
);

namespace {

//..............................................................................

enum {
	TestErrorCode = 42
};

// overload selection leaves no run-time trace of itself -- a mis-resolved call still
// compiles and yields a truthy value. pin each shape down by its return type instead.

#define TEST_OVERLOAD(expr, type) \
	static_assert(std::is_same<decltype(expr), type>::value, "wrong overload: " #expr)

//..............................................................................

// Error::create/format

void
testCreate() {
	err::Error error;

	error.create("abc");
	AXL_TEST_ASSERT(error.getDescription() == "abc");

	error.create(sl::StringRef("xyz"));
	AXL_TEST_ASSERT(error.getDescription() == "xyz");

	error.create(err::g_stdErrorGuid, err::StdErrorCode_String, "guid-code-string");
	AXL_TEST_ASSERT(error.getDescription() == "guid-code-string");

	error.create(g_testErrorGuid, TestErrorCode);
	AXL_TEST_ASSERT(error.isKindOf(g_testErrorGuid, TestErrorCode));

	error.create((uint_t)5);
	AXL_TEST_ASSERT(error.isKindOf(err::g_systemErrorGuid, 5));

	error.format("%s-%d", "abc", 42);
	AXL_TEST_ASSERT(error.getDescription() == "abc-42");

	// create() takes a plain string; only format() expands conversions
	error.create("100%% literal");
	AXL_TEST_ASSERT(error.getDescription() == "100%% literal");

	error.format("100%% formatted");
	AXL_TEST_ASSERT(error.getDescription() == "100% formatted");
}

//..............................................................................

// Error::push -- a push goes on top of the stack, and descriptions
// are joined with ": " from the top down

void
testPush() {
	err::Error error;

	// push on an empty error creates it
	error.push("a");
	AXL_TEST_ASSERT(error.getDescription() == "a");

	error.push(sl::StringRef("b"));
	AXL_TEST_ASSERT(error.getDescription() == "b: a");

	error.push(err::g_stdErrorGuid, err::StdErrorCode_String, "c");
	AXL_TEST_ASSERT(error.getDescription() == "c: b: a");

	error.push("%d", 42);
	AXL_TEST_ASSERT(error.getDescription() == "42: c: b: a");

	err::Error nested;
	nested.create("x");
	error.push(nested);
	AXL_TEST_ASSERT(error.getDescription() == "x: 42: c: b: a");
	AXL_TEST_ASSERT(error.isKindOf(err::g_stdErrorGuid, err::StdErrorCode_Stack));
}

//..............................................................................

// setError/pushError

size_t
setErrorVa(
	const char* formatString,
	...
) {
	AXL_VA_DECL(va, formatString);
	return err::setError_va(formatString, va);
}

void
testSetError() {
	err::setError("abc");
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "abc");

	err::setError(sl::StringRef("xyz"));
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "xyz");

	err::setError(g_testErrorGuid, TestErrorCode);
	AXL_TEST_ASSERT(err::getLastError().isKindOf(g_testErrorGuid, TestErrorCode));

	err::Error error;
	error.create("boom");
	err::setError(error);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "boom");

	err::setError("%s-%d", "abc", 42);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "abc-42");

	setErrorVa("%s/%d", "va", 7);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "va/7");

	// a lone string is a message, never a format string -- neither as a literal...
	err::setError("100%% literal");
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "100%% literal");

	// ...nor as a run-time buffer, which is where a format string would be a hazard
	char buffer[64];
	strncpy(buffer, "buffer %s %d", sizeof(buffer));
	err::setError(buffer);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "buffer %s %d");

	const char* p = "pointer %n";
	err::setError(p);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "pointer %n");
}

void
testPushError() {
	err::setError("a");
	err::pushError("b");
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "b: a");

	err::pushError(sl::StringRef("c"));
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "c: b: a");

	err::pushError("%d", 42);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "42: c: b: a");

	AXL_TEST_ASSERT(err::getLastError()->isStack());
}

//..............................................................................

// fail
//
// fail(failResult, error) and fail(formatString, arg) take the same argument
// shapes; the format overloads bind the format string as an array, so only a
// literal selects them. these must not drift -- a mis-resolved fail() compiles
// and returns a truthy value at every bool return site.

void
testFail() {
	err::ErrorRef errorRef;
	sl::StringRef stringRef;
	const char* sz = "x";

	TEST_OVERLOAD(err::fail(errorRef),                     bool);
	TEST_OVERLOAD(err::fail(stringRef),                    bool);
	TEST_OVERLOAD(err::fail("abc"),                        bool);
	TEST_OVERLOAD(err::fail(sz),                           bool);
	TEST_OVERLOAD(err::fail("a %s", "b"),                  bool);
	TEST_OVERLOAD(err::fail("a %d %s", 1, sz),             bool);
	TEST_OVERLOAD(err::fail<size_t>(-1, errorRef),         size_t);
	TEST_OVERLOAD(err::fail<size_t>(-1, "abc"),            size_t);
	TEST_OVERLOAD(err::fail<size_t>(-1, "a %s", "b"),      size_t);
	TEST_OVERLOAD(err::fail<const char*>(NULL, "abc"),     const char*);
	TEST_OVERLOAD(err::fail<const char*>(NULL, errorRef),  const char*);

	AXL_TEST_ASSERT(err::fail("abc") == false);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "abc");

	AXL_TEST_ASSERT(err::fail(sl::StringRef("xyz")) == false);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "xyz");

	AXL_TEST_ASSERT(err::fail("100%% literal") == false);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "100%% literal");

	AXL_TEST_ASSERT(err::fail("a %s", "b") == false);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "a b");

	AXL_TEST_ASSERT(err::fail("a %d %s", 1, sz) == false);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "a 1 x");

	AXL_TEST_ASSERT(err::fail<size_t>(-1, "sized") == (size_t)-1);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "sized");

	AXL_TEST_ASSERT(err::fail<size_t>(-1, "a %s", "b") == (size_t)-1);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "a b");

	AXL_TEST_ASSERT(err::fail<const char*>(NULL, "pointered") == NULL);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "pointered");

	err::Error error;
	error.create("boom");

	AXL_TEST_ASSERT(err::fail(error) == false);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "boom");

	AXL_TEST_ASSERT(err::fail<const char*>(NULL, error) == NULL);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "boom");

	AXL_TEST_ASSERT(err::fail<size_t>(-1, error) == (size_t)-1);
	AXL_TEST_ASSERT(err::getLastErrorDescription() == "boom");
}

//..............................................................................

// derived error classes -- create() in a subclass hides every base overload, so
// each must reach the base explicitly

void
testDerived() {
	err::Errno errnoError(EINVAL);
	AXL_TEST_ASSERT(errnoError.isKindOf(err::g_errnoGuid, EINVAL));

#if (_AXL_OS_WIN)
	sys::win::WinError winError(ERROR_ACCESS_DENIED);
	AXL_TEST_ASSERT(winError.isKindOf(sys::win::g_winErrorGuid, ERROR_ACCESS_DENIED));

	sys::win::CfgMgrError cfgMgrError(7);
	AXL_TEST_ASSERT(cfgMgrError.isKindOf(sys::win::g_cfgMgrErrorGuid, 7));
#endif
}

//..............................................................................

// packed errors

void
testPack() {
	err::Error error;

	size_t size = error.pack<sl::PackSeq_4<
		const char*,
		int,
		const char*,
		int
	> > (
		err::g_stdErrorGuid,
		TestErrorCode,
		"abc", 0xdead, "xyz", 0xbeaf
	);

	AXL_TEST_ASSERT(size == error.getSize());
	AXL_TEST_ASSERT(size > sizeof(err::ErrorHdr));
	AXL_TEST_ASSERT(error.isKindOf(err::g_stdErrorGuid, TestErrorCode));
}

//..............................................................................

// a custom guid renders through a registered provider; without one the
// description falls back to guid::code

class TestErrorProvider: public err::ErrorProvider {
public:
	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error) {
		return sl::StringRef(
			(const char*)(error + 1),
			error->m_size - sizeof(err::ErrorHdr) - 1
		);
	}
};

void
testProvider() {
	err::Error error;
	error.create(g_testErrorGuid, TestErrorCode, "provided");
	AXL_TEST_ASSERT(error.isKindOf(g_testErrorGuid, TestErrorCode));

	AXL_TEST_ASSERT(error.getDescription() == sl::formatString(
		"%s::%d",
		g_testErrorGuid.getString().sz(),
		TestErrorCode
	));

	static TestErrorProvider provider;
	err::getErrorMgr()->registerProvider(g_testErrorGuid, &provider);
	AXL_TEST_ASSERT(error.getDescription() == "provided");
}

//..............................................................................

void
run() {
	testCreate();
	testPush();
	testSetError();
	testPushError();
	testFail();
	testDerived();
	testPack();
	testProvider(); // registers a provider -- keep last
}

//..............................................................................

AXL_TEST_ADD("test_Error", run)

}
