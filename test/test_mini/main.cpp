#include "pch.h"
#include "axl_g_WarningSuppression.h"

#if (_AXL_ENV == AXL_ENV_WIN)

#include "axl_err_NtError.h"

//.............................................................................

#define STATUS_NO_MORE_FILES   0x80000006
#define STATUS_NO_MORE_ENTRIES 0x8000001a

#define DIRECTORY_QUERY               0x0001
#define DIRECTORY_TRAVERSE            0x0002
#define DIRECTORY_CREATE_OBJECT       0x0004
#define DIRECTORY_CREATE_SUBDIRECTORY 0x0008

enum FILE_INFORMATION_CLASS
{
	FileDirectoryInformation = 1,
};

//.............................................................................

struct IO_STATUS_BLOCK
{
	LONG Status;
	ULONG Information;
};

typedef IO_STATUS_BLOCK* PIO_STATUS_BLOCK;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct UNICODE_STRING 
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
};

typedef UNICODE_STRING* PUNICODE_STRING;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct OBJECT_ATTRIBUTES 
{
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
};

typedef OBJECT_ATTRIBUTES* POBJECT_ATTRIBUTES;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct FILE_DIRECTORY_INFORMATION 
{ 
	ULONG NextEntryOffset;
	ULONG FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG FileAttributes;
	ULONG FileNameLength;
	WCHAR FileName[1]; 
};

typedef FILE_DIRECTORY_INFORMATION* PFILE_DIRECTORY_INFORMATION;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct OBJECT_DIRECTORY_INFORMATION 
{
	UNICODE_STRING Name;
	UNICODE_STRING TypeName;
};

typedef OBJECT_DIRECTORY_INFORMATION* POBJECT_DIRECTORY_INFORMATION;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef
NTSTATUS 
NTAPI 
NtQueryDirectoryFile (
	IN HANDLE FileHandle,
	IN HANDLE Event,
	IN PVOID ApcRoutine,
	IN PVOID ApcContext,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN INT FileInformationClass,
	IN BOOLEAN ReturnSingleEntry,
	IN PVOID FileName,
	IN BOOLEAN RestartScan 
	);

typedef
NTSTATUS 
NTAPI
NtOpenDirectoryObject (
	OUT PHANDLE DirectoryHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
	);

typedef
NTSTATUS 
NTAPI
NtQueryDirectoryObject(
	IN HANDLE DirectoryHandle,
	OUT PVOID Buffer,
	IN ULONG Length,
	IN BOOLEAN ReturnSingleEntry,
	IN BOOLEAN RestartScan,
	IN PULONG Context,
	OUT PULONG ReturnLength
	);

typedef
NTSTATUS 
NTAPI 
NtOpenSymbolicLinkObject (
	OUT PHANDLE LinkHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
	);

typedef
NTSTATUS 
NTAPI 
NtQuerySymbolicLinkObject (
	IN HANDLE LinkHandle,
	OUT PUNICODE_STRING LinkTarget,
	OUT PULONG ReturnedLength
	);

//.............................................................................

NtQueryDirectoryFile* ntQueryDirectoryFile = NULL;
NtOpenDirectoryObject* ntOpenDirectoryObject = NULL;
NtQueryDirectoryObject* ntQueryDirectoryObject = NULL;
NtOpenSymbolicLinkObject* ntOpenSymbolicLinkObject = NULL;
NtQuerySymbolicLinkObject* ntQuerySymbolicLinkObject = NULL;

void
initNtFunctions ()
{
	HMODULE ntdll = ::GetModuleHandleW (L"ntdll.dll");
	ASSERT (ntdll);

	ntQueryDirectoryFile = (NtQueryDirectoryFile*) ::GetProcAddress (ntdll, "NtQueryDirectoryFile");
	ntOpenDirectoryObject = (NtOpenDirectoryObject*) ::GetProcAddress (ntdll, "NtOpenDirectoryObject");
	ntQueryDirectoryObject = (NtQueryDirectoryObject*) ::GetProcAddress (ntdll, "NtQueryDirectoryObject");
	ntOpenSymbolicLinkObject = (NtOpenSymbolicLinkObject*) ::GetProcAddress (ntdll, "NtOpenSymbolicLinkObject");
	ntQuerySymbolicLinkObject = (NtQuerySymbolicLinkObject*) ::GetProcAddress (ntdll, "NtQuerySymbolicLinkObject");

	ASSERT (ntQueryDirectoryFile);
	ASSERT (ntOpenDirectoryObject);
	ASSERT (ntQueryDirectoryObject);
	ASSERT (ntOpenSymbolicLinkObject);
	ASSERT (ntQuerySymbolicLinkObject);
}

#endif

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

void
printAdapterAddress (const IP_ADDR_STRING* ipAddrString)
{
	for (size_t i = 1; ipAddrString; ipAddrString = ipAddrString->Next, i++)
	{
		printf ("  #%-2d IP:   %s\n", i, ipAddrString->IpAddress.String);
		printf ("  #%-2d MASK: %s\n", i, ipAddrString->IpMask.String);
	}
}

void
printAdapterInfo (IP_ADAPTER_INFO* ipAdapter)
{
	switch (ipAdapter->Type)
	{
	case MIB_IF_TYPE_LOOPBACK:
		printf ("MIB_IF_TYPE_LOOPBACK\n");
		break;

	case MIB_IF_TYPE_ETHERNET:
		printf ("MIB_IF_TYPE_ETHERNET\n");
		break;

	case IF_TYPE_IEEE80211:
		printf ("IF_TYPE_IEEE80211\n");
		break;

	case MIB_IF_TYPE_PPP:
		printf ("MIB_IF_TYPE_PPP\n");
		break;

	case MIB_IF_TYPE_TOKENRING:
		printf ("MIB_IF_TYPE_TOKENRING\n");
		break;

	case MIB_IF_TYPE_SLIP:
		printf ("MIB_IF_TYPE_SLIP\n");
		break;

	default:
		printf ("unknown adapter type #%d\n", ipAdapter->Type);
	}

	printf ("AdapterName = %s\n", ipAdapter->AdapterName);
	printf ("Description = %s\n", ipAdapter->Description);
	printf ("MAC         = %02x:%02x:%02x:%02x:%02x:%02x\n", 
		ipAdapter->Address [0],
		ipAdapter->Address [1],
		ipAdapter->Address [2],
		ipAdapter->Address [3],
		ipAdapter->Address [4],
		ipAdapter->Address [5]
		);
	printf ("DHCP        = %d\n", ipAdapter->DhcpEnabled);

	printf ("ADDRESS:\n");
	printAdapterAddress (&ipAdapter->IpAddressList);

	printf ("GATEWAY:\n");
	printAdapterAddress (&ipAdapter->GatewayList);

	printf ("DHCP:\n");
	printAdapterAddress (&ipAdapter->DhcpServer);

	printf ("---\n\n");
}

void
testWinNetworkAdapterList ()
{
	printf ("Using GetAdaptersInfo...\n\n");

	dword_t size = 0;
	dword_t error = ::GetAdaptersInfo (NULL, &size);
	if (error != ERROR_BUFFER_OVERFLOW)
	{
		printf ("GetAdaptersInfo failed (%d)\n", error);
		return;
	}

	char buffer [256];
	rtl::Array <char> bufferArray (ref::BufKind_Stack, buffer, sizeof (buffer));
	bufferArray.setCount (size);

	IP_ADAPTER_INFO* ipAdapter = (IP_ADAPTER_INFO*) bufferArray.a ();
	error = ::GetAdaptersInfo (ipAdapter, &size);
	if (error != ERROR_SUCCESS)
	{
		printf ("GetAdaptersInfo failed (%d)\n", error);
		return;
	}

	size_t count = 0;
	for (; ipAdapter; ipAdapter = ipAdapter->Next, count++)
		printAdapterInfo (ipAdapter);

	printf ("%d adapters found\n---------------------------------\n\n", count);
}

void
testWinNetworkAdapterList2 ()
{
	printf ("Using WinIoctl...\n\n");

	printf (
		"sizeof (sockaddr) = %d\n"
		"sizeof (sockaddr_in) = %d\n"
		"sizeof (sockaddr_in6) = %d\n"
		"sizeof (sockaddr_storage) = %d\n"
		"sizeof (io::SockAddr) = %d\n",
		sizeof (sockaddr),
		sizeof (sockaddr_in),
		sizeof (sockaddr_in6),
		sizeof (sockaddr_storage),
		sizeof (io::SockAddr)
		);

	io::win::Socket socket;
	bool result = socket.wsaOpen (AF_INET, SOCK_DGRAM, 0);
	if (!result)
	{
		printf ("socket.wsaOpen failed (%s)\n", err::getLastErrorDescription ().cc ());
		return;
	}

	size_t size = 16 * sizeof (INTERFACE_INFO);
	rtl::Array <char> buffer;
	buffer.setCount (size);

	dword_t actualSize;

	for (;;)
	{	
		int result = ::WSAIoctl (
			socket,
			SIO_GET_INTERFACE_LIST,
			NULL, 
			0,
			buffer.a (),
			size,
			&actualSize,
			NULL, 
			NULL
			);

		if (result == ERROR_SUCCESS)
			break;

		dword_t error = ::WSAGetLastError ();
		if (error != WSAENOBUFS)
		{
			printf ("WSAIoctl failed (%s)\n", err::Error (error)->getDescription ().cc ());
			return;
		}

		size *= 2;
		buffer.setCount (size);
	}

	const INTERFACE_INFO* iface = (const INTERFACE_INFO*) buffer.ca ();
	size_t ifaceCount = actualSize / sizeof (INTERFACE_INFO);
	
	for (size_t i = 0; i < ifaceCount; iface++, i++) 
	{
		printf ("Interface #%d\n", i);
		printf ("  Address   = %s\n", io::getSockAddrString ((const sockaddr*) &iface->iiAddress).cc ());
		printf ("  Broadcast = %s\n", io::getSockAddrString ((const sockaddr*) &iface->iiBroadcastAddress).cc ());
		printf ("  Netmask   = %s\n", io::getSockAddrString ((const sockaddr*) &iface->iiNetmask).cc ());

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

	printf ("%d adapters found\n---------------------------------\n\n", ifaceCount);
}

#endif

//.............................................................................

void
testNetworkAdapterList ()
{
	rtl::StdList <io::NetworkAdapterDesc> adapterList;
	io::createNetworkAdapterDescList (&adapterList);

	rtl::Iterator <io::NetworkAdapterDesc> adapterIt = adapterList.getHead ();
	for (; adapterIt; adapterIt++)
	{
		io::NetworkAdapterDesc* adapter = *adapterIt;

		printf ("Name        = %s\n", adapter->getName ().cc ());
		printf ("Description = %s\n", adapter->getDescription ().cc ());
		printf ("Type        = %s\n", io::getNetworkAdapterTypeString (adapter->getType ()));
		printf ("Flags       = %s\n", io::getNetworkAdapterFlagString (adapter->getFlags ()).cc ());

		rtl::ConstList <io::NetworkAdapterAddress> addressList = adapter->getAddressList ();
		rtl::Iterator <io::NetworkAdapterAddress> addressIt = addressList.getHead ();
		for (size_t i = 1; addressIt; addressIt++, i++)
		{
			io::NetworkAdapterAddress* address = *addressIt;
			
			uint_t family = address->m_address.m_addr.sa_family;
			printf ("%-11s = %s", 
				io::getSockAddrFamilyString (family),
				address->m_address.getString ().cc ()
				);

			if (family == AF_INET)
			{
				io::SockAddr netMask;
				netMask.createNetMask_ip4 (address->m_netMaskBitCount);
				printf (" (mask %s)\n", netMask.getString ().cc ());
			}
			else
			{
				printf ("/%d\n", address->m_netMaskBitCount);
			}
		}

		printf ("\n");
	}

	printf ("%d adapters found\n", adapterList.getCount ());
}

//.............................................................................

void
testParseFormatIp6 ()
{
	printf ("main ()\n");

	bool result;

/*
	io::SockAddr sockAddr;
	result = sockAddr.parse ("tibbo.com:80");
	printf ("result = %d, addr = %s\n", result, sockAddr.getString ().cc ());

	result = sockAddr.parse ("::123e:325f:0%24393");
	printf ("result = %d, addr = %s\n", result, sockAddr.getString ().cc ());
*/

	sockaddr_in6 addr = { 0 };
	addr.sin6_family = AF_INET6;

	for (;;)
	{
		uint16_t* ip = (uint16_t*) &addr.sin6_addr;

		ip [0] = rand () % 2 ? rand () : 0;
		ip [1] = rand () % 2 ? rand () : 0;
		ip [2] = rand () % 2 ? rand () : 0;
		ip [3] = rand () % 2 ? rand () : 0;
		ip [4] = rand () % 2 ? rand () : 0;
		ip [5] = rand () % 2 ? rand () : rand () % 2 ? 0xffff : 0;
		ip [6] = rand () % 2 ? rand () : 0;
		ip [7] = rand () % 2 ? rand () : 0;
#if (_AXL_ENV == AXL_ENV_WIN)
		addr.sin6_port = rand () % 2 ? rand () : 0;
		addr.sin6_scope_id = rand () % 2 ? rand () : 0;
#endif

		rtl::String addrString = io::getSockAddrString ((const sockaddr*) &addr).cc ();
		printf ("addr1 = %s\n", addrString.cc ());

		char addrString2 [1024] = { 0 };
		dword_t size = sizeof (addrString2);

#if (_AXL_ENV == AXL_ENV_WIN)
		WSAAddressToStringA ((sockaddr*) &addr, sizeof (addr), NULL, addrString2, &size);
#elif (_AXL_ENV == AXL_ENV_POSIX)
		inet_ntop (AF_INET6, &addr.sin6_addr, addrString2, size);
#endif

		printf ("addr2 = %s\n\n", addrString2);
		ASSERT (addrString.cmp (addrString2) == 0);

		sockaddr_in6 addr2;
		result = io::parseSockAddr ((sockaddr*) &addr2, sizeof (addr2), addrString);
		ASSERT (result && memcmp (&addr, &addr2, sizeof (addr)) == 0);
	}
}

//.............................................................................

void
testSocketIp6 ()
{
	bool result;

	io::Socket socket;
	result = socket.open (AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (!result)
	{
		printf ("socket.open failed (%s)\n", err::getLastErrorDescription ().cc ());
		return;
	}
	
	io::SockAddr addr;
	result = addr.parse ("[::1]:80");
	if (!result)
	{
		printf ("socket.open failed (%s)\n", err::getLastErrorDescription ().cc ());
		return;
	}

	result = socket.connect (addr);
	if (!result)
	{
		printf ("socket.connect failed (%s)\n", err::getLastErrorDescription ().cc ());
		return;
	}

}

//.............................................................................

void
testAddrInfoIp6 ()
{
	const char* name = "tibbo.com";

	rtl::Array <io::SockAddr> addrArray;
	bool result = io::resolveHostName (&addrArray, name, AF_INET6);
	if (!result)
	{
		printf ("io::resolveHostName failed (%s)\n", err::getLastErrorDescription ().cc ());
		return;
	}

	printf ("host name %s resolved to:\n", name);

	size_t count = addrArray.getCount ();
	for (size_t i = 0; i < count; i++)
		printf ("    %s\n", addrArray [i].getString ().cc ());
}

//.............................................................................

void
testDynamicLibrary ()
{
	mt::DynamicLibrary dl;
	
	bool result = dl.load ("libc.so.6");
	if (!result)
	{
		printf ("dl.load failed: %s\n", err::getLastError ().getDescription ().cc ());
		return;
	}

	typedef int Printf (const char*, ...);
	Printf* prn = (Printf*) dl.getFunction ("printf");
	if (!prn)
	{
		printf ("dl.load failed: %s\n", err::getLastError ().getDescription ().cc ());
		return;
	}

	prn ("hui govno i muravei %d\n", 123);
}

//.............................................................................

void 
bar ()
{
	printf ("bar -- throwing...\n");
	AXL_MT_LONG_JMP_THROW ();	
}

void 
testLongJmpTry ()
{
	AXL_MT_BEGIN_LONG_JMP_TRY ()
	{
		bar ();
	}
	AXL_MT_LONG_JMP_CATCH ()
	{
		printf ("exception caught\n");
	}
	AXL_MT_END_LONG_JMP_TRY ()

	AXL_MT_BEGIN_LONG_JMP_TRY ()
	{
		bar ();
	}
	AXL_MT_END_LONG_JMP_TRY ()

	printf ("done\n");
}

//.............................................................................

void
testRegExp ()
{
	fsm::StdRegExpNameMgr nameMgr;

	nameMgr.addName ("ws",  "[ \\t\\r\\n]");
	nameMgr.addName ("dec", "[0-9]");

	fsm::RegExp regExp;	
	fsm::RegExpCompiler regExpCompiler (&regExp, &nameMgr);

/*	char const* src [] = 
	{
		"'\\x02' V '\\r'",
	};

	for (size_t i = 0; i < countof (src); i++)
	{
		bool result = regExpCompiler.incrementalCompile (src [i]);
		if (!result)
		{
			printf ("error: %s\n", err::getLastError ().getDescription ().cc ());
			return;
		}
	}

	regExpCompiler.finalize ();
	regExp.print ();
*/

	bool result = regExpCompiler.compile ("\"abc\" w* dec+");
	if (!result)
	{
		printf ("error: %s\n", err::getLastError ().getDescription ().cc ());
		return;
	}

	regExp.print ();
}

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

//.............................................................................

enum
{
	BufferSize = 4 * 1024,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
testNamedPipes ()
{
	NTSTATUS status;

	HMODULE ntdll = ::GetModuleHandleW (L"ntdll.dll");
	ASSERT (ntdll);

	NtQueryDirectoryFile* ntQueryDirectoryFile = (NtQueryDirectoryFile*) ::GetProcAddress (ntdll, "NtQueryDirectoryFile");
	if (!ntQueryDirectoryFile)
	{
		err::setLastSystemError ();
		printf ("cannot find NtQueryDirectoryFile: %s\n", err::getLastErrorDescription ().cc ());
		return; 
	}

	io::win::File pipeDir;
	bool result = pipeDir.create (
		L"\\\\.\\pipe\\",
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		0
		);

	if (!result) 
	{
		err::setLastSystemError ();
		printf ("cannot open pipe dir: %s\n", err::getLastErrorDescription ().cc ());
		return; 
	}
 
	rtl::Array <char> dirBuffer;
	dirBuffer.setCount (BufferSize);

	rtl::String_utf16 fileName;

	BOOLEAN isFirstQuery = TRUE;

	for (;;)
	{ 
		IO_STATUS_BLOCK ioStatus;

		status = ntQueryDirectoryFile (
			pipeDir, 
			NULL, 
			NULL, 
			0, 
			&ioStatus,
			dirBuffer, 
			dirBuffer.getCount (),
			FileDirectoryInformation,
			FALSE, 
			NULL, 
			isFirstQuery
			);

		if (status < 0)
		{
			if (status == STATUS_NO_MORE_FILES)
				break;

			err::setError (err::NtError (status));
			printf ("cannot open pipe dir: %s\n", err::getLastErrorDescription ().cc ());
			return; 
		}

		FILE_DIRECTORY_INFORMATION* dirInfo = (FILE_DIRECTORY_INFORMATION*) dirBuffer.a ();
		for (;;) 
		{
			fileName.copy (dirInfo->FileName, dirInfo->FileNameLength / 2);

			printf (
				dirInfo->AllocationSize.LowPart == -1 ? "%S (%d)\n" : "%S (%d of %d)\n", 
				fileName.cc (), 
				dirInfo->EndOfFile.LowPart,
				dirInfo->AllocationSize.LowPart
				);
 
			if (!dirInfo->NextEntryOffset) 
				break;
 
			dirInfo = (FILE_DIRECTORY_INFORMATION*) ((char*) dirInfo + dirInfo->NextEntryOffset); 
		}

		isFirstQuery = FALSE; 
	}
}

//.............................................................................

bool
querySymbolicLink (
	rtl::String_utf16* string,
	HANDLE dir,
	UNICODE_STRING* uniName
	)
{
	NTSTATUS status;

	string->clear ();

	OBJECT_ATTRIBUTES oa = { 0 };
	oa.Length = sizeof (oa);
	oa.RootDirectory = dir;
	oa.ObjectName = uniName;

	io::win::File link;
	status = ntOpenSymbolicLinkObject (
		link.getHandlePtr (), 
		GENERIC_READ,
		&oa
		);

	if (status < 0)
	{
		err::setError (err::NtError (status));
		printf ("cannot open symbolic link: %s\n", err::getLastErrorDescription ().cc ());
		return false;
	}

	wchar_t* p = string->getBuffer (BufferSize);
	size_t length = string->getLength ();

	UNICODE_STRING uniTarget;
	uniTarget.Buffer = p;
	uniTarget.Length = 0;
	uniTarget.MaximumLength = length + sizeof (wchar_t);

	status = ntQuerySymbolicLinkObject (link, &uniTarget, NULL);
	if (status < 0)
	{
		err::setError (err::NtError (status));
		printf ("cannot query symbolic link: %s\n", err::getLastErrorDescription ().cc ());
		return false;
	}

	string->setReducedLength (uniTarget.Length / sizeof (wchar_t));
	return true;
}

void 
enumerateDirectory (
	HANDLE baseDir,
	const wchar_t* name,
	size_t level
	)
{ 
	NTSTATUS status;

	UNICODE_STRING uniName;
	uniName.Buffer = (wchar_t*) name;
	uniName.Length = wcslen (name) * sizeof (wchar_t);
	uniName.MaximumLength = uniName.Length + sizeof (wchar_t);
	
	OBJECT_ATTRIBUTES oa = { 0 };
	oa.Length = sizeof (oa);
	oa.RootDirectory = baseDir;
	oa.ObjectName = &uniName;
		
	io::win::File dir;
	status = ntOpenDirectoryObject (
		dir.getHandlePtr (), 
		DIRECTORY_QUERY | DIRECTORY_TRAVERSE,
		&oa
		);

	if (status < 0)
	{
		err::setError (err::NtError (status));
		printf ("cannot open directory: %s\n", err::getLastErrorDescription ().cc ());
		return; 
	}

	rtl::Array <char> buffer;
	buffer.setCount (BufferSize);

	ULONG queryContext = 0;
	BOOLEAN isFirstQuery = TRUE;

	rtl::String_utf16 dirName;
	rtl::String_utf16 dirTypeName;
	rtl::String_utf16 symLinkTargetName;

	level++;
	rtl::String indent ((utf32_t) ' ', level * 2);

	for (;;)
	{
		ULONG actualSize;

		status = ntQueryDirectoryObject (
			dir, 
			buffer, 
			buffer.getCount (), 
			FALSE,
			isFirstQuery,
			&queryContext,
			&actualSize
			);

		if (status < 0)
		{
			if (status == STATUS_NO_MORE_ENTRIES)
				break;

			err::setError (err::NtError (status));
			printf ("cannot query directory: %s\n", err::getLastErrorDescription ().cc ());
			return; 
		}

		OBJECT_DIRECTORY_INFORMATION* dirInfo = (OBJECT_DIRECTORY_INFORMATION*) buffer.a ();
		for (; dirInfo->Name.Buffer; dirInfo++)
		{
			dirName.copy (dirInfo->Name.Buffer, dirInfo->Name.Length / sizeof (wchar_t));
			dirTypeName.copy (dirInfo->TypeName.Buffer, dirInfo->TypeName.Length / sizeof (wchar_t));

			printf ("%s%S (%S)\n", indent.cc (), dirName.cc (), dirTypeName.cc ());

			if (dirTypeName.cmp (L"Directory") == 0)
			{
				enumerateDirectory (dir, dirName, level);
			}
			else if (dirTypeName.cmp (L"SymbolicLink") == 0)
			{
				bool result = querySymbolicLink (&symLinkTargetName, dir, &dirInfo->Name);
				if (result)
					printf ("%s  --> %S\n", indent.cc (), symLinkTargetName.cc ());
			}

		}

		isFirstQuery = FALSE;
	}
}

void
testDirectoryObjects ()
{
	printf ("\\\n");
	enumerateDirectory (NULL, L"\\", 0);
}

#endif

void
testTcp ()
{
	int s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ASSERT (s != -1);

	linger l;
	l.l_onoff = 1;
	l.l_linger = 0;
	setsockopt (s, SOL_SOCKET, SO_LINGER, (const char*) &l, sizeof (l));

	sockaddr_in a = { 0 };
	a.sin_family = AF_INET;
	a.sin_addr.s_addr = inet_addr ("127.0.0.1");
	a.sin_port = htons (10001);

	printf ("connecting...\n");
	int result = connect (s, (const sockaddr*) &a, sizeof (a));	
	printf ("result = %d\n", result);

	char buf [512];
	scanf ("%d", buf);

	int error = 0;
	socklen_t len = sizeof (error);
	result = getsockopt (s, SOL_SOCKET, SO_ERROR, &error, &len);

	printf ("receiving...\n");
	result = recv (s, buf, sizeof (buf), 0);
	printf ("result = %d\n", result);

	printf ("receiving...\n");
	result = recv (s, buf, 0, 0);
	printf ("result = %d\n", result);


	printf ("closing...\n");
	close (s);

	printf ("done.\n");
}

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)
int
wmain (
	int argc,
	wchar_t* argv []
	)
#else
int
main (
	int argc,
	char* argv []
	)
#endif
{
#if (_AXL_ENV == AXL_ENV_WIN)
	WSADATA wsaData;
	WSAStartup (0x0202, &wsaData);	

	initNtFunctions ();
#endif
	
	testTcp ();
	return 0;
}

//.............................................................................
