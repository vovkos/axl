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

#include <axl_cry_X509.h>
#include "pch.h"
#include "axl_sys_Thread.h"
#include "axl_err_Errno.h"
#include "../../include/axl_spy_ImportEnumerator.h"

#include "axl_spy_ImportEnumerator.h"
#include "axl_spy_ImportWriteProtection.h"
#include "axl_spy_ModuleEnumerator.h"
#include "axl_spy_Hook.h"

#include "axl_cry_AppStoreReceipt.h"

//..............................................................................

#if (_AXL_OS_WIN)

void
printAdapterAddress(const IP_ADDR_STRING* ipAddrString)
{
	for (size_t i = 1; ipAddrString; ipAddrString = ipAddrString->Next, i++)
	{
		printf("  #%-2d IP:   %s\n", i, ipAddrString->IpAddress.String);
		printf("  #%-2d MASK: %s\n", i, ipAddrString->IpMask.String);
	}
}

void
printAdapterInfo(IP_ADAPTER_INFO* ipAdapter)
{
	switch (ipAdapter->Type)
	{
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
testWinNetworkAdapterList()
{
	printf("Using GetAdaptersInfo...\n\n");

	dword_t size = 0;
	dword_t error = ::GetAdaptersInfo(NULL, &size);
	if (error != ERROR_BUFFER_OVERFLOW)
	{
		printf("GetAdaptersInfo failed (%d)\n", error);
		return;
	}

	char buffer[256];
	sl::Array<char> bufferArray(ref::BufKind_Stack, buffer, sizeof(buffer));
	bufferArray.setCount(size);

	IP_ADAPTER_INFO* ipAdapter = (IP_ADAPTER_INFO*)bufferArray.p();
	error = ::GetAdaptersInfo(ipAdapter, &size);
	if (error != ERROR_SUCCESS)
	{
		printf("GetAdaptersInfo failed (%d)\n", error);
		return;
	}

	size_t count = 0;
	for (; ipAdapter; ipAdapter = ipAdapter->Next, count++)
		printAdapterInfo(ipAdapter);

	printf("%d adapters found\n---------------------------------\n\n", count);
}

void
testWinNetworkAdapterList2()
{
	printf("Using WinIoctl...\n\n");

	printf(
		"sizeof (sockaddr) = %d\n"
		"sizeof (sockaddr_in) = %d\n"
		"sizeof (sockaddr_in6) = %d\n"
		"sizeof (sockaddr_storage) = %d\n"
		"sizeof (io::SockAddr) = %d\n",
		sizeof(sockaddr),
		sizeof(sockaddr_in),
		sizeof(sockaddr_in6),
		sizeof(sockaddr_storage),
		sizeof(io::SockAddr)
		);

	io::win::Socket socket;
	bool result = socket.wsaOpen(AF_INET, SOCK_DGRAM, 0);
	if (!result)
	{
		printf("socket.wsaOpen failed (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	size_t size = 16 * sizeof(INTERFACE_INFO);
	sl::Array<char> buffer;
	buffer.setCount(size);

	dword_t actualSize;

	for (;;)
	{
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
		if (error != WSAENOBUFS)
		{
			printf("WSAIoctl failed (%s)\n", err::Error (error).getDescription().sz());
			return;
		}

		size *= 2;
		buffer.setCount(size);
	}

	const INTERFACE_INFO* iface = (const INTERFACE_INFO*) buffer.cp();
	size_t ifaceCount = actualSize / sizeof(INTERFACE_INFO);

	for (size_t i = 0; i < ifaceCount; iface++, i++)
	{
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
getFileTimes_nt(const char* fileName)
{
	using namespace axl::sys::win;

	io::File file;
	bool result = file.open(fileName);
	if (!result)
	{
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

	if (status < 0)
	{
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
getFileTimes_win(const char* fileName)
{
	io::File file;
	bool result = file.open(fileName);
	if (!result)
	{
		printf("can't open %s: %s\n", fileName, err::getLastErrorDescription().sz());
		return false;
	}

	FILE_BASIC_INFO basicInfo2 = { 0 };
	result = ::GetFileInformationByHandleEx(file.m_file, FileBasicInfo, &basicInfo2, sizeof(basicInfo2)) != 0;
	if (!result)
	{
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
testFileTime()
{
	getFileTimes_win("c:/1/far/ioninja-3.7.4/bin/ioninja.exe");
	getFileTimes_nt("c:/1/far/ioninja-3.7.4/bin/ioninja.exe");
	getFileTimes_win("c:/1/7z/ioninja-3.7.4/bin/ioninja.exe");
	getFileTimes_nt("c:/1/7z/ioninja-3.7.4/bin/ioninja.exe");
}

#endif

//..............................................................................

void
testNetworkAdapterList()
{
	sl::List<io::NetworkAdapterDesc> adapterList;
	io::createNetworkAdapterDescList(&adapterList);

	sl::Iterator<io::NetworkAdapterDesc> adapterIt = adapterList.getHead();
	for (; adapterIt; adapterIt++)
	{
		io::NetworkAdapterDesc* adapter = *adapterIt;

		printf("Name        = %s\n", adapter->getName().sz());
		printf("Description = %s\n", adapter->getDescription().sz());
		printf("Type        = %s\n", io::getNetworkAdapterTypeString(adapter->getType()));
		printf("Flags       = %s\n", io::getNetworkAdapterFlagString(adapter->getFlags()).sz());
		printf("Mac         = %s\n", io::getMacAddressString(adapter->getMacAddress()).sz());

		sl::ConstList<io::NetworkAdapterAddress> addressList = adapter->getAddressList();
		sl::ConstIterator<io::NetworkAdapterAddress> addressIt = addressList.getHead();
		for (size_t i = 1; addressIt; addressIt++, i++)
		{
			const io::NetworkAdapterAddress* address = *addressIt;

			uint_t family = address->m_address.m_addr.sa_family;
			printf("%-11s = %s",
				io::getSockAddrFamilyString(family),
				address->m_address.getString().sz()
				);

			if (family == AF_INET)
			{
				io::SockAddr netMask;
				netMask.createNetMask_ip4(address->m_netMaskBitCount);
				printf(" (mask %s)\n", netMask.getString().sz());
			}
			else
			{
				printf("/%d\n", address->m_netMaskBitCount);
			}
		}

		printf("\n");
	}

	printf("%d adapters found\n", adapterList.getCount());
}

//..............................................................................

void
testParseFormatIp6()
{
	printf("main ()\n");

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

	for (;;)
	{
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
testKeepAlives(const sl::StringRef& addrString)
{
	dword_t value = 1;
	dword_t delay = 3;

	io::Socket socket;

	printf("Opening a TCP socket (%d sec keep-alives)...\n", delay);

	bool result =
		socket.open(AF_INET, SOCK_STREAM, IPPROTO_TCP) &&
		socket.setOption(SOL_SOCKET, SO_KEEPALIVE, &value, sizeof(value)) &&
		socket.setOption(IPPROTO_TCP, TCP_KEEPIDLE, &delay, sizeof(delay)) &&
		socket.setOption(IPPROTO_TCP, TCP_KEEPINTVL, &delay, sizeof(delay));

	if (!result)
	{
		printf("socket.open failed (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("Parsing address '%s'...\n", addrString.sz());

	io::SockAddr addr;
	result = addr.parse(addrString);
	if (!result)
	{
		printf("addr.parse failed (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("Connecting to %s...\n", addr.getString().sz());

	result = socket.connect(addr);
	if (!result)
	{
		printf("socket.connect failed (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("Press CTRL+C to exit...\n");

	for (;;)
	{
		sys::sleep(1000);
	}
}

//..............................................................................

void
testAddrInfoIp6()
{
	const char* name = "tibbo.com";

	sl::Array<io::SockAddr> addrArray;
	bool result = io::resolveHostName(&addrArray, name, AF_INET6);
	if (!result)
	{
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
testDynamicLib()
{
	sys::DynamicLib dl;

	bool result = dl.open("libc.so.6");
	if (!result)
	{
		printf("dl.load failed: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	typedef int Printf(const char*, ...);
	Printf* prn = (Printf*)dl.getFunction("printf");
	if (!prn)
	{
		printf("dl.load failed: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	prn("hui govno i muravei %d\n", 123);
}

//..............................................................................

void
bar()
{
	printf("bar -- throwing...\n");
	AXL_SYS_SJLJ_THROW();
}

void
testLongJmpTry()
{
	AXL_SYS_BEGIN_SJLJ_TRY()
	{
		bar();
	}
	AXL_SYS_SJLJ_CATCH()
	{
		printf("exception caught\n");
	}
	AXL_SYS_SJLJ_FINALLY()
	{
		printf("finally\n");
	}
	AXL_SYS_END_SJLJ_TRY()

	AXL_SYS_BEGIN_SJLJ_TRY()
	{
		bar();
	}
	AXL_SYS_SJLJ_CATCH()
	{
		printf("exception caught\n");
	}
	AXL_SYS_END_SJLJ_TRY()

	AXL_SYS_BEGIN_SJLJ_TRY()
	{
		bar();
	}
	AXL_SYS_SJLJ_FINALLY()
	{
		printf("finally\n");
	}
	AXL_SYS_END_SJLJ_TRY()

	AXL_SYS_BEGIN_SJLJ_TRY()
	{
		bar();
	}
	AXL_SYS_END_SJLJ_TRY()

	printf("done\n");
}

//..............................................................................

void
testRegex()
{
	fsm::StdRegexNameMgr nameMgr;

	nameMgr.addName("ws",  "[ \\t\\r\\n]");
	nameMgr.addName("dec", "[0-9]");

	uint_t flags = 0; // fsm::RegexCompiler::Flag_SparseSyntax;

	fsm::Regex regex;
	fsm::RegexCompiler regexCompiler(flags, &regex, &nameMgr);

/*	char const* src[] =
	{
		"'\\x02' V '\\r'",
	};

	for (size_t i = 0; i < countof(src); i++)
	{
		bool result = regexCompiler.incrementalCompile(src[i]);
		if (!result)
		{
			printf("error: %s\n", err::getLastErrorDescription().sz());
			return;
		}
	}

	regexCompiler.finalize();
	regex.print();
*/

	bool result = regexCompiler.compile(".*ray[.].*");

/*	bool result =
		regexCompiler.incrementalCompile("(\\h{2})   ' '+ (\\d{2})") &&
		regexCompiler.incrementalCompile("([a-z]{3}) ' '+ ([A-Z]{3})\\n");
*/
	if (!result)
	{
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	regexCompiler.finalize();
	regex.print();

	char fileName[] = "C:\\Projects\\repos\\ioninja\\jancy\\src\\jnc_ext\\jnc_std\\jnc\\std_Array.jnc";
	result = regex.match(fileName);
	printf("match: %d\n", result);
}

//..............................................................................

#if (_AXL_OS_WIN)

//..............................................................................

enum
{
	BufferSize = 4 * 1024,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
testNamedPipes()
{
	using namespace axl::sys::win;

	NTSTATUS status;

	HMODULE ntdll = ::GetModuleHandleW(L"ntdll.dll");
	ASSERT(ntdll);

	io::win::File pipeDir;
	bool result = pipeDir.create(
		L"\\\\.\\pipe\\",
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		0
		);

	if (!result)
	{
		err::setLastSystemError();
		printf("cannot open pipe dir: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	sl::Array<char> dirBuffer;
	dirBuffer.setCount(BufferSize);

	sl::String_utf16 fileName;

	BOOLEAN isFirstQuery = TRUE;

	for (;;)
	{
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

		if (status < 0)
		{
			if (status == STATUS_NO_MORE_FILES)
				break;

			err::setError(sys::win::NtStatus(status));
			printf("cannot open pipe dir: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		FILE_DIRECTORY_INFORMATION* dirInfo = (FILE_DIRECTORY_INFORMATION*)dirBuffer.p();
		for (;;)
		{
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
	)
{
	using namespace axl::sys::win;

	NTSTATUS status;

	string->clear();

	OBJECT_ATTRIBUTES oa = { 0 };
	oa.Length = sizeof(oa);
	oa.RootDirectory = dir;
	oa.ObjectName = uniName;

	io::win::File link;
	status = ntOpenSymbolicLinkObject(
		link.p(),
		GENERIC_READ,
		&oa
		);

	if (status < 0)
	{
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
	if (status < 0)
	{
		err::setError(sys::win::NtStatus(status));
		printf("cannot query symbolic link: %s\n", err::getLastErrorDescription().sz());
		return false;
	}

	string->setReducedLength(uniTarget.Length / sizeof(wchar_t));
	return true;
}

void
enumerateDirectory(
	HANDLE baseDir,
	const wchar_t* name,
	size_t level
	)
{
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

	io::win::File dir;
	status = ntOpenDirectoryObject(
		dir.p(),
		DIRECTORY_QUERY | DIRECTORY_TRAVERSE,
		&oa
		);

	if (status < 0)
	{
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

	for (;;)
	{
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

		if (status < 0)
		{
			if (status == STATUS_NO_MORE_ENTRIES)
				break;

			err::setError(sys::win::NtStatus(status));
			printf("cannot query directory: %s\n", err::getLastErrorDescription().sz());
			return;
		}

		OBJECT_DIRECTORY_INFORMATION* dirInfo = (OBJECT_DIRECTORY_INFORMATION*)buffer.p();
		for (; dirInfo->Name.Buffer; dirInfo++)
		{
			dirName.copy(dirInfo->Name.Buffer, dirInfo->Name.Length / sizeof(wchar_t));
			dirTypeName.copy(dirInfo->TypeName.Buffer, dirInfo->TypeName.Length / sizeof(wchar_t));

			printf("%s%S (%S)\n", indent.sz(), dirName.sz(), dirTypeName.sz());

			if (dirTypeName.cmp(L"Directory") == 0)
			{
				enumerateDirectory(dir, dirName, level);
			}
			else if (dirTypeName.cmp(L"SymbolicLink") == 0)
			{
				bool result = querySymbolicLink(&symLinkTargetName, dir, &dirInfo->Name);
				if (result)
					printf("%s  --> %S\n", indent.sz(), symLinkTargetName.sz());
			}

		}

		isFirstQuery = FALSE;
	}
}

void
testDirectoryObjects()
{
	using namespace axl::sys::win;

	sl::String_utf16 s;

	io::win::File f;
	bool result = f.create(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING);

	struct ObjectInfo: OBJECT_NAME_INFORMATION
	{
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
testTcp()
{
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
	)
{
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
testWinError()
{
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
testTimestamp()
{
	for (int i = 0; i < 20; i++)
	{
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
	)
{
	sl::String text;

	if (!device->isOpen())
	{
		text = "NOT OPENED";
	}
	else
	{
		bool result = device->getStringDesrciptor(index, &text);
		if (!result)
			text.format("ERROR (%s)", err::getLastErrorDescription().sz());
	}

	return text;
}

void
printUsbIfaceDesc(const libusb_interface_descriptor* ifaceDesc)
{
	printf("    Interface:   %d\n", ifaceDesc->bInterfaceNumber);
	printf("    Alt setting: %d\n", ifaceDesc->bAlternateSetting);
	printf("    Class:       %s\n", io::getUsbClassCodeString((libusb_class_code) ifaceDesc->bInterfaceClass));
	printf("    Subclass:    %d\n", ifaceDesc->bInterfaceSubClass);
	printf("    Protocol:    %d\n", ifaceDesc->bInterfaceProtocol);
	printf("    Endpoints:   %d\n", ifaceDesc->bNumEndpoints);

	for (size_t i = 0; i < ifaceDesc->bNumEndpoints; i++)
	{
		const libusb_endpoint_descriptor* endpointDesc = &ifaceDesc->endpoint[i];

		printf("\n");

		printf("      Endpoint:        0x%02x\n", endpointDesc->bEndpointAddress);
		printf("      Direction:       %s\n", (endpointDesc->bEndpointAddress & LIBUSB_ENDPOINT_IN) ? "In" : "Out");
		printf("      Type:            %s\n", io::getUsbTransferTypeString((libusb_transfer_type) (endpointDesc->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK)));
		printf("      Max packet size: %d\n", endpointDesc->wMaxPacketSize);
	}
}

void
printUsbConfigDesc(const libusb_config_descriptor* configDesc)
{
	printf("  Configuration: %d\n", configDesc->bConfigurationValue);
	printf("  Max power:     %d mA\n", configDesc->MaxPower * 2);
	printf("  Interfaces:    %d\n", configDesc->bNumInterfaces);

	for (size_t i = 0; i < configDesc->bNumInterfaces; i++)
	{
		const libusb_interface* iface = &configDesc->interface[i];

		if (!iface->num_altsetting)
		{
			printf("\n    Interface #%d is not configured\n", i);
		}
		else for (size_t j = 0; j < (size_t)iface->num_altsetting; j++)
		{
			printf("\n");
			printUsbIfaceDesc(&iface->altsetting[j]);
		}
	}
}

void
printUsbDevice(io::UsbDevice* device)
{
	bool result;

	libusb_device_descriptor deviceDesc;
	result = device->getDeviceDescriptor(&deviceDesc);
	if (!result)
	{
		printf("Cannot get device descriptor (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("HWID:           VID_%04x&PID_%04x\n", deviceDesc.idVendor, deviceDesc.idProduct);
	printf("Class:          %s\n", io::getUsbClassCodeString((libusb_class_code) deviceDesc.bDeviceClass));
	printf("Manufacturer:   %s\n", getUsbStringDescriptorText(device, deviceDesc.iManufacturer).sz());
	printf("Product name:   %s\n", getUsbStringDescriptorText(device, deviceDesc.iProduct).sz());
	printf("Serial number:  %s\n", getUsbStringDescriptorText(device, deviceDesc.iSerialNumber).sz());

	printf("Address:        %d\n", device->getDeviceAddress ());
	printf("Bus:            %d\n", device->getBusNumber ());
#if (_AXL_IO_USBDEVICE_PORT)
	printf("Port:           %d\n", device->getPortNumber ());
#endif
	printf("Speed:          %s\n", io::getUsbSpeedString(device->getDeviceSpeed ()));
	printf("Port path:      ");

#if (_AXL_IO_USBDEVICE_PORT)
	uint8_t path[8];
	size_t pathLength = device->getPortPath(path, countof(path));
	if (pathLength == -1)
	{
		printf("ERROR (%s)\n", err::getLastErrorDescription().sz());
	}
	else if (pathLength != -1)
	{
		for (size_t i = 0; i < pathLength; i++)
			printf("-> %d", path [i]);

		printf("\n");
	}
#else
	printf("\n");
#endif

	printf("Configurations: %d\n", deviceDesc.bNumConfigurations);

	for (size_t i = 0; i < deviceDesc.bNumConfigurations; i++)
	{
		printf("\n");

		io::UsbConfigDescriptor configDesc;
		bool result = device->getConfigDescriptor(i, &configDesc);
		if (!result)
			printf("  Cannot get config descriptor #%d (%s)\n", i, err::getLastErrorDescription().sz());
		else
			printUsbConfigDesc(configDesc);
	}
}

void
testUsbEnum()
{
	bool result;

	io::registerUsbErrorProvider();
	io::getUsbDefaultContext()->createDefault();

	io::UsbDeviceList deviceList;
	size_t count = deviceList.enumerateDevices();
	if (count == -1)
	{
		printf("Cannot enumerate USB devices (%s)\n", err::getLastErrorDescription().sz());
		return;
	}

	io::UsbDeviceList dev;

	libusb_device** pp = deviceList;
	for (size_t i = 0; *pp; pp++, i++)
	{
		printf("----------------------\nDevice #%d\n", i);

		io::UsbDevice device;
		device.setDevice(*pp);

		result = device.open();
		if (!result)
			printf("Cannot open device (%s)\n", err::getLastErrorDescription().sz());

		printUsbDevice(&device); // even if not opened
	}
}

class UsbRead
{
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
		)
	{
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
	public UsbRead
{
public:
	UsbReadThread(
		io::UsbDevice* device,
		uint_t endpointId,
		uint_t timeout = 1000
		): UsbRead(device, endpointId, timeout)
	{
	}

	void
	threadFunc()
	{
		sl::Array<char> buffer;
		buffer.setCount(m_maxPacketSize);

		size_t totalSize = 0;
		while (totalSize < 1024)
		{
			size_t size;
			switch (m_endpointType)
			{
			case LIBUSB_TRANSFER_TYPE_BULK:
				size = m_device->bulkTransfer(m_endpointId, buffer, m_maxPacketSize, m_timeout);
				break;

			case LIBUSB_TRANSFER_TYPE_INTERRUPT:
				size = m_device->interruptTransfer(m_endpointId, buffer, m_maxPacketSize, m_timeout);
				break;

			default:
				ASSERT(false);
			}

			if (size == -1)
			{
				printf("interrupt transfer error: %s\n", err::getLastErrorDescription().sz());
			}
			else
			{
				printf("received %d bytes\n", size);
				totalSize += size;
			}
		}
	}
};

class UsbAsyncTransfer:
	public sys::ThreadImpl<UsbReadThread>,
	public UsbRead
{
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
		): UsbRead(device, endpointId, timeout)
	{
		m_transfer.create();
		m_buffer.setCount(m_maxPacketSize);
		m_totalSize = 0;
	}

	bool
	next()
	{
		switch (m_endpointType)
		{
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
	wait(uint_t timeout = -1)
	{
		return m_completionEvent.wait(timeout);
	}

protected:
	static
	void
	LIBUSB_CALL
	onCompleted(libusb_transfer* transfer)
	{
		UsbAsyncTransfer* self = (UsbAsyncTransfer*)transfer->user_data;

		printf(
			"libusb_transfer completed:\n"
			"    status:        %s (%x)\n"
			"    actual length: %d\n",
			io::getUsbTransferStatusString(transfer->status),
			transfer->status,
			transfer->actual_length
			);

		if (transfer->status == LIBUSB_TRANSFER_COMPLETED)
		{
			self->m_totalSize += transfer->actual_length;
			if (self->m_totalSize > 1024)
			{
				self->m_completionEvent.signal();
				return;
			}
		}

		self->next();
	}
};

void
testUsbMouse()
{
	enum
	{
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
	if (!result)
	{
		printf("Error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("Reading device properties...\n");
	printUsbDevice(&device);

	if (io::hasUsbCapability(LIBUSB_CAP_SUPPORTS_DETACH_KERNEL_DRIVER))
	{
		printf("Setting auto-detach for kernel driver...\n");
		result = device.setAutoDetachKernelDriver(true);
		if (!result)
		{
			printf("Error: %s\n", err::getLastErrorDescription().sz());
			return;
		}
	}

	printf("Claiming interface #%d...\n", InterfaceId);
	result = device.claimInterface(InterfaceId);
	if (!result)
	{
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

	if (size == -1)
	{
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

class MyThread: public sys::ThreadImpl<MyThread>
{
public:
	volatile bool m_terminateFlag;

	MyThread()
	{
		m_terminateFlag = false;
	}

	void
	threadFunc()
	{
		uint64_t tid = getThreadId();

		for (;;)
		{
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

void* suspendThread(pthread_t thread)
{
	return NULL;
}

bool resumeThread(pthread_t thread)
{
	return false;
}

void testSuspendThread()
{
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
class Gc
{
protected:
	enum HandshakeKind
	{
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
	Gc()
	{
		m_guardPage.map(
			NULL,
			4 * 1024, // typical page size -- OS will not give us less than that, anyway
			PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS,
			-1,
			0
			);

		m_handshakeKind = HandshakeKind_None;
		installSignalHandlers();
	}

	void
	gcSafePoint()
	{
		*(volatile int*) m_guardPage.p() = 0;
	}

	void
	stopTheWorld()
	{
		m_handshakeKind = HandshakeKind_StopTheWorld;
		m_handshakeCounter = m_threadMap.getCount();
		m_guardPage.protect(PROT_NONE);
		m_handshakeSem.wait();
		m_handshakeKind = HandshakeKind_None;
	}

	void
	resumeTheWorld()
	{
		m_guardPage.protect(PROT_READ | PROT_WRITE);

		m_handshakeKind = HandshakeKind_ResumeTheWorld;
		m_handshakeCounter = m_threadArray.getCount();
		for (size_t i = 0; i < m_handshakeCounter; i++)
			pthread_kill((pthread_t)m_threadArray[i], SIGUSR1); // resume

		m_handshakeSem.wait();
		m_handshakeKind = HandshakeKind_None;
	}

	bool
	registerThread(uint64_t threadId)
	{
		sl::HashTableIterator<uint64_t, bool> it = m_threadMap.visit(threadId);
		if (it->m_value)
			return false;

		m_threadArray.append(threadId);
		it->m_value = true;
		return true;
	}

protected:
	bool
	installSignalHandlers()
	{
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
		)
	{
		if (signal != SIGSEGV ||
			signalInfo->si_addr != g_gc->m_guardPage ||
			g_gc->m_handshakeKind != HandshakeKind_StopTheWorld)
			return; // ignore

		int32_t count = sys::atomicDec(&g_gc->m_handshakeCounter);
		if (!count)
			g_gc->m_handshakeSem.signal();

		do
		{
			sigsuspend(&g_gc->m_signalWaitMask);
		} while (g_gc->m_handshakeKind != HandshakeKind_ResumeTheWorld);

		count = sys::atomicDec(&g_gc->m_handshakeCounter);
		if (!count)
			g_gc->m_handshakeSem.signal();
	}

	static
	void
	signalHandler_SIGUSR1(int signal)
	{
		// do nothing (we handshake manually). but we still need a handler
	}
};

#define GC_BEGIN()
#define GC_END()

#elif (_AXL_OS_WIN)

class Gc
{
protected:
	enum HandshakeKind
	{
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
	Gc()
	{
		m_guardPage.alloc(4 * 1024);
		m_handshakeKind = HandshakeKind_None;
	}

	~Gc()
	{
	}

	void
	gcSafePoint()
	{
		sys::atomicXchg((volatile int*) m_guardPage.p(), 0);
	}

	void
	stopTheWorld()
	{
		m_handshakeKind = HandshakeKind_StopTheWorld;
		m_handshakeCounter = m_threadMap.getCount();
		m_resumeEvent.reset();
		m_guardPage.protect(PAGE_NOACCESS);
		m_handshakeEvent.wait();
		m_handshakeKind = HandshakeKind_None;
	}

	void
	resumeTheWorld()
	{
		m_guardPage.protect(PAGE_READWRITE);

		m_handshakeKind = HandshakeKind_ResumeTheWorld;
		m_handshakeCounter = m_threadMap.getCount();
		m_resumeEvent.signal();
		m_handshakeEvent.wait();
		m_handshakeKind = HandshakeKind_None;
		m_resumeEvent.reset();
	}

	bool
	registerThread(uint64_t threadId)
	{
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
		)
	{
		if (code != EXCEPTION_ACCESS_VIOLATION ||
		   exceptionPointers->ExceptionRecord->ExceptionInformation[1] != (uintptr_t)m_guardPage.p() ||
		   m_handshakeKind != HandshakeKind_StopTheWorld)
		  return EXCEPTION_CONTINUE_SEARCH;

		int32_t count = sys::atomicDec(&g_gc->m_handshakeCounter);
		if (!count)
			g_gc->m_handshakeEvent.signal();

		do
		{
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

class MutatorThread: public sys::ThreadImpl<MutatorThread>
{
protected:
	volatile bool m_terminateFlag;

public:
	MutatorThread()
	{
		m_terminateFlag = false;
	}

	void
	stop()
	{
		m_terminateFlag = true;
		waitAndClose();
		m_terminateFlag = false;
	}

	void
	threadFunc()
	{
		uint64_t threadId = sys::getCurrentThreadId();

		GC_BEGIN()

		for (;;)
		{
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
testGcSafePoints()
{
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

struct IfaceHdr
{
	const char* m_p1;
	const char* m_p2;
	const char* m_p3;
};

class Foo: public IfaceHdr
{
public:
	Foo()
	{
		printf("m_p1 = %s; m_p2 = %s; m_p3 = %s\n", m_p1, m_p2, m_p3);
	}
};

void
testInheritance()
{
	char buffer[sizeof(Foo)];
	IfaceHdr* iface = (IfaceHdr*)buffer;
	iface->m_p1 = "hui";
	iface->m_p2 = "govno";
	iface->m_p3 = "muravei";

	new(buffer)Foo();
}

//..............................................................................

void
testTimestamps()
{
	uint64_t ts1 = sys::getTimestamp();
	uint64_t ts2 = sys::getPreciseTimestamp();

	printf("time (0) = %s\n", sys::Time(0, 0).format("%h:%m:%s.%l.%c").sz());
	printf("ts1 = %s\n", sys::Time(ts1).format("%h:%m:%s.%l.%c").sz());
	printf("ts2 = %s\n", sys::Time(ts2).format("%h:%m:%s.%l.%c").sz());

	uint64_t t0 = sys::getPreciseTimestamp();

	size_t n = 10000000;

	for (size_t i = 0; i < n; i++)
	{
		sys::getTimestamp();
	}

	uint64_t t2 = sys::getPreciseTimestamp();
	uint64_t d = t2 - t0;
	printf("time (ts1) = %s\n", sys::Time(d, 0).format("%h:%m:%s.%l.%c").sz());

	t0 = sys::getPreciseTimestamp();

	for (size_t i = 0; i < n; i++)
	{
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
testProcess()
{
	sl::Array<char> output;
	dword_t exitCode;

	const wchar_t* cmdLine = L"C:\\Projects\\ioninja\\devmon\\build\\msvc10\\bin\\Debug\\tdevmon.exe --start-core-service";

	bool result = sys::win::syncExec(cmdLine, &output, &exitCode);
	if (!result)
	{
		printf("sys::win::syncExec failed: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	output.append(0);
	printf("process returned %d:\n%s\n", exitCode, output.cp ());
}
#endif

//..............................................................................

namespace shm_test {

const size_t TotalSize = 64 * 1024 * 1024;
const size_t MaxBlockSize = 64;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ReqCode
{
	ReqCode_Command,
	ReqCode_Reply
};

struct ReqHdr
{
	uint32_t m_code;
	uint32_t m_size;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Transport
{
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

class ServerThread: public sys::ThreadImpl<ServerThread>
{
public:
	Transport* m_transport;

public:
	ServerThread(Transport* transport)
	{
		m_transport = transport;
	}

	uint_t
	threadFunc()
	{
		ASSERT(m_transport);

		sl::Array<char> buffer;

		static char data[MaxBlockSize] = { 0 };

		for (;;)
		{
			size_t size = m_transport->readMessage(&buffer);
			if (size == -1)
			{
				printf("server: read error: %s\n", err::getLastErrorDescription().sz());
				return -1;
			}

			if (size < sizeof(ReqHdr))
			{
				printf("server: buffer too small\n");
				return -1;
			}

			ReqHdr* command = (ReqHdr*)buffer.cp();
			if (command->m_code != ReqCode_Command || sizeof(ReqHdr) + command->m_size != size)
			{
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

class ClientWriterThread: public sys::ThreadImpl<ClientWriterThread>
{
protected:
	Transport* m_transport;

public:
	ClientWriterThread(Transport* transport)
	{
		m_transport = transport;
	}

	uint_t
	threadFunc()
	{
		ASSERT(m_transport);

		size_t totalSize = 0;

		sl::Array<char> buffer;
		static char data[MaxBlockSize] = { 0 };

		while (totalSize < TotalSize)
		{
			ReqHdr command;
			command.m_code = ReqCode_Command;
			command.m_size = ::rand() % MaxBlockSize;;

			bool result = m_transport->writeMessage(&command, data, command.m_size);
			if (!result)
			{
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

class ClientReaderThread: public sys::ThreadImpl<ClientReaderThread>
{
protected:
	Transport* m_transport;

public:
	ClientReaderThread(Transport* transport)
	{
		m_transport = transport;
	}

	uint_t
	threadFunc()
	{
		ASSERT(m_transport);

		int percentage = 0;
		size_t totalSize = 0;

		sl::Array<char> buffer;

		while (totalSize < TotalSize)
		{
			size_t receivedSize = m_transport->readMessage(&buffer);
			if (receivedSize == -1)
			{
				printf("client: read error: %s\n", err::getLastErrorDescription().sz());
				return -1;
			}

			if (receivedSize < sizeof(ReqHdr))
			{
				printf("client: buffer too small\n");
				return -1;
			}

			ReqHdr* reply = (ReqHdr*)buffer.cp();
			if (reply->m_code != ReqCode_Reply || sizeof(ReqHdr) + reply->m_size != receivedSize)
			{
				printf("client: invalid reply: %d (%d)\n", reply->m_code, reply->m_size);
				return -1;
			}

			totalSize += reply->m_size;

			int newPercentage = (uint64_t)totalSize * 100 / TotalSize;
			if (newPercentage != percentage)
			{
				percentage = newPercentage;
				printf("\b\b\b\b%d%%", percentage);
			}
		}

		printf("\nReading 100%% (done), %zd bytes\n", totalSize);
		return 0;
	}
};

//..............................................................................

class ShmTransport: public Transport
{
public:
	io::SharedMemoryReader m_reader;
	io::SharedMemoryWriter m_writer;

public:
	virtual
	size_t
	readMessage(sl::Array<char>* buffer)
	{
		return m_reader.read(buffer);
	}

	virtual
	bool
	writeMessage(
		const ReqHdr* hdr,
		const void* p,
		size_t size
		)
	{
		if (!size)
			return m_writer.write(hdr, sizeof(ReqHdr)) != -1;

		const void* blockArray[] = { hdr, p };
		size_t sizeArray[] = { sizeof(ReqHdr), size };

		return m_writer.write(blockArray, sizeArray, 2) != -1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class PipeTransport: public Transport
{
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
	readMessage(sl::Array<char>* buffer)
	{
		size_t size = 0;

		size_t bufferSize = buffer->getCount();

		if (bufferSize < sizeof(ReqHdr))
			buffer->setCount(sizeof(ReqHdr));

		bufferSize = sizeof(ReqHdr);

		while (size < bufferSize)
		{
#if (_AXL_OS_WIN)
			dword_t actualSize;
			bool_t result = ::ReadFile(m_readPipe, buffer->p() + size, bufferSize - size, &actualSize, NULL);
#elif (_AXL_OS_POSIX)
			int actualSize = ::read(m_readPipe, buffer->p() + size, bufferSize - size);
			bool result = actualSize != -1;
#endif

			if (!result)
			{
				err::setLastSystemError();
				return -1;
			}

			size += actualSize;
		}

		ASSERT(size == bufferSize);

		ReqHdr* hdr = (ReqHdr*)buffer->p();
		if (!hdr->m_size)
		{
			ASSERT(size == sizeof(ReqHdr));
			buffer->setCount(sizeof(ReqHdr));
			return sizeof(ReqHdr);
		}

		bufferSize = sizeof(ReqHdr) + hdr->m_size;
		buffer->setCount(bufferSize);

		while (size < bufferSize)
		{
#if (_AXL_OS_WIN)
			dword_t actualSize;
			bool_t result = ::ReadFile(m_readPipe, buffer->p() + size, bufferSize - size, &actualSize, NULL);
#elif (_AXL_OS_POSIX)
			int actualSize = ::read(m_readPipe, buffer->p() + size, bufferSize - size);
			bool result = actualSize != -1;
#endif

			if (!result)
			{
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
		)
	{
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
testSharedMemoryTransport()
{
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
			io::SharedMemoryTransportFlag_Message |
			io::FileFlag_DeleteOnClose
			) &&
		serverTransport.m_writer.open(
			HOME_DIR "/shmt-test-srv-cli",
			"shmt-test-srv-cli-r",
			"shmt-test-srv-cli-w",
			io::SharedMemoryTransportFlag_Message |
			io::FileFlag_DeleteOnClose
			) &&
		clientTransport.m_reader.open(
			HOME_DIR "/shmt-test-srv-cli",
			"shmt-test-srv-cli-r",
			"shmt-test-srv-cli-w",
			io::SharedMemoryTransportFlag_Message |
			io::FileFlag_OpenExisting |
			io::FileFlag_Unlink
			) &&
		clientTransport.m_writer.open(
			HOME_DIR "/shmt-test-cli-srv",
			"shmt-test-cli-srv-r",
			"shmt-test-cli-srv-w",
			io::SharedMemoryTransportFlag_Message |
			io::FileFlag_OpenExisting |
			io::FileFlag_Unlink
			);

	if (!result)
	{
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
testPipeTransport()
{
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

	if (!result)
	{
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

void
testZip()
{
	bool result;

	zip::ZipReader reader;

	result = reader.openFile("C:/Program Files/Tibbo/Ninja 3/bin/io_base.jncx");
	if (!result)
	{
		printf("can't open file: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	sl::Array<char> buffer;

	sl::String dir = "C:/\xd0\xa1\xd1\x83\xd0\xba\xd0\xb0/";

	size_t count = reader.getFileCount();
	for (size_t i = 0; i < count; i++)
	{
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

		if (!isDir)
		{
			reader.extractFileToMem(i, &buffer);
			printf("<<<\n%s\n>>>\n", buffer.cp ());

			sl::String dstFileName = dir + fileName;
			result = reader.extractFileToFile(i, dstFileName);
			if (!result)
			{
				printf("can't extract file: %s\n", err::getLastErrorDescription().sz());
				return;
			}
		}

		printf("\n");
	}
}

//..............................................................................

void
testEnumSerial()
{
	sl::List<io::SerialPortDesc> portList;
	io::createSerialPortDescList(&portList);

	sl::Iterator<io::SerialPortDesc> it = portList.getHead();
	for (; it; it++)
		printf("device name: %s\ndescription: %s\n\n", it->getDeviceName ().sz(), it->getDescription().sz());

	printf("%d ports total\n", portList.getCount());
}

//..............................................................................

void
testEncoding()
{
	sl::String s;
	enc::EscapeEncoding::encode(&s, "\\\\.\\pipe\\mypipe");
	printf("%s\n", s.sz());
}

//..............................................................................

// AXL_NO_ASAN
void
testAddressSanitizer()
{
	char c = 10;
	char* p = &c;
	p++;
	*p = 0;
}

int foo(jmp_buf buf)
{
	return 0;
}

//..............................................................................

#pragma pack()

struct F0
{
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack(1)

struct F1
{
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack(2)

struct F2
{
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack(4)

struct F4
{
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack(8)

struct F8
{
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

void
testPacking()
{
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
	)
{
	static uint16_t const crcTable[256] =
	{
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

	for (; size; size--)
	{
		uint8_t j = *b++ ^ crc;
		crc = (crc >> 8) ^ crcTable[j];
	}

	return crc;
}

uint16_t crc16_ccit(
	uint8_t const* p,
	size_t size,
	uint16_t seed = 0
	)
{
	static uint16_t const crcTable[256] =
	{
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

	for (; size; size--)
	{
		uint8_t j = *b++ ^ (crc >> 8);
		crc = (crc << 8) ^ crcTable[j];
	}

	return (uint16_t)crc;
}

void
testModBus()
{
	uint8_t packet[] =
	{
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

	if (baud > 19200)
	{
		timeout_1_5 = 7500;
		timeout_3_5 = 17500;
	}
	else
	{
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
	)
{
	uint16_t const* p = (uint16_t const*) p0;
	void const* end = p0 + (size & ~1);

	uint_t checksum = 0;

	for (; p < end; p++)
		checksum += htons(*p);

	if (size & 1)
		checksum += *(uint8_t const*) p << 8;

	return checksum;
}

uint16_t adjustIpCheckSum(uint_t checksum)
{
	checksum = (checksum >> 16) + (checksum & 0xffff);
	checksum += checksum >> 16;
	return ~checksum;
}

uint16_t calcIpHdrChecksum(const uint8_t* ipHdr, size_t size)
{
	uint_t checksum = calcChecksum16(ipHdr, size);
	return adjustIpCheckSum(checksum);
}

void
testIpChecksum()
{
	uint8_t data1_0[] =
	{
		0x45, 0x00, 0x00, 0x1e, 0x7e, 0x8b, 0x00, 0x00, 0x80, 0x11, 0x37, 0x7d, 0xc0, 0xa8, 0x01, 0x77,
		0xc0, 0xa8, 0x01, 0xff
	};

	uint8_t data1[] =
	{
		0x45, 0x00, 0x00, 0x1e, 0x7e, 0x8b, 0x00, 0x00, 0x80, 0x11, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x77,
		0xc0, 0xa8, 0x01, 0xff
	};

	uint8_t data2_0[] =
	{
		0x45, 0x00, 0x00, 0x73, 0x00, 0x00, 0x40, 0x00, 0x40, 0x11, 0xb8, 0x61, 0xc0, 0xa8, 0x00, 0x01,
		0xc0, 0xa8, 0x00, 0xc7
	};

	uint8_t data2[] =
	{
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
	)
{
	size_t i = 0;
	size_t j = 0;

	for (; i < 16; i++)
	{
		uchar_t c = name[i];
		if (!c)
			break;

		c = toupper(c);

		buffer[j++] = 'A' + (c >> 4);
		buffer[j++] = 'A' + (c & 0x0f);
	}

	char paddingByteHi = 'A' + (paddingChar >> 4);
	char paddingByteLo = 'A' + (paddingChar & 0x0f);

	while (j < 32)
	{
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
	)
{
	ASSERT(!(length & 1)); // must be even

	size_t i = 0;
	size_t j = 0;

	while (i < length)
	{
		uchar_t hi = name[i++] - 'A';
		uchar_t lo = name[i++] - 'A';

		buffer[j++] = lo + (hi << 4);
	}

	return j;
}

void
testNetBios()
{
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

BOOL WriteSlot(HANDLE hSlot, const char* lpszMessage)
{
   BOOL fResult;
   DWORD cbWritten;

   fResult = ::WriteFile(hSlot,
	 lpszMessage,
	 strlen(lpszMessage),
	 &cbWritten,
	 (LPOVERLAPPED)NULL);

   if (!fResult)
   {
	  printf("WriteFile failed with %d.\n", GetLastError());
	  return FALSE;
   }

   printf("Slot written to successfully.\n");

   return TRUE;
}

BOOL ReadSlot(HANDLE hSlot)
{
   BOOL fResult;
   DWORD cbRead;

   char buffer[256];

   fResult = ReadFile(hSlot, buffer, sizeof(buffer) - 1, &cbRead, NULL);
   if (!fResult)
   {
	  printf("ReadFile failed with %d.\n", GetLastError());
	  return FALSE;
   }

   buffer[cbRead] = 0;
   printf("Read from slot: %s\n", buffer);

   return TRUE;
}

void
testMailSlot()
{
	printf("1 for server, 2 for client:\n");
	char buffer[256];
	fgets(buffer, countof(buffer), stdin);

	if (buffer[0] == '1')
	{
	char slotName[256];
	printf("slot name (e.g. \\\\.\\mailslot\\foo):\n");
	fgets(slotName, countof(slotName), stdin);

		HANDLE hFile = CreateMailslotA(slotName, 0, -1, NULL);
	   if (hFile == INVALID_HANDLE_VALUE)
	   {
		  printf("CreateMailslot failed with %d.\n", GetLastError());
		  return;
	   }

	  printf("Server is running...\n");

		for (;;)
		{
		   ReadSlot(hFile);
		}
	}
	else for (;;)
	{

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

	   if (hFile == INVALID_HANDLE_VALUE)
	   {
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

class MyXmlParser: public xml::ExpatParser<MyXmlParser>
{
protected:
	size_t m_indent;

public:
	MyXmlParser()
	{
		m_indent = 0;
	}

	void
	onStartElement(
		const char* name,
		const char** attributes
		)
	{
		printIndent();
		printf("<%s", name);

		if (*attributes)
		{
			printf("\n");
			m_indent++;

			while (*attributes)
			{
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
	onEndElement(const char* name)
	{
		m_indent--;
		printIndent();
		printf("</%s>\n", name);
	}

	void
	onCharacterData(
		const char* string,
		size_t length
		)
	{
		printIndent();
		printf("%s\n", sl::String(string, length).sz());
	}

protected:
	void
	printIndent()
	{
		for (size_t i = 0; i < m_indent; i++)
			printf("  ");
	}
};

void
testXml()
{
	bool result;

	MyXmlParser parser;
	result = parser.parseFile("c:/projects/playground/doxygen/xml/index.xml", 100);
	if (!result)
	{
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("success\n");
}

#endif

void
testFileEnum()
{
	io::FileEnumerator fileEnum;
	bool result = fileEnum.openDir(".");
	if (!result)
	{
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	while (fileEnum.hasNextFile())
	{
		sl::String fileName = fileEnum.getNextFileName();
		printf("%s\n", fileName.sz());
	}
}

void
testStringCase()
{
	sl::String s = "Hui Govno i Muravei";
	s.makeLowerCase();
	printf("s = %s\n", s.sz());

	s.makeUpperCase();
	printf("s = %s\n", s.sz());
}

//..............................................................................

enum CmdLineSwitchKind
{
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

class CmdLineParser: public sl::CmdLineParser<CmdLineParser, CmdLineSwitchTable>
{
	friend class sl::CmdLineParser<CmdLineParser, CmdLineSwitchTable>;

protected:
	bool
	onValue(const sl::StringRef& value)
	{
		printf("onValue (%s)\n", value.sz());
		return true;
	}

	bool
	onSwitch(
		SwitchKind switchKind,
		const sl::StringRef& value
		)
	{
		switch (switchKind)
		{
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

class ServerThread: public sys::ThreadImpl<ServerThread>
{
public:
	sys::Event m_startEvent;

public:
	intptr_t threadFunc()
	{
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
		if (!result)
		{
			printf("failed: %s\n", err::getLastErrorDescription().sz());
			return -1;
		}


		printf("client connected from: %s\n", addr.getString().sz());

		for (;;)
		{
			char buffer[1024];
			int x = clientSocket.recv(buffer, sizeof(buffer) - 1);

			if (x > 0)
			{
				buffer[x] = 0;
				printf("client sent %d bytes: %s\n", x, buffer);
			}
			else if (x < 0)
			{
				printf("client reset.\n");
				break;
			}
			else
			{
				printf("client disconnected.\n");
				break;
			}
		}

		printf("server thread done.\n");

		return 0;
	}
};

bool
testConn()
{
/*	printf("startion server thread...\n");

	ServerThread thread;
	thread.start();
	thread.m_startEvent.wait();
*/
	io::SockAddr addr;
	addr.parse("127.0.0.1:1002");

	printf("connecting to %s...\n", addr.getString().sz());

	io::Socket socket;
	bool result =
		socket.open(AF_INET, SOCK_STREAM, IPPROTO_TCP) &&
		socket.connect(addr);

	if (!result)
	{
		printf("failed: %s\n", err::getLastErrorDescription().sz());
		return false;
	}

	static char data[] = "hui govno i muravei";

	printf("sending %d bytes...\n", sizeof (data));
	socket.send(data, sizeof(data));

	printf("closing...\n");
	socket.close();

//	printf ("waiting for server thread...\n");
//	thread.wait ();

	printf("done.\n");
	return true;
}

bool
testSerial()
{
	char const* port = "COM1";

	printf("opening %s...\n", port);

	io::SerialSettings settings;
	settings.m_baudRate = 38400;
	settings.m_dataBits = 8;
	settings.m_stopBits = io::SerialStopBits_1;
	settings.m_flowControl = io::SerialFlowControl_None;
	settings.m_parity = io::SerialParity_Odd;

	io::Serial serial;
	bool result =
		serial.open(port) &&
		serial.setSettings(&settings);

	if (!result)
	{
		printf("failed: %s\n", err::getLastErrorDescription().sz());
		return false;
	}

#if (_AXL_OS_WIN)
	serial.m_serial.setWaitMask(EV_ERR);

	for (;;)
	{
		char buf[1024] = { 0 };

		size_t result = serial.read(buf, sizeof(buf));
		if (result == -1)
		{
			printf("failed: %s\n", err::getLastErrorDescription().sz());
			return false;
		}

		buf[result] = 0;
		printf("received: %s\n", buf);

		dword_t events;
		bool result2 = serial.m_serial.wait(&events);
		if (!result2)
		{
			printf("failed: %s\n", err::getLastErrorDescription().sz());
			return false;
		}

		printf("events: %x\n", events);

		dword_t errors;
		COMSTAT stat;
		result2 = serial.m_serial.clearError(&errors, &stat);
		if (!result2)
		{
			printf("failed: %s\n", err::getLastErrorDescription().sz());
			return false;
		}

		printf("errors: %x\n", errors);
	}
#endif

	size_t totalSize = 0;
	size_t targetSize = 20 * 1024;

	printf("\rwriting... %3d%%; bytes: %5d", totalSize * 100 / targetSize, totalSize);

	while (totalSize < targetSize)
	{
		static char data[] = "123456789abcdef";
		size_t size = serial.write(data, sizeof(data));
		if (size == -1)
		{
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
testStringReplace()
{
	sl::String title = "contains: < (less-than) > (greater-than) & (ampersand)";
	title.replace("<", "&lt;");
	title.replace(">", "&gt;");
	title.replace("&", "&amp;");

	printf("title = %s\n", title.sz());
}

//..............................................................................

void
testEvent()
{
	for (int i = 0; i < 10; i++)
	{
		sys::NotificationEvent e;

		bool r = e.wait(500);
		if (!r)
			printf("[%s] ---wait failed: %s\n", sys::Time(sys::getTimestamp ()).format("%h:%m:%s.%l").sz(), err::getLastErrorDescription().sz());
		else
			printf("[%s] ---wait SUCCEEDED! (WTF?!)\n", sys::Time(sys::getTimestamp ()).format("%h:%m:%s.%l").sz());
	}
}

//..............................................................................

void
testBase32()
{
	sl::Array<char> source;

	for (int i = 0; i < 500; i++)
	{
		size_t size = rand() % 64 + 16;
		source.setCount(size);
		for (size_t j = 0; j < size; j++)
			source[j] = rand() % 256;

		sl::String enc = enc::Base32Encoding::encode(source, size);
		sl::Array<char> dec = enc::Base32Encoding::decode(enc);

		ASSERT(dec.getCount() == size && memcmp(dec, source, size) == 0);
	}
}

//..............................................................................

void
testTime()
{
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
testSerial2()
{
	bool result;

	io::Serial serial;
	result = serial.open("/dev/ttyUSB0");

	io::SerialSettings settings;
	settings.m_baudRate = 115200;
	settings.m_dataBits = 8;
	settings.m_stopBits = io::SerialStopBits_1;
	settings.m_flowControl = io::SerialFlowControl_None;
	settings.m_parity = io::SerialParity_None;

	result = serial.setSettings(&settings);

	serial.write("\r", 1);

	char buffer[1024];
	size_t size = serial.read(buffer, sizeof(buffer));

	sl::String s0 = "takie dela";

	sl::String s = enc::HexEncoding::encode(buffer, size);
	printf("incoming: %s\n", s.sz());
}

//..............................................................................

void
testSymbolicLinks()
{
	sl::String target;
	bool result = io::getSymbolicLinkTarget(&target, "/home/vladimir/a");
	if (!result)
	{
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("target: %s\n", target.sz());
}

//..............................................................................

void
testBoyerMoore()
{
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

struct RecordFileHdr
{
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

struct Record
{
	uint32_t m_signature; // RecordFileConst_RecordSignature
	uint32_t m_code;
	uint64_t m_timestamp;
	uint32_t m_dataSize;
	uint32_t _m_padding;

	// followed by record data (if any)
};

void
testIoPerformance()
{
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
	if (!result)
	{
		printf("can't open %s: %s\n", fileNameSrc, err::getLastErrorDescription().sz());
		return;
	}

	uint64_t baseTimestamp;
	uint64_t time;
	uint64_t offset;

	sl::Array<char> buffer;

	struct Rec: Record
	{
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

	if (!result)
	{
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

	while (offset < endOffset)
	{
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

	if (!result)
	{
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
	while (offset < hdr.m_totalRecordSize)
	{
#if (!_DUMMY_WRITE)
		Record rec;
		simpleFileSrc.read(&rec, sizeof(rec));
		if (rec.m_dataSize)
		{
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
	public sys::ThreadImpl<RwLockThread>
{
public:
	enum
	{
		MaxSleepTime = 10,
	};

protected:
	sys::ReadWriteLock* m_lock;
	size_t m_index;
	size_t m_iterationCount;
	bool m_isWriter;

public:
	RwLockThread(
		sys::ReadWriteLock* lock,
		size_t index,
		size_t iterationCount,
		bool isWriter
		)
	{
		m_lock = lock;
		m_index = index;
		m_iterationCount = iterationCount;
		m_isWriter = isWriter;
	}

	void
	threadFunc()
	{
		uint64_t tid = sys::getCurrentThreadId();

		if (m_isWriter)
			for (size_t i = 0; i < m_iterationCount; i++)
			{
				m_lock->writeLock();
				ASSERT(g_readerCount == 0);
				ASSERT(g_writerCount == 0);
				sys::atomicInc(&g_writerCount);
				printf("writer #%d (TID:%llx) is writing (%d/%d)...\n", m_index, tid, i, m_iterationCount);
				sys::sleep(rand() % MaxSleepTime);
				sys::atomicDec(&g_writerCount);
				m_lock->writeUnlock();
			}
		else
			for (size_t i = 0; i < m_iterationCount; i++)
			{
				m_lock->readLock();
				ASSERT(g_writerCount == 0);
				sys::atomicInc(&g_readerCount);
				printf("reader #%d (TID:%llx) is reading (%d/%d; %d readers total)...\n", m_index, tid, i, m_iterationCount, g_readerCount);
				sys::sleep(rand() % MaxSleepTime);
				sys::atomicDec(&g_readerCount);
				m_lock->readUnlock();
			}
	}
};

void
testReadWriteLock()
{
	enum
	{
		ReaderCount = 10,
		ReaderIterationCount = 10,

		WriterCount = 5,
		WriterIterationCount = 10,
	};

	sys::ReadWriteLock lock;
	lock.create();

	sl::List<RwLockThread> threadList;

	for (size_t i = 0; i < ReaderCount; i++)
	{
		RwLockThread* thread = AXL_MEM_NEW_ARGS(RwLockThread, (&lock, i, ReaderIterationCount, false));
		thread->start();
		threadList.insertTail(thread);
	}

	for (size_t i = 0; i < ReaderCount; i++)
	{
		RwLockThread* thread = AXL_MEM_NEW_ARGS(RwLockThread, (&lock, i, WriterIterationCount, true));
		thread->start();
		threadList.insertTail(thread);
	}

	sl::Iterator<RwLockThread> it = threadList.getHead();
	for (; it; it++)
		it->waitAndClose();
}

//..............................................................................

template <typename T>
void
foo(T* p)
{
	printf("foo - 1\n");
}

template <typename T>
void
foo(const T* p)
{
	printf("foo - 2\n");
}

struct Point: sl::ListLink
{
	int m_x;
	int m_y;
};

void
testConstList()
{
	sl::List<Point> list;

	Point* point = AXL_MEM_NEW(Point);
	point->m_x = 10;
	point->m_y = 20;
	list.insertTail(point);

	point = AXL_MEM_NEW(Point);
	point->m_x = 30;
	point->m_y = 40;
	list.insertTail(point);

	sl::Iterator<Point> it = list.getHead();
	sl::ConstIterator<Point> it2 = it;
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
testBitIdx()
{
	size_t size = sl::getAllocSize(0);
	ASSERT(size == 0);

	size_t x = sl::getPowerOf2Ge(0);
	ASSERT(x == 0);

	size_t y = sl::getHiBit(0);
	ASSERT(y == 0);

	size_t z = sl::getLoBit(0);
	ASSERT(z == 0);

	for (size_t i = 1; i < 128 * 1024 * 1024; i++)
	{
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

class PcapThread: public sys::ThreadImpl<PcapThread>
{
public:
	io::Pcap m_pcap;

public:
	void threadFunc()
	{
		for (;;)
		{
			char buffer[1024];
			size_t result = m_pcap.read(buffer, sizeof(buffer));
			printf("result: %d\n", result);

			if (result == 0)
				sys::sleep(1000);
		}
	}
};

void
testPcap()
{
#if (_AXL_OS_WIN)
	::SetDllDirectoryW(io::win::getSystemDir() + L"\\npcap");
#endif

	const char* version = pcap_lib_version();
	printf("version: %s\n", version);

	pcap_if* ifaceList = NULL;
	char errorBuffer[PCAP_ERRBUF_SIZE] = { 0 };
	int result = pcap_findalldevs(&ifaceList, errorBuffer);
	if (result == -1)
	{
		printf("error: %s\n", errorBuffer);
		return;
	}

	size_t count = 0;

	sl::String s;

	for (pcap_if* iface = ifaceList; iface; iface = iface->next, count++)
		printf("%s - %s\n", iface->name, iface->description);

	PcapThread thread;

	result = thread.m_pcap.openLive("lo", 4096, true, 200);
	if (!result)
	{
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

class UdpThread: public sys::ThreadImpl<UdpThread>
{
protected:
	io::Socket m_socket;

public:
	void threadFunc()
	{
		io::Socket socket;

		bool result =
			socket.open(AF_INET, SOCK_DGRAM, IPPROTO_UDP) &&
			socket.bind(io::SockAddr((uint_t)0, 1234));

		printf("socket opened: %d\n", result);

		for (;;)
		{
			char buffer[256];
			io::SockAddr addr;
			size_t result = socket.recvFrom(buffer, sizeof(buffer), &addr);
			printf("received: %d from: %s\n", result, addr.getString().sz());
		}
	}
};

void
testUdp()
{
	UdpThread thread;
	thread.start();

	io::SockAddr addr(0x7f000001, 1234);

	for (;;)
	{
		io::Socket socket;
		socket.open(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		size_t result = socket.sendTo(NULL, 0, addr);
		printf("result: %zd\n", result);
		sys::sleep(1000);
	}
}

//..............................................................................

void testProcName()
{
	uint_t pidTable[] =
	{
		1,
		272,
		279,
		476,
	};

	for (size_t i = 0; i < countof(pidTable); i++)
	{
		uint_t pid = pidTable[i];
		printf("%d: %s\n", pid, sys::getProcessImageName(pid).sz());
	}
}

//..............................................................................

#if (_AXL_OS_WIN)

void testUnnamedPipes()
{
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
printX509Name(X509_name_st* name)
{
	int count = X509_NAME_entry_count(name);
	for (int i = 0; i < count; i++)
	{
		X509_NAME_ENTRY* entry = X509_NAME_get_entry(name, i);
		ASN1_OBJECT* object = X509_NAME_ENTRY_get_object(entry);
		ASN1_STRING* data = X509_NAME_ENTRY_get_data(entry);

		printf("    %4d: %s: %s\n", OBJ_obj2nid(object), cry::getAsn1ObjectString(object).sz(), cry::getAsn1StringString(data).sz());
	}
}


void
printSslCertificate(X509* cert)
{
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
copyPrintSslCertificate(X509* cert0)
{
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
	)
{
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
	)
{
	printf("sslInfoCallback(0x%x, %d) -> %s\n", type, value, SSL_state_string_long(ssl));
}

void testSsl()
{
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

	if (!result)
	{
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

	if (!result)
	{
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	ssl.setVerify(SSL_VERIFY_PEER, sslVerifyCallback);
	ssl.setInfoCallback(sslInfoCallback);
	ssl.setBio(bio.detach());
	ssl.setConnectState();

	printf("handshaking...\n");

	result = ssl.doHandshake();
	if (!result)
	{
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return;
	}

	printf("------------------------\n");

	STACK_OF(X509)* chain = ssl.getPeerCertificateChain();

	printf("Peer certificate(s):\n");

	int count = sk_X509_num(chain);
	if (count == 1)
	{
		X509* cert = sk_X509_value(chain, 0);
		printSslCertificate(cert);
		copyPrintSslCertificate(cert);
	}
	else for (int i = 0; i < count; i++)
	{
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

	for (size_t i = 0; i < 10; i++)
	{
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
exec(const sl::StringRef& commandLine) // returns on failure only
{
	char buffer[256];
	sl::Array<char*> argv(ref::BufKind_Stack, buffer, sizeof(buffer));

	sl::String string = commandLine;

	size_t length = string.getLength();
	for (;;)
	{
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

	if (argv.isEmpty())
	{
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
	)
{
	sl::String hexString;
	char buffer[4 * 1024];

	for (;;)
	{
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
	)
{
	int result;
	int tmp = AXL_MAX(childStdoutFd, childStderrFd);
	int selectFd = AXL_MAX(tmp, STDIN_FILENO) + 1;

	for (;;)
	{
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
testExecPassthrough(const sl::StringRef& cmdLine)
{
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
	switch (pid)
	{
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
testPty(const sl::StringRef& cmdLine)
{
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
	switch (pid)
	{
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

size_t g_indentSlot;

size_t
incrementIndent(intptr_t delta)
{
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
	)
{
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
	)
{
	size_t indent = incrementIndent(-1) - 1;

	if (!frameBase) // abandoned
	{
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
		regRetBlock->m_rax,
		regRetBlock->m_rax
		);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define _SPY_TEST_TRACE_HOOKING_MODULE   1
// #define _SPY_TEST_TRACE_HOOKING_FUNCTION 1

bool
spyModule(const spy::ModuleIterator& moduleIt)
{
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

	for (; it; it++)
	{
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
			it.getSymbolName() == "TlsSetValue"))
		{
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
			 it.getSymbolName() == "pthread_setspecific"))
		{
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
			it.getSymbolName() == "_pthread_setspecific"))
		{
			printf("  skipping %s for now...\n", functionName.sz());
			continue;
		}

		size_t slotVmAddr = it.getSlotVmAddr();
		bool isUnique = hookSet.addIfNotExists(slotVmAddr, true);
		if (!isUnique)
		{
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
spyGlobalTest()
{
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
	)
{
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
	)
{
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
	)
{
	if (!frameBase) // abandoned
	{
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
spyParamTest()
{
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
foo(int a) // one argument is all it takes for ret <n> to mess up the stack
{
	printf("foo(%d)\n", a);
	return 10;
}

spy::HookAction
fooHookEnter(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
	)
{
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
	)
{
	spy::RegRetBlock* regRetBlock = (spy::RegRetBlock*)(frameBase + spy::FrameOffset_RegRetBlock);

	printf(
		"fooHookLeave(func: %p, param: %p, frame: %p, retval: %zd/0x%zx)\n",
		targetFunc,
		callbackParam,
		(void*)frameBase,
		regRetBlock->m_rax,
		regRetBlock->m_rax
		);
}

} // namespace spy_stdcall_test

int
spyStdcallTest()
{
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

//..............................................................................

int
receiptTest()
{
	OpenSSL_add_all_algorithms();
	cry::registerCryptoErrorProviders();

	bool result;
	int retCode;

#if (_AXL_OS_WIN)
	const char* filePath = "C:/Projects/playground/receipt/xcode-receipt";
#elif (_AXL_OS_DARWIN)
	const char* filePath = "/Users/vladimir/Projects/playground/receipt/xcode-receipt";
#endif

	io::SimpleMappedFile file;
	result = file.open(filePath, io::FileFlag_ReadOnly);
	if (!result)
	{
		printf("file error: %s\n", err::getLastErrorDescription().sz());
		return -1;
	}

	printf("file size: %d\n", file.getFileSize());

#if (_AXL_OS_DARWIN)
	uint_t flags = 0;
#else
	uint_t flags = cry::VerifyAppReceiptFlag_SkipHashCheck;
#endif

	cry::AppStoreReceipt receipt;
	result = cry::verifyAppStoreReceipt(&receipt, file.p(), file.getMappingSize(), flags);
	if (!result)
	{
		printf("verify error: %s\n", err::getLastErrorDescription().sz());
		return -1;
	}

	printf(
		"RECEIPT:\n"
		"  bundle ID:               %s\n"
		"  app version:             %s\n"
		"  original app version:    %s\n"
		"  receipt creation date:   %s\n"
		"  receipt expiration date: %s\n"
		"  ------------------------\n"
		"  SHA1 hash: %s\n"
		"  opaque:    %s\n",
		receipt.m_bundleId.sz(),
		receipt.m_appVersion.sz(),
		receipt.m_originalAppVersion.sz(),
		receipt.m_receiptCreationDate.sz(),
		receipt.m_receiptExpirationDate.sz(),
		enc::HexEncoding::encode(receipt.m_sha1Hash, receipt.m_sha1Hash.getCount()).sz(),
		enc::HexEncoding::encode(receipt.m_opaque, receipt.m_opaque.getCount()).sz()
		);

	return 0;
}

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

	FILE* f = stdout;

	g::getModule()->setTag("axl_test_con");
	srand((uint_t)sys::getTimestamp());

#if (_AXL_OS_WIN)
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
#endif

	receiptTest();

	return 0;
}

//..............................................................................
