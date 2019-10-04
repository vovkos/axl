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

typedef
void
TestFunc();

typedef
sl::StringHashTable<TestFunc*> TestSet;

inline
TestSet*
getTestCaseSet()
{
	static int32_t flag = 0;
	return sl::getSingleton<TestSet> (&flag);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class AddTestCase
{
public:
	AddTestCase(
		const char* name,
		TestFunc* func
		)
	{
		getTestCaseSet()->add(name, func);
	}
};

#define ADD_TEST_CASE(name, func) \
	static AddTestCase testCaseAdder_ ## func(name, func);

//..............................................................................

// make ASSERT work even in Release

#undef ASSERT
#define ASSERT(e) ((bool) (e) ? \
	(void) 0 : \
	(void)(throw "Assertion failure at: " __FILE__ "(" AXL_ITOA(__LINE__) "): " #e))

//..............................................................................
