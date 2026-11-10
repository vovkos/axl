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

namespace {

//..............................................................................

// parse, check the family & the formatted string

void
testRoundTrip(
	const char* string,
	uint_t family,
	const char* expectedString
) {
	io::SockAddr addr;
	bool result = addr.parse(string);
	AXL_TEST_ASSERT(result);
	AXL_TEST_ASSERT(addr.m_addr.sa_family == family);
	AXL_TEST_ASSERT(addr.getString() == expectedString);

	// re-parse the formatted string -- must give the same address

	io::SockAddr addr2(addr.getString());
	AXL_TEST_ASSERT(addr2.isEqual(addr));
}

void
testRoundTrip(
	const char* string,
	uint_t family
) {
	testRoundTrip(string, family, string);
}

void
testParseFail(const char* string) {
	io::SockAddr addr;
	AXL_TEST_ASSERT(!addr.parse(string));
}

// compare the parsed address bytes against inet_pton

void
testAddr_ip4(const char* string) {
	in_addr expected;
	AXL_TEST_ASSERT(inet_pton(AF_INET, string, &expected) == 1);

	io::SockAddr addr;
	AXL_TEST_ASSERT(addr.parse_ip4(string));
	AXL_TEST_ASSERT(memcmp(&addr.m_addr_ip4.sin_addr, &expected, sizeof(expected)) == 0);
}

void
testAddr_ip6(const char* string) {
	in6_addr expected;
	AXL_TEST_ASSERT(inet_pton(AF_INET6, string, &expected) == 1);

	io::SockAddr addr;
	AXL_TEST_ASSERT(addr.parse_ip6(string));
	AXL_TEST_ASSERT(memcmp(&addr.m_addr_ip6.sin6_addr, &expected, sizeof(expected)) == 0);

	char buffer[INET6_ADDRSTRLEN];
	AXL_TEST_ASSERT(inet_ntop(AF_INET6, &expected, buffer, sizeof(buffer)));
	AXL_TEST_ASSERT(addr.getString_ip6() == buffer);
}

//..............................................................................

void
testIp4() {
	testRoundTrip("192.168.1.1", AF_INET);
	testRoundTrip("192.168.1.1:8080", AF_INET);
	testRoundTrip("0.0.0.0", AF_INET);
	testRoundTrip("255.255.255.255:65535", AF_INET);
	testRoundTrip("8080", AF_INET); // port only
	testRoundTrip("0.0.0.0:0", AF_INET, "0.0.0.0");

	testAddr_ip4("192.168.1.1");
	testAddr_ip4("10.0.0.255");
	testAddr_ip4("127.0.0.1");

	io::SockAddr addr("10.1.2.3:4567");
	AXL_TEST_ASSERT(addr.m_addr.sa_family == AF_INET);
	AXL_TEST_ASSERT(addr.getPort() == 4567);

	in_addr expected;
	inet_pton(AF_INET, "10.1.2.3", &expected);
	AXL_TEST_ASSERT(memcmp(&addr.m_addr_ip4.sin_addr, &expected, sizeof(expected)) == 0);

	// empty string = IPv4 any

	io::SockAddr emptyAddr;
	AXL_TEST_ASSERT(emptyAddr.parse(""));
	AXL_TEST_ASSERT(emptyAddr.m_addr.sa_family == AF_INET);
	AXL_TEST_ASSERT(emptyAddr.getString() == "0.0.0.0");

	testParseFail("1.2.3");
	testParseFail("1.2.3.");
	testParseFail("abc");
	testParseFail("1.2.3.4:");
}

void
testIp6() {
	testRoundTrip("::1", AF_INET6);
	testRoundTrip("2001:db8::1", AF_INET6);
	testRoundTrip("1:2:3:4:5:6:7:8", AF_INET6);
	testRoundTrip("1:0:2:3:4:5:6:7", AF_INET6); // a single zero is not compressed
	testRoundTrip("1:0:0:2:0:0:0:3", AF_INET6, "1:0:0:2::3"); // the longest run is compressed
	testRoundTrip("2001:0DB8:0000:0000:0000:0000:0000:0001", AF_INET6, "2001:db8::1");
	testRoundTrip("fe80::1%3", AF_INET6);
	testRoundTrip("[fe80::1%3]:80", AF_INET6);
	testRoundTrip("[2001:db8::1]:443", AF_INET6);
	testRoundTrip("[::1]", AF_INET6, "::1");
	testRoundTrip("::ffff:192.168.1.1", AF_INET6); // IPv4-mapped
	testRoundTrip("::1.2.3.4", AF_INET6); // IPv4-compatible

	testAddr_ip6("::1");
	testAddr_ip6("1:2:3:4:5:6:1.2.3.4");
	testAddr_ip6("2001:db8::1");
	testAddr_ip6("2001:db8:85a3::8a2e:370:7334");
	testAddr_ip6("fe80::1:2:3:4");
	testAddr_ip6("1:2:3:4:5:6:7:8");
	testAddr_ip6("1:0:0:2:0:0:0:3");
	testAddr_ip6("::ffff:10.0.0.1");

	io::SockAddr addr("[fe80::abcd%7]:5353");
	AXL_TEST_ASSERT(addr.m_addr.sa_family == AF_INET6);
	AXL_TEST_ASSERT(addr.getPort() == 5353);
	AXL_TEST_ASSERT(addr.m_addr_ip6.sin6_scope_id == 7);

	testParseFail("1:2:3");
	testParseFail("[::1");
	testParseFail("[::1]:");
	testParseFail("fe80::1%");
	testParseFail(":");
	testParseFail("1:");
	testParseFail("1:2:3:4:5:6:7");
}

void
testEqualMatch() {
	io::SockAddr addr("192.168.1.1:80");
	AXL_TEST_ASSERT(addr.isEqual(io::SockAddr("192.168.1.1:80")));
	AXL_TEST_ASSERT(!addr.isEqual(io::SockAddr("192.168.1.1:81")));
	AXL_TEST_ASSERT(!addr.isEqual(io::SockAddr("192.168.1.2:80")));
	AXL_TEST_ASSERT(!addr.isEqual(io::SockAddr("[::1]:80")));

	// filter: zero address/port matches any

	AXL_TEST_ASSERT(addr.isMatch(io::SockAddr("192.168.1.1")));
	AXL_TEST_ASSERT(addr.isMatch(io::SockAddr("80")));
	AXL_TEST_ASSERT(addr.isMatch(io::SockAddr("0.0.0.0")));
	AXL_TEST_ASSERT(!addr.isMatch(io::SockAddr("81")));
	AXL_TEST_ASSERT(!addr.isMatch(io::SockAddr("192.168.1.2")));

	io::SockAddr addr6("[2001:db8::1]:80");
	AXL_TEST_ASSERT(addr6.isEqual(io::SockAddr("[2001:db8::1]:80")));
	AXL_TEST_ASSERT(!addr6.isEqual(io::SockAddr("[2001:db8::2]:80")));
	AXL_TEST_ASSERT(!addr6.isMatch(addr));
}

#if (_AXL_IO_SOCKADDR_UN)

void
testUnix() {
	io::UnixSockAddr addr;
	AXL_TEST_ASSERT(addr.parse("/tmp/axl-test.sock"));
	AXL_TEST_ASSERT(addr.m_addr.sa_family == AF_UNIX);
	AXL_TEST_ASSERT(strcmp(addr.m_addr_un.sun_path, "/tmp/axl-test.sock") == 0);
	AXL_TEST_ASSERT(addr.getString() == "/tmp/axl-test.sock");
	AXL_TEST_ASSERT(io::getSockAddrSize(addr) == sizeof(sockaddr_un));
	AXL_TEST_ASSERT(strcmp(io::getSockAddrFamilyString(AF_UNIX), "Unix") == 0);

	// the whole string is the path -- no port or whitespace processing

	io::UnixSockAddr addr2(" relative/path:80 ");
	AXL_TEST_ASSERT(addr2.getString() == " relative/path:80 ");

	// path length limit (leave room for '\0')

	size_t maxLength = sizeof(addr.m_addr_un.sun_path) - 1;
	sl::String path('a', maxLength);
	AXL_TEST_ASSERT(addr2.parse(path));
	AXL_TEST_ASSERT(addr2.getString() == path);

	path.append('a');
	AXL_TEST_ASSERT(!addr2.parse(path));
	AXL_TEST_ASSERT(!addr2.parse(""));

	// equal & match

	AXL_TEST_ASSERT(addr.isEqual(io::UnixSockAddr("/tmp/axl-test.sock")));
	AXL_TEST_ASSERT(!addr.isEqual(io::UnixSockAddr("/tmp/axl-test2.sock")));
	AXL_TEST_ASSERT(!addr.isEqual(io::SockAddr("192.168.1.1")));
	AXL_TEST_ASSERT(addr.isMatch(io::UnixSockAddr("/tmp/axl-test.sock")));
	AXL_TEST_ASSERT(!addr.isMatch(io::UnixSockAddr("/tmp/other.sock")));
	AXL_TEST_ASSERT(!io::SockAddr("192.168.1.1").isMatch(io::UnixSockAddr("/tmp/axl-test.sock")));

	io::UnixSockAddr anyAddr; // empty path matches any
	anyAddr.m_addr.sa_family = AF_UNIX;
	AXL_TEST_ASSERT(addr.isMatch(anyAddr));

	// setup from a generic sockaddr

	io::UnixSockAddr addr3((const sockaddr*)addr);
	AXL_TEST_ASSERT(addr3.isEqual(addr));
	AXL_TEST_ASSERT(addr3.getString() == "/tmp/axl-test.sock");

	// the generic formatter dispatches on the family

	AXL_TEST_ASSERT(io::getSockAddrString(addr) == "/tmp/axl-test.sock");
}

#endif

//..............................................................................

void
run() {
	testIp4();
	testIp6();
	testEqualMatch();
#if (_AXL_IO_SOCKADDR_UN)
	testUnix();
#endif
}

//..............................................................................

AXL_TEST_ADD("test_SockAddr", run)

}
