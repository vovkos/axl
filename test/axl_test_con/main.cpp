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

//..............................................................................

#if (_AXL_OS_WIN)

void
printAdapterAddress(const IP_ADDR_STRING* ipAddrString) {
	for (size_t i = 1; ipAddrString; ipAddrString = ipAddrString->Next, i++) {
		printf("  #%-2d IP:   %s\n", i, ipAddrString->IpAddress.String);
		printf("  #%-2d MASK: %s\n", i, ipAddrString->IpMask.String);
	}
}

void
printAdapterInfo(IP_ADAPTER_INFO* ipAdapter) {
	switch (ipAdapter->Type) {
	case MIB_IF_TYPE_LOOPBACK:
		printf("MIB_IF_TYPE_LOOPBACK\n");
		break;

	case MIB_IF_TYPE_ETHERNET:
		printf("MIB_IF_TYPE_ETHERNET\n");
		break;

	case IF_TYPE_IEEE80211:
		printf("IF_TYPE_IEEE80211\n");
		break;

	case MIB_IF_TYPE_PPP:
		printf("MIB_IF_TYPE_PPP\n");
		break;

	case MIB_IF_TYPE_TOKENRING:
		printf("MIB_IF_TYPE_TOKENRING\n");
		break;

	case MIB_IF_TYPE_SLIP:
		printf("MIB_IF_TYPE_SLIP\n");
		break;

	default:
		printf("unknown adapter type #%d\n", ipAdapter->Type);
	}

	printf("AdapterName = %s\n", ipAdapter->AdapterName);
	printf("Description = %s\n", ipAdapter->Description);
	printf("MAC         = %02x:%02x:%02x:%02x:%02x:%02x\n",
		ipAdapter->Address[0],
		ipAdapter->Address[1],
		ipAdapter->Address[2],
		ipAdapter->Address[3],
		ipAdapter->Address[4],
		ipAdapter->Address[5]
	);
	printf("DHCP        = %d\n", ipAdapter->DhcpEnabled);

	printf("ADDRESS:\n");
	printAdapterAddress(&ipAdapter->IpAddressList);

	printf("GATEWAY:\n");
	printAdapterAddress(&ipAdapter->GatewayList);

	printf("DHCP:\n");
	printAdapterAddress(&ipAdapter->DhcpServer);

	printf("---\n\n");
}

void
testWinNetworkAdapterList() {
	printf("Using GetAdaptersInfo...\n\n");

	dword_t size = 0;
	dword_t error = ::GetAdaptersInfo(NULL, &size);
	if (error != ERROR_BUFFER_OVERFLOW) {
		printf("GetAdaptersInfo failed (%d)\n", error);
		return;
	}

	char buffer[256];
	sl::Array<char> bufferArray(rc::BufKind_Stack, buffer, sizeof(buffer));
	bufferArray.setCount(size);

	IP_ADAPTER_INFO* ipAdapter = (IP_ADAPTER_INFO*)bufferArray.p();
	error = ::GetAdaptersInfo(ipAdapter, &size);
	if (error != ERROR_SUCCESS) {
		printf("GetAdaptersInfo failed (%d)\n", error);
		return;
	}

	size_t count = 0;
	for (; ipAdapter; ipAdapter = ipAdapter->Next, count++)
		printAdapterInfo(ipAdapter);

	printf("%d adapters found\n---------------------------------\n\n", count);
}

void
testWinNetworkAdapterList2() {
	printf("Using WinIoctl...\n\n");

	printf(
		"sizeof(sockaddr) = %d\n"
		"sizeof(sockaddr_in) = %d\n"
		"sizeof(sockaddr_in6) = %d\n"
		"sizeof(sockaddr_storage) = %d\n"
		"sizeof(io::SockAddr) = %d\n",
		sizeof(sockaddr),
		sizeof(sockaddr_in),
		sizeof(sockaddr_in6),
		sizeof(sockaddr_storage),
		sizeof(io::SockAddr)
	);

	io::win::Socket socket;
	bool result = socket.wsaOpen(AF_INET, SOCK_DGRAM, 0);
	if (!result) {
		printf("socket.wsaOpen failed (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	size_t size = 16 * sizeof(INTERFACE_INFO);
	sl::Array<char> buffer;
	buffer.setCount(size);

	dword_t actualSize;

	for (;;) {
		int result = ::WSAIoctl(
			socket,
			SIO_GET_INTERFACE_LIST,
			NULL,
			0,
			buffer.p(),
			size,
			&actualSize,
			NULL,
			NULL
		);

		if (result == ERROR_SUCCESS)
			break;

		dword_t error = ::WSAGetLastError();
		if (error != WSAENOBUFS) {
			printf("WSAIoctl failed (%s)\n", err::Error (error).getDescription().sz());
			return;
		}

		size *= 2;
		buffer.setCount(size);
	}

	const INTERFACE_INFO* iface = (const INTERFACE_INFO*) buffer.cp();
	size_t ifaceCount = actualSize / sizeof(INTERFACE_INFO);

	for (size_t i = 0; i < ifaceCount; iface++, i++) {
		printf("Interface #%d\n", i);
		printf("  Address   = %s\n", io::getSockAddrString((const sockaddr*) &iface->iiAddress).sz());
		printf("  Broadcast = %s\n", io::getSockAddrString((const sockaddr*) &iface->iiBroadcastAddress).sz());
		printf("  Netmask   = %s\n", io::getSockAddrString((const sockaddr*) &iface->iiNetmask).sz());

/*		cout << endl;

		sockaddr_in *pAddress;
		pAddress = (sockaddr_in *) & (InterfaceList[i].iiAddress);
		cout << " " << inet_ntoa(pAddress->sin_addr);

		pAddress = (sockaddr_in *) & (InterfaceList[i].iiBroadcastAddress);
		cout << " has bcast " << inet_ntoa(pAddress->sin_addr);

		pAddress = (sockaddr_in *) & (InterfaceList[i].iiNetmask);
		cout << " and netmask " << inet_ntoa(pAddress->sin_addr) << endl;

		cout << " Iface is ";
		u_long nFlags = InterfaceList[i].iiFlags;
		if (nFlags & IFF_UP) cout << "up";
		else                 cout << "down";
		if (nFlags & IFF_POINTTOPOINT) cout << ", is point-to-point";
		if (nFlags & IFF_LOOPBACK)     cout << ", is a loopback iface";
		cout << ", and can do: ";
		if (nFlags & IFF_BROADCAST) cout << "bcast ";
		if (nFlags & IFF_MULTICAST) cout << "multicast ";
		cout << endl; */
	}

	printf("%d adapters found\n---------------------------------\n\n", ifaceCount);
}

bool
getFileTimes_nt(const char* fileName) {
	using namespace axl::sys::win;

	io::File file;
	bool result = file.open(fileName);
	if (!result) {
		printf("can't open %s: %s\n", fileName, err::getLastErrorDescription().sz());
		return false;
	}

	IO_STATUS_BLOCK ioStatus;
	FILE_BASIC_INFORMATION basicInfo = { 0 };

	NTSTATUS status = ntQueryInformationFile(
		(HANDLE)file.m_file,
		&ioStatus,
		&basicInfo,
		sizeof(FILE_BASIC_INFORMATION),
		FileBasicInformation
	);

	if (status < 0) {
		printf("can't query file information: %s: %s\n", fileName, sys::win::NtStatus (status).getDescription().sz());
		return false;
	}

	printf("NT File times for: %s\n", fileName);
	printf("  CreationTime:   %s\n", sys::Time(basicInfo.CreationTime.QuadPart).format().sz());
	printf("  LastWriteTime:  %s\n", sys::Time(basicInfo.LastWriteTime.QuadPart).format().sz());
	printf("  LastAccessTime: %s\n", sys::Time(basicInfo.LastAccessTime.QuadPart).format().sz());
	printf("  ChangeTime:     %s\n", sys::Time(basicInfo.ChangeTime.QuadPart).format().sz());

	return true;
}

bool
getFileTimes_win(const char* fileName) {
	io::File file;
	bool result = file.open(fileName);
	if (!result) {
		printf("can't open %s: %s\n", fileName, err::getLastErrorDescription().sz());
		return false;
	}

	FILE_BASIC_INFO basicInfo2 = { 0 };
	result = ::GetFileInformationByHandleEx(file.m_file, FileBasicInfo, &basicInfo2, sizeof(basicInfo2)) != 0;
	if (!result) {
		err::setLastSystemError();
		printf("can't query file information: %s, %s\n", fileName, err::getLastErrorDescription().sz());
		return false;
	}

	printf("WIN File times for: %s\n", fileName);
	printf("  CreationTime:   %s\n", sys::Time(basicInfo2.CreationTime.QuadPart).format().sz());
	printf("  LastWriteTime:  %s\n", sys::Time(basicInfo2.LastWriteTime.QuadPart).format().sz());
	printf("  LastAccessTime: %s\n", sys::Time(basicInfo2.LastAccessTime.QuadPart).format().sz());
	printf("  ChangeTime:     %s\n", sys::Time(basicInfo2.ChangeTime.QuadPart).format().sz());

	return true;
}

void
testFileTime() {
	getFileTimes_win("c:/1/far/ioninja-3.7.4/bin/ioninja.exe");
	getFileTimes_nt("c:/1/far/ioninja-3.7.4/bin/ioninja.exe");
	getFileTimes_win("c:/1/7z/ioninja-3.7.4/bin/ioninja.exe");
	getFileTimes_nt("c:/1/7z/ioninja-3.7.4/bin/ioninja.exe");
}

#endif

//..............................................................................

void
testNetworkAdapterList() {
	printf("Enumerating adapters...\n");

	sl::List<io::NetworkAdapterDesc> adapterList;
	io::enumerateNetworkAdapters(&adapterList);

	printf("%d adapters found.\n", adapterList.getCount());

	sl::Iterator<io::NetworkAdapterDesc> adapterIt = adapterList.getHead();
	for (; adapterIt; adapterIt++) {
		io::NetworkAdapterDesc* adapter = *adapterIt;

		printf("Name        = %s\n", adapter->m_name.sz());
		printf("Description = %s\n", adapter->m_description.sz());
		printf("Type        = %s\n", io::getNetworkAdapterTypeString(adapter->m_type));
		printf("Flags       = %s\n", io::getNetworkAdapterFlagString(adapter->m_flags).sz());
		printf("Mac         = %s\n", io::getMacAddressString(adapter->m_macAddress).sz());

		sl::ConstList<io::NetworkAdapterAddress> addressList = adapter->m_addressList;
		sl::ConstIterator<io::NetworkAdapterAddress> addressIt = addressList.getHead();
		for (size_t i = 1; addressIt; addressIt++, i++) {
			const io::NetworkAdapterAddress* address = *addressIt;

			uint_t family = address->m_address.m_addr.sa_family;
			printf("%-11s = %s",
				io::getSockAddrFamilyString(family),
				address->m_address.getString().sz()
			);

			if (family == AF_INET) {
				io::SockAddr netMask;
				netMask.createNetMask_ip4(address->m_netMaskBitCount);
				printf(" (mask %s)\n", netMask.getString().sz());
			} else {
				printf("/%d\n", address->m_netMaskBitCount);
			}
		}

		printf("\n");
	}

	printf("%d adapters found\n", adapterList.getCount());
}

//..............................................................................

void
testParseFormatIp6() {
	printf("main()\n");

	bool result;

	io::SockAddr sockAddr;

	result = sockAddr.parse("6");
	printf("result = %d, addr = %s\n", result, sockAddr.getString().sz());

	result = sockAddr.parse("[1::2:3:4:5]:6");
	printf("result = %d, addr = %s\n", result, sockAddr.getString().sz());

	result = sockAddr.parse("[fe80::c990:d16e:a986:d56b%11]:10001");
	printf("result = %d, addr = %s\n", result, sockAddr.getString().sz());

	sockaddr_in6 addr = { 0 };
	addr.sin6_family = AF_INET6;

	for (;;) {
		uint16_t* ip = (uint16_t*) &addr.sin6_addr;

		ip[0] = rand() % 2 ? rand() : 0;
		ip[1] = rand() % 2 ? rand() : 0;
		ip[2] = rand() % 2 ? rand() : 0;
		ip[3] = rand() % 2 ? rand() : 0;
		ip[4] = rand() % 2 ? rand() : 0;
		ip[5] = rand() % 2 ? rand() : rand() % 2 ? 0xffff : 0;
		ip[6] = rand() % 2 ? rand() : 0;
		ip[7] = rand() % 2 ? rand() : 0;
#if (_AXL_OS_WIN)
		addr.sin6_port = rand() % 2 ? rand() : 0;
		addr.sin6_scope_id = rand() % 2 ? rand() : 0;
#endif

		sl::String addrString = io::getSockAddrString((const sockaddr*) &addr).sz();
		printf("addr1 = %s\n", addrString.sz());

		char addrString2[1024] = { 0 };
		dword_t size = sizeof(addrString2);

#if (_AXL_OS_WIN)
		WSAAddressToStringA((sockaddr*) &addr, sizeof(addr), NULL, addrString2, &size);
#elif (_AXL_OS_POSIX)
		inet_ntop(AF_INET6, &addr.sin6_addr, addrString2, size);
#endif

		printf("addr2 = %s\n\n", addrString2);
		ASSERT(addrString.cmp(addrString2) == 0);

		sockaddr_in6 addr2;
		result = io::parseSockAddr((sockaddr*) &addr2, sizeof(addr2), addrString);
		ASSERT(result && memcmp(&addr, &addr2, sizeof(addr)) == 0);
	}
}

//..............................................................................

#ifndef TCP_KEEPIDLE
#	define TCP_KEEPIDLE  TCP_KEEPALIVE
#endif

#ifndef TCP_KEEPINTVL
#	define TCP_KEEPINTVL 17
#endif

void
testKeepAlives(const sl::StringRef& addrString) {
	dword_t value = 1;
	dword_t delay = 3;

	io::Socket socket;

	printf("Opening a TCP socket (%d sec keep-alives)...\n", delay);

	bool result =
		socket.open(AF_INET, SOCK_STREAM, IPPROTO_TCP) &&
		socket.setOption(SOL_SOCKET, SO_KEEPALIVE, &value, sizeof(value)) &&
		socket.setOption(IPPROTO_TCP, TCP_KEEPIDLE, &delay, sizeof(delay)) &&
		socket.setOption(IPPROTO_TCP, TCP_KEEPINTVL, &delay, sizeof(delay));

	if (!result) {
		printf("socket.open failed (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("Parsing address '%s'...\n", addrString.sz());

	io::SockAddr addr;
	result = addr.parse(addrString);
	if (!result) {
		printf("addr.parse failed (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("Connecting to %s...\n", addr.getString().sz());

	result = socket.connect(addr);
	if (!result) {
		printf("socket.connect failed (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("Press CTRL+C to exit...\n");

	for (;;) {
		sys::sleep(1000);
	}
}

//..............................................................................

void
testAddrInfoIp6() {
	const char* name = "tibbo.com";

	sl::Array<io::SockAddr> addrArray;
	bool result = io::resolveHostName(&addrArray, name, AF_INET6);
	if (!result) {
		printf("io::resolveHostName failed (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("host name %s resolved to:\n", name);

	size_t count = addrArray.getCount();
	for (size_t i = 0; i < count; i++)
		printf("    %s\n", addrArray [i].getString().sz());
}

//..............................................................................

void
testDynamicLib() {
	sys::DynamicLib dl;

	bool result = dl.open("libc.so.6");
	if (!result) {
		printf("dl.load failed: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	typedef int Printf(const char*, ...);
	Printf* prn = (Printf*)dl.getFunction("printf");
	if (!prn) {
		printf("dl.load failed: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	prn("hui govno i muravei %d\n", 123);
}

//..............................................................................

void
bar() {
	printf("bar -- throwing...\n");
	AXL_SYS_SJLJ_THROW();
}

void
testLongJmpTry() {
	AXL_SYS_BEGIN_SJLJ_TRY() {
		bar();
	}
	AXL_SYS_SJLJ_CATCH() {
		printf("exception caught\n");
	}
	AXL_SYS_SJLJ_FINALLY() {
		printf("finally\n");
	}
	AXL_SYS_END_SJLJ_TRY()

	AXL_SYS_BEGIN_SJLJ_TRY() {
		bar();
	}
	AXL_SYS_SJLJ_CATCH() {
		printf("exception caught\n");
	}
	AXL_SYS_END_SJLJ_TRY()

	AXL_SYS_BEGIN_SJLJ_TRY() {
		bar();
	}
	AXL_SYS_SJLJ_FINALLY() {
		printf("finally\n");
	}
	AXL_SYS_END_SJLJ_TRY()

	AXL_SYS_BEGIN_SJLJ_TRY() {
		bar();
	}
	AXL_SYS_END_SJLJ_TRY()

	printf("done\n");
}

//..............................................................................

#if (_AXL_RE)

void
testCharSet() {
	re::CharSet charSet;

	charSet.add(10, 12);
	charSet.add(14, 20);
	printf("%s\n", charSet.getString().sz());

	charSet.invert();
	printf("%s\n", charSet.getString().sz());

	charSet.invert();
	printf("%s\n", charSet.getString().sz());

	charSet.add(13);
	printf("%s\n", charSet.getString().sz());

	charSet.invert();
	printf("%s\n", charSet.getString().sz());

	charSet.invert();
	printf("%s\n", charSet.getString().sz());

	for (utf32_t c = 5; c < 25; c++)
		printf("isSet(%d): %d\n", c, charSet.isSet(c));
}

#define _AXL_RE_TEST_SIMPLE_MATCH    0
#define _AXL_RE_TEST_STREAM    0
#define _AXL_RE_TEST_SWITCH    1
#define _AXL_RE_TEST_LOAD_SAVE 1
#define _AXL_RE_TEST_FULL_DFA  1

void
testUsbRegex() {
	static const char file[] =
		/* "#\n"
		"#	List of USB ID's\n"
		"#\n"
		"#	Maintained by Stephen J. Gowdy <linux.usb.ids@gmail.com>\n"
		"#	If you have any new entries, please submit them via\n"
		"#		http://www.linux-usb.org/usb-ids.html\n"
		"#	or send entries as patches (diff -u old new) in the\n"
		"#	body of your email (a bot will attempt to deal with it).\n"
		"#	The latest version can be obtained from\n"
		"#		http://www.linux-usb.org/usb.ids\n"
		"#\n"
		"# Version: 2016.10.13\n"
		"# Date:    2016-10-13 20:34:14\n"
		"#\n"
		"\n"
		"# Vendors, devices and interfaces. Please keep sorted.\n"
		"\n"
		"# Syntax:\n"
		"# vendor  vendor_name\n"
		"#	device  device_name				<-- single tab\n"
		"#		interface  interface_name		<-- two tabs\n"
		"\n" */
		"0001  Fry's Electronics\n"
		"	7778  Counterfeit flash drive [Kingston]\n"
		"0002  Ingram\n"
		"0003  Club Mac\n"
		"0004  Nebraska Furniture Mart\n"
		"0011  Unknown\n"
		"	7788  counterfeit flash drive\n"
		"0053  Planex\n"
		"	5301  GW-US54ZGL 802.11bg\n"
		"0079  DragonRise Inc.\n"
		"	0006  PC TWIN SHOCK Gamepad\n"
		"	0011  Gamepad\n"
		"0105  Trust International B.V.\n"
		"	145f  NW-3100 802.11b/g 54Mbps Wireless Network Adapter [zd1211]\n"
		"0127  IBP\n"
		"	0002  HDM Interface\n"
		"0145  Unknown\n"
		"	0112  Card Reader\n"
		"017c  MLK\n"
		"	145f  Trust Deskset\n"
		"0200  TP-Link\n"
		"	0201  MA180 UMTS Modem\n"
		"0204  Chipsbank Microelectronics Co., Ltd\n"
		"	6025  CBM2080 / CBM2090 Flash drive controller\n"
		"	6026  CBM1180 Flash drive controller\n"
		;

	re::Regex regex;

	regex.createSwitch();
	regex.compileSwitchCase("(\\h{4})\\s+([^\\n]+[^\\s])\\s*\\n");
	regex.compileSwitchCase("\\t(\\h{4})\\s+([^\\n]+[^\\s])\\s*\\n");
	regex.compileSwitchCase("\\s*#[^\\n]*\\n");
	regex.compileSwitchCase("\\s*\\n");
	regex.finalizeSwitch();

	// printf("NFA:\n");
	// regex.printNfa();

	/*
	regex.compile("a{3}");//|a+|ba");
	printf("NFA:\n");
	regex.printNfa();
	regex.buildFullDfa();
	 printf("\nDFA:\n");
	regex.printDfa();
	regex.buildFullReverseDfa();
	printf("\nrDFA:\n");
	regex.printReverseDfa();
	// */

	const char* p = file;
	const char* end = p + lengthof(file);

	re::State state;

	while (p < end) {
		re::ExecResult result = regex.exec(&state, p, end - p);
		if (!result) {
			printf("mismatch @%d\n", p - file);
			break;
		}

		const re::Match* match = state.getMatch();
		printf(
			"match-%d: @%d(%dB) '%s'\n",
			state.getMatchAcceptId(),
			match->getOffset(),
			match->getSize(),
			match->getText().sz()
		); // all matches are nl-terminated

		p += match->getSize();
	}
}

void
testRegex() {
	re::Regex regex;
	re::State state;
	const re::Match* match = NULL;
	size_t count = 0;
	bool result;
	const char* p;
	const char* end;

	do {

		result = regex.compile("\x91");
		if (!result) {
			printf("error: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		regex.buildFullDfa();
		regex.buildFullReverseDfa();
#if (_AXL_DEBUG)
		printf("\nNFA:\n");
		regex.printNfa();
		printf("\nDFA:\n");
		regex.printDfa();
		printf("\nrDFA:\n");
		regex.printReverseDfa();
#endif

		sl::String s("abc\x91ijk\x93lmn");

		re::State state;
		re::ExecResult result = regex.exec(&state, s);
		ASSERT(result == re::ExecResult_Match);

		printf(
			"match(%d): %d..%d (%s)\n",
			state.getMatchAcceptId(),
			state.getMatch()->getOffset(),
			state.getMatch()->getEndOffset(),
			state.getMatch()->getText().sz()
		);

		return;
	} while (0);

	do {
		result = regex.compile("^abc");
		if (!result) {
			printf("error: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		regex.buildFullDfa();
		regex.buildFullReverseDfa();
#if (_AXL_DEBUG)
		printf("\nNFA:\n");
		regex.printNfa();
		printf("\nDFA:\n");
		regex.printDfa();
		printf("\nrDFA:\n");
		regex.printReverseDfa();
#endif

		re::State state;
		re::ExecResult result = regex.exec(&state, "abcd");
		ASSERT(result == re::ExecResult_Match);

		printf(
			"match(%d): %d..%d (%s)\n",
			state.getMatchAcceptId(),
			state.getMatch()->getOffset(),
			state.getMatch()->getEndOffset(),
			state.getMatch()->getText().sz()
		);

		result = regex.exec(&state, "xyz\nabcd");
		ASSERT(result == re::ExecResult_Match);

		printf(
			"match(%d): %d..%d (%s)\n",
			state.getMatchAcceptId(),
			state.getMatch()->getOffset(),
			state.getMatch()->getEndOffset(),
			state.getMatch()->getText().sz()
		);

		return;
	} while (0);

	do {
		regex.createSwitch();

		result = regex.compileSwitchCase("\\s*(foo\\d*)\\s+(bar\\d*)?\\s*");
		if (!result) {
			printf("error: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		regex.finalizeSwitch();

		re::StateInit init(re::ExecFlag_AnchorDataBegin);
		re::State state(init);
		re::ExecResult result = regex.exec(&state, " foo123 bar567 ");
		ASSERT(result == re::ExecResult_Match);

		printf(
			"match(%d): %d..%d (%s)\n",
			state.getMatchAcceptId(),
			state.getMatch()->getOffset(),
			state.getMatch()->getEndOffset(),
			state.getMatch()->getText().sz()
		);

		return;
	} while (0);

	do {
		regex.createSwitch();

		result =
			regex.compileSwitchCase("open[ \t]*\\d*[\r\n]") &&
			regex.compileSwitchCase("close[ \t]*\\d*[\r\n]") &&
			regex.compileSwitchCase("connect[ \t]*\\d*[\r\n]") &&
			regex.compileSwitchCase("\\s+");

		if (!result) {
			printf("error: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		regex.finalizeSwitch();

		re::StateInit init(re::ExecFlag_AnchorDataBegin | re::ExecFlag_Stream);
		re::State state(init);
		re::ExecResult result = regex.exec(&state, "op");
		ASSERT(result == re::ExecResult_Continue);

		result = regex.exec(&state, "en 12\n con");
		ASSERT(result == re::ExecResult_MatchOffsetsOnly);

		printf(
			"match(%d): %d..%d\n",
			state.getMatchAcceptId(),
			state.getMatch()->getOffset(),
			state.getMatch()->getEndOffset()
		);

		return;
	} while (0);

	do {
		result = regex.compile("<[^>]*>");
		if (!result) {
			printf("error: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		regex.buildFullDfa();
		regex.buildFullReverseDfa();
#if (_AXL_DEBUG)
		printf("\nDFA:\n");
		regex.printDfa();
		printf("\nrDFA:\n");
		regex.printReverseDfa();
#endif

		re::StateInit init(re::ExecFlag_Stream | re::ExecFlag_DisableCapture);
		re::State state(init);
		re::ExecResult result = regex.exec(&state, "abc<def>ghi");
		ASSERT(result == re::ExecResult_Match);

		printf(
			"match: %d..%d: '%s'\n",
			state.getMatch()->getOffset(),
			state.getMatch()->getEndOffset(),
			state.getMatch()->getText().sz()
		);
		return;
	} while (0);

	do {
		result = regex.compile("char|[a-z]*1");
		if (!result) {
			printf("error: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		regex.buildFullDfa();
		regex.buildFullReverseDfa();
#if (_AXL_DEBUG)
		printf("\nDFA:\n");
		regex.printDfa();
		printf("\nrDFA:\n");
		regex.printReverseDfa();
#endif

		re::StateInit init(re::ExecFlag_Stream | re::ExecFlag_DisableCapture);
		re::State state(init);
		re::ExecResult result = regex.exec(&state, "abcharzzz");
		ASSERT(result == re::ExecResult_Continue);

		result = regex.eof(&state, false);
		ASSERT(result == re::ExecResult_ContinueBackward);

		result = regex.exec(&state, "zz");
		ASSERT(result == re::ExecResult_ContinueBackward);

		result = regex.exec(&state, "abccharz");
		ASSERT(result == re::ExecResult_Match);

		printf(
			"match: %d..%d: '%s'\n",
			state.getMatch()->getOffset(),
			state.getMatch()->getEndOffset(),
			state.getMatch()->getText().sz()
		);
		return;
	} while (0);

	do {
		result = regex.compile("[5-9]+");
		if (!result) {
			printf("error: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		regex.buildFullDfa();
		regex.buildFullReverseDfa();
#if (_AXL_DEBUG)
		printf("\nDFA:\n");
		regex.printDfa();
		printf("\nrDFA:\n");
		regex.printReverseDfa();
#endif

		re::StateInit init(re::ExecFlag_Stream | re::ExecFlag_DisableCapture);
		re::State state(init);
		re::ExecResult result = regex.exec(&state, "56");
		ASSERT(result == re::ExecResult_Continue);

		result = regex.exec(&state, "");
		ASSERT(result == re::ExecResult_Continue);

		result = regex.exec(&state, "78");
		ASSERT(result == re::ExecResult_Continue);

		printf("state: %d\n", state.getDfaStateId());

		result = regex.eof(&state, false);
		ASSERT(result == re::ExecResult_ContinueBackward);

		result = regex.exec(&state, "");
		ASSERT(result == re::ExecResult_ContinueBackward);

		result = regex.exec(&state, "12345678");
		ASSERT(result == re::ExecResult_Match);

		printf(
			"match: %d..%d: '%s'\n",
			state.getMatch()->getOffset(),
			state.getMatch()->getEndOffset(),
			state.getMatch()->getText().sz()
		);
		return;
	} while (0);

	do {
		result = regex.compile("[a-z]+");
		if (!result) {
			printf("error: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		regex.buildFullDfa();
		regex.buildFullReverseDfa();
#if (_AXL_DEBUG)
		printf("\nDFA:\n");
		regex.printDfa();
		printf("\nrDFA:\n");
		regex.printReverseDfa();
#endif

		re::State state(re::ExecFlag_Stream | re::ExecFlag_DisableCapture);
		re::ExecResult result = regex.exec(&state, "abc\n\n");
		ASSERT(result == re::ExecResult_Match);
		printf("match: %d..%d\n", state.getMatch()->getOffset(), state.getMatch()->getEndOffset());

		result = regex.exec(&state, "\n\n");
		ASSERT(result == re::ExecResult_Continue);

		result = regex.exec(&state, "abc\n\n");
		ASSERT(result == re::ExecResult_ContinueBackward);

		result = regex.exec(&state, "abc\n\n");
		ASSERT(result == re::ExecResult_MatchOffsetsOnly);
		printf("match: %d..%d\n", state.getMatch()->getOffset(), state.getMatch()->getEndOffset());
		return;
	} while (0);

	do {
		result = regex.compile("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
		if (!result) {
			printf("error: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		regex.buildFullDfa();
		regex.buildFullReverseDfa();
#if (_AXL_DEBUG)
		printf("\nDFA:\n");
		regex.printDfa();
		printf("\nrDFA:\n");
		regex.printReverseDfa();
#endif

		re::State state(re::ExecFlag_Stream | re::ExecFlag_DisableCapture);

		re::ExecResult result = regex.exec(&state, "  ");
		result = regex.exec(&state, "  abcdefghijklmnopqrstuvwxyz");
		result = regex.exec(&state, "abcdefghijklmnopqrstuvwxyz");

		const re::Match* match = state.getMatch();
		if (match)
			printf("match: %d..%d\n", match->getOffset(), match->getEndOffset());
		else
			printf("mismatch\n");

		return;
	} while (0);

	do {
		result = regex.compile("a|ba");
		if (!result) {
			printf("error: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		regex.buildFullDfa();
		regex.buildFullReverseDfa();
#if (_AXL_DEBUG)
		printf("\nDFA:\n");
		regex.printDfa();
		printf("\nrDFA:\n");
		regex.printReverseDfa();
#endif

		re::State state = regex.exec("...ba...");
		const re::Match* match = state.getMatch();
		if (match)
			printf("match: %d..%d '%s'\n", match->getOffset(), match->getEndOffset(), match->getText().sz());
		else
			printf("mismatch\n");

		return;
	} while (0);

	do {
		printf("compiling\n");

		regex.createSwitch();
		regex.compileSwitchCase("[0-9]+");
		regex.compileSwitchCase(".");
		regex.finalizeSwitch();

		printf("matching\n");

		re::ExecResult result;
		re::State state(re::StateInit(re::ExecFlag_AnchorDataBegin | re::ExecFlag_Stream, 1000));

		result = regex.exec(&state, "\r123456\r");
		printf("regex result: %d, match offset: %d, match length: %d, match: %s\n", result, state.getMatch()->getOffset(), state.getMatch()->getSize(), state.getMatch()->getText().sz());
		ASSERT(result == re::ExecResult_Match && state.getMatchAcceptId() == 1 && state.getMatch()->getSize() == 1);

		result = regex.exec(&state, "123456");
		printf("regex result: %d\n", result);
		ASSERT(result == re::ExecResult_Continue);

		result = regex.exec(&state, "\x2e");
		printf("regex result: %d, match offset: %d, match length: %d\n", result, state.getMatch()->getOffset(), state.getMatch()->getSize());
		ASSERT(result == re::ExecResult_MatchOffsetsOnly && state.getMatchAcceptId() == 0 && state.getMatch()->getSize() == 6);

		result = regex.exec(&state, "123456");
		result = regex.eof(&state, true);
		printf("regex result: %d, match offset: %d, match length: %d, match: %s\n", result, state.getMatch()->getOffset(), state.getMatch()->getSize(), state.getMatch()->getText().sz());
		ASSERT(result == re::ExecResult_Match && state.getMatchAcceptId() == 0 && state.getMatch()->getSize() == 6);

		return;
	} while (0);


	// const char src[] = "[\xd0\xb1-\xd0\xb3]+";
	// const char src[] = "a|[a-z]+1";
	// const char src[] = "x(a*b)*(a*c)*";
	const char src[] = "\\b([a-c]+)([0-9]+)\\b";
	// const char src[] = "^$";

	printf("REGEX: %s\n\n", src);

	result = regex.compile(src);
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

#if (_AXL_DEBUG)
	printf("NFA:\n");
	regex.printNfa();

#	if (_AXL_RE_TEST_DFA)
	regex.buildFullDfa();
	regex.buildFullReverseDfa();
	regex.buildFullRollbackDfa();

	printf("\nDFA:\n");
	regex.printDfa();
	printf("\nrDFA:\n");
	regex.printReverseDfa();
#	endif
#endif

#if (_AXL_RE_TEST_LOAD_SAVE)
	sl::Array<char> storage;
	regex.save(&storage);
	printf("\nNFA storage: %d B\n", storage.getCount());

	re::Regex regex2;
	result = regex2.load(storage) != -1;
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

#	if (_AXL_DEBUG)
	regex2.printNfa();
#	endif
#endif

//	const char text[] = "suka\xd0\xb0\xd0\xb1\xd0\xb2\xd0\xb3\xd0\xb4\xd0\xb5\xd0\xb6hui";
//	const char text[] = "ahgbcbcbcdedsdds";
	const char text[] = "   abc123   ";
//	const char text[] = "xaaabbbbcd";

#if (_AXL_RE_TEST_SIMPLE_MATCH)
	printf("\nMATCHING TEXT: '%s'\n", text);

	state = regex.exec(text);
	if (!state) {
		printf("NO MATCH!\n");
		return;
	}

	match = state.getMatch();
	ASSERT(match);

	printf(
		"$0: %p(%d) '%s'\n",
		match->getOffset(),
		match->getSize(),
		match->getText().sz()
	);

	count = state.getCaptureCount();
	for (size_t i = 1; i < count; i++) {
		const re::Match* capture = state.getCapture(i);
		if (capture)
			printf(
				"$%d: %p(%d) '%s'\n",
				i,
				capture->getOffset(),
				capture->getSize(),
				capture->getText().sz()
			);
	}
#endif

#if (_AXL_RE_TEST_STREAM)
	printf("STREAM MATCH: '%s':\n", text);

	state.initialize(re::ExecFlag_Stream);

	p = text;
	end = text + lengthof(text);
	for (; p < end; p++) {
		re::ExecResult result = regex.exec(&state, p, 1);
		if (result != re::ExecResult_Continue) {
			if (result != re::ExecResult_ContinueBackward)
				break;

			for (p--; p >= text; p--) {
				result = regex.exec(&state, p, 1);
				if (result >= 0)
					break;
			}

			break;
		}
	}

	match = state.getMatch();
	if (!match) {
		printf("NO MATCH!\n");
		return;
	}

	printf(
		"$0: %p(%d)\n",
		match->getOffset(),
		match->getSize()
	);
#endif

#if (_AXL_RE_TEST_SWITCH)
	regex.createSwitch();
	regex.compileSwitchCase("char");
	regex.compileSwitchCase("int");
	regex.compileSwitchCase("long");
	regex.compileSwitchCase("[0-9]+");
	regex.compileSwitchCase("0x[0-9a-fA-F]+");
	regex.compileSwitchCase("[a-zA-Z_][a-zA-Z_0-9]*");
	regex.compileSwitchCase("\\s+");
	regex.finalizeSwitch();

	static const char* caseNameMap[] = {
		"<char>",
		"<int>",
		"<long>",
		"<decimal>",
		"<hexadecimal>",
		"<identifier>",
		"<whitespace>",
	};

#if (_AXL_DEBUG)
	printf("\nNFA:\n");
	regex.printNfa();

	printf("\nDFA:\n");
	regex.buildFullDfa();
	regex.printDfa();
#endif

#if (_AXL_RE_TEST_LOAD_SAVE)
	regex.save(&storage);
	printf("\nNFA storage: %d B\n", storage.getCount());

	result = regex2.load(storage) != -1;
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

#	if (_AXL_DEBUG)
	regex2.printNfa();
#	endif
#endif

	static const char source[] =
		"suka\n"
		" int\n"
		"  long\n"
		"12345\n"
		" 0xabcdef\n"
		"  0x123abdef\n"
		"suka\n"
		" suka_hui_123\n";

	p = source;
	end = p + lengthof(source);

	state.initialize(re::ExecFlag_AnchorDataBegin);
	while (p < end) {
		re::ExecResult result = regex.exec(&state, p, end - p);
		if (!result) {
			printf("NO MATCH!\n");
			break;
		}

		size_t id = state.getMatchAcceptId();
		match = state.getMatch();
		ASSERT(match);

		printf("#%d %s: %p(%d) '%s'\n",
			id,
			caseNameMap[id],
			match->getOffset(),
			match->getSize(),
			match->getText().sz()
		);

		p += match->getSize();
	}
#endif
}

void
testRegex2() {
	re::Regex regex;
	bool result;

	static const char src[] = "[a-b]*c+";
	printf("REGEX: %s\n\n", src);

	result = regex.compile(re::CompileFlag_DisableCapture, src);
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

#if (_AXL_DEBUG)
	printf("NFA:\n");
	regex.printNfa();

	printf("\nDFA:\n");
	regex.buildFullDfa();
	regex.printDfa();
#endif
}

#endif

//..............................................................................

#if (_AXL_OS_WIN)

//..............................................................................

enum {
	BufferSize = 4 * 1024,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
testNamedPipes() {
	using namespace axl::sys::win;

	NTSTATUS status;

	io::win::File pipeDir;
	bool result = pipeDir.create(
		L"\\\\.\\pipe\\",
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		0
	);

	if (!result) {
		err::setLastSystemError();
		printf("cannot open pipe dir: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	sl::Array<char> dirBuffer;
	dirBuffer.setCount(BufferSize);

	sl::String_utf16 fileName;

	BOOLEAN isFirstQuery = TRUE;

	for (;;) {
		IO_STATUS_BLOCK ioStatus;

		status = ntQueryDirectoryFile(
			pipeDir,
			NULL,
			NULL,
			0,
			&ioStatus,
			dirBuffer,
			dirBuffer.getCount(),
			sys::win::FileDirectoryInformation,
			FALSE,
			NULL,
			isFirstQuery
		);

		if (status < 0) {
			if (status == STATUS_NO_MORE_FILES)
				break;

			err::setError(sys::win::NtStatus(status));
			printf("cannot open pipe dir: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		FILE_DIRECTORY_INFORMATION* dirInfo = (FILE_DIRECTORY_INFORMATION*)dirBuffer.p();
		for (;;) {
			fileName.copy(dirInfo->FileName, dirInfo->FileNameLength / 2);

			printf(
				dirInfo->AllocationSize.LowPart == -1 ? "%S (%d)\n" : "%S (%d of %d)\n",
				fileName.sz(),
				dirInfo->EndOfFile.LowPart,
				dirInfo->AllocationSize.LowPart
			);

			if (!dirInfo->NextEntryOffset)
				break;

			dirInfo = (FILE_DIRECTORY_INFORMATION*)((char*)dirInfo + dirInfo->NextEntryOffset);
		}

		isFirstQuery = FALSE;
	}
}

//..............................................................................

bool
querySymbolicLink(
	sl::String_utf16* string,
	HANDLE dir,
	UNICODE_STRING* uniName
) {
	using namespace axl::sys::win;

	NTSTATUS status;

	string->clear();

	OBJECT_ATTRIBUTES oa = { 0 };
	oa.Length = sizeof(oa);
	oa.RootDirectory = dir;
	oa.ObjectName = uniName;

	sys::win::Handle link; // NT uses NULL for invalid handle value
	status = ntOpenSymbolicLinkObject(
		link.p(),
		GENERIC_READ,
		&oa
	);

	if (status < 0) {
		err::setError(sys::win::NtStatus(status));
		printf("cannot open symbolic link: %s\n", err::getLastErrorDescription().sz());
		return false;
	}

	wchar_t* p = string->createBuffer(BufferSize);
	size_t length = string->getLength();

	UNICODE_STRING uniTarget;
	uniTarget.Buffer = p;
	uniTarget.Length = 0;
	uniTarget.MaximumLength = length + sizeof(wchar_t);

	status = ntQuerySymbolicLinkObject(link, &uniTarget, NULL);
	if (status < 0) {
		err::setError(sys::win::NtStatus(status));
		printf("cannot query symbolic link: %s\n", err::getLastErrorDescription().sz());
		return false;
	}

	string->overrideLength(uniTarget.Length / sizeof(wchar_t));
	return true;
}

void
enumerateDirectory(
	HANDLE baseDir,
	const wchar_t* name,
	size_t level
) {
	using namespace axl::sys::win;

	NTSTATUS status;

	UNICODE_STRING uniName;
	uniName.Buffer = (wchar_t*)name;
	uniName.Length = wcslen_s(name)* sizeof(wchar_t);
	uniName.MaximumLength = uniName.Length + sizeof(wchar_t);

	OBJECT_ATTRIBUTES oa = { 0 };
	oa.Length = sizeof(oa);
	oa.RootDirectory = baseDir;
	oa.ObjectName = &uniName;

	sys::win::Handle dir; // NT uses NULL for invalid handle value
	status = ntOpenDirectoryObject(
		dir.p(),
		DIRECTORY_QUERY | DIRECTORY_TRAVERSE,
		&oa
	);

	if (status < 0) {
		err::setError(sys::win::NtStatus(status));
		printf("cannot open directory: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	sl::Array<char> buffer;
	buffer.setCount(BufferSize);

	ULONG queryContext = 0;
	BOOLEAN isFirstQuery = TRUE;

	sl::String_utf16 dirName;
	sl::String_utf16 dirTypeName;
	sl::String_utf16 symLinkTargetName;

	level++;
	sl::String indent((utf32_t) ' ', level * 2);

	for (;;) {
		ULONG actualSize;

		status = ntQueryDirectoryObject(
			dir,
			buffer,
			buffer.getCount(),
			FALSE,
			isFirstQuery,
			&queryContext,
			&actualSize
		);

		if (status < 0) {
			if (status == STATUS_NO_MORE_ENTRIES)
				break;

			err::setError(sys::win::NtStatus(status));
			printf("cannot query directory: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		OBJECT_DIRECTORY_INFORMATION* dirInfo = (OBJECT_DIRECTORY_INFORMATION*)buffer.p();
		for (; dirInfo->Name.Buffer; dirInfo++) {
			dirName.copy(dirInfo->Name.Buffer, dirInfo->Name.Length / sizeof(wchar_t));
			dirTypeName.copy(dirInfo->TypeName.Buffer, dirInfo->TypeName.Length / sizeof(wchar_t));

			printf("%s%S (%S)\n", indent.sz(), dirName.sz(), dirTypeName.sz());

			if (dirTypeName.cmp(L"Directory") == 0) {
				enumerateDirectory(dir, dirName, level);
			} else if (dirTypeName.cmp(L"SymbolicLink") == 0) {
				bool result = querySymbolicLink(&symLinkTargetName, dir, &dirInfo->Name);
				if (result)
					printf("%s  --> %S\n", indent.sz(), symLinkTargetName.sz());
			}

		}

		isFirstQuery = FALSE;
	}
}

void
testDirectoryObjects() {
	using namespace axl::sys::win;

	sl::String_utf16 s;

	io::win::File f;
	bool result = f.create(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING);

	struct ObjectInfo: OBJECT_NAME_INFORMATION {
		char m_buffer[1024];
	} objectNameInfo;

	ULONG actualSize;
	NTSTATUS status = ntQueryObject(f, ObjectNameInformation, &objectNameInfo, sizeof(objectNameInfo), &actualSize);

	sl::String ss;
	result = io::getSymbolicLinkTarget(&ss, "COM3");

	WCHAR* linkName = L"\\\\.\\COM3";
	size_t length = wcslen(linkName);

	UNICODE_STRING uniLink;
	uniLink.Buffer = linkName;
	uniLink.Length = length * sizeof(WCHAR);
	uniLink.MaximumLength = (length + 1) * sizeof(wchar_t);

	querySymbolicLink(&s, NULL, &uniLink);

	printf("target: %S\n", s.sz());


	printf("\\\n");
	enumerateDirectory(NULL, L"\\", 0);
}

#endif

void
testTcp() {
#if (_AXL_OS_WIN)
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
	int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
	ASSERT(s != -1);

	linger l;
	l.l_onoff = 1;
	l.l_linger = 0;
	setsockopt(s, SOL_SOCKET, SO_LINGER, (const char*) &l, sizeof(l));

	sockaddr_in a = { 0 };
	a.sin_family = AF_INET;
	a.sin_addr.s_addr = inet_addr("127.0.0.1");
	a.sin_port = htons(10001);

	printf("connecting...\n");
	int result = connect(s, (const sockaddr*) &a, sizeof(a));
	printf("result = %d\n", result);

	char buf[512];
	scanf("%d", buf);

	int error = 0;
	socklen_t len = sizeof(error);
	result = getsockopt(s, SOL_SOCKET, SO_ERROR, (char*) &error, &len);

	printf("receiving...\n");
	result = recv(s, buf, sizeof(buf), 0);
	printf("result = %d\n", result);

	printf("receiving...\n");
	result = recv(s, buf, 0, 0);
	printf("result = %d\n", result);

	printf("closing...\n");
#if (_AXL_OS_WIN)
	closesocket(s);
#elif (_AXL_OS_POSIX)
	close(s);
#endif

	printf("done.\n");
}

//..............................................................................

#if (_AXL_OS_WIN)

void
printWinErrorDescription(
	dword_t code,
	uint_t langId,
	uint_t subLangId
) {
	wchar_t* message = NULL;

	::FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_MAX_WIDTH_MASK, // no line breaks please
		NULL,
		code,
		MAKELANGID(langId, subLangId),
		(LPWSTR) &message,
		0,
		NULL
	);

	sl::String description = message;
	::LocalFree(message);

	printf("%d/%d: %s\n", langId, subLangId, description.sz());
}

void
testWinError() {
	dword_t error = ERROR_ACCESS_DENIED;

	printWinErrorDescription(error, LANG_NEUTRAL, SUBLANG_NEUTRAL);
	printWinErrorDescription(error, LANG_NEUTRAL, SUBLANG_DEFAULT);
	printWinErrorDescription(error, LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);

	printWinErrorDescription(error, LANG_SYSTEM_DEFAULT, SUBLANG_NEUTRAL);
	printWinErrorDescription(error, LANG_SYSTEM_DEFAULT, SUBLANG_DEFAULT);
	printWinErrorDescription(error, LANG_SYSTEM_DEFAULT, SUBLANG_SYS_DEFAULT);

	printWinErrorDescription(error, LANG_ENGLISH, SUBLANG_NEUTRAL);
	printWinErrorDescription(error, LANG_ENGLISH, SUBLANG_DEFAULT);
	printWinErrorDescription(error, LANG_ENGLISH, SUBLANG_SYS_DEFAULT);
	printWinErrorDescription(error, LANG_ENGLISH, SUBLANG_ENGLISH_US);
	printWinErrorDescription(error, LANG_ENGLISH, SUBLANG_ENGLISH_UK);
}

#endif

void
testTimestamp() {
	for (int i = 0; i < 20; i++) {
		uint64_t t1 = sys::getTimestamp();
		printf("%s: %llu\n", sys::Time(t1).format("%h:%m:%s.%l.%c").sz(), t1);
	}
}

//..............................................................................

#ifdef _AXL_IO_USB

sl::String
getUsbStringDescriptorText(
	io::UsbDevice* device,
	size_t index
) {
	if (!index)
		return "NONE";

	if (!device->isOpen())
		return "NOT OPENED";

	sl::String text;
	bool result = device->getStringDescriptorAscii(&text, index) != -1;
	return result ?
		text :
		sl::formatString("ERROR (%s)", err::getLastErrorDescription().sz());
}

void
printUsbIfaceDesc(const libusb_interface_descriptor* ifaceDesc) {
	printf("    Interface:   %d\n", ifaceDesc->bInterfaceNumber);
	printf("    Alt setting: %d\n", ifaceDesc->bAlternateSetting);
	printf("    Class:       %s\n", io::getUsbClassString(ifaceDesc->bInterfaceClass));
	printf("    Subclass:    %d\n", ifaceDesc->bInterfaceSubClass);
	printf("    Protocol:    %d\n", ifaceDesc->bInterfaceProtocol);
	printf("    Endpoints:   %d\n", ifaceDesc->bNumEndpoints);

	for (size_t i = 0; i < ifaceDesc->bNumEndpoints; i++) {
		const libusb_endpoint_descriptor* endpointDesc = &ifaceDesc->endpoint[i];

		printf("\n");

		printf("      Endpoint:        0x%02x\n", endpointDesc->bEndpointAddress);
		printf("      Direction:       %s\n", (endpointDesc->bEndpointAddress & LIBUSB_ENDPOINT_IN) ? "In" : "Out");
		printf("      Type:            %s\n", io::getUsbTransferTypeString((libusb_transfer_type) (endpointDesc->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK)));
		printf("      Max packet size: %d\n", endpointDesc->wMaxPacketSize);
	}
}

void
printUsbConfigDesc(const libusb_config_descriptor* configDesc) {
	printf("  Configuration: %d\n", configDesc->bConfigurationValue);
	printf("  Max power:     %d mA\n", configDesc->MaxPower * 2);
	printf("  Interfaces:    %d\n", configDesc->bNumInterfaces);

	for (size_t i = 0; i < configDesc->bNumInterfaces; i++) {
		const libusb_interface* iface = &configDesc->interface[i];

		if (!iface->num_altsetting) {
			printf("\n    Interface #%d is not configured\n", i);
		} else for (size_t j = 0; j < (size_t)iface->num_altsetting; j++) {
			printf("\n");
			printUsbIfaceDesc(&iface->altsetting[j]);
		}
	}
}

void
printUsbDevice(io::UsbDevice* device) {
	bool result;

	libusb_device_descriptor deviceDesc;
	result = device->getDeviceDescriptor(&deviceDesc);
	if (!result) {
		printf("Cannot get device descriptor (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("HWID:           VID_%04x&PID_%04x\n", deviceDesc.idVendor, deviceDesc.idProduct);
	printf("Class:          %s\n", io::getUsbClassString(deviceDesc.bDeviceClass));
	printf("Manufacturer:   %s\n", getUsbStringDescriptorText(device, deviceDesc.iManufacturer).sz());
	printf("Product name:   %s\n", getUsbStringDescriptorText(device, deviceDesc.iProduct).sz());
	printf("Serial number:  %s\n", getUsbStringDescriptorText(device, deviceDesc.iSerialNumber).sz());

	printf("Address:        %d\n", device->getDeviceAddress());
	printf("Bus:            %d\n", device->getBusNumber());
#if (_AXL_IO_USBDEVICE_PORT)
	printf("Port:           %d\n", device->getPortNumber());
#endif
	printf("Speed:          %s\n", io::getUsbSpeedString(device->getDeviceSpeed()));
	printf("Port path:      ");

#if (_AXL_IO_USBDEVICE_PORT)
	uint8_t path[8];
	size_t pathLength = device->getPortPath(path, countof(path));
	if (pathLength == -1) {
		printf("ERROR (%s)\n", err::getLastErrorDescription().sz());
	} else if (pathLength != -1) {
		for (size_t i = 0; i < pathLength; i++)
			printf("-> %d", path [i]);

		printf("\n");
	}
#else
	printf("\n");
#endif

	printf("Configurations: %d\n", deviceDesc.bNumConfigurations);

	for (size_t i = 0; i < deviceDesc.bNumConfigurations; i++) {
		printf("\n");

		io::UsbConfigDescriptor configDesc;
		bool result = device->getConfigDescriptor(&configDesc, i);
		if (!result)
			printf("  Cannot get config descriptor #%d (%s)\n", i, err::getLastErrorDescription().sz());
		else
			printUsbConfigDesc(configDesc);
	}
}

void
testUsbEnum() {
	bool result;

	io::registerUsbErrorProvider();
	io::getUsbDefaultContext()->createDefault();
	// io::getUsbDefaultContext()->setDebugLevel(LIBUSB_LOG_LEVEL_DEBUG);

	io::UsbDeviceList deviceList;
	size_t count = deviceList.enumerateDevices();
	if (count == -1) {
		printf("Cannot enumerate USB devices (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	io::UsbDeviceList dev;

	libusb_device** pp = deviceList;
	for (size_t i = 0; *pp; pp++, i++) {
		printf("----------------------\nDevice #%d\n", i);

		io::UsbDevice device;
		device.setDevice(*pp);

		result = device.open();
		if (!result)
			printf("Cannot open device (%s)\n", err::getLastErrorDescription().sz());

		printUsbDevice(&device); // even if not opened
	}
}

class UsbRead {
protected:
	io::UsbDevice* m_device;
	uint_t m_endpointId;
	libusb_transfer_type m_endpointType;
	size_t m_maxPacketSize;
	uint_t m_timeout;

public:
	UsbRead(
		io::UsbDevice* device,
		uint_t endpointId,
		uint_t timeout = 1000
	) {
		m_device = device;
		m_endpointId = endpointId;
		m_timeout = timeout;

		io::UsbConfigDescriptor configDesc;
		m_device->getActiveConfigDescriptor(&configDesc);
		const libusb_endpoint_descriptor* endpointDesc = io::findUsbEndpointDescriptor(configDesc, m_endpointId);
		ASSERT(endpointDesc);

		m_endpointType = (libusb_transfer_type)(endpointDesc->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK);
		m_maxPacketSize = endpointDesc->wMaxPacketSize;
	}
};

class UsbReadThread:
	public sys::ThreadImpl<UsbReadThread>,
	public UsbRead {
public:
	UsbReadThread(
		io::UsbDevice* device,
		uint_t endpointId,
		uint_t timeout = 1000
	): UsbRead(device, endpointId, timeout) {}

	void
	threadFunc() {
		sl::Array<char> buffer;
		buffer.setCount(m_maxPacketSize);

		size_t totalSize = 0;
		while (totalSize < 1024) {
			size_t size;
			switch (m_endpointType) {
			case LIBUSB_TRANSFER_TYPE_BULK:
				size = m_device->bulkTransfer(m_endpointId, buffer, m_maxPacketSize, m_timeout);
				break;

			case LIBUSB_TRANSFER_TYPE_INTERRUPT:
				size = m_device->interruptTransfer(m_endpointId, buffer, m_maxPacketSize, m_timeout);
				break;

			default:
				ASSERT(false);
			}

			if (size == -1) {
				printf("interrupt transfer error: %s\n", err::getLastErrorDescription().sz());
			} else {
				printf("received %d bytes\n", size);
				totalSize += size;
			}
		}
	}
};

class UsbAsyncTransfer:
	public sys::ThreadImpl<UsbReadThread>,
	public UsbRead {
protected:
	io::UsbTransfer m_transfer;
	sys::NotificationEvent m_completionEvent;
	sl::Array<char> m_buffer;
	size_t m_totalSize;

public:
	UsbAsyncTransfer(
		io::UsbDevice* device,
		uint_t endpointId,
		uint_t timeout = 1000
	): UsbRead(device, endpointId, timeout) {
		m_transfer.create();
		m_buffer.setCount(m_maxPacketSize);
		m_totalSize = 0;
	}

	bool
	next() {
		switch (m_endpointType) {
		case LIBUSB_TRANSFER_TYPE_BULK:
			m_transfer.fillBulkTransfer(
				m_device->getOpenHandle(),
				m_endpointId,
				m_buffer,
				m_maxPacketSize,
				onCompleted,
				this,
				m_timeout
			);

			break;

		case LIBUSB_TRANSFER_TYPE_INTERRUPT:
			m_transfer.fillInterruptTransfer(
				m_device->getOpenHandle(),
				m_endpointId,
				m_buffer,
				m_maxPacketSize,
				onCompleted,
				this,
				m_timeout
			);
			break;

		default:
			ASSERT(false);
		}

		return m_transfer.submit();
	}

	bool
	wait(uint_t timeout = -1) {
		return m_completionEvent.wait(timeout);
	}

protected:
	static
	void
	LIBUSB_CALL
	onCompleted(libusb_transfer* transfer) {
		UsbAsyncTransfer* self = (UsbAsyncTransfer*)transfer->user_data;

		printf(
			"libusb_transfer completed:\n"
			"    status:        %s (%x)\n"
			"    actual length: %d\n",
			io::getUsbTransferStatusString(transfer->status),
			transfer->status,
			transfer->actual_length
		);

		if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
			self->m_totalSize += transfer->actual_length;
			if (self->m_totalSize > 1024) {
				self->m_completionEvent.signal();
				return;
			}
		}

		self->next();
	}
};

void
testUsbMouse() {
	enum {
		// Logitech Gaming Mouse
		VendorId  = 0x046d,
		ProductId = 0xc246,

		InterfaceId = 0,
		EndpointId  = 0x81,
	};

	bool result;

	io::registerUsbErrorProvider();
	io::getUsbDefaultContext()->createDefault();
	io::getUsbDefaultContextEventThread()->start();

	printf("Opening device...\n");

	io::UsbDevice device;
	result = device.open(VendorId, ProductId);
	if (!result) {
		printf("Error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("Reading device properties...\n");
	printUsbDevice(&device);

	if (io::hasUsbCapability(LIBUSB_CAP_SUPPORTS_DETACH_KERNEL_DRIVER)) {
		printf("Setting auto-detach for kernel driver...\n");
		result = device.setAutoDetachKernelDriver(true);
		if (!result) {
			printf("Error: %s\n", err::getLastErrorDescription().sz());
			return;
		}
	}

	printf("Claiming interface #%d...\n", InterfaceId);
	result = device.claimInterface(InterfaceId);
	if (!result) {
		printf("Error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	io::UsbConfigDescriptor configDesc;
	device.getActiveConfigDescriptor(&configDesc);
	const libusb_endpoint_descriptor* endpointDesc = io::findUsbEndpointDescriptor(configDesc, EndpointId);
	ASSERT(endpointDesc);

	printf("Sync read...\n");

	sl::Array<char> buffer;
	buffer.setCount(endpointDesc->wMaxPacketSize);
	size_t size = device.interruptTransfer(EndpointId, buffer, endpointDesc->wMaxPacketSize);

	if (size == -1) {
		printf("Error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("%d bytes read...\n", size);

/*	UsbReadThread readThread(&device, EndpointId);
	printf("Starting read thread...\n");
	readThread.start();
	readThread.waitAndClose(); */

	UsbAsyncTransfer asyncTransfer(&device, EndpointId);
	printf("Starting async transfer...\n");
	asyncTransfer.next();
	asyncTransfer.wait();

	device.releaseInterface(InterfaceId);
}

#endif

//..............................................................................

#if (_AXL_OS_POSIX)

class MyThread: public sys::ThreadImpl<MyThread> {
public:
	volatile bool m_terminateFlag;

	MyThread() {
		m_terminateFlag = false;
	}

	void
	threadFunc() {
		uint64_t tid = getThreadId();

		for (;;) {
			sys::sleep(1000);
			printf("MyThread -- TID: %lld\n", tid);

			if (m_terminateFlag)
				break;
		}

		printf("MyThread -- TID: %lld -- TERMINATE\n", tid);
	}
};

#if 0
static void signal_segv(int signum, siginfo_t* info, void*ptr) {
	static const char *si_codes[3] = {"", "SEGV_MAPERR", "SEGV_ACCERR"};

	int i, f = 0;
	ucontext_t *ucontext = (ucontext_t*)ptr;
	Dl_info dlinfo;
	void **bp = 0;
	void *ip = 0;

	sigsegv_outp("Segmentation Fault!");
	sigsegv_outp("info.si_signo = %d", signum);
	sigsegv_outp("info.si_errno = %d", info->si_errno);
	sigsegv_outp("info.si_code  = %d (%s)", info->si_code, si_codes[info->si_code]);
	sigsegv_outp("info.si_addr  = %p", info->si_addr);
	for(i = 0; i < NGREG; i++)
		sigsegv_outp("reg[%02d]       = 0x" REGFORMAT, i, ucontext->uc_mcontext.gregs[i]);

#ifndef SIGSEGV_NOSTACK
#if defined(SIGSEGV_STACK_IA64) || defined(SIGSEGV_STACK_X86)
#if defined(SIGSEGV_STACK_IA64)
	ip = (void*)ucontext->uc_mcontext.gregs[REG_RIP];
	bp = (void**)ucontext->uc_mcontext.gregs[REG_RBP];
#elif defined(SIGSEGV_STACK_X86)
	ip = (void*)ucontext->uc_mcontext.gregs[REG_EIP];
	bp = (void**)ucontext->uc_mcontext.gregs[REG_EBP];
#endif

	sigsegv_outp("Stack trace:");
	while(bp && ip) {
		if(!dladdr(ip, &dlinfo))
			break;

		const char *symname = dlinfo.dli_sname;

#ifndef NO_CPP_DEMANGLE
		int status;
		char * tmp = __cxa_demangle(symname, NULL, 0, &status);

		if (status == 0 && tmp)
			symname = tmp;
#endif

		sigsegv_outp("% 2d: %p <%s+%lu> (%s)",
				 ++f,
				 ip,
				 symname,
				 (unsigned long)ip - (unsigned long)dlinfo.dli_saddr,
				 dlinfo.dli_fname);

#ifndef NO_CPP_DEMANGLE
		if (tmp)
			free(tmp);
#endif

		if(dlinfo.dli_sname && !strcmp(dlinfo.dli_sname, "main"))
			break;

		ip = bp[1];
		bp = (void**)bp[0];
	}
#else
	sigsegv_outp("Stack trace (non-dedicated):");
	sz = backtrace(bt, 20);
	strings = backtrace_symbols(bt, sz);
	for(i = 0; i < sz; ++i)
		sigsegv_outp("%s", strings[i]);
#endif
	sigsegv_outp("End of stack trace.");
#else
	sigsegv_outp("Not printing stack strace.");
#endif
	_exit(-1);
}

#endif

void* suspendThread(pthread_t thread) {
	return NULL;
}

bool resumeThread(pthread_t thread) {
	return false;
}

void testSuspendThread() {
	MyThread thread1;
	MyThread thread2;

	thread1.start();
	sys::sleep(500);
	thread2.start();

	sys::sleep(3000);

	suspendThread(thread2.m_thread);
	printf("thread2 is suspended...\n");

	sys::sleep(3000);

	printf("thread2 is resumed...\n");
	resumeThread(thread2.m_thread);

	sys::sleep(3000);

	printf("terminating threads...\n");

	thread1.m_terminateFlag = true;
	thread2.m_terminateFlag = true;

	thread1.waitAndClose();
	thread2.waitAndClose();

/*
	sigaction action = { 0 };
	action.sa_sigaction = signal_segv;
	action.sa_flags = SA_SIGINFO;

	sigaction(SIGSEGV, &action, NULL);
		perror("sigaction");


	threadArray[i].suspend();
*/

}

#endif

//..............................................................................

class Gc;

Gc* g_gc = NULL;

#if (_AXL_OS_POSIX)
class Gc {
protected:
	enum HandshakeKind {
		HandshakeKind_None,
		HandshakeKind_StopTheWorld,
		HandshakeKind_ResumeTheWorld,
	};

protected:
	io::psx::Mapping m_guardPage;

	sl::Array<uint64_t> m_threadArray;
	sl::HashTable<uint64_t, bool, sl::HashId<uint64_t> > m_threadMap;

	volatile HandshakeKind m_handshakeKind;
	volatile int32_t m_handshakeCounter;

#if (_AXL_OS_DARWIN)
	sys::drw::Semaphore m_handshakeSem;
#else
	sys::psx::Sem m_handshakeSem;
#endif

	sigset_t m_signalWaitMask;

public:
	Gc() {
		m_guardPage.map(
			NULL,
			4 * 1024, // typical page size -- OS will not give us less than that, anyway
			PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON,
			-1,
			0
		);

		m_handshakeKind = HandshakeKind_None;
		installSignalHandlers();
	}

	void
	gcSafePoint() {
		*(volatile int*) m_guardPage.p() = 0;
	}

	void
	stopTheWorld() {
		m_handshakeKind = HandshakeKind_StopTheWorld;
		m_handshakeCounter = m_threadMap.getCount();
		m_guardPage.protect(PROT_NONE);
		m_handshakeSem.wait();
		m_handshakeKind = HandshakeKind_None;
	}

	void
	resumeTheWorld() {
		m_guardPage.protect(PROT_READ | PROT_WRITE);

		m_handshakeKind = HandshakeKind_ResumeTheWorld;
		m_handshakeCounter = m_threadArray.getCount();
		for (size_t i = 0; i < m_handshakeCounter; i++)
			pthread_kill((pthread_t)m_threadArray[i], SIGUSR1); // resume

		m_handshakeSem.wait();
		m_handshakeKind = HandshakeKind_None;
	}

	bool
	registerThread(uint64_t threadId) {
		sl::HashTableIterator<uint64_t, bool> it = m_threadMap.visit(threadId);
		if (it->m_value)
			return false;

		m_threadArray.append(threadId);
		it->m_value = true;
		return true;
	}

protected:
	bool
	installSignalHandlers() {
		sigemptyset(&m_signalWaitMask); // don't block any signals when servicing SIGSEGV

		struct sigaction sigAction = { 0 };
		sigAction.sa_flags = SA_SIGINFO;
		sigAction.sa_sigaction = signalHandler_SIGSEGV;
		sigAction.sa_mask = m_signalWaitMask;

		struct sigaction prevSigAction;
		int result = sigaction(SIGSEGV, &sigAction, &prevSigAction);
		ASSERT(result == 0);

		sigAction.sa_flags = 0;
		sigAction.sa_handler = signalHandler_SIGUSR1;
		result = sigaction(SIGUSR1, &sigAction, &prevSigAction);
		ASSERT(result == 0);

		return true;
	}

	static
	void
	signalHandler_SIGSEGV(
		int signal,
		siginfo_t* signalInfo,
		void* context
	) {
		if (signal != SIGSEGV ||
			signalInfo->si_addr != g_gc->m_guardPage ||
			g_gc->m_handshakeKind != HandshakeKind_StopTheWorld)
			return; // ignore

		int32_t count = sys::atomicDec(&g_gc->m_handshakeCounter);
		if (!count)
			g_gc->m_handshakeSem.signal();

		do {
			sigsuspend(&g_gc->m_signalWaitMask);
		} while (g_gc->m_handshakeKind != HandshakeKind_ResumeTheWorld);

		count = sys::atomicDec(&g_gc->m_handshakeCounter);
		if (!count)
			g_gc->m_handshakeSem.signal();
	}

	static
	void
	signalHandler_SIGUSR1(int signal) {
		// do nothing (we handshake manually). but we still need a handler
	}
};

#define GC_BEGIN()
#define GC_END()

#elif (_AXL_OS_WIN)

class Gc {
protected:
	enum HandshakeKind {
		HandshakeKind_None,
		HandshakeKind_StopTheWorld,
		HandshakeKind_ResumeTheWorld,
	};

protected:
	sys::win::VirtualMemory m_guardPage;

	sl::HashTable<uint64_t, bool, sl::HashId<uint64_t> > m_threadMap;

	volatile HandshakeKind m_handshakeKind;
	volatile int32_t m_handshakeCounter;
	sys::Event m_handshakeEvent;
	sys::NotificationEvent m_resumeEvent;

public:
	Gc() {
		m_guardPage.alloc(4 * 1024);
		m_handshakeKind = HandshakeKind_None;
	}

	~Gc() {}

	void
	gcSafePoint() {
		sys::atomicXchg((volatile int*) m_guardPage.p(), 0);
	}

	void
	stopTheWorld() {
		m_handshakeKind = HandshakeKind_StopTheWorld;
		m_handshakeCounter = m_threadMap.getCount();
		m_resumeEvent.reset();
		m_guardPage.protect(PAGE_NOACCESS);
		m_handshakeEvent.wait();
		m_handshakeKind = HandshakeKind_None;
	}

	void
	resumeTheWorld() {
		m_guardPage.protect(PAGE_READWRITE);

		m_handshakeKind = HandshakeKind_ResumeTheWorld;
		m_handshakeCounter = m_threadMap.getCount();
		m_resumeEvent.signal();
		m_handshakeEvent.wait();
		m_handshakeKind = HandshakeKind_None;
		m_resumeEvent.reset();
	}

	bool
	registerThread(uint64_t threadId) {
		sl::HashTableIterator<uint64_t, bool> it = m_threadMap.visit(threadId);
		if (it->m_value)
			return false;

		it->m_value = true;
		return true;
	}

	int
	handleException(
		uint_t code,
		EXCEPTION_POINTERS* exceptionPointers
	) {
		if (code != EXCEPTION_ACCESS_VIOLATION ||
		   exceptionPointers->ExceptionRecord->ExceptionInformation[1] != (uintptr_t)m_guardPage.p() ||
		   m_handshakeKind != HandshakeKind_StopTheWorld)
		  return EXCEPTION_CONTINUE_SEARCH;

		int32_t count = sys::atomicDec(&g_gc->m_handshakeCounter);
		if (!count)
			g_gc->m_handshakeEvent.signal();

		do {
			g_gc->m_resumeEvent.wait();
		} while (m_handshakeKind != HandshakeKind_ResumeTheWorld);

		count = sys::atomicDec(&g_gc->m_handshakeCounter);
		if (!count)
			g_gc->m_handshakeEvent.signal();

		return EXCEPTION_CONTINUE_EXECUTION;
	}
};

#define GC_BEGIN() __try {
#define GC_END() } __except(g_gc->handleException(GetExceptionCode(), GetExceptionInformation())) { }

#endif

class MutatorThread: public sys::ThreadImpl<MutatorThread> {
protected:
	volatile bool m_terminateFlag;

public:
	MutatorThread() {
		m_terminateFlag = false;
	}

	void
	stop() {
		m_terminateFlag = true;
		waitAndClose();
		m_terminateFlag = false;
	}

	void
	threadFunc() {
		uint64_t threadId = sys::getCurrentThreadId();

		GC_BEGIN()

		for (;;) {
			if (m_terminateFlag)
				break;

			printf("  mutator thread TID:%lld is working...\n", threadId);
			sys::sleep(200);
			g_gc->gcSafePoint();
		}

		printf("  mutator thread TID:%lld is finished.\n", threadId);

		GC_END()
	}
};

void
testGcSafePoints() {
	Gc gc;

	g_gc = &gc;

	printf("starting mutator threads...\n");

	MutatorThread thread1;
	MutatorThread thread2;

	thread1.start();
	thread2.start();

	gc.registerThread(thread1.getThreadId());
	gc.registerThread(thread2.getThreadId());

	sys::sleep(1000);

	printf("stopping the world...\n");
	gc.stopTheWorld();
	printf("the world is stopped.\n");

	sys::sleep(2000);

	printf("resuming the world...\n");
	gc.resumeTheWorld();

	sys::sleep(1000);

	printf("stopping mutator threads...\n");

	thread1.stop();
	thread2.stop();

	printf("done.\n");

	g_gc = NULL;
}

//..............................................................................

struct IfaceHdr {
	const char* m_p1;
	const char* m_p2;
	const char* m_p3;
};

class Foo: public IfaceHdr {
public:
	Foo() {
		printf("m_p1 = %s; m_p2 = %s; m_p3 = %s\n", m_p1, m_p2, m_p3);
	}
};

void
testInheritance() {
	char buffer[sizeof(Foo)];
	IfaceHdr* iface = (IfaceHdr*)buffer;
	iface->m_p1 = "hui";
	iface->m_p2 = "govno";
	iface->m_p3 = "muravei";

	new(buffer)Foo();
}

//..............................................................................

void
testTimestamps() {
	uint64_t ts1 = sys::getTimestamp();
	uint64_t ts2 = sys::getPreciseTimestamp();

	printf("time (0) = %s\n", sys::Time(0, 0).format("%h:%m:%s.%l.%c").sz());
	printf("ts1 = %s\n", sys::Time(ts1).format("%h:%m:%s.%l.%c").sz());
	printf("ts2 = %s\n", sys::Time(ts2).format("%h:%m:%s.%l.%c").sz());

	uint64_t t0 = sys::getPreciseTimestamp();

	size_t n = 10000000;

	for (size_t i = 0; i < n; i++) {
		sys::getTimestamp();
	}

	uint64_t t2 = sys::getPreciseTimestamp();
	uint64_t d = t2 - t0;
	printf("time (ts1) = %s\n", sys::Time(d, 0).format("%h:%m:%s.%l.%c").sz());

	t0 = sys::getPreciseTimestamp();

	for (size_t i = 0; i < n; i++) {
		sys::getPreciseTimestamp();
	}

	t2 = sys::getPreciseTimestamp();
	d = t2 - t0;
	printf("time (ts2) = %s\n", sys::Time(d, 0).format("%h:%m:%s.%l.%c").sz());

	ts1 = sys::getTimestamp();
	ts2 = sys::getPreciseTimestamp();

	printf("ts1 = %s\n", sys::Time(ts1).format("%h:%m:%s.%l.%c").sz());
	printf("ts2 = %s\n", sys::Time(ts2).format("%h:%m:%s.%l.%c").sz());

}

//..............................................................................

#if (_AXL_OS_WIN)
void
testProcess() {
	sl::Array<char> output;
	dword_t exitCode;

	const wchar_t* cmdLine = L"C:\\Projects\\ioninja\\devmon\\build\\msvc10\\bin\\Debug\\tdevmon.exe --start-core-service";

	bool result = sys::win::syncExec(cmdLine, &output, &exitCode);
	if (!result) {
		printf("sys::win::syncExec failed: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	output.append(0);
	printf("process returned %d:\n%s\n", exitCode, output.cp());
}
#endif

#if (_AXL_IO_SHMT)

//..............................................................................

namespace shm_test {

const size_t TotalSize = 64 * 1024 * 1024;
const size_t MaxBlockSize = 64;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ReqCode {
	ReqCode_Command,
	ReqCode_Reply
};

struct ReqHdr {
	uint32_t m_code;
	uint32_t m_size;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Transport {
public:
	virtual
	size_t
	readMessage(sl::Array<char>* buffer) = 0;

	virtual
	bool
	writeMessage(
		const ReqHdr* hdr,
		const void* p,
		size_t size
	) = 0;
};

//..............................................................................

class ServerThread: public sys::ThreadImpl<ServerThread> {
public:
	Transport* m_transport;

public:
	ServerThread(Transport* transport) {
		m_transport = transport;
	}

	uint_t
	threadFunc() {
		ASSERT(m_transport);

		sl::Array<char> buffer;

		static char data[MaxBlockSize] = { 0 };

		for (;;) {
			size_t size = m_transport->readMessage(&buffer);
			if (size == -1) {
				printf("server: read error: %s\n", err::getLastErrorDescription().sz());
				return -1;
			}

			if (size < sizeof(ReqHdr)) {
				printf("server: buffer too small\n");
				return -1;
			}

			ReqHdr* command = (ReqHdr*)buffer.cp();
			if (command->m_code != ReqCode_Command || sizeof(ReqHdr) + command->m_size != size) {
				printf("server: invalid command: %d (%d)\n", command->m_code, command->m_size);
				return -1;
			}

			ReqHdr reply;
			reply.m_code = ReqCode_Reply;
			reply.m_size = command->m_size;

			m_transport->writeMessage(&reply, data, reply.m_size);
		}

		return 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ClientWriterThread: public sys::ThreadImpl<ClientWriterThread> {
protected:
	Transport* m_transport;

public:
	ClientWriterThread(Transport* transport) {
		m_transport = transport;
	}

	uint_t
	threadFunc() {
		ASSERT(m_transport);

		size_t totalSize = 0;

		sl::Array<char> buffer;
		static char data[MaxBlockSize] = { 0 };

		while (totalSize < TotalSize) {
			ReqHdr command;
			command.m_code = ReqCode_Command;
			command.m_size = ::rand() % MaxBlockSize;;

			bool result = m_transport->writeMessage(&command, data, command.m_size);
			if (!result) {
				printf("client: write error: %s\n", err::getLastErrorDescription().sz());
				return -1;
			}

			totalSize += command.m_size;
		}

		printf("\nWriting 100%% (done), %zd bytes\n", totalSize);
		return 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ClientReaderThread: public sys::ThreadImpl<ClientReaderThread> {
protected:
	Transport* m_transport;

public:
	ClientReaderThread(Transport* transport) {
		m_transport = transport;
	}

	uint_t
	threadFunc() {
		ASSERT(m_transport);

		int percentage = 0;
		size_t totalSize = 0;

		sl::Array<char> buffer;

		while (totalSize < TotalSize) {
			size_t receivedSize = m_transport->readMessage(&buffer);
			if (receivedSize == -1) {
				printf("client: read error: %s\n", err::getLastErrorDescription().sz());
				return -1;
			}

			if (receivedSize < sizeof(ReqHdr)) {
				printf("client: buffer too small\n");
				return -1;
			}

			ReqHdr* reply = (ReqHdr*)buffer.cp();
			if (reply->m_code != ReqCode_Reply || sizeof(ReqHdr) + reply->m_size != receivedSize) {
				printf("client: invalid reply: %d (%d)\n", reply->m_code, reply->m_size);
				return -1;
			}

			totalSize += reply->m_size;

			int newPercentage = (uint64_t)totalSize * 100 / TotalSize;
			if (newPercentage != percentage) {
				percentage = newPercentage;
				printf("\b\b\b\b%d%%", percentage);
			}
		}

		printf("\nReading 100%% (done), %zd bytes\n", totalSize);
		return 0;
	}
};

//..............................................................................

class ShmTransport: public Transport {
public:
	io::ShmtReader m_reader;
	io::ShmtWriter m_writer;

public:
	virtual
	size_t
	readMessage(sl::Array<char>* buffer) {
		return m_reader.read(buffer);
	}

	virtual
	bool
	writeMessage(
		const ReqHdr* hdr,
		const void* p,
		size_t size
	) {
		if (!size)
			return m_writer.write(hdr, sizeof(ReqHdr)) != -1;

		const void* blockArray[] = { hdr, p };
		size_t sizeArray[] = { sizeof(ReqHdr), size };

		return m_writer.write(blockArray, sizeArray, 2) != -1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class PipeTransport: public Transport {
public:
#if (_AXL_OS_WIN)
	HANDLE m_readPipe;
	HANDLE m_writePipe;
#elif (_AXL_OS_POSIX)
	int m_readPipe;
	int m_writePipe;
#endif

	sl::Array<char> m_leftover;

public:
	virtual
	size_t
	readMessage(sl::Array<char>* buffer) {
		size_t size = 0;

		size_t bufferSize = buffer->getCount();

		if (bufferSize < sizeof(ReqHdr))
			buffer->setCount(sizeof(ReqHdr));

		bufferSize = sizeof(ReqHdr);

		while (size < bufferSize) {
#if (_AXL_OS_WIN)
			dword_t actualSize;
			bool_t result = ::ReadFile(m_readPipe, buffer->p() + size, bufferSize - size, &actualSize, NULL);
#elif (_AXL_OS_POSIX)
			int actualSize = ::read(m_readPipe, buffer->p() + size, bufferSize - size);
			bool result = actualSize != -1;
#endif

			if (!result) {
				err::setLastSystemError();
				return -1;
			}

			size += actualSize;
		}

		ASSERT(size == bufferSize);

		ReqHdr* hdr = (ReqHdr*)buffer->p();
		if (!hdr->m_size) {
			ASSERT(size == sizeof(ReqHdr));
			buffer->setCount(sizeof(ReqHdr));
			return sizeof(ReqHdr);
		}

		bufferSize = sizeof(ReqHdr) + hdr->m_size;
		buffer->setCount(bufferSize);

		while (size < bufferSize) {
#if (_AXL_OS_WIN)
			dword_t actualSize;
			bool_t result = ::ReadFile(m_readPipe, buffer->p() + size, bufferSize - size, &actualSize, NULL);
#elif (_AXL_OS_POSIX)
			int actualSize = ::read(m_readPipe, buffer->p() + size, bufferSize - size);
			bool result = actualSize != -1;
#endif

			if (!result) {
				err::setLastSystemError();
				return -1;
			}

			size += actualSize;
		}

		ASSERT(size == bufferSize);
		return size;
	}

	virtual
	bool
	writeMessage(
		const ReqHdr* hdr,
		const void* p,
		size_t size
	) {
#if (_AXL_OS_WIN)
		dword_t actualSize;

		bool result = ::WriteFile(m_writePipe, hdr, sizeof(ReqHdr), &actualSize, NULL) != 0;

		if (result && size)
			result = ::WriteFile(m_writePipe, p, size, &actualSize, NULL) != 0;
#elif (_AXL_OS_POSIX)
		bool result = ::write(m_writePipe, hdr, sizeof(ReqHdr)) != -1;

		if (result && size)
			result = ::write(m_writePipe, p, size) != -1;
#endif

		return result;
	}
};

} // namespace shm_test

//..............................................................................

void
testSharedMemoryTransport() {
	using namespace shm_test;

	bool result;

	ShmTransport serverTransport;
	ShmTransport clientTransport;

#if (_AXL_OS_WIN)
#	define HOME_DIR "C:/Users/Vladimir"
#elif (_AXL_OS_LINUX)
#	define HOME_DIR "/home/vladimir"
#elif (_AXL_OS_DARWIN)
#	define HOME_DIR "/Users/vladimir"
#endif

#if (_AXL_OS_POSIX)
	setvbuf(stdout, NULL, _IONBF, 0);
	::sem_unlink("shmt-test-cli-srv-r");
	::sem_unlink("shmt-test-cli-srv-w");
	::sem_unlink("shmt-test-srv-cli-r");
	::sem_unlink("shmt-test-srv-cli-w");
#endif

	result =
		serverTransport.m_reader.open(
			HOME_DIR "/shmt-test-cli-srv",
			"shmt-test-cli-srv-r",
			"shmt-test-cli-srv-w",
			io::ShmtFlag_Message |
			io::FileFlag_DeleteOnClose
		) &&
		serverTransport.m_writer.open(
			HOME_DIR "/shmt-test-srv-cli",
			"shmt-test-srv-cli-r",
			"shmt-test-srv-cli-w",
			io::ShmtFlag_Message |
			io::FileFlag_DeleteOnClose
		) &&
		clientTransport.m_reader.open(
			HOME_DIR "/shmt-test-srv-cli",
			"shmt-test-srv-cli-r",
			"shmt-test-srv-cli-w",
			io::ShmtFlag_Message |
			io::FileFlag_OpenExisting |
			io::FileFlag_Unlink
		) &&
		clientTransport.m_writer.open(
			HOME_DIR "/shmt-test-cli-srv",
			"shmt-test-cli-srv-r",
			"shmt-test-cli-srv-w",
			io::ShmtFlag_Message |
			io::FileFlag_OpenExisting |
			io::FileFlag_Unlink
		);

	if (!result) {
		printf("can't initialize: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	ServerThread serverThread(&serverTransport);
	ClientWriterThread clientWriterThread(&clientTransport);
	ClientReaderThread clientReaderThread(&clientTransport);

	uint64_t time0 = sys::getTimestamp();

	serverThread.start();
	clientReaderThread.start();
	clientWriterThread.start();

	clientReaderThread.waitAndClose();

	clientTransport.m_reader.disconnect();
	clientTransport.m_writer.disconnect();

	clientWriterThread.waitAndClose();
	serverThread.waitAndClose();

	uint64_t time2 = sys::getTimestamp();

	printf("shm test completed: %s\n", sys::Time(time2 - time0, 0).format("%m:%s.%l").sz());
}

//..............................................................................

void
testPipeTransport() {
	using namespace shm_test;

	bool result;

	PipeTransport serverTransport;
	PipeTransport clientTransport;

#if (_AXL_OS_WIN)
	HANDLE pipeA[2] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };
	HANDLE pipeB[2] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };

	result =
		::CreatePipe(&pipeA[0], &pipeA[1], NULL, 0) &&
		::CreatePipe(&pipeB[0], &pipeB[1], NULL, 0);

#elif (_AXL_OS_POSIX)
	setvbuf(stdout, NULL, _IONBF, 0);

	int pipeA[2] = { 0 };
	int pipeB[2] = { 0 };

	result =
		::pipe(pipeA) == 0 &&
		::pipe(pipeB) == 0;
#endif

	if (!result) {
		err::setLastSystemError();
		printf("can't initialize: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	serverTransport.m_readPipe = pipeA[0];
	serverTransport.m_writePipe = pipeB[1];
	clientTransport.m_readPipe = pipeB[0];
	clientTransport.m_writePipe = pipeA[1];

	ServerThread serverThread(&serverTransport);
	ClientReaderThread clientReaderThread(&clientTransport);
	ClientWriterThread clientWriterThread(&clientTransport);

	uint64_t time0 = sys::getTimestamp();

	serverThread.start();
	clientReaderThread.start();
	clientWriterThread.start();

	clientReaderThread.waitAndClose();

	// first close the writer side

#if (_AXL_OS_WIN)
	::CloseHandle(pipeA[1]);
	::CloseHandle(pipeB[1]);
	::CloseHandle(pipeA[0]);
	::CloseHandle(pipeB[0]);
#elif (_AXL_OS_POSIX)
	::close(pipeA[1]);
	::close(pipeB[1]);
	::close(pipeA[0]);
	::close(pipeB[0]);
#endif

	clientWriterThread.waitAndClose();
	serverThread.waitAndClose();

	uint64_t time2 = sys::getTimestamp();

	printf("pipe test completed: %s\n", sys::Time(time2 - time0, 0).format("%m:%s.%l").sz());
}

//..............................................................................

#endif // _AXL_IO_SHMT

#if (_AXL_ZIP)

void
testZip() {
	bool result;

	zip::ZipReader reader;

	result = reader.openFile("C:/Program Files/Tibbo/Ninja 3/bin/io_base.jncx");
	if (!result) {
		printf("can't open file: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	sl::Array<char> buffer;

	sl::String dir = "C:/\xd0\xa1\xd1\x83\xd0\xba\xd0\xb0/";

	size_t count = reader.getFileCount();
	for (size_t i = 0; i < count; i++) {
		sl::String fileName = reader.getFileName(i);

		zip::ZipFileInfo fileInfo;
		reader.getFileInfo(i, &fileInfo);

		bool isDir = reader.isDirectoryFile(i);

		printf(
			"%-10s         %s\n"
			"compressed size:   %d\n"
			"uncompressed size: %d\n"
			"timestamp:         %s\n"
			"method:            %d\n",
			isDir ? "directory:" : "file:",
			fileName.sz(),
			(size_t)fileInfo.m_compressedSize,
			(size_t)fileInfo.m_uncompressedSize,
			sys::Time(fileInfo.m_timestamp).format().sz(),
			fileInfo.m_method
		);

		if (!isDir) {
			reader.extractFileToMem(i, &buffer);
			printf("<<<\n%s\n>>>\n", buffer.cp());

			sl::String dstFileName = dir + fileName;
			result = reader.extractFileToFile(i, dstFileName);
			if (!result) {
				printf("can't extract file: %s\n", err::getLastErrorDescription().sz());
				return;
			}
		}

		printf("\n");
	}
}

#endif

//..............................................................................

void
testEnumSerial() {
	sl::List<io::SerialPortDesc> portList;
	io::enumerateSerialPorts(&portList, io::SerialPortDescFlag_All);

	sl::Iterator<io::SerialPortDesc> it = portList.getHead();
	for (; it; it++)
		printf(
			"device name:     %s\n"
			"description:     %s\n"
			"hardwared ID(s): %s\n"
			"manufacturer:    %s\n"
			"driver:          %s\n"
			"location:        %s\n"
			"\n",
			it->m_deviceName.sz(),
			it->m_description.sz(),
			it->m_hardwareIds.sz(),
			it->m_manufacturer.sz(),
			it->m_driver.sz(),
			it->m_location.sz()
		);

	printf("%d ports total\n", portList.getCount());
}

//..............................................................................

void
testEncoding() {
	const char source[] = "abc\x80 def";
	const wchar_t source_w[] = L"abc\x80 def";

	sl::String s, s2;
	sl::String_w sw, sw2;
	sl::Array<char> b, b2;

	enc::EscapeEncoding::encode(&s, source);
	printf("ESC (char): %s\n", s.sz());

	enc::EscapeEncoding_w::encode(&sw, source_w);
	printf("ESC (wchar_t): %S\n", sw.sz());

	enc::EscapeEncoding::decode(&s, source);
	enc::HexEncoding::encode(&s2, s.cp(), s.getLength());
	printf("HEX (char): %s\n", s2.sz());

	enc::EscapeEncoding_w::decode(&sw, source_w);
	enc::HexEncoding::encode(&s2, sw.cp(), sw.getLength() * sizeof(wchar_t));
	printf("HEX (wchar_t): %s\n", s2.sz());

	enc::EscapeEncodingDynamic::decode(enc::CharCodecKind_Utf8, &b, source, lengthof(source));
	enc::HexEncoding::encode(&s, b.cp(), b.getCount());
	printf("HEX (char, dynamic): %s\n", s.sz());

	enc::EscapeEncodingDynamic::encode(enc::CharCodecKind_Utf8, &b2, b.cp(), b.getCount());
	b2.append(0);
	printf("ESC (char, dynamic): %s\n", b2.cp());

	enc::EscapeEncodingDynamic::decode(enc::CharCodecKind_Utf16, &b, source_w, lengthof(source_w) * sizeof(wchar_t));
	enc::HexEncoding::encode(&s, b.cp(), b.getCount());
	printf("HEX (wchar_t, dynamic): %s\n", s.sz());

	enc::EscapeEncodingDynamic::encode(enc::CharCodecKind_Utf16, &b2, b.cp(), b.getCount());
	b2.append(0);
	b2.append(0);
	printf("ESC (wchar_t, dynamic): %S\n", b2.cp());
}

//..............................................................................

// AXL_NO_ASAN
void
testAddressSanitizer() {
	char c = 10;
	char* p = &c;
	p++;
	*p = 0;
}

int foo(jmp_buf buf) {
	return 0;
}

//..............................................................................

#pragma pack()

struct F0 {
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack(1)

struct F1 {
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack(2)

struct F2 {
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack(4)

struct F4 {
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack(8)

struct F8 {
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

void
testPacking() {
	size_t s0 = sizeof(F0);
	size_t s1 = sizeof(F1);
	size_t s2 = sizeof(F2);
	size_t s4 = sizeof(F4);
	size_t s8 = sizeof(F8);

	printf("s0 = %d; s1 = %d; s2 = %d; s4 = %d; s8 = %d\n", s0, s1, s2, s4, s8);
}

//..............................................................................

uint16_t crc16_ansi(
	void const* p,
	size_t size,
	uint16_t seed = 0
) {
	static uint16_t const crcTable[256] = {
		0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
		0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
		0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
		0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
		0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
		0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
		0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
		0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
		0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
		0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
		0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
		0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
		0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
		0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
		0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
		0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
		0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
		0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
		0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
		0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
		0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
		0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
		0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
		0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
		0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
		0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
		0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
		0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
		0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
		0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
		0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
		0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040,
	};

	uint8_t const* b = (uint8_t const*) p;
	uint16_t crc = seed;

	for (; size; size--) {
		uint8_t j = *b++ ^ crc;
		crc = (crc >> 8) ^ crcTable[j];
	}

	return crc;
}

uint16_t crc16_ccit(
	uint8_t const* p,
	size_t size,
	uint16_t seed = 0
) {
	static uint16_t const crcTable[256] = {
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
		0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
		0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
		0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
		0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
		0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
		0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
		0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
		0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
		0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
		0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
		0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
		0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
		0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
		0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
		0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
		0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
		0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
		0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
		0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
		0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
		0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
		0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
		0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
		0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
		0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
		0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
		0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
		0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
		0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
		0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
		0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
	};

	uint8_t const* b = (uint8_t const*) p;
	uint16_t crc = seed;

	for (; size; size--) {
		uint8_t j = *b++ ^ (crc >> 8);
		crc = (crc << 8) ^ crcTable[j];
	}

	return (uint16_t)crc;
}

void
testModBus() {
	uint8_t packet[] = {
		0x01, 0x03, 0x00, 0x30, 0x00, 0x01, // 0x00, 0x00,
	};

	printf(
		"crc16        = %04x\n"
		"crc16 modbus = %04x\n"
		"crc16 xmodem = %04x\n"
		"crc16 ffff   = %04x\n",
		crc16_ansi(packet, sizeof(packet), 0),
		crc16_ansi(packet, sizeof(packet), 0xffff),
		crc16_ccit(packet, sizeof(packet), 0),
		crc16_ccit(packet, sizeof(packet), 0xffff)
	);

	uint_t baud = 19200;

	uint64_t timeout_1_5;
	uint64_t timeout_3_5;

	if (baud > 19200) {
		timeout_1_5 = 7500;
		timeout_3_5 = 17500;
	} else {
		timeout_1_5 = 150000000 / baud;
		timeout_3_5 = 350000000 / baud;
	}

	sl::String s_1_5 = sys::Time(timeout_1_5, false).format("%s.%l.%c");
	sl::String s_3_5 = sys::Time(timeout_3_5, false).format("%s.%l.%c");

	printf("t_1_5 = %s\nt_3_5 = %s\n", s_1_5.sz(), s_3_5.sz());
}

//..............................................................................

uint_t calcChecksum16(
	const uint8_t* p0,
	size_t size
) {
	uint16_t const* p = (uint16_t const*) p0;
	void const* end = p0 + (size & ~1);

	uint_t checksum = 0;

	for (; p < end; p++)
		checksum += htons(*p);

	if (size & 1)
		checksum += *(uint8_t const*) p << 8;

	return checksum;
}

uint16_t adjustIpCheckSum(uint_t checksum) {
	checksum = (checksum >> 16) + (checksum & 0xffff);
	checksum += checksum >> 16;
	return ~checksum;
}

uint16_t calcIpHdrChecksum(const uint8_t* ipHdr, size_t size) {
	uint_t checksum = calcChecksum16(ipHdr, size);
	return adjustIpCheckSum(checksum);
}

void
testIpChecksum() {
	uint8_t data1_0[] = {
		0x45, 0x00, 0x00, 0x1e, 0x7e, 0x8b, 0x00, 0x00, 0x80, 0x11, 0x37, 0x7d, 0xc0, 0xa8, 0x01, 0x77,
		0xc0, 0xa8, 0x01, 0xff
	};

	uint8_t data1[] = {
		0x45, 0x00, 0x00, 0x1e, 0x7e, 0x8b, 0x00, 0x00, 0x80, 0x11, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x77,
		0xc0, 0xa8, 0x01, 0xff
	};

	uint8_t data2_0[] = {
		0x45, 0x00, 0x00, 0x73, 0x00, 0x00, 0x40, 0x00, 0x40, 0x11, 0xb8, 0x61, 0xc0, 0xa8, 0x00, 0x01,
		0xc0, 0xa8, 0x00, 0xc7
	};

	uint8_t data2[] = {
		0x45, 0x00, 0x00, 0x73, 0x00, 0x00, 0x40, 0x00, 0x40, 0x11, 0x00, 0x00, 0xc0, 0xa8, 0x00, 0x01,
		0xc0, 0xa8, 0x00, 0xc7
	};

	uint16_t x = calcIpHdrChecksum(data2, sizeof(data2));
	printf("%x\n", x);
}

size_t
encodeNetBiosName(
	char* buffer,
	const char* name,
	char paddingChar
) {
	size_t i = 0;
	size_t j = 0;

	for (; i < 16; i++) {
		uchar_t c = name[i];
		if (!c)
			break;

		c = toupper(c);

		buffer[j++] = 'A' + (c >> 4);
		buffer[j++] = 'A' + (c & 0x0f);
	}

	char paddingByteHi = 'A' + (paddingChar >> 4);
	char paddingByteLo = 'A' + (paddingChar & 0x0f);

	while (j < 32) {
		buffer[j++] = paddingByteHi;
		buffer[j++] = paddingByteLo;
	}

	return j;
}

size_t
decodeNetBiosName(
	char* buffer,
	const char* name,
	size_t length
) {
	ASSERT(!(length & 1)); // must be even

	size_t i = 0;
	size_t j = 0;

	while (i < length) {
		uchar_t hi = name[i++] - 'A';
		uchar_t lo = name[i++] - 'A';

		buffer[j++] = lo + (hi << 4);
	}

	return j;
}

void
testNetBios() {
	const char* name = "*";

	char buffer[1024] = "CKAAAAAAA" "AAAAAAAAAAAAAAAA" "AAAAAAA"; //{ 0 };
	size_t length = 32; //encodeNetBiosName (buffer, name);
	printf("%s\n", buffer);

	char buffer2[1024] = { 0 };
	decodeNetBiosName(buffer2, buffer, length);
	printf("%s\n", buffer2);
}

//..............................................................................

#if (_AXL_OS_WIN)

BOOL WriteSlot(HANDLE hSlot, const char* lpszMessage) {
   BOOL fResult;
   DWORD cbWritten;

   fResult = ::WriteFile(hSlot,
	 lpszMessage,
	 strlen(lpszMessage),
	 &cbWritten,
	 (LPOVERLAPPED)NULL);

   if (!fResult) {
	  printf("WriteFile failed with %d.\n", GetLastError());
	  return FALSE;
   }

   printf("Slot written to successfully.\n");

   return TRUE;
}

BOOL ReadSlot(HANDLE hSlot) {
   BOOL fResult;
   DWORD cbRead;

   char buffer[256];

   fResult = ReadFile(hSlot, buffer, sizeof(buffer) - 1, &cbRead, NULL);
   if (!fResult) {
	  printf("ReadFile failed with %d.\n", GetLastError());
	  return FALSE;
   }

   buffer[cbRead] = 0;
   printf("Read from slot: %s\n", buffer);

   return TRUE;
}

void
testMailSlot() {
	printf("1 for server, 2 for client:\n");
	char buffer[256];
	fgets(buffer, countof(buffer), stdin);

	if (buffer[0] == '1') {
	char slotName[256];
	printf("slot name (e.g. \\\\.\\mailslot\\foo):\n");
	fgets(slotName, countof(slotName), stdin);

		HANDLE hFile = CreateMailslotA(slotName, 0, -1, NULL);
	   if (hFile == INVALID_HANDLE_VALUE) {
		  printf("CreateMailslot failed with %d.\n", GetLastError());
		  return;
	   }

	  printf("Server is running...\n");

		for (;;) {
		   ReadSlot(hFile);
		}
	} else for (;;) {

	char slotName[256];
	printf("slot name (e.g. \\\\*\\mailslot\\foo):\n");
	fgets(slotName, countof(slotName), stdin);

		HANDLE hFile;

	   hFile = CreateFileA(
		   slotName,
		 GENERIC_READ | GENERIC_WRITE,
		 FILE_SHARE_READ | FILE_SHARE_WRITE,
		 (LPSECURITY_ATTRIBUTES)NULL,
		 OPEN_EXISTING,
		 FILE_ATTRIBUTE_NORMAL,
		 (HANDLE)NULL);

	   if (hFile == INVALID_HANDLE_VALUE) {
		  printf("CreateFile failed with %d.\n", GetLastError());
		  return;
	   }

	   ReadSlot(hFile);

	   WriteSlot(hFile, "Message one for mailslot.");
	   WriteSlot(hFile, "Message two for mailslot.");

	   CloseHandle(hFile);
	}
}

#endif

//..............................................................................

#ifdef _AXL_XML

class MyXmlParser: public xml::ExpatParser<MyXmlParser> {
protected:
	size_t m_indent;

public:
	MyXmlParser() {
		m_indent = 0;
	}

	void
	onStartElement(
		const char* name,
		const char** attributes
	) {
		printIndent();
		printf("<%s", name);

		if (*attributes) {
			printf("\n");
			m_indent++;

			while (*attributes) {
				printIndent();
				printf("%s = %s\n", attributes [0], attributes [1]);
				attributes += 2;
			}

			m_indent--;
			printIndent();
		}

		printf(">\n");
		m_indent++;
	}

	void
	onEndElement(const char* name) {
		m_indent--;
		printIndent();
		printf("</%s>\n", name);
	}

	void
	onCharacterData(
		const char* string,
		size_t length
	) {
		printIndent();
		printf("%s\n", sl::String(string, length).sz());
	}

protected:
	void
	printIndent() {
		for (size_t i = 0; i < m_indent; i++)
			printf("  ");
	}
};

void
testXml() {
	bool result;

	MyXmlParser parser;
	result = parser.parseFile("c:/projects/playground/doxygen/xml/index.xml", 100);
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("success\n");
}

#endif

void
testFileEnum() {
	io::FileEnumerator fileEnum;
	bool result = fileEnum.openDir(".");
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	while (fileEnum.hasNextFile()) {
		sl::String fileName = fileEnum.getNextFileName();
		printf("%s\n", fileName.sz());
	}
}

void
testStringCase() {
	sl::String s = "Hui Govno i Muravei";
	s.makeLowerCase();
	printf("s = %s\n", s.sz());

	s.makeUpperCase();
	printf("s = %s\n", s.sz());
}

//..............................................................................

enum CmdLineSwitchKind {
	CmdLineSwitchKind_Undefined = 0,
	CmdLineSwitchKind_Help,
	CmdLineSwitchKind_Version,
	CmdLineSwitchKind_Verbose,
	CmdLineSwitchKind_Add,
	CmdLineSwitchKind_Remove,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SL_BEGIN_CMD_LINE_SWITCH_TABLE(CmdLineSwitchTable, CmdLineSwitchKind)
	AXL_SL_CMD_LINE_SWITCH_2(
		CmdLineSwitchKind_Help,
		"h", "help", NULL,
		"Display this help"
	)
	AXL_SL_CMD_LINE_SWITCH_2(
		CmdLineSwitchKind_Version,
		"v", "version", NULL,
		"Display version"
	)
	AXL_SL_CMD_LINE_SWITCH_2(
		CmdLineSwitchKind_Verbose,
		"z", "verbose", NULL,
		"Verbose mode"
	)
	AXL_SL_CMD_LINE_SWITCH_2(
		CmdLineSwitchKind_Add,
		"a", "add", "<item>",
		"Add item <item>"
	)
	AXL_SL_CMD_LINE_SWITCH_2(
		CmdLineSwitchKind_Remove,
		"r", "remove", "<item>",
		"Remove item <item>"
	)
AXL_SL_END_CMD_LINE_SWITCH_TABLE()

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CmdLineParser: public sl::CmdLineParser<CmdLineParser, CmdLineSwitchTable> {
	friend class sl::CmdLineParser<CmdLineParser, CmdLineSwitchTable>;

protected:
	bool
	onValue(const sl::StringRef& value) {
		printf("onValue (%s)\n", value.sz());
		return true;
	}

	bool
	onSwitch(
		SwitchKind switchKind,
		const sl::StringRef& value
	) {
		switch (switchKind) {
		case CmdLineSwitchKind_Help:
			printf("CmdLineSwitchKind_Help\n");
			break;

		case CmdLineSwitchKind_Version:
			printf("CmdLineSwitchKind_Version\n");
			break;

		case CmdLineSwitchKind_Verbose:
			printf("CmdLineSwitchKind_Verbose\n");
			break;

		case CmdLineSwitchKind_Add:
			printf("CmdLineSwitchKind_Add (%s)\n", value.sz());
			break;

		case CmdLineSwitchKind_Remove:
			printf("CmdLineSwitchKind_Remove (%s)\n", value.sz());
			break;

		};

		return true;
	}
};

#if (_AXL_OS_WIN)
void
testCmdLine(
	int argc,
	wchar_t* argv[]
)
#else
void
testCmdLine(
	int argc,
	char* argv[]
)
#endif
{
	CmdLineParser parser;
	bool result = parser.parse(argc, argv);
	if (!result)
		printf("Error parsing command line: %s\n", err::getLastErrorDescription().sz());

#if (_AXL_OS_WIN)

	printf("Now, using GetCommandLineW()...\n");

	result = parser.parse(sl::String(::GetCommandLineW()));
	if (!result)
		printf("Error parsing command line: %s\n", err::getLastErrorDescription().sz());
#endif
}

//..............................................................................

class ServerThread: public sys::ThreadImpl<ServerThread> {
public:
	sys::Event m_startEvent;

public:
	intptr_t threadFunc() {
		io::SockAddr addr;
		addr.parse("0.0.0.0:1002");

		printf("listening on TCP %s...\n", addr.getString().sz());

		io::Socket serverSocket;
		io::Socket clientSocket;

		bool result =
			serverSocket.open(AF_INET, SOCK_STREAM, IPPROTO_TCP) &&
			serverSocket.bind(addr) &&
			serverSocket.listen(8);

		m_startEvent.signal();

		printf("waiting for clients...\n");
		result = serverSocket.accept(&clientSocket, &addr);
		if (!result) {
			printf("failed: %s\n", err::getLastErrorDescription().sz());
			return -1;
		}


		printf("client connected from: %s\n", addr.getString().sz());

		for (;;) {
			char buffer[1024];
			int x = clientSocket.recv(buffer, sizeof(buffer) - 1);

			if (x > 0) {
				buffer[x] = 0;
				printf("client sent %d bytes: %s\n", x, buffer);
			} else if (x < 0) {
				printf("client reset.\n");
				break;
			} else {
				printf("client disconnected.\n");
				break;
			}
		}

		printf("server thread done.\n");

		return 0;
	}
};

bool
testConn() {
/*	printf("startion server thread...\n");

	ServerThread thread;
	thread.start();
	thread.m_startEvent.wait();
*/
	io::SockAddr addr;
	addr.parse("192.168.1.73:1002");

	printf("connecting to %s...\n", addr.getString().sz());

	io::Socket socket;
	bool result =
		socket.open(AF_INET, SOCK_STREAM, IPPROTO_TCP) &&
		socket.connect(addr);

	if (!result) {
		printf("failed: %s\n", err::getLastErrorDescription().sz());
		return false;
	}

	for (;;) {
		static char data[] = "hui govno i muravei";

		printf("%s: sending %d bytes...", sys::Time(sys::getTimestamp()).format("%h:%m:%s").sz(), sizeof(data));
		size_t result = socket.send(data, sizeof(data));
		printf("%d bytes sent\n", result);

		sys::sleep(5000);
	}

	printf("closing...\n");
	socket.close();

//	printf ("waiting for server thread...\n");
//	thread.wait();

	printf("done.\n");
	return true;
}

bool
testSerial() {
	char const* port = "COM6";

	printf("opening %s...\n", port);

	io::SerialSettings settings;
	settings.m_baudRate = 38400;
	settings.m_dataBits = 8;
	settings.m_stopBits = io::SerialStopBits_1;
	settings.m_flowControl = io::SerialFlowControl_RtsCts;
	settings.m_parity = io::SerialParity_None;

	io::Serial serial;
	bool result =
		serial.open(port) &&
		serial.setSettings(&settings);

	if (!result) {
		printf("failed: %s\n", err::getLastErrorDescription().sz());
		return false;
	}

	printf("writing data...\n");
	char data[] = "abcd";
	size_t size = serial.write(data, sizeof(data));
	printf("written: %d byte(s)\n", size);

#if (_AXL_OS_WIN)
	printf("reading data...\n");

	io::win::StdOverlapped overlapped;
	char buffer[1024];
	result = serial.m_serial.overlappedRead(buffer, sizeof(buffer), &overlapped);
	printf("result: %d\n", result);

	printf("waiting...\n");

	result = overlapped.m_completionEvent.wait(5000);

	printf("wait result: %d\n", result);

	if (result) {
		size = serial.m_serial.getOverlappedResult(&overlapped);
		printf("overlapped result: %d\n", size);

		if (size != -1) {
			buffer[size] = 0;
			printf("read: %s\n", buffer);
		}
	}
#endif

	printf("purging port...\n");
	serial.purge();

	printf("closing port...\n");
	serial.close();

	printf("done!\n");
	return true;

#if (_AXL_OS_WIN)
	serial.m_serial.setWaitMask(EV_ERR);

	for (;;) {
		char buf[1024] = { 0 };

		size_t result = serial.read(buf, sizeof(buf));
		if (result == -1) {
			printf("failed: %s\n", err::getLastErrorDescription().sz());
			return false;
		}

		buf[result] = 0;
		printf("received: %s\n", buf);

		dword_t events;
		bool result2 = serial.m_serial.wait(&events);
		if (!result2) {
			printf("failed: %s\n", err::getLastErrorDescription().sz());
			return false;
		}

		printf("events: %x\n", events);

		dword_t errors;
		COMSTAT stat;
		result2 = serial.m_serial.clearError(&errors, &stat);
		if (!result2) {
			printf("failed: %s\n", err::getLastErrorDescription().sz());
			return false;
		}

		printf("errors: %x\n", errors);
	}
#endif

	size_t totalSize = 0;
	size_t targetSize = 20 * 1024;

	printf("\rwriting... %3d%%; bytes: %5d", totalSize * 100 / targetSize, totalSize);

	while (totalSize < targetSize) {
		static char data[] = "123456789abcdef";
		size_t size = serial.write(data, sizeof(data));
		if (size == -1) {
			printf("can't write: %s\n", err::getLastErrorDescription().sz());
			return false;
		}

		totalSize += size;
		printf("\rwriting... %3d%%; bytes: %5d", totalSize * 100 / targetSize, totalSize);
	}

	printf("\ndone\n");
	return true;
}

void
testStringReplace() {
	sl::String title = "contains: < (less-than) > (greater-than) & (ampersand)";
	title.replace("<", "&lt;");
	title.replace(">", "&gt;");
	title.replace("&", "&amp;");

	printf("title = %s\n", title.sz());
}

//..............................................................................

void
testEvent() {
	for (int i = 0; i < 10; i++) {
		sys::NotificationEvent e;

		bool r = e.wait(500);
		if (!r)
			printf("[%s] ---wait failed: %s\n", sys::Time(sys::getTimestamp()).format("%h:%m:%s.%l").sz(), err::getLastErrorDescription().sz());
		else
			printf("[%s] ---wait SUCCEEDED! (WTF?!)\n", sys::Time(sys::getTimestamp()).format("%h:%m:%s.%l").sz());
	}
}

//..............................................................................

void
testBase32() {
	char s[] = "NB2WSLBAM5XXM3TPEBUSA3LVOJQXMZLJFQQG25LSMF3GK2JMEBTW65TON4QGSIDIOVUSCIJBEE======";

	sl::Array<char> dec = enc::Base32Encoding::decode(s);
	dec.append(0);
	printf("decoded: %s\n", dec.cp());

	sl::String enc = enc::Base32Encoding::encode(dec, dec.getCount() - 1);
	printf("encoded: %s\n", enc.sz());
	printf("orginal: %s\n", s);
	ASSERT(enc == s);

	sl::Array<char> source;

	for (int i = 0; i < 500; i++) {
		size_t size = rand() % 64 + 16;
		source.setCount(size);
		for (size_t j = 0; j < size; j++)
			source[j] = (char)rand();

		sl::String enc = enc::Base32Encoding::encode(source, size);
		sl::Array<char> dec = enc::Base32Encoding::decode(enc);
		ASSERT(dec.getCount() == size && memcmp(dec, source, size) == 0);
	}
}

void
testBase64() {
	char s[] = "aHVpLCBnb3ZubyBpIG11cmF2ZWk7IG11cmF2ZWksIGdvdm5vIGkgaHVpISEhIQ==";

	sl::Array<char> dec = enc::Base64Encoding::decode(s);
	dec.append(0);
	printf("decoded: %s\n", dec.cp());

	sl::String enc = enc::Base64Encoding::encode(dec, dec.getCount() - 1);
	printf("encoded: %s\n", enc.sz());
	printf("orginal: %s\n", s);
	ASSERT(enc == s);

	sl::Array<char> source;

	for (int i = 0; i < 500; i++) {
		size_t size = rand() % 64 + 16;
		source.setCount(size);
		for (size_t j = 0; j < size; j++)
			source[j] = (char)rand();

		sl::String enc = enc::Base64Encoding::encode(source, size);
		sl::Array<char> dec = enc::Base64Encoding::decode(enc);
		ASSERT(dec.getCount() == size && memcmp(dec, source, size) == 0);
	}
}

//..............................................................................

void
testTime() {
	// windows:
	// timestamp = 0x1d245fe80cc7e08; time = Thursday 24 November 2016 10:57:33
	// timestamp = 0x1d245fe906433ed; time = Thursday 24 November 2016 10:57:59
	// timestamp = 0x1d245fe9b7e771d; time = Thursday 24 November 2016 10:58:17

	// linux:
	// timestamp = 0x1d246026f5fa498; time = Thursday 24 November 2016 11:25:41
	// timestamp = 0x1d2460277e1f810; time = Thursday 24 November 2016 11:25:56
	// timestamp = 0x1d2460280f3b836; time = Thursday 24 November 2016 11:26:11

	uint64_t timestamp = sys::getTimestamp();
	printf("timestamp = 0x%llx; time = %s\n", timestamp, sys::Time(timestamp).format().sz());
}

//..............................................................................

void
testSerial2(uint_t baud) {
	bool result;

#ifdef TCSETS
	printf(
		"TCSETS:   %x\nTCSETSW:  %x\nTCSETSF:  %x\n",
		TCSETS,
		TCSETSW,
		TCSETSF
	);
#endif

#ifdef TCSETS2
	printf(
		"TCSETS2:  %x\nTCSETSW2: %x\nTCSETSF2: %x\n",
		TCSETS2,
		TCSETSW2,
		TCSETSF2
	);
#endif

	io::Serial serial;

	printf("opening port...\n");

	result = serial.open("/dev/cu.usbserial-1460");
	if (!result) {
		printf("open error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("port successfully opened\n");

	io::SerialSettings settings;
	settings.m_baudRate = baud;
	settings.m_dataBits = 8;
	settings.m_stopBits = io::SerialStopBits_1;
	settings.m_flowControl = io::SerialFlowControl_None;
	settings.m_parity = io::SerialParity_None;

	result = serial.setSettings(&settings);
	if (!result) {
		printf("settings error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("port successfully configured\n");
	serial.write("hui", 3);

	sys::sleep(500);
	serial.close();
	return;

	serial.write("\r", 1);

	char buffer[1024];
	size_t size = serial.read(buffer, sizeof(buffer));

	sl::String s0 = "takie dela";

	sl::String s = enc::HexEncoding::encode(buffer, size);
	printf("incoming: %s\n", s.sz());
}

//..............................................................................

void
testSymbolicLinks() {
	sl::String target;
	bool result = io::getSymbolicLinkTarget(&target, "/home/vladimir/a");
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("target: %s\n", target.sz());
}

//..............................................................................

void
testBoyerMoore() {
#if (0)
	static const char pattern[] = "muravei";

	sl::BinaryBoyerMooreFind find;
	find.setPattern(pattern, lengthof(pattern), sl::BoyerMooreFlag_Reverse);

	static const char chunk1[] = "vei! mura";
	static const char chunk2[] = "vei, go";

	size_t offset;

	sl::BinaryBoyerMooreFind::IncrementalContext incrementalContext;
	offset = find.find(&incrementalContext, 0, chunk2, lengthof(chunk2));
	offset = find.find(&incrementalContext, 0, chunk1, lengthof(chunk1));

	printf("offset = %d\n", offset);
#else
	static const char pattern[] = "s";

	sl::TextBoyerMooreFind find;
	find.setPattern(
		enc::CharCodecKind_Utf8,
		pattern,
		lengthof(pattern),
		sl::BoyerMooreFlag_Reverse | sl::TextBoyerMooreFlag_CaseInsensitive
	);

	static const utf32_t chunk1[] = { 'S', 'e' };

	size_t offset;

	sl::TextBoyerMooreFind::IncrementalContext incrementalContext;
	offset = find.find(&incrementalContext, enc::CharCodecKind_Utf32, 0, chunk1, sizeof(chunk1));

#endif
}

//..............................................................................

struct RecordFileHdr {
	uint32_t m_signature; // RecordFileConst_LogFileSignature
	uint32_t m_version;
	uint64_t m_recordCount;
	uint64_t m_totalRecordSize;
	uint32_t _m_reserved;
	uint32_t m_recordOffset  : 24;
	uint32_t m_auxClassCount : 8;
	sl::Guid m_classGuid;

	// followed by sl::Guid [m_classCount]
	// followed by aux headers (may be added in further versions)
	// followed by log records at m_recordOffset
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct Record {
	uint32_t m_signature; // RecordFileConst_RecordSignature
	uint32_t m_code;
	uint64_t m_timestamp;
	uint32_t m_dataSize;
	uint32_t _m_padding;

	// followed by record data (if any)
};

void
testIoPerformance() {
	#define _READ_ONLY 0
	#define _DUMMY_WRITE 0

	io::File simpleFileSrc;
#if (!_READ_ONLY)
	io::File simpleFileDst;
#endif

	static char fileNameSrc[] = "G:/Temp/Test MTK TCP/Test MTK TCP.njlog";
	static char fileNameDst_s[] = "G:/Temp/Test MTK TCP/Test MTK TCP - 2.njlog";
	static char fileNameDst_m[] = "G:/Temp/Test MTK TCP/Test MTK TCP - 3.njlog";

	bool result = simpleFileSrc.open(fileNameSrc, io::FileFlag_ReadOnly);
	if (!result) {
		printf("can't open %s: %s\n", fileNameSrc, err::getLastErrorDescription().sz());
		return;
	}

	uint64_t baseTimestamp;
	uint64_t time;
	uint64_t offset;

	sl::Array<char> buffer;

	struct Rec: Record {
		char m_data[1024];
	};

	Rec staticRec;
	memset(&staticRec, 0, sizeof(staticRec));
	staticRec.m_dataSize = sizeof(staticRec.m_data);

	io::MappedFile mappedFileSrc;
	io::MappedFile mappedFileDst;

	result = mappedFileSrc.open(fileNameSrc, io::FileFlag_ReadOnly);

#if (!_READ_ONLY)
	result =
		result &&
		mappedFileDst.open(fileNameDst_m) &&
		mappedFileDst.setSize(0);
#endif

	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("testing mapped file...\n");
	baseTimestamp = sys::getTimestamp();

	RecordFileHdr* hdr2 = (RecordFileHdr*)mappedFileSrc.view(0, sizeof(RecordFileHdr), true);

#if (!_READ_ONLY)
	RecordFileHdr* hdr3 = (RecordFileHdr*)mappedFileDst.view(0, sizeof(RecordFileHdr), true);
	*hdr3 = *hdr2;
#endif

	offset = sizeof(RecordFileHdr);
	uint64_t endOffset = offset + hdr2->m_totalRecordSize;

	while (offset < endOffset) {
#if (_DUMMY_WRITE)
		Record* rec = &staticRec;
		size_t recSize = sizeof(rec);
#else
		Record* rec = (Record*)mappedFileSrc.view(offset, sizeof(Record));
		size_t recSize = sizeof(Record) + rec->m_dataSize;
		if (rec->m_dataSize)
			rec = (Record*)mappedFileSrc.view(offset, recSize);
#endif

#if (!_READ_ONLY)
		void* p = mappedFileDst.view(offset, recSize);
		memcpy(p, rec, recSize);
#endif

		offset += recSize;
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m.%s.%l").sz());

#if (!_READ_ONLY)
	result =
		result	&&
		simpleFileDst.open(fileNameDst_s) &&
		simpleFileDst.setSize(0);
#endif

	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("testing simple file...\n");
	baseTimestamp = sys::getTimestamp();

	RecordFileHdr hdr;
	simpleFileSrc.read(&hdr, sizeof(hdr));

#if (!_READ_ONLY)
	simpleFileDst.write(&hdr, sizeof(hdr));
#endif

	offset = 0;
	while (offset < hdr.m_totalRecordSize) {
#if (!_DUMMY_WRITE)
		Record rec;
		simpleFileSrc.read(&rec, sizeof(rec));
		if (rec.m_dataSize) {
			buffer.setCount(rec.m_dataSize);
			simpleFileSrc.read(buffer, rec.m_dataSize);
		}

#	if (!_READ_ONLY)
		simpleFileDst.write(&rec, sizeof(rec));
		simpleFileDst.write(buffer, rec.m_dataSize);
#	endif

		offset += sizeof(rec) + rec.m_dataSize;
#else
		simpleFileDst.write(&staticRec, sizeof(staticRec));
		offset += sizeof(staticRec);
#endif
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m.%s.%l").sz());
}

//..............................................................................

volatile size_t g_readerCount = 0;
volatile size_t g_writerCount = 0;

class RwLockThread:
	public sl::ListLink,
	public sys::ThreadImpl<RwLockThread> {
public:
	enum {
		MaxSleepTime = 10,
	};

protected:
	sys::ReadWriteLock m_lock;
	size_t m_index;
	size_t m_iterationCount;
	bool m_isWriter;

public:
	RwLockThread(
		const sl::StringRef& mappingName,
		const sl::StringRef& readEventName,
		const sl::StringRef& writeEventName,
		size_t index,
		size_t iterationCount,
		bool isWriter
	) {
		bool result = m_lock.open(mappingName, readEventName, writeEventName);
		ASSERT(result);

		m_index = index;
		m_iterationCount = iterationCount;
		m_isWriter = isWriter;
	}

	void
	threadFunc() {
		uint64_t tid = sys::getCurrentThreadId();
		printf("Starting thread %llx\n", tid);

		if (m_isWriter)
			for (size_t i = 0; i < m_iterationCount; i++) {
				m_lock.writeLock();
				ASSERT(g_readerCount == 0);
				ASSERT(g_writerCount == 0);
				sys::atomicInc(&g_writerCount);
				printf("writer #%d (TID:%llx) is writing (%d/%d)...\n", m_index, tid, i, m_iterationCount);
				sys::sleep(rand() % MaxSleepTime);
				sys::atomicDec(&g_writerCount);
				m_lock.writeUnlock();
				sys::yieldProcessor();
			} else
			for (size_t i = 0; i < m_iterationCount; i++) {
				m_lock.readLock();
				ASSERT(g_writerCount == 0);
				sys::atomicInc(&g_readerCount);
				printf("reader #%d (TID:%llx) is reading (%d/%d; %d readers total)...\n", m_index, tid, i, m_iterationCount, g_readerCount);
				sys::sleep(rand() % MaxSleepTime);
				sys::atomicDec(&g_readerCount);
				m_lock.readUnlock();
				sys::yieldProcessor();
			}
	}
};

void
testReadWriteLock() {
	enum {
		ReaderCount    = 8,
		WriterCount    = 2,
		IterationCount = -1, // infinite
	};

	static const char mappingName[] = "rwl-mapping";
	static const char readEventName[] = "rwl-read-event";
	static const char writeEventName[] = "rwl-write-event";

#if (_AXL_OS_POSIX)
	io::psx::SharedMemory::unlink(mappingName);
	io::psx::SharedMemory::unlink(readEventName);
	sys::psx::NamedSem::unlink(readEventName);
	io::psx::SharedMemory::unlink(writeEventName);
	sys::psx::NamedSem::unlink(writeEventName);
#endif

	bool result;

	sys::ReadWriteLock lock;
	result = lock.create(mappingName, readEventName, writeEventName);
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	sl::List<RwLockThread> threadList;

	lock.writeLock();

	for (size_t i = 0; i < ReaderCount; i++) {
		RwLockThread* thread = AXL_MEM_NEW_ARGS(RwLockThread, (mappingName, readEventName, writeEventName, i, IterationCount, false));
		thread->start();
		threadList.insertTail(thread);
	}

	for (size_t i = 0; i < WriterCount; i++) {
		RwLockThread* thread = AXL_MEM_NEW_ARGS(RwLockThread, (mappingName, readEventName, writeEventName, i, IterationCount, true));
		thread->start();
		threadList.insertTail(thread);
	}

	lock.writeUnlock();

	sl::Iterator<RwLockThread> it = threadList.getHead();
	for (; it; it++)
		it->waitAndClose();
}

//..............................................................................

template <typename T>
void
foo(T* p) {
	printf("foo - 1\n");
}

template <typename T>
void
foo(const T* p) {
	printf("foo - 2\n");
}

struct Point2d : sl::ListLink {
	int m_x;
	int m_y;
};

void
testConstList() {
	sl::List<Point2d> list;

	Point2d* point = AXL_MEM_NEW(Point2d);
	point->m_x = 10;
	point->m_y = 20;
	list.insertTail(point);

	point = AXL_MEM_NEW(Point2d);
	point->m_x = 30;
	point->m_y = 40;
	list.insertTail(point);

	sl::Iterator<Point2d> it = list.getHead();
	sl::ConstIterator<Point2d> it2 = it;
	it2 = it;
	it2++;

	it2 = it.getInc(1);

	int x = it->m_x;
	int y = it2->m_y;

	it->m_x = 0;

//	it->m_x = 10;
//	it->m_y = 20;
}

void
testBitIdx() {
	size_t size = sl::getAllocSize(0);
	ASSERT(size == 0);

	size_t x = sl::getPowerOf2Ge(0);
	ASSERT(x == 0);

	size_t y = sl::getHiBit(0);
	ASSERT(y == 0);

	size_t z = sl::getLoBit(0);
	ASSERT(z == 0);

	for (size_t i = 1; i < 128 * 1024 * 1024; i++) {
		size_t size = sl::getAllocSize(i);
		ASSERT(size >= i);

		size_t x = sl::getPowerOf2Ge(i);
		ASSERT(x >= i);

		size_t y = sl::getHiBit(i);
		ASSERT((i & y) && y <= i);

		size_t z = sl::getLoBit(i);
		ASSERT((i & z) && z <= y);
	}
}

//..............................................................................

#if (_AXL_IO_PCAP)

class PcapThread: public sys::ThreadImpl<PcapThread> {
public:
	io::Pcap m_pcap;

public:
	void threadFunc() {
		for (;;) {
			char buffer[1024];
			size_t result = m_pcap.read(buffer, sizeof(buffer));
			printf("result: %d\n", result);

			if (result == 0)
				sys::sleep(1000);
		}
	}
};

void
testPcap() {
#if (_AXL_OS_WIN)
	::SetDllDirectoryW(io::win::getSystemDir() + L"\\npcap");
#endif

	const char* version = pcap_lib_version();
	printf("version: %s\n", version);

	pcap_if* ifaceList = NULL;
	char errorBuffer[PCAP_ERRBUF_SIZE] = { 0 };
	int result = pcap_findalldevs(&ifaceList, errorBuffer);
	if (result == -1) {
		printf("error: %s\n", errorBuffer);
		return;
	}

	size_t count = 0;

	sl::String s;

	for (pcap_if* iface = ifaceList; iface; iface = iface->next, count++)
		printf("%s - %s\n", iface->name, iface->description);

	PcapThread thread;

	result = thread.m_pcap.openLive("lo", 4096, true, 200);
	if (!result) {
		printf("error opening lo: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	thread.m_pcap.setBlockingMode(false);

	thread.start();

	printf("main thread is waiting now...\n");
	for (;;)
		sys::sleep(1000);
}

#endif

//..............................................................................

class UdpThread: public sys::ThreadImpl<UdpThread> {
protected:
	io::Socket m_socket;

public:
	void threadFunc() {
		io::Socket socket;

		bool result =
			socket.open(AF_INET, SOCK_DGRAM, IPPROTO_UDP) &&
			socket.bind(io::SockAddr((uint_t)0, 1234));

		printf("socket opened: %d\n", result);

		for (;;) {
			char buffer[256];
			io::SockAddr addr;
			size_t result = socket.recvFrom(buffer, sizeof(buffer), &addr);
			printf("received: %d from: %s\n", result, addr.getString().sz());
		}
	}
};

void
testUdp() {
	UdpThread thread;
	thread.start();

	io::SockAddr addr(0x7f000001, 1234);

	for (;;) {
		io::Socket socket;
		socket.open(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		size_t result = socket.sendTo(NULL, 0, addr);
		printf("result: %zd\n", result);
		sys::sleep(1000);
	}
}

//..............................................................................

void testProcName() {
	uint_t pidTable[] = {
		1,
		272,
		279,
		476,
	};

	for (size_t i = 0; i < countof(pidTable); i++) {
		uint_t pid = pidTable[i];
		printf("%d: %s\n", pid, sys::getProcessImageName(pid).sz());
	}
}

//..............................................................................

#if (_AXL_OS_WIN)

void testUnnamedPipes() {
	HANDLE hReadPipe;
	HANDLE hWritePipe;
	dword_t actualSize;

	::CreatePipe(&hReadPipe, &hWritePipe, NULL, 0);

	char data[] = "hui, govno i muravei";
	::WriteFile(hWritePipe, data, sizeof(data), &actualSize, NULL);

	char buffer[1024];
	::ReadFile(hReadPipe, buffer, sizeof(buffer), &actualSize, NULL);

	::CloseHandle(hReadPipe);
	::CloseHandle(hWritePipe);
}

#endif

//..............................................................................

#if (_AXL_IO_SSL)

void
printX509Name(X509_name_st* name) {
	int count = X509_NAME_entry_count(name);
	for (int i = 0; i < count; i++) {
		X509_NAME_ENTRY* entry = X509_NAME_get_entry(name, i);
		ASN1_OBJECT* object = X509_NAME_ENTRY_get_object(entry);
		ASN1_STRING* data = X509_NAME_ENTRY_get_data(entry);

		printf("    %4d: %s: %s\n", OBJ_obj2nid(object), cry::getAsn1ObjectString(object).sz(), cry::getAsn1StringString(data).sz());
	}
}


void
printSslCertificate(X509* cert) {
//	int result;

	ASN1_INTEGER* serialNumber = X509_get_serialNumber(cert);
	ASN1_TIME* notBeforeTime = X509_get_notBefore(cert);
	ASN1_TIME* notAfterTime = X509_get_notAfter(cert);
	X509_name_st* subjectName = X509_get_subject_name(cert);
	X509_name_st* issuerName = X509_get_issuer_name(cert);

	cry::BigNum bigNum;
	bigNum.create();
	ASN1_INTEGER_to_BN(serialNumber, bigNum);
	printf("  serial:     %s\n", bigNum.getHexString().sz());
	printf("  valid from: %s\n", cry::getAsn1TimeString(notBeforeTime).sz());
	printf("  valid to:   %s\n", cry::getAsn1TimeString(notAfterTime).sz());

	printf("  subject:\n");
	printX509Name(subjectName);

	printf("  issuer:\n");
	printX509Name(issuerName);
}

void
copyPrintSslCertificate(X509* cert0) {
	cry::X509Cert cert = cert0;
	cry::X509Cert copyCert;

	sl::Array<char> buffer = cert.saveDer();
	copyCert.loadDer(buffer, buffer.getCount());
	printf("DER-copy:\n");
	printSslCertificate(copyCert);

	sl::String string = cert.savePem();
	copyCert.loadPem(string);
	printf("PEM-copy:\n");
	printSslCertificate(copyCert);

	cert.detach();
}

int
sslVerifyCallback(
	int isPreVerifyOk,
	X509_STORE_CTX* ctx
) {
	printf("sslVerifyCallback(%d, %p)\n", isPreVerifyOk, ctx);

	X509* cert = X509_STORE_CTX_get_current_cert(ctx);
	printSslCertificate(cert);
	int err = X509_STORE_CTX_get_error(ctx);
    printf("  error: %d: %s\n", err, X509_verify_cert_error_string(err));

	return true;
}

void
sslInfoCallback(
	const SSL* ssl,
	int type,
	int value
) {
	printf("sslInfoCallback(0x%x, %d) -> %s\n", type, value, SSL_state_string_long(ssl));
}

void testSsl() {
	bool result;

	::SSL_library_init();
	::SSL_load_error_strings();
	cry::registerCryptoErrorProviders();

//	const char* ipString = "216.58.200.238";  // google.com
	const char* ipString = "104.236.152.161"; // ioninja.com
//	const char* ipString = "93.174.104.89";   // tibbo.com
	int port = 443;

	io::SockAddr addr;
	addr.parse_ip4(ipString);
	addr.setPort(port);

	io::Socket socket;
	printf("connecting to %s:%d...\n", ipString, port);

	result =
		socket.open(AF_INET, SOCK_STREAM, IPPROTO_IP) &&
		socket.connect(addr);

	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	cry::Bio bio;
	io::SslCtx ctx;
	io::Ssl ssl;

	result =
		ctx.create() &&
		ctx.loadVerifyLocations("C:/Users/Vladimir/DigiCertSHA2ExtendedValidationServerCA.pem") &&
		//ctx.loadVerifyLocations("c:/ubuntu/etc/ssl/certs/ca-certificates.crt") &&
		ssl.create(ctx) &&
		bio.createSocket(socket.m_socket);

	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	ssl.setVerify(SSL_VERIFY_PEER, sslVerifyCallback);
	ssl.setInfoCallback(sslInfoCallback);
	ssl.setBio(bio.detach());
	ssl.setConnectState();

	printf("handshaking...\n");

	result = ssl.doHandshake();
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("------------------------\n");

	STACK_OF(X509)* chain = ssl.getPeerCertificateChain();

	printf("Peer certificate(s):\n");

	int count = sk_X509_num(chain);
	if (count == 1) {
		X509* cert = sk_X509_value(chain, 0);
		printSslCertificate(cert);
		copyPrintSslCertificate(cert);
	} else for (int i = 0; i < count; i++) {
		printf("Certificate[%d]:\n", i);
		X509* cert = sk_X509_value(chain, i);
		printSslCertificate(cert);
		copyPrintSslCertificate(cert);
	}

	long verifyResult = ssl.getVerifyResult();
	printf("verify result: %s\n", cry::X509Error(verifyResult).getDescription().sz());

	printf("getting front page...\n");

	const char get[] =
		"GET / HTTP/1.1\n"
		"Host: www.ioninja.com\n"
		"Connection: close\n"
		"\n\n";

	ssl.write(get, lengthof(get));

	for (size_t i = 0; i < 10; i++) {
		char buffer[256] = { 0 };
		size_t size = ssl.read(buffer, sizeof(buffer) - 1);
		if (size != -1)
			printf("%s", buffer);
		else
			sys::sleep(200);
	}

	printf("\nshutting down...\n");

	ssl.shutdown();

	printf("done.\n");
}

#endif

//..............................................................................

#if (_AXL_OS_POSIX)

#include "axl_io_psx_Pipe.h"

void
exec(const sl::StringRef& commandLine) { // returns on failure only
	char buffer[256];
	sl::Array<char*> argv(rc::BufKind_Stack, buffer, sizeof(buffer));

	sl::String string = commandLine;

	size_t length = string.getLength();
	for (;;) {
		string.trimLeft();
		if (string.isEmpty())
			break;

		argv.append(string.p());

		size_t pos = string.findOneOf(sl::StringDetails::getWhitespace());
		if (pos == -1)
			break;

		string[pos] = 0;
		string = string.getSubString(pos + 1);
	}

	if (argv.isEmpty()) {
		err::setError("empty command line");
		return;
	}

	argv.append(NULL);

	int result = ::execvp(argv[0], argv.p());
	ASSERT(result == -1);
	err::setLastSystemError();
}

FILE* g_log = fopen("/home/vladimir/child-stdio-log.txt", "w");

bool
pump(
	const char* prefix,
	int fin,
	int fout
) {
	sl::String hexString;
	char buffer[4 * 1024];

	for (;;) {
		ssize_t result = ::read(fin, buffer, sizeof(buffer));
		if (result < 0)
			return errno == EAGAIN;

		if (result == 0)
			return true;

		::write(fout, buffer, result);

		enc::HexEncoding::encode(
			&hexString,
			buffer,
			result,
			enc::HexEncodingFlag_Multiline
		);

		fprintf(g_log, "%s (%d bytes):\n%s\n", prefix, result, hexString.sz());
		fflush(g_log);
	}
}

void
pumpChildStdio(
	int childStdinFd,
	int childStdoutFd,
	int childStderrFd
) {
	int result;
	int tmp = AXL_MAX(childStdoutFd, childStderrFd);
	int selectFd = AXL_MAX(tmp, STDIN_FILENO) + 1;

	for (;;) {
		fd_set readSet = { 0 };

		FD_SET(STDIN_FILENO, &readSet);
		FD_SET(childStdoutFd, &readSet);

		if (childStderrFd != -1)
			FD_SET(childStderrFd, &readSet);

		result = ::select(selectFd, &readSet, NULL, NULL, NULL);
		if (result == -1)
			break;

		if (FD_ISSET(STDIN_FILENO, &readSet))
			if (!pump("STDIN", STDIN_FILENO, childStdinFd))
				break;

		if (FD_ISSET(childStdoutFd, &readSet))
			if (!pump("STDOUT", childStdoutFd, STDOUT_FILENO))
				break;

		if (FD_ISSET(childStderrFd, &readSet))
			if (!pump("STDERR", childStderrFd, STDERR_FILENO))
				break;
	}

	printf("PROCESS FINISHED!\n");
}

bool
testExecPassthrough(const sl::StringRef& cmdLine) {
	axl::io::psx::Pipe stdinPipe;
	axl::io::psx::Pipe stdoutPipe;
	axl::io::psx::Pipe stderrPipe;
	axl::io::psx::Pipe execPipe;

	bool result =
		stdinPipe.create() &&
		stdinPipe.m_writeFile.setBlockingMode(false) &&
		stdoutPipe.create() &&
		stdoutPipe.m_readFile.setBlockingMode(false) &&
		stderrPipe.create() &&
		stderrPipe.m_readFile.setBlockingMode(false) &&
		execPipe.create();

	if (!result)
		return false;

	execPipe.m_writeFile.fcntl(F_SETFD, FD_CLOEXEC);

	err::Error error;

	pid_t pid = ::fork();
	switch (pid) {
	case -1:
		err::setLastSystemError();
		return false;

	case 0:
		::dup2(stdinPipe.m_readFile, STDIN_FILENO);
		::dup2(stdoutPipe.m_writeFile, STDOUT_FILENO);
		::dup2(stderrPipe.m_writeFile, STDERR_FILENO);

		exec(cmdLine);

		error = err::getLastError();
		execPipe.m_writeFile.write(error, error->m_size);
		execPipe.m_writeFile.flush();

		::_exit(-1);
		ASSERT(false);

	default:
		execPipe.m_writeFile.close();

		fd_set rdset;
		FD_ZERO(&rdset);
		FD_SET(execPipe.m_readFile, &rdset);

		char buffer[256];
		size_t size = execPipe.m_readFile.read(buffer, sizeof(buffer));
		if (size == 0 || size == -1)
			break;

		if (((err::ErrorHdr*)buffer)->m_size == size)
			err::setError((err::ErrorHdr*)buffer);
		else
			err::setError("POSIX execvp failed"); // unlikely fallback

		return false;
	}

	axl::io::psx::File stdinFile;
	stdinFile.attach(STDIN_FILENO);
	stdinFile.setBlockingMode(false);

	pumpChildStdio(stdinPipe.m_writeFile, stdoutPipe.m_readFile, stderrPipe.m_readFile);
	return true;
}

bool
testPty(const sl::StringRef& cmdLine) {
	printf("Opening master PTY...\n");

	axl::io::psx::Pty masterPty;
	axl::io::psx::File slavePty;
	axl::io::psx::Pipe execPipe;

	bool result =
		masterPty.open() &&
		masterPty.grant() &&
		masterPty.unlock() &&
		masterPty.setBlockingMode(false) &&
		slavePty.open(masterPty.getSlaveFileName()) &&
		execPipe.create();

	if (!result)
		return false;

	execPipe.m_writeFile.fcntl(F_SETFD, FD_CLOEXEC);
	err::Error error;

	pid_t pid = ::fork();
	switch (pid) {
	case -1:
		err::setLastSystemError();
		return false;

	case 0:
		::setsid();

		slavePty.ioctl(TIOCSCTTY, (int)0);

		::dup2(slavePty, STDIN_FILENO);
		::dup2(slavePty, STDOUT_FILENO);
		::dup2(slavePty, STDERR_FILENO);

		slavePty.close();

		exec(cmdLine);

		error = err::getLastError();
		execPipe.m_writeFile.write(error, error->m_size);
		execPipe.m_writeFile.flush();

		::_exit(-1);
		ASSERT(false);

	default:
		execPipe.m_writeFile.close();
		slavePty.close();

		fd_set rdset;
		FD_ZERO(&rdset);
		FD_SET(execPipe.m_readFile, &rdset);

		char buffer[256];
		size_t size = execPipe.m_readFile.read(buffer, sizeof(buffer));
		if (size == 0 || size == -1)
			break;

		if (((err::ErrorHdr*)buffer)->m_size == size)
			err::setError((err::ErrorHdr*)buffer);
		else
			err::setError("POSIX execvpe failed"); // unlikely fallback

		return false;
	}

	axl::io::psx::File stdinFile;
	stdinFile.attach(STDIN_FILENO);
	stdinFile.setBlockingMode(false);

	struct winsize winSize;
	winSize.ws_col = 90;
	winSize.ws_row = 40;
	masterPty.ioctl(TIOCSWINSZ, &winSize);

	pumpChildStdio(masterPty, masterPty, -1);
	return true;
}

#endif

//..............................................................................

#if (_AXL_SPY)

size_t g_indentSlot;

size_t
incrementIndent(intptr_t delta) {
	size_t indent = sys::getSimpleTlsValue(g_indentSlot);
	sys::setSimpleTlsValue(g_indentSlot, indent + delta);
	return indent;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

spy::HookAction
hookEnter(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
) {
	size_t indent = incrementIndent(1);

	printf(
		"%*sTID %llx: sp: %p +%s\n",
		indent * 2,
		"",
		sys::getCurrentThreadId(),
		(void*)frameBase,
		(char*)callbackParam
	);


	return spy::HookAction_Default;
}

void
hookLeave(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
) {
	size_t indent = incrementIndent(-1) - 1;

	if (!frameBase) { // abandoned
		printf(
			"%*sTID %llx: ~%s\n",
			indent * 2,
			"",
			sys::getCurrentThreadId(),
			(char*)callbackParam
		);

		return;
	}

	spy::RegRetBlock* regRetBlock = (spy::RegRetBlock*)(frameBase + spy::FrameOffset_RegRetBlock);

	printf(
		"%*sTID %llx: sp: %p -%s -> %zd/0x%zx\n",
		indent * 2,
		"",
		sys::getCurrentThreadId(),
		(void*)frameBase,
		(char*)callbackParam,
#if (_AXL_CPU_AMD64)
		regRetBlock->m_rax,
		regRetBlock->m_rax
#elif (_AXL_CPU_X86)
		regRetBlock->m_eax,
		regRetBlock->m_eax
#endif
		);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define _SPY_TEST_TRACE_HOOKING_MODULE   1
// #define _SPY_TEST_TRACE_HOOKING_FUNCTION 1

bool
spyModule(const spy::ModuleIterator& moduleIt) {
	sl::String fileName = moduleIt.getModuleFileName();
	sl::String moduleName = io::getFileName(fileName);

#if (_SPY_TEST_TRACE_HOOKING_MODULE)
	printf("Hooking %s...\n", fileName.sz());
#endif

#if (_AXL_OS_DARWIN)
	sl::SimpleHashTable<size_t, bool> hookSet;
#endif

	spy::ImportWriteProtectionBackup backup;
	bool result = spy::disableImportWriteProtection(moduleIt, &backup);
	if (!result)
		return false;

	static int32_t stringCacheFlag = 0;
	static int32_t hookArenaFlag = 0;

	sl::BoxList<sl::String>* stringCache = sl::getSingleton<sl::BoxList<sl::String> >(&stringCacheFlag);
	spy::HookArena* hookArena = sl::getSingleton<spy::HookArena>(&hookArenaFlag);

	spy::ImportIterator it = spy::enumerateImports(moduleIt);

	char const* currentModuleName = NULL;

	for (; it; it++) {
#if (_AXL_OS_WIN)
		sl::String& functionName = *stringCache->insertTail();
		functionName = moduleName + ":" + it.getModuleName() + ":";

		if (it.getOrdinal() != -1)
			functionName.appendFormat("@%d", it.getOrdinal());
		else
			functionName += it.getSymbolName();

		if (it.getSymbolName().isPrefix("Tls") &&
			(it.getSymbolName() == "TlsAlloc" ||
			it.getSymbolName() == "TlsGetValue" ||
			it.getSymbolName() == "TlsSetValue")) {
			printf("  skipping %s for now...\n", functionName.sz());
			continue;
		}

		void** slot = it.getSlot();
		void* targetFunc = *slot;
		spy::Hook* hook = hookArena->allocate(targetFunc, functionName.p(), hookEnter, hookLeave);
#	if (_SPY_TEST_TRACE_HOOKING_FUNCTION)
		printf("  hooking [%p] %p -> %p: %s...\n", slot, targetFunc, hook, functionName.sz());
#	endif
		*slot = hook;
#elif (_AXL_OS_LINUX)
		sl::String& functionName = *stringCache->insertTail();
		functionName = moduleName + ":" + it.getSymbolName();

		if (it.getSymbolName().isPrefix("pthread_") &&
			(it.getSymbolName() == "pthread_key_create" ||
			 it.getSymbolName() == "pthread_getspecific" ||
			 it.getSymbolName() == "pthread_setspecific")) {
			printf("  skipping %s for now...\n", functionName.sz());
			continue;
		}

		void** slot = it.getSlot();
		void* targetFunc = *slot;
		spy::Hook* hook = hookArena->allocate(targetFunc, functionName.p(), hookEnter, hookLeave);
#	if (_SPY_TEST_TRACE_HOOKING_FUNCTION)
		printf(
			"  hooking [%p] %p -> %p %s...\n",
			slot,
			targetFunc,
			hook,
			functionName.sz()
		);
#	endif
		*slot = hook;
#elif (_AXL_OS_DARWIN)
		sl::String& functionName = *stringCache->insertTail();
		sl::String exportModuleName = io::getFileName(it.getModuleName()); // on darwin, it's a full path
		functionName = moduleName + ":" + exportModuleName + ":" + it.getSymbolName();

		if (it.getSymbolName().isPrefix("_pthread_") &&
			(it.getSymbolName() == "_pthread_key_create" ||
			it.getSymbolName() == "_pthread_getspecific" ||
			it.getSymbolName() == "_pthread_setspecific")) {
			printf("  skipping %s for now...\n", functionName.sz());
			continue;
		}

		size_t slotVmAddr = it.getSlotVmAddr();
		bool isUnique = hookSet.addIfNotExists(slotVmAddr, true);
		if (!isUnique) {
#	if (_SPY_TEST_TRACE_HOOKING_FUNCTION)
			printf("  already hooked [%08llx] %s\n", slotVmAddr, functionName.sz());
#	endif
			continue;
		}

		void** slot = it.getSlot();
		void* targetFunc = *slot;
		spy::Hook* hook = hookArena->allocate(targetFunc, functionName.p(), hookEnter, hookLeave);
#	if (_SPY_TEST_TRACE_HOOKING_FUNCTION)
		printf(
			"  hooking %16s:%16s [%08llx] %p -> %p %s...\n",
			it.getSegmentName().sz(),
			it.getSectionName().sz(),
			slotVmAddr,
			targetFunc,
			hook,
			functionName.sz()
		);
#	endif
		*slot = hook;
#endif
	}

	result = spy::restoreImportWriteProtection(&backup);
	ASSERT(result);
	return true;
}

int
spyGlobalTest() {
	printf("PID: %d\n", sys::getCurrentProcessId());

	g_indentSlot = sys::createSimpleTlsSlot();

	spy::ModuleIterator it = spy::enumerateModules();
	for (; it; it++)
		spyModule(it);

	printf("Hooking done, enabling hooks...\n");
	spy::enableHooks();

	// for testing, don't use printf as it will mess up the output
	sl::String dir = io::getCurrentDir();

	std::string* p = new std::string;
	delete p;

	return 0;
}

namespace spy_param_test {

//..............................................................................

// target function -- all register arguments are used up

int foo(
	int a, double b,
	int c, double d,
	int e, double f,  // on microsoft x64, e passed on stack
	int g, double h,
	int i, double j,
	int k, double l,
	int m, double n,  // on systemv amd64, m passed on stack
	int o, double p,
	int q, double r   // on systemv amd64, r passed on stack
) {
	printf(
		"foo("
		"%d, %f, %d, %f, %d, %f, "
		"%d, %f, %d, %f, %d, %f, "
		"%d, %f, %d, %f, %d, %f)\n",
		a, b, c, d, e, f,
		g, h, i, j, k, l,
		m, n, o, p, q, r
	);

	return 123;
}

//..............................................................................

spy::HookAction
fooHookEnter(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
) {
	printf(
		"+foo(func: %p, param: %p):\n",
		targetFunc,
		callbackParam
	);

#if (_AXL_CPU_AMD64)
#	if (_AXL_CPP_MSC)
	spy::RegArgBlock* regArgBlock = (spy::RegArgBlock*)(frameBase + spy::FrameOffset_RegArgBlock);

	int a    = (int)regArgBlock->m_rcx;
	double b = regArgBlock->m_xmm1[0];
	int c    = (int)regArgBlock->m_r8;
	double d = regArgBlock->m_xmm3[0];

	spy::VaList va;
	spy::vaStart(va, frameBase);

	int e = spy::vaArg<int>(va);
	double f = spy::vaArg<double>(va);
	int g = spy::vaArg<int>(va);
	double h = spy::vaArg<double>(va);
	int i = spy::vaArg<int>(va);
	double j = spy::vaArg<double>(va);
	int k = spy::vaArg<int>(va);
	double l = spy::vaArg<double>(va);
	int m = spy::vaArg<int>(va);
	double n = spy::vaArg<double>(va);
	int o = spy::vaArg<int>(va);
	double p = spy::vaArg<double>(va);
	int q = spy::vaArg<int>(va);
	double r = spy::vaArg<double>(va);

#	elif (_AXL_CPP_GCC)
	spy::RegArgBlock* regArgBlock = (spy::RegArgBlock*)(frameBase + spy::FrameOffset_RegArgBlock);

	int a = (int)regArgBlock->m_rdi;
	int c = (int)regArgBlock->m_rsi;
	int e = (int)regArgBlock->m_rdx;
	int g = (int)regArgBlock->m_rcx;
	int i = (int)regArgBlock->m_r8;
	int k = (int)regArgBlock->m_r9;
	double b = regArgBlock->m_xmm0[0];
	double d = regArgBlock->m_xmm1[0];
	double f = regArgBlock->m_xmm2[0];
	double h = regArgBlock->m_xmm3[0];
	double j = regArgBlock->m_xmm4[0];
	double l = regArgBlock->m_xmm5[0];
	double n = regArgBlock->m_xmm6[0];
	double p = regArgBlock->m_xmm7[0];

	spy::VaList va;
	spy::vaStart(va, frameBase);

	int m = spy::vaArg<int>(va);
	int o = spy::vaArg<int>(va);
	int q = spy::vaArg<int>(va);
	double r = spy::vaArg<double>(va);

#	endif
#elif (_AXL_CPU_X86)
	spy::VaList va;
	spy::vaStart(va, frameBase);

	int a = spy::vaArg<int>(va);
	double b = spy::vaArg<double>(va);
	int c = spy::vaArg<int>(va);
	double d = spy::vaArg<double>(va);
	int e = spy::vaArg<int>(va);
	double f = spy::vaArg<double>(va);
	int g = spy::vaArg<int>(va);
	double h = spy::vaArg<double>(va);
	int i = spy::vaArg<int>(va);
	double j = spy::vaArg<double>(va);
	int k = spy::vaArg<int>(va);
	double l = spy::vaArg<double>(va);
	int m = spy::vaArg<int>(va);
	double n = spy::vaArg<double>(va);
	int o = spy::vaArg<int>(va);
	double p = spy::vaArg<double>(va);
	int q = spy::vaArg<int>(va);
	double r = spy::vaArg<double>(va);

#endif

	printf(
		"  ("
		"%d, %f, %d, %f, %d, %f, "
		"%d, %f, %d, %f, %d, %f, "
		"%d, %f, %d, %f, %d, %f)\n",
		a, b, c, d, e, f,
		g, h, i, j, k, l,
		m, n, o, p, q, r
	);

	return spy::HookAction_Default;
}

void
fooHookLeave(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
) {
	if (!frameBase) { // abandoned
		printf("~foo(func: %p, param: %p)\n", targetFunc, callbackParam);
		return;
	}

	spy::RegRetBlock* regRetBlock = (spy::RegRetBlock*)(frameBase + spy::FrameOffset_RegRetBlock);

#if (_AXL_CPU_AMD64)
	int retval = (int)regRetBlock->m_rax;
#elif (_AXL_CPU_X86)
	int retval = regRetBlock->m_eax;
#endif

	printf(
		"-foo(func: %p, param: %p) -> %d\n",
		targetFunc,
		callbackParam,
		retval
	);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

} // namespace spy_param_test

int
spyParamTest() {
	typedef int FooFunc(
		int, double,
		int, double,
		int, double,
		int, double,
		int, double,
		int, double,
		int, double,
		int, double,
		int, double
	);

	spy::HookArena arena;

	spy::Hook* fooHook = arena.allocate(
		(void*)spy_param_test::foo,
		(void*)0xabcdef,
		spy_param_test::fooHookEnter,
		spy_param_test::fooHookLeave
	);

	((FooFunc*)fooHook)(
		1, 10.1,
		2, 20.2,
		3, 30.3,
		4, 40.4,
		5, 50.5,
		6, 60.6,
		7, 70.7,
		8, 80.8,
		9, 90.9
	);

	return 0;
}

//..............................................................................

namespace spy_stdcall_test {

int
AXL_STDCALL
foo(int a) { // one argument is all it takes for ret <n> to mess up the stack
	printf("foo(%d)\n", a);
	return 10;
}

spy::HookAction
fooHookEnter(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
) {
	printf(
		"fooHookEnter(func: %p, param: %p, frame: %p)\n",
		targetFunc,
		callbackParam,
		(void*)frameBase
	);

#if (_AXL_CPU_X86)
	spy::VaList va;
	spy::vaStart(va, frameBase);
	int a = spy::vaArg<int>(va);
	printf("  (%d)\n", a);
#endif

	return spy::HookAction_Default;
}

void
fooHookLeave(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
) {
	spy::RegRetBlock* regRetBlock = (spy::RegRetBlock*)(frameBase + spy::FrameOffset_RegRetBlock);

	printf(
		"fooHookLeave(func: %p, param: %p, frame: %p, retval: %zd/0x%zx)\n",
		targetFunc,
		callbackParam,
		(void*)frameBase,
#if (_AXL_CPU_AMD64)
		regRetBlock->m_rax,
		regRetBlock->m_rax
#elif (_AXL_CPU_X86)
		regRetBlock->m_eax,
		regRetBlock->m_eax
#endif
		);
}

} // namespace spy_stdcall_test

int
spyStdcallTest() {
	typedef
	int
	AXL_STDCALL
	FooFunc(int);

	spy::HookArena arena;

	spy::Hook* fooHook = arena.allocate(
		(void*)spy_stdcall_test::foo,
		(void*)0xabcdef,
		spy_stdcall_test::fooHookEnter,
		spy_stdcall_test::fooHookLeave
	);

	spy::enableHooks();
	((FooFunc*)fooHook)(10);
	return 0;
}

#endif // _AXL_SPY

//..............................................................................

#ifdef _AXL_CRY

int
receiptTest() {
	OpenSSL_add_all_algorithms();
	cry::registerCryptoErrorProviders();

	bool result;

#if (_AXL_OS_WIN)
	const char* filePath = "C:/Projects/playground/receipt/iap-receipt";
#elif (_AXL_OS_DARWIN)
	const char* filePath = "/Users/vladimir/Projects/playground/receipt/iap-receipt";
#elif (_AXL_OS_LINUX)
	const char* filePath = "/home/vladimir/Projects/playground/receipt/xcode-receipt";
#endif

	io::SimpleMappedFile file;
	result = file.open(filePath, io::FileFlag_ReadOnly);
	if (!result) {
		printf("file error: %s\n", err::getLastErrorDescription().sz());
		return -1;
	}

	printf("file size: %d\n", file.getFileSize());

#if (_AXL_OS_DARWIN)
	uint_t flags = cry::VerifyAppStoreReceiptFlag_SkipHashCheck | cry::VerifyAppStoreReceiptFlag_SkipSignatureCheck;
//	uint_t flags = 0;
#else
	uint_t flags = cry::VerifyAppStoreReceiptFlag_SkipHashCheck;
#endif

	cry::AppStoreReceipt receipt;
	result = cry::verifyAppStoreReceipt(&receipt, file.p(), file.getMappingSize(), flags);
	if (!result) {
		printf("verify error: %s\n", err::getLastErrorDescription().sz());
		return -1;
	}

	printf(
		"RECEIPT:\n"
		"  bundle ID:               %s\n"
		"  app version:             %s\n"
		"  original app version:    %s\n"
		"  receipt creation date:   %s%s\n"
		"  receipt expiration date: %s%s\n"
		"  ------------------------\n"
		"  SHA1 hash: %s\n"
		"  opaque:    %s\n",
		receipt.m_bundleId.sz(),
		receipt.m_appVersion.sz(),
		receipt.m_originalAppVersion.sz(),
		receipt.m_receiptCreationDateString.sz(),
		receipt.m_receiptCreationTimestamp ?
			sys::Time(receipt.m_receiptCreationTimestamp).format(" (%Y-%M-%D %h:%m:%s)").sz() :
			"",
		receipt.m_receiptExpirationDateString.sz(),
		receipt.m_receiptExpirationTimestamp ?
			sys::Time(receipt.m_receiptExpirationTimestamp).format(" (%Y-%M-%D %h:%m:%s)").sz() :
			"",
		enc::HexEncoding::encode(receipt.m_sha1Hash, receipt.m_sha1Hash.getCount()).sz(),
		enc::HexEncoding::encode(receipt.m_opaque, receipt.m_opaque.getCount()).sz()
	);

	sl::Iterator<cry::AppStoreIap> it = receipt.m_iapList.getHead();
	for (; it; it++) {
		cry::AppStoreIap* iap = *it;

		printf(
			"IAP:\n"
			"  quantity:                %d\n"
			"  product ID:              %s\n"
			"  transaction ID:          %s\n"
			"  original transaction ID: %s\n"
  			"  web order line ID        %d\n"
			"  purchase date:           %s%s\n"
			"  original purchase date:  %s%s\n"
			"  subs expiration date:    %s%s\n"
			"  cancellation date:       %s%s\n",
			iap->m_quantity,
			iap->m_productId.sz(),
			iap->m_transactionId.sz(),
			iap->m_originalTransactionId.sz(),
			iap->m_webOrderLineItemId,
			iap->m_purchaseDateString.sz(),
			iap->m_purchaseTimestamp ?
				sys::Time(iap->m_purchaseTimestamp).format(" (%Y-%M-%D %h:%m:%s)").sz() :
				"",
			iap->m_originalPurchaseDateString.sz(),
			iap->m_originalPurchaseTimestamp ?
				sys::Time(iap->m_originalPurchaseTimestamp).format(" (%Y-%M-%D %h:%m:%s)").sz() :
				"",
			iap->m_subscriptionExpirationDateString.sz(),
			iap->m_subscriptionExpirationTimestamp ?
				sys::Time(iap->m_subscriptionExpirationTimestamp).format(" (%Y-%M-%D %h:%m:%s)").sz() :
				"",
			iap->m_cancellationDateString.sz(),
			iap->m_cancellationTimestamp ?
				sys::Time(iap->m_cancellationTimestamp).format(" (%Y-%M-%D %h:%m:%s)").sz() :
				""
			);
	}

	return 0;
}

#endif // _AXL_CRY

//..............................................................................

#if (_AXL_OS_WIN)

int
authenticodeTest(const wchar_t* fileName = NULL) {
	if (!fileName) {
		static const wchar_t* fileNameArray[] = {
			L"C:/Windows/system32/kernel32.dll",
			L"C:/Program Files/Tibbo/TDST/tdsman.exe",
			L"C:/Program Files/Tibbo/TDST/tvspman.exe",
			L"C:/Program Files/Tibbo/Ninja 3/bin/ioninja.exe",
		};

		for (size_t i = 0; i < countof(fileNameArray); i++)
			authenticodeTest(fileNameArray[i]);

		return 0;
	}

	printf("%S...\n", fileName);

	sl::Array<char> serialNumber;
	sl::String_w programName;
	sl::String_w subjectName;
	sl::String_w issuerName;
	uint64_t timestamp;

	bool result = sys::win::verifyAuthenticodeSignature(
		fileName,
		&programName,
		&subjectName,
		&issuerName,
		&serialNumber,
		&timestamp
	);

	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return -1;
	}

	printf(
		"program:   %S\n"
		"subject:   %S\n"
		"issuer:    %S\n"
		"serial:    %s\n"
		"timestamp: %s\n",
		programName.sz(),
		subjectName.sz(),
		issuerName.sz(),
		enc::HexEncoding::encode(serialNumber, serialNumber.getCount()).sz(),
		timestamp ? sys::Time(timestamp).format("%Y-%M-%d %h:%m:%s").sz() : "?"
	);

	return 0;
}

#endif

#if (_AXL_OS_DARWIN)

void
bookmarkTest() {
	bool result;

	bool isSandbox = ::getenv("APP_SANDBOX_CONTAINER_ID") != NULL;
	printf("is sandbox: %d\n", isSandbox);

	printf("creating URL...\n");

	cf::Url url;
	url.create("/Users/vladimir/abc.njlg", kCFURLPOSIXPathStyle, false);
	printf("url: %s\n", url.getString().sz());

	printf("start accessing security-scoped resource...\n");

	result = url.startAccessingSecurityScopedResource();
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("creating bookmark...\n");

	cf::Data bookmark;
	result = url.createBookmark(&bookmark, kCFURLBookmarkCreationMinimalBookmarkMask | kCFURLBookmarkCreationWithSecurityScope);
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("bookmark: %s\n", enc::HexEncoding::encode(bookmark.getBytePtr(), bookmark.getLength()).sz());

	printf("resolving bookmark...\n");

	cf::Url url2;
	result = url2.resolveBookmark(bookmark, kCFURLBookmarkResolutionWithoutUIMask | kCFURLBookmarkResolutionWithSecurityScope);
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("url2: %s\n", url2.getString().sz());
}

#endif

//..............................................................................

void
testHomeDir() {
	printf("Home dir: %s\n", io::getHomeDir().sz());
}

//..............................................................................

namespace trace_test {

int
traceArg() {
	return printf("traceArg\n");
}

void
testTrace() {
	TRACE("testTrace(%d)\n", traceArg());
}

} // namespace trace_test

//..............................................................................

#ifdef _AXL_INI

class MyParser: public ini::Parser<MyParser> {
protected:
	enum SectionKind {
		SectionKind_Undefined,
		SectionKind_Session,
		SectionKind_Scripts,
	};

	enum KeyKind {
		KeyKind_Undefined,
		KeyKind_Name,
		KeyKind_Description,
		KeyKind_Guid,
		KeyKind_Icon,
		KeyKind_LogRepresenterClass,
		KeyKind_SessionClass,
	};

	AXL_SL_BEGIN_STRING_HASH_TABLE(SectionNameMap, SectionKind)
		AXL_SL_HASH_TABLE_ENTRY("session", SectionKind_Session)
		AXL_SL_HASH_TABLE_ENTRY("scripts", SectionKind_Scripts)
	AXL_SL_END_HASH_TABLE()

	AXL_SL_BEGIN_STRING_HASH_TABLE(KeyNameMap, KeyKind)
		AXL_SL_HASH_TABLE_ENTRY("name", KeyKind_Name)
		AXL_SL_HASH_TABLE_ENTRY("description", KeyKind_Description)
		AXL_SL_HASH_TABLE_ENTRY("guid", KeyKind_Guid)
		AXL_SL_HASH_TABLE_ENTRY("icon", KeyKind_Icon)
		AXL_SL_HASH_TABLE_ENTRY("session-class", KeyKind_SessionClass)
		AXL_SL_HASH_TABLE_ENTRY("log-representer-class", KeyKind_LogRepresenterClass)
	AXL_SL_END_HASH_TABLE()

public:
	bool
	onSection(const char* name) {
		SectionNameMap::Iterator it = SectionNameMap::find(name);
		SectionKind section = it ? it->m_value : SectionKind_Undefined;
		printf("OnSection '%s'\n", name);
		return true;
	}

	bool
	onKeyValue(
		const char* name,
		const char* value
	) {
		KeyNameMap::Iterator it = KeyNameMap::find(name);
		KeyKind key = it ? it->m_value : KeyKind_Undefined;
		printf("OnKeyValue '%s' = '%s'\n", name, value);
		return true;
	}
};

void
iniTest() {
	bool result;

	const char* filePath = "c:/projects/repos/ioninja/ioninja/conf/ias.conf";

	io::MappedFile file;
	result = file.open(filePath, io::FileFlag_ReadOnly);
	if (!result) {
		printf("error opening file: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	const char* p = (const char*) file.view();
	uint64_t size = file.getSize();

	MyParser parser;
	result = parser.parse(filePath, sl::StringRef(p, (size_t)size));
	if (!result) {
		printf("error parsing file: %s\n", err::getLastErrorDescription().sz());
		return;
	}
}

#endif

//..............................................................................

void
mappedFileTest() {
	io::SimpleMappedFile file;
	bool result = file.open("/home/vladimir/test.jnc");
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	void* p = file.p();
	size_t size = file.getMappingSize();

	file.close();
}

//..............................................................................

#if (_AXL_SYS_CA)

void
codeAuthenticatorTest() {
	sys::CodeAuthenticator authenticator;

#if (_AXL_OS_WIN)
#	define JNC_LIB_DIR "c:/Projects/repos/ioninja/jancy/build/make-amd64/bin/Debug"

	authenticator.setup("Tibbo Technology Inc.");
#elif (_AXL_OS_LINUX)
#	define JNC_LIB_DIR "/home/vladimir/Projects/repos/ioninja/jancy/build/make-amd64/lib/Debug"

	const char publicKeyPem[] =
		"-----BEGIN PUBLIC KEY-----\n"
		"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAqlJw5tH4SYipoxczk3S9\n"
		"pIYV/H5old8TGzQ0Mq2AFLhW0aBewsuQOJUxLdzu4ym+KNpV2+6tp9qZFpa/zu7S\n"
		"qzztgvnWsL4EsdqXGLfTDa+rM9GlhsrDchF4a13V10fKBtBlCn1OSBLYY1BWkrjK\n"
		"66rv4J/8uGuhZi9ufZh3jML0aFUklPIiPly6ndRMnxKFqqwqSy6ymDn3kAdHZfMp\n"
		"c21hG26vjG7guYflmd4krvF8++yBUGJV9ToT5BKF4pzUGh9Pt7lt+MAUdJnTT9ZF\n"
		"6SyVnDkMM8s5DK8lblKrxur4LRsX0CJ1557T6fFWgXsEnyqFWUm8fVJKaRoozOjl\n"
		"zQIDAQAB\n"
		 "-----END PUBLIC KEY-----\n";

	authenticator.setup(".njsig", publicKeyPem);
#elif (_AXL_OS_DARWIN)
#	define JNC_LIB_DIR "/Users/vladimir/Projects/ioninja/jancy/build/make-amd64/lib/Debug"

	sec::registerErrorProvider();
	authenticator.setup("MHV447DZEV");
#endif

	const char* fileNameTable[] = {
		JNC_LIB_DIR "/io_base-amd64.bin",
		JNC_LIB_DIR "/io_pcap-amd64.bin",
		JNC_LIB_DIR "/io_ssh-amd64.bin",
		JNC_LIB_DIR "/io_ssl-amd64.bin",
		JNC_LIB_DIR "/io_usb-amd64.bin",
	};

	for (size_t i = 0; i < countof(fileNameTable); i++) {
		const char* fileName = fileNameTable[i];
		printf("Verifying %s...\n", fileName);

		bool result = authenticator.verifyFile(fileName);
		if (!result)
			fprintf(stderr, "Error: %s...\n", err::getLastErrorDescription().sz());
	}

	printf("Done.\n");
}

#endif

//..............................................................................

void
testListSort() {
	sl::BoxList<int> list;

	for (size_t i = 0; i < 10; i++)
		list.insertTail(rand());

	sl::BoxIterator<int> it = list.getHead();
	for (; it; it++)
		printf("%d ", *it);

	printf("\nSorting...\n");
	list.sort();

	it = list.getHead();
	for (; it; it++)
		printf("%d ", *it);

	printf("\n");
}

namespace utf {


//..............................................................................

// Hoehrmann

namespace utf8_0 {

enum {
	UTF8_ACCEPT = 0,
	UTF8_REJECT = 1,
};

static const uint8_t utf8d[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
  8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
  0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
  0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
  0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
  1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
  1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
  1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
};

uint32_t inline
decode(uint32_t* state, uint32_t* codep, uint32_t byte) {
  uint32_t type = utf8d[byte];

  *codep = (*state != UTF8_ACCEPT) ?
    (byte & 0x3fu) | (*codep << 6) :
    (0xff >> type) & (byte);

  *state = utf8d[256 + *state*16 + type];
  return *state;
}

} // namespace utf8_0

// Copyright (c) 2008-2010 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

enum {
	UTF8_ACCEPT = 0,
	UTF8_REJECT = 12,
};

static const uint8_t utf8d[] = {
  // The first part of the table maps bytes to character classes that
  // to reduce the size of the transition table and create bitmasks.
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
   8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

  // The second part is a transition table that maps a combination
  // of a state of the automaton and a character class to a state.
   0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
  12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
  12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
  12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
  12,36,12,12,12,12,12,12,12,12,12,12,
};

uint32_t inline
decode(uint32_t* state, uint32_t* codep, uint32_t byte) {
  uint32_t type = utf8d[byte];

  *codep = (*state != UTF8_ACCEPT) ?
    (byte & 0x3fu) | (*codep << 6) :
    (0xff >> type) & (byte);

  *state = utf8d[256 + *state + type];
  return *state;
}

//..............................................................................

const uchar_t g_data[] = {
	'a',
	0xf1, 0x81, 0x82,
	'b',
	0xe1, 0x83,
	'c',
	0xc1,
	'd',
	0xf1, 0x84, 0x85,
	0xe1, 0x86,
	0xc1,
	0xe1, 0x87,
	0xf1, 0x88, 0x89,
	'x',
	0x8a, 0x8b, 0x8c,
	't', 'a', 'k', 'i', 'e',
	0xf1, 0x8d, 0x8e,
	0xd0, 0xb0, 0xd0, 0xb1, 0xd0, 0xb2, 0xd0, 0xb3, 0xd0, 0xb4,
	'd', 'e', 'l', 'a', '.',
};

//..............................................................................

template<typename C>
class Emitter {
public:
	bool
	canEmit() const {
		return true;
	}

	void
	emitCp(
		const C* p,
		utf32_t cp
	) {
		printf("%x\n", cp);
	}

	void
	emitCu(
		const C* p,
		utf32_t cp
	) {
		printf("%x *\n", cp);
	}

	void
	emitCpAfterCu(
		const C* p,
		utf32_t cp
	) {
		emitCp(p, cp);
	}
};

typedef Emitter<utf8_t> Emitter_utf8;
typedef Emitter<utf16_t> Emitter_utf16;

void
testUtf8() {
	{
		Emitter_utf8 emitter;

		printf("forward...\n");
		enc::Utf8::Decoder::decode(emitter, (utf8_t*)g_data, (utf8_t*)g_data + sizeof(g_data));

		printf("backward...\n");
		enc::Utf8::ReverseDecoder::decode(emitter, (utf8_t*)g_data + sizeof(g_data) - 1, (utf8_t*)g_data - 1);

		return;
	}

	uint32_t prevState = UTF8_ACCEPT;
	uint32_t state = UTF8_ACCEPT;
	uint32_t cp;

	const uchar_t* data = (uchar_t*)g_data;
	size_t length = sizeof(g_data);

	printf("Hoehrmann:\n");

	for (size_t i = 0; i < length; i++) {
		uchar_t c = data[i];
		if (!decode(&state, &cp, c))
			printf("cp 0x%02x '%c'\n", cp, cp);
		else if (state == UTF8_REJECT) {
			printf("broken cp\n");
			state = UTF8_ACCEPT;
			if (prevState != UTF8_ACCEPT) {
				if (!decode(&state, &cp, c))
					printf("cp 0x%02x '%c'\n", cp, cp);
				else if (state == UTF8_REJECT)
					printf("broken cp\n");
			}
		}

		prevState = state;
	}

	printf("\nAXL UTF-8 DFA:\n");

	enc::Utf8Dfa dfa;

	for (size_t i = 0, j = i; i < length; i++) {
		enc::Utf8Dfa next = dfa.decode(data[i]);
		if (next.isError(state))
			if (next.getState() == enc::Utf8Dfa::State_Error) // standalone error
				do {
					printf("@%04x:%04x: broken cp\n", j, j);
				} while (++j <= i);
			else // combined error (minus last byte)
				do {
					printf("@%04x:%04x: broken cp\n", j, j);
				} while (++j < i);

		if (next.isReady()) {
			printf("@%04x:%04x: cp 0x%02x '%c'\n", j, i, next.getCodePoint(), next.getCodePoint());
			j = i + 1;
		}

		dfa = next;
	}

	printf("\nAXL UTF-8 rDFA:\n");

	enc::Utf8ReverseDfa dfa2;

	for (intptr_t i = length - 1, j = i; i >= 0; i--) {
		enc::Utf8ReverseDfa next = dfa2.decode(data[i]);
		if (next.isError())
			if (next.getState() == enc::Utf8ReverseDfa::State_Error) // standalone error
				do {
					printf("@%04x:%04x: broken cp\n", j, j);
				} while (--j >= i);
			else { // combined error
					printf("@%04x:%04x: combined error\n", j, j);
				/* size_t errorCount = enc::Utf8ReverseDfa::getCombinedErrorCount(state);
				intptr_t k = j - errorCount;
				do {
					printf("@%04x:%04x: broken cp\n", j, j);
				} while (--j > k); */
			}

		if (next.isReady()) {
			printf("@%04x:%04x: cp 0x%02x '%c'\n", i, j, next.getCodePoint(), next.getCodePoint());
			j = i - 1;
		}

		dfa2 = next;
	}

	printf("sequence end: @%04x\n", length);
	printf("\ndone!\n");
}

//..............................................................................

const uint16_t g_data16[] = {
	'a',
	0xd83d,
	0xde00,
	'b',
	0xd801,
	0xdc00,
	'c',
	0xd802,
	0xd803, // missing trail surrogate
	'd',
	0xd804, // unpaired trail surrogate
	'x', 'y', 'z', '.',
};

void
testUtf16() {
	sl::String_utf16 data((char*)g_data, sizeof(g_data));
	size_t length = data.getLength();

	printf("\nUTF-16 DFA:\n");

	printf("UTF16 forward...\n");

	Emitter_utf16 emitter;

	enc::Utf16::Decoder::decode(emitter, (utf16_t*)g_data16, (utf16_t*)g_data16 + countof(g_data16));

	printf("rUTF16 backward...\n");
	enc::Utf16::ReverseDecoder::decode(emitter, (utf16_t*)g_data16 + countof(g_data16) - 1, (utf16_t*)g_data16 - 1);

	Emitter<char> emitter2;

	printf("UTF16s forward...\n");
	enc::Utf16s::Decoder::decode(emitter2, (char*)g_data16, (char*)g_data16 + sizeof(g_data16));

	printf("rUTF16s backward...\n");
	enc::Utf16s::ReverseDecoder::decode(emitter2, (char*)g_data16 + sizeof(g_data16) - 1, (char*)g_data16 - 1);

	return;

	enc::Utf16Dfa dfa;

	for (size_t i = 0, j = i; i < length; i++) {
		enc::Utf16Dfa next = dfa.decode(data[i]);
		if (next.isError()) {
			printf("@%04x:%04x: broken cp\n", j, j);
			j--;
		}

		if (next.isReady()) {
			printf("@%04x:%04x: cp 0x%02x '%c'\n", j, i, next.getCodePoint(), next.getCodePoint());
			j = i + 1;
		}

		dfa = next;
	}

	printf("\nUTF-16 rDFA:\n");

	enc::Utf16ReverseDfa dfa2;

	for (intptr_t i = length - 1, j = i; i >= 0; i--) {
		enc::Utf16ReverseDfa next = dfa2.decode(data[i]);
		if (next.isError()) {
			printf("@%04x:%04x: broken cp\n", j, j);
			j--;
		}

		if (next.isReady()) {
			printf("@%04x:%04x: cp 0x%02x '%c'\n", i, j, next.getCodePoint(), next.getCodePoint());
			j = i - 1;
		}

		dfa2 = next;
	}

	char* data2 = (char*)data.cp();
	size_t length2 = length * 2;

	printf("\nUTF-16-S DFA:\n");

	enc::Utf16sDfa dfa3;

	for (size_t i = 0, j = i; i < length2; i++) {
		enc::Utf16sDfa next = dfa3.decode(data2[i]);
		if (next.isError()) {
			printf("@%04x:%04x: broken cp\n", j, j);
			j--;
		}

		if (next.isReady()) {
			printf("@%04x:%04x: cp 0x%02x '%c'\n", j, i, next.getCodePoint(), next.getCodePoint());
			j = i + 1;
		}

		dfa3 = next;
	}

	printf("\nUTF-16-S rDFA:\n");

	enc::Utf16sReverseDfa dfa4;

	for (intptr_t i = length2 - 1, j = i; i >= 0; i--) {
		enc::Utf16sReverseDfa next = dfa4.decode(data2[i]);
		if (next.isError()) {
			printf("@%04x:%04x: broken cp\n", j, j);
			j--;
		}

		if (next.isReady()) {
			printf("@%04x:%04x: cp 0x%02x '%c'\n", i, j, next.getCodePoint(), next.getCodePoint());
			j = i - 1;
		}

		dfa4 = next;
	}

	printf("sequence end: @%04x\n", length2);
	printf("\ndone!\n");

	enum {
//		LoopCount = 500,
//		LoopCount = 5000,
		LoopCount = 5000000,
	};

	printf("original...");

	dfa.reset();

	size_t errorCount = 0;
	size_t cpCount = 0;

	uint64_t time = sys::getPreciseTimestamp();

	for (size_t i = 0; i < LoopCount; i++)
		for (size_t j = 0; j < length; j++) {
			enc::Utf16Dfa next = dfa.decode(data[j]);
			if (next.isError())
				errorCount++;

			if (next.isReady())
				cpCount++;

			dfa = next;
		}

	time = sys::getPreciseTimestamp() - time;

	printf("done (%d/%d); %.3f sec\n", cpCount, errorCount, (double)time / 10000000);

	printf("reverse....");

	dfa2.reset();

	errorCount = 0;
	cpCount = 0;

	time = sys::getPreciseTimestamp();

	for (size_t i = 0; i < LoopCount; i++)
		for (intptr_t j = length - 1; j >= 0; j--) {
			enc::Utf16ReverseDfa next = dfa2.decode(data[j]);
			if (next.isError())
				errorCount++;

			if (next.isReady())
				cpCount++;

			dfa2 = next;
		}

	time = sys::getPreciseTimestamp() - time;

	printf("done (%d/%d); %.3f sec\n", cpCount, errorCount, (double)time / 10000000);

	printf("new........");

	dfa3.reset();

	errorCount = 0;
	cpCount = 0;

	time = sys::getPreciseTimestamp();

	for (size_t i = 0; i < LoopCount; i++)
		for (size_t j = 0; j < length2; j++) {
			dfa3 = dfa3.decode(data2[j]);
			if (dfa3.isError())
				errorCount++;

			if (dfa3.isReady())
				cpCount++;
		}

	time = sys::getPreciseTimestamp() - time;

	printf("done (%d/%d), %.3f sec\n", cpCount, errorCount, (double)time / 10000000);

	printf("new........");

	dfa4.reset();

	errorCount = 0;
	cpCount = 0;

	time = sys::getPreciseTimestamp();

	for (size_t i = 0; i < LoopCount; i++)
		for (intptr_t j = length2 - 1; j >= 0; j--) {
			dfa4 = dfa4.decode(data2[j]);
			if (dfa4.isError())
				errorCount++;

			if (dfa4.isReady())
				cpCount++;
		}

	time = sys::getPreciseTimestamp() - time;

	printf("done (%d/%d), %.3f sec\n", cpCount, errorCount, (double)time / 10000000);
}

void
encodeCodePoint(
	utf8_t* p,
	utf32_t x
) {
	if (x < 0x80) {
		p[0] = (uint8_t)x;                             // 0xxxxxxx
	} else if (x < 0x10000) {
		if (x < 0x800) {
			p[0] = (uint8_t)(x >> 6)          | 0xc0;  // 110xxxxx 10xxxxxx
			p[1] = (uint8_t)(x & 0x3f)        | 0x80;
		} else {
			p[0] = (uint8_t)(x >> 12)         | 0xe0;  // 1110xxxx 10xxxxxx 10xxxxxx
			p[1] = (uint8_t)((x >> 6) & 0x3f) | 0x80;
			p[2] = (uint8_t)(x & 0x3f)        | 0x80;
		}
	} else if (x < 0x200000) {
		p[0] = (uint8_t)(x >> 18)             | 0xf0; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		p[1] = (uint8_t)((x >> 12) & 0x3f)    | 0x80;
		p[2] = (uint8_t)((x >> 6) & 0x3f)     | 0x80;
		p[3] = (uint8_t)(x & 0x3f)            | 0x80;
	} else {
		p[0] = -1;
	}
}

void
encodeCodePoint_old(
	utf8_t* p,
	utf32_t x
) {
	if (x < 0x80) { // 0xxxxxxx
		p[0] = (uint8_t)x;
	} else if (x < 0x800) { // 110xxxxx 10xxxxxx
		p[0] = (uint8_t)(x >> 6)           | 0xc0;
		p[1] = (uint8_t)(x & 0x3f)         | 0x80;
	} else if (x < 0x10000) { // 1110xxxx 10xxxxxx 10xxxxxx
		p[0] = (uint8_t)(x >> 12)          | 0xe0;
		p[1] = (uint8_t)((x >> 6) & 0x3f)  | 0x80;
		p[2] = (uint8_t)(x & 0x3f)         | 0x80;
	} else if (x < 0x200000) { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		p[0] = (uint8_t)(x >> 18)          | 0xf0;
		p[1] = (uint8_t)((x >> 12) & 0x3f) | 0x80;
		p[2] = (uint8_t)((x >> 6) & 0x3f)  | 0x80;
		p[3] = (uint8_t)(x & 0x3f)         | 0x80;
	} else {
		p[0] = -1;
	}
}

volatile uint32_t g_v;

void
testUtf8Encode() {
	union {
		utf8_t m_c[4];
		uint32_t m_x;
	} utf8[2];

	printf("verifying...\n");

	for (utf32_t c = 0; c < 0x200000; c++) {
		utf8[0].m_x = 0;
		utf8[1].m_x = 0;

		encodeCodePoint(utf8[0].m_c, c);
		encodeCodePoint_old(utf8[1].m_c, c);
		ASSERT(utf8[0].m_x == utf8[1].m_x);
		g_v = utf8[0].m_x;
		g_v = utf8[1].m_x;
	}

	printf("done\n");

	enum {
//		LoopCount = 500,
//		MaxChar   = 0x200000,

//		LoopCount = 5000,
//		MaxChar   = 0x10000,

		LoopCount = 500000,
		MaxChar   = 0x800,
	};

	printf("original...");

	uint64_t time = sys::getPreciseTimestamp();

	for (size_t i = 0; i < LoopCount; i++)
		for (utf32_t c = 0; c < MaxChar; c++) {
			utf8[0].m_x = 0;
			encodeCodePoint_old(utf8[0].m_c, c);
			g_v = utf8[0].m_x;
		}

	time = sys::getPreciseTimestamp() - time;

	printf("done, %.3f sec\n", (double)time / 10000000);

	printf("new........");

	time = sys::getPreciseTimestamp();

	for (size_t i = 0; i < LoopCount; i++)
		for (utf32_t c = 0; c < MaxChar; c++) {
			utf8[0].m_x = 0;
			encodeCodePoint(utf8[0].m_c, c);
			g_v = utf8[0].m_x;
		}

	time = sys::getPreciseTimestamp() - time;

	printf("done, %.3f sec\n", (double)time / 10000000);
}

} // namespace utf

//..............................................................................

#if (_AXL_IO_USBMON)

enum {
	VidSerialTap = 0x16d0,
	PidSerialTap = 0x0e26,
};

bool
testUsbMon() {
#if (_AXL_OS_LINUX)
	io::registerUsbErrorProvider();
	io::getUsbDefaultContext()->createDefault();
#endif

	const io::UsbMonDeviceDesc* serialTap = NULL;
	sl::List<io::UsbMonDeviceDesc> deviceList;
	io::enumerateUsbMonDevices(&deviceList, io::UsbDeviceStringId_All);
	sl::ConstIterator<io::UsbMonDeviceDesc> it = deviceList.getHead();
	for (size_t i = 0; it; it++, i++) {
		printf("device #%d\n", i);
		printf("  capture:        %s\n", it->m_captureDeviceName.sz());
		printf("  address:        %d\n", it->m_address);
		printf("  VID:            %04x\n", it->m_vendorId);
		printf("  PID:            %04x\n", it->m_productId);
		printf("  class:          %d\n", it->m_class);
		printf("  subclass:       %d\n", it->m_subClass);
		printf("  speed:          %s\n", io::getUsbSpeedString((libusb_speed)it->m_speed));

		if (!it->m_description.isEmpty())
			printf("  description:    %s\n", it->m_description.sz());

		if (!it->m_manufacturer.isEmpty())
			printf("  manufacturer:   %s\n", it->m_manufacturer.sz());

		if (!it->m_driver.isEmpty())
			printf("  driver:         %s\n", it->m_driver.sz());

		if (!it->m_manufacturerDescriptor.isEmpty())
			printf("  manufacturer*:  %s\n", it->m_manufacturerDescriptor.sz());

		if (!it->m_productDescriptor.isEmpty())
			printf("  product*:       %s\n", it->m_productDescriptor.sz());

		if (!it->m_serialNumberDescriptor.isEmpty())
			printf("  serial number*: %s\n", it->m_serialNumberDescriptor.sz());

		if (it->m_vendorId == VidSerialTap && it->m_productId == PidSerialTap)
			serialTap = *it;

		printf("\n");
	}

	if (!serialTap) {
		printf("Serial Tap is not found\n");
		return false;
	}

	bool result;

	enum {
		BufferSize = 1 * 1024 * 1024,
	};

#if (_AXL_OS_WIN)
	io::win::UsbPcapTransferParser parser;

	io::win::UsbPcap monitor;
	result =
		monitor.open(serialTap->m_captureDeviceName) &&
		monitor.setSnapshotLength(io::win::UsbPcap::DefaultSnapshotLength) &&
		monitor.setKernelBufferSize(BufferSize) &&
		monitor.setFilter(serialTap->m_address) &&
		monitor.readPcapHdr();

#elif (_AXL_OS_LINUX)
	io::lnx::UsbMonTransferParser parser;

	io::lnx::UsbMon monitor;
	result =
		monitor.open(serialTap->m_captureDeviceName) &&
		monitor.setKernelBufferSize(BufferSize);
#endif

	if (!result) {
		printf("error opening USB monitor: %s\n", err::getLastErrorDescription().sz());
		return false;
	}

	sl::String string;
	sl::Array<char> buffer;
	sl::Array<char> payload;
	buffer.setCount(BufferSize);

	printf("Capturing USB on the Serial Tap (buffer %d B)...\n", BufferSize);

	for (;;) {
		size_t size;

#if (_AXL_OS_WIN)
		size = monitor.read(buffer, BufferSize);
#elif (_AXL_OS_LINUX)
		fd_set readSet = { 0 };
		FD_SET(monitor, &readSet);
		int result = ::select(monitor + 1, &readSet, NULL, NULL, NULL);
		if (result == -1) {
			printf("select error: %s\n", err::Errno(errno).getDescription().sz());
			return false;
		}

		size = monitor.read(buffer, BufferSize);
		if (!size) {
			printf("EOF (?!)\n");
			break;
		}
#endif

		if (size == -1) {
			printf("read error: %s\n", err::getLastErrorDescription().sz());
			return false;
		}

		const char* p = buffer;
		const char* end = p + size;

		while (p < end) {
			size = parser.parse(p, end - p);
			if (size == -1) {
				printf("parse error: %s\n", err::getLastErrorDescription().sz());
				return false;
			}

			io::UsbMonTransferParserState state = parser.getState();
			switch (state) {
			case io::UsbMonTransferParserState_CompleteHeader: {
				const io::UsbMonTransferHdr* hdr = parser.getTransferHdr();
				printf("\nUsbMonTransferHdr:\n");
				printf("  m_id:           0x%016llx\n", hdr->m_id);
				printf("  m_timestamp:    %s\n", sys::Time(hdr->m_timestamp).format("%Y-%M-%D %h:%m:%s.%l").sz());
				printf("  m_status:       %d - %s\n", hdr->m_status, err::Errno(-hdr->m_status).getDescription().sz());
				printf("  m_flags:        0x%02x %s\n", hdr->m_flags, io::getUsbMonTransferFlagsString(hdr->m_flags).sz());
				printf("  m_transferType: %d - %s\n", hdr->m_transferType, io::getUsbTransferTypeString((libusb_transfer_type)hdr->m_transferType));
				printf("  m_bus:          %d\n", hdr->m_bus);
				printf("  m_address:      %d\n", hdr->m_address);
				printf("  m_endpoint:     0x%02x\n", hdr->m_endpoint);
				printf("  m_originalSize: %d\n", hdr->m_originalSize);
				printf("  m_captureSize:  %d\n", hdr->m_captureSize);

				switch (hdr->m_transferType) {
				case LIBUSB_TRANSFER_TYPE_CONTROL: {
					if (hdr->m_flags & io::UsbMonTransferFlag_Completed)
						break;

					const io::UsbMonControlSetup& cs = hdr->m_controlSetup;
					const io::UsbMonControlRequestType& crt = cs.m_requestType;
					printf("Control transfer:\n");
					printf("  m_requestType:  0x%02x\n", crt.m_value);
					printf("    m_recipient:  %d - %s\n", crt.m_recipient, io::getUsbMonControlRecipientString((io::UsbMonControlRecipient)crt.m_recipient));
					printf("    m_type:       %d - %s\n", crt.m_type, io::getUsbMonControlTypeString((io::UsbMonControlType)crt.m_type));
					printf("    m_direction:  %d - %s\n", crt.m_direction, io::getUsbMonControlDirectionString((io::UsbMonControlDirection)crt.m_direction));
					printf("  m_request:      %d - %s\n", cs.m_request, io::getUsbMonControlStdRequestString((io::UsbMonControlStdRequest)cs.m_request));
					printf("  m_value:        0x%04x\n", cs.m_value);
					printf("  m_index:        0x%04x\n", cs.m_index);
					printf("  m_length:       %d\n", cs.m_length);
					break;
					}

				case LIBUSB_TRANSFER_TYPE_ISOCHRONOUS: {
					const io::UsbMonIsochronousHdr& iso = hdr->m_isochronousHdr;
					printf("Isochronous:\n");
					printf("  m_startFrame:  %d\n", iso.m_startFrame);
					printf("  m_packetCount: %d\n", iso.m_packetCount);
					printf("  m_errorCount:  %d\n", iso.m_errorCount);
					break;
					}
				}

				break;
				}

			case io::UsbMonTransferParserState_IncompleteData:
				payload.append(p, size);
				break;

			case io::UsbMonTransferParserState_CompleteData:
				const io::UsbMonTransferHdr* hdr = parser.getTransferHdr();
				payload.append(p, size);
				ASSERT(payload.getCount() == hdr->m_captureSize);

				printf("Payload:\n");
				enc::HexEncoding::encode(
					&string,
					payload,
					hdr->m_captureSize,
					enc::HexEncodingFlag_Multiline
				);

				printf("%s\n", string.sz());
				payload.clear();
			}

			p += size;
		}
	}

	return true;
}

#endif

//..............................................................................

#if (_AXL_IO_USB)

void
testUsbEnum2() {
	io::registerUsbErrorProvider();
	io::getUsbDefaultContext()->createDefault();

	printf("ENUMERATING WITH AXL_IO_USB\n");
	printf("===========================\n");

	do {
		io::UsbDeviceList loDeviceList;
		sl::List<io::UsbDeviceEntry> deviceList;
		io::enumerateUsbDevices(&loDeviceList, &deviceList, io::UsbDeviceStringId_All);
		sl::ConstIterator<io::UsbDeviceEntry> it = deviceList.getHead();
		for (size_t i = 0; it; it++, i++) {
			io::UsbDevice device(it->m_device);
			libusb_device_descriptor deviceDescriptor;
			device.getDeviceDescriptor(&deviceDescriptor);

			printf("device #%d\n", i);
			printf("  bus:            %d\n", device.getBusNumber());
			printf("  address:        %d\n", device.getDeviceAddress());
			printf("  port:           %d\n", device.getPortNumber());
			printf("  VID:            0x%04x\n", deviceDescriptor.idVendor);
			printf("  PID:            0x%04x\n", deviceDescriptor.idProduct);
			printf("  class:          0x%02x - %s\n", deviceDescriptor.bDeviceClass, io::getUsbClassString(deviceDescriptor.bDeviceClass));
			printf("  subclass:       0x%02x\n", deviceDescriptor.bDeviceSubClass);
			printf("  speed:          %s\n", io::getUsbSpeedString(device.getDeviceSpeed()));

			if (!it->m_description.isEmpty())
				printf("  description:    %s\n", it->m_description.sz());

			if (!it->m_manufacturer.isEmpty())
				printf("  manufacturer:   %s\n", it->m_manufacturer.sz());

			if (!it->m_driver.isEmpty())
				printf("  driver:         %s\n", it->m_driver.sz());

			if (!it->m_manufacturerDescriptor.isEmpty())
				printf("  manufacturer*:  %s\n", it->m_manufacturerDescriptor.sz());

			if (!it->m_productDescriptor.isEmpty())
				printf("  product*:       %s\n", it->m_productDescriptor.sz());

			if (!it->m_serialNumberDescriptor.isEmpty())
				printf("  serial number*: %s\n", it->m_serialNumberDescriptor.sz());

			printf("\n");
		}
	} while (0);

#if (_AXL_IO_USBMON)
	printf("ENUMERATING WITH AXL_IO_USBMON\n");
	printf("==============================\n");

	do {
		sl::List<io::UsbMonDeviceDesc> deviceList;
		io::enumerateUsbMonDevices(&deviceList, io::UsbDeviceStringId_All);
		sl::ConstIterator<io::UsbMonDeviceDesc> it = deviceList.getHead();
		for (size_t i = 0; it; it++, i++) {
			printf("device #%d\n", i);
			printf("  capture:        %s\n", it->m_captureDeviceName.sz());
			printf("  VID:            0x%04x\n", it->m_vendorId);
			printf("  PID:            0x%04x\n", it->m_productId);
			printf("  address:        %d\n", it->m_address);
			printf("  port:           %d\n", it->m_port);
			printf("  class:          0x%02x - %s\n", it->m_class, io::getUsbClassString(it->m_class));
			printf("  subclass:       0x%02x\n", it->m_subClass);
			printf("  speed:          %s\n", io::getUsbSpeedString((libusb_speed)it->m_speed));

			if (!it->m_description.isEmpty())
				printf("  description:    %s\n", it->m_description.sz());

			if (!it->m_manufacturer.isEmpty())
				printf("  manufacturer:   %s\n", it->m_manufacturer.sz());

			if (!it->m_driver.isEmpty())
				printf("  driver:         %s\n", it->m_driver.sz());

			if (!it->m_manufacturerDescriptor.isEmpty())
				printf("  manufacturer*:  %s\n", it->m_manufacturerDescriptor.sz());

			if (!it->m_productDescriptor.isEmpty())
				printf("  product*:       %s\n", it->m_productDescriptor.sz());

			if (!it->m_serialNumberDescriptor.isEmpty())
				printf("  serial number*: %s\n", it->m_serialNumberDescriptor.sz());

			printf("\n");
		}
	} while (0);
#endif // _AXL_IO_USBMON
}

#endif // _AXL_IO_USB

//..............................................................................

#if (_AXL_OS_WIN)
int
wmain(
	int argc,
	wchar_t* argv[]
)
#else
int
main(
	int argc,
	char* argv[]
)
#endif
{
#if (_AXL_OS_POSIX)
	setvbuf(stdout, NULL, _IOLBF, 1024);
#endif

	g::getModule()->setTag("axl_test_con");
	srand((uint_t)sys::getTimestamp());

#if (_AXL_OS_WIN)
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
#elif (_AXL_OS_POSIX)
	signal(SIGPIPE, SIG_IGN);
#endif

#if (_AXL_IO_USB)
	testUsbEnum();
	testUsbEnum2();
#endif

	return 0;
}

//..............................................................................
