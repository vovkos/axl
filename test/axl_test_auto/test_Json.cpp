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
#include "axl_json_YajlGen.h"
#include "axl_json_YajlParser.h"
#include "test.h"

namespace {

//..............................................................................

// flattens the whole callback sequence into one string, so a single comparison
// checks values, types and order

class TestParser: public json::YajlParser<TestParser> {
public:
	sl::String m_log;

public:
	bool
	onNull() {
		m_log += "null;";
		return true;
	}

	bool
	onBoolean(bool value) {
		m_log += value ? "true;" : "false;";
		return true;
	}

	// onNumber overrides onInteger/onDouble -- numbers arrive as raw text

	bool
	onNumber(const sl::StringRef& value) {
		return append("num", value);
	}

	bool
	onString(const sl::StringRef& value) {
		return append("str", value);
	}

	bool
	onStartMap() {
		m_log += "{;";
		return true;
	}

	bool
	onMapKey(const sl::StringRef& key) {
		return append("key", key);
	}

	bool
	onEndMap() {
		m_log += "};";
		return true;
	}

	bool
	onStartArray() {
		m_log += "[;";
		return true;
	}

	bool
	onEndArray() {
		m_log += "];";
		return true;
	}

protected:
	bool
	append(
		const sl::StringRef& tag,
		const sl::StringRef& value
	) {
		m_log += tag;
		m_log += ':';
		m_log += value;
		m_log += ';';
		return true;
	}
};

//..............................................................................

void
checkLog(
	const TestParser& parser,
	const sl::StringRef& expected
) {
	if (parser.m_log != expected) {
		printf("expected: %s\n", expected.sz());
		printf("actual:   %s\n", parser.m_log.sz());
	}

	TEST_ASSERT(parser.m_log == expected);
}

// every writer, round-tripped through the parser

void
testGen() {
	json::YajlGen gen;

	bool result =
		gen.openMap() &&
		gen.addNull("null") &&
		gen.addBool("bool", true) &&
		gen.addInteger("int", -42) &&
		gen.addDouble("double", 0.5) &&
		gen.addNumber("number", "12345678901234567890") &&
		gen.addString("string", "a\"b\\c\r\nd\te") &&
		gen.openArray("array") &&
		gen.addInteger(1) &&
		gen.addBool(false) &&
		gen.addNull() &&
		gen.closeArray() &&
		gen.openMap("map") &&
		gen.addString("k", "v") &&
		gen.closeMap() &&
		gen.closeMap();

	TEST_ASSERT(result);

	sl::String json = gen.getString();
	printf("generated: %s\n", json.sz());

	// the quote, backslash and CR/LF must have been escaped, not passed through

	TEST_ASSERT(json.find("\\\"") != -1);
	TEST_ASSERT(json.find("\\\\") != -1);
	TEST_ASSERT(json.find("\\r\\n") != -1);
	TEST_ASSERT(json.find('\r') == -1);

	TestParser parser;
	TEST_ASSERT(parser.parse(json));

	checkLog(
		parser,
		"{;"
		"key:null;null;"
		"key:bool;true;"
		"key:int;num:-42;"
		"key:double;num:0.5;"
		"key:number;num:12345678901234567890;"
		"key:string;str:a\"b\\c\r\nd\te;"
		"key:array;[;num:1;false;null;];"
		"key:map;{;key:k;str:v;};"
		"};"
	);
}

// a hardcoded document, including the escapes a generator would produce

void
testParse() {
	static const char json[] =
		"{\n"
		"    \"name\": \"IO Ninja\",\n"
		"    \"version\": 5,\n"
		"    \"beta\": false,\n"
		"    \"extra\": null,\n"
		"    \"tags\": [ \"a\", \"b\" ],\n"
		"    \"nested\": { \"x\": 0.25 },\n"
		"    \"escaped\": \"l1\\r\\nl2 \\\"q\\\" b\\\\s \\u0041\"\n"
		"}";

	TestParser parser;
	TEST_ASSERT(parser.parse(json));

	checkLog(
		parser,
		"{;"
		"key:name;str:IO Ninja;"
		"key:version;num:5;"
		"key:beta;false;"
		"key:extra;null;"
		"key:tags;[;str:a;str:b;];"
		"key:nested;{;key:x;num:0.25;};"
		"key:escaped;str:l1\r\nl2 \"q\" b\\s A;"
		"};"
	);
}

// malformed input must fail, and the description must come from yajl

void
testParseError() {
	TestParser parser;
	TEST_ASSERT(!parser.parse("{ \"a\": }"));
	printf("parse error: %s\n", err::getLastErrorDescription().sz());
}

// a non-string key is the generator's own error path, rendered by the provider

void
testGenError() {
	json::YajlGen gen;
	TEST_ASSERT(gen.openMap());
	TEST_ASSERT(!gen.addInteger(1));
	printf("gen error: %s\n", err::getLastErrorDescription().sz());
}

//..............................................................................

void
run() {
	json::registerYajlGenErrorProvider();

	testGen();
	testParse();
	testParseError();
	testGenError();
}

//..............................................................................

ADD_TEST_CASE("test_Json", run)

}
