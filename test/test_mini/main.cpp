#include "pch.h"
#include "axl_g_WarningSuppression.h"
#include "axl_sys_Thread.h"
#include "axl_err_Errno.h"

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
	sl::Array <char> bufferArray (ref::BufKind_Stack, buffer, sizeof (buffer));
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
	sl::Array <char> buffer;
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
	sl::StdList <io::NetworkAdapterDesc> adapterList;
	io::createNetworkAdapterDescList (&adapterList);

	sl::Iterator <io::NetworkAdapterDesc> adapterIt = adapterList.getHead ();
	for (; adapterIt; adapterIt++)
	{
		io::NetworkAdapterDesc* adapter = *adapterIt;

		printf ("Name        = %s\n", adapter->getName ().cc ());
		printf ("Description = %s\n", adapter->getDescription ().cc ());
		printf ("Type        = %s\n", io::getNetworkAdapterTypeString (adapter->getType ()));
		printf ("Flags       = %s\n", io::getNetworkAdapterFlagString (adapter->getFlags ()).cc ());

		sl::ConstList <io::NetworkAdapterAddress> addressList = adapter->getAddressList ();
		sl::Iterator <io::NetworkAdapterAddress> addressIt = addressList.getHead ();
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

	io::SockAddr sockAddr;

	result = sockAddr.parse ("6");
	printf ("result = %d, addr = %s\n", result, sockAddr.getString ().cc ());

	result = sockAddr.parse ("[1::2:3:4:5]:6");
	printf ("result = %d, addr = %s\n", result, sockAddr.getString ().cc ());

	result = sockAddr.parse ("[fe80::c990:d16e:a986:d56b%11]:10001");
	printf ("result = %d, addr = %s\n", result, sockAddr.getString ().cc ());

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

		sl::String addrString = io::getSockAddrString ((const sockaddr*) &addr).cc ();
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

	sl::Array <io::SockAddr> addrArray;
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
	sys::DynamicLibrary dl;
	
	bool result = dl.open ("libc.so.6");
	if (!result)
	{
		printf ("dl.load failed: %s\n", err::getLastErrorDescription ().cc ());
		return;
	}

	typedef int Printf (const char*, ...);
	Printf* prn = (Printf*) dl.getFunction ("printf");
	if (!prn)
	{
		printf ("dl.load failed: %s\n", err::getLastErrorDescription ().cc ());
		return;
	}

	prn ("hui govno i muravei %d\n", 123);
}

//.............................................................................

void 
bar ()
{
	printf ("bar -- throwing...\n");
	AXL_SYS_SJLJ_THROW ();	
}

void 
testLongJmpTry ()
{
	AXL_SYS_BEGIN_SJLJ_TRY ()
	{
		bar ();
	}
	AXL_SYS_SJLJ_CATCH ()
	{
		printf ("exception caught\n");
	}
	AXL_SYS_SJLJ_FINALLY ()
	{
		printf ("finally\n");
	}
	AXL_SYS_END_SJLJ_TRY ()

	AXL_SYS_BEGIN_SJLJ_TRY ()
	{
		bar ();
	}
	AXL_SYS_SJLJ_CATCH ()
	{
		printf ("exception caught\n");
	}
	AXL_SYS_END_SJLJ_TRY ()

	AXL_SYS_BEGIN_SJLJ_TRY ()
	{
		bar ();
	}
	AXL_SYS_SJLJ_FINALLY ()
	{
		printf ("finally\n");
	}
	AXL_SYS_END_SJLJ_TRY ()

	AXL_SYS_BEGIN_SJLJ_TRY ()
	{
		bar ();
	}
	AXL_SYS_END_SJLJ_TRY ()

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
			printf ("error: %s\n", err::getLastErrorDescription ().cc ());
			return;
		}
	}

	regExpCompiler.finalize ();
	regExp.print ();
*/

//	bool result = regExpCompiler.compile ("\"abc\" ws* dec+");
	bool result = regExpCompiler.compile ("'abc' \\n");
	if (!result)
	{
		printf ("error: %s\n", err::getLastErrorDescription ().cc ());
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
 
	sl::Array <char> dirBuffer;
	dirBuffer.setCount (BufferSize);

	sl::String_utf16 fileName;

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
	sl::String_utf16* string,
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
		link.p (), 
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
	uniName.Length = axl_wcslen (name) * sizeof (wchar_t);
	uniName.MaximumLength = uniName.Length + sizeof (wchar_t);
	
	OBJECT_ATTRIBUTES oa = { 0 };
	oa.Length = sizeof (oa);
	oa.RootDirectory = baseDir;
	oa.ObjectName = &uniName;
		
	io::win::File dir;
	status = ntOpenDirectoryObject (
		dir.p (), 
		DIRECTORY_QUERY | DIRECTORY_TRAVERSE,
		&oa
		);

	if (status < 0)
	{
		err::setError (err::NtError (status));
		printf ("cannot open directory: %s\n", err::getLastErrorDescription ().cc ());
		return; 
	}

	sl::Array <char> buffer;
	buffer.setCount (BufferSize);

	ULONG queryContext = 0;
	BOOLEAN isFirstQuery = TRUE;

	sl::String_utf16 dirName;
	sl::String_utf16 dirTypeName;
	sl::String_utf16 symLinkTargetName;

	level++;
	sl::String indent ((utf32_t) ' ', level * 2);

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
#if (_AXL_ENV == AXL_ENV_WIN)
	SOCKET s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
	int s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
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
	result = getsockopt (s, SOL_SOCKET, SO_ERROR, (char*) &error, &len);

	printf ("receiving...\n");
	result = recv (s, buf, sizeof (buf), 0);
	printf ("result = %d\n", result);

	printf ("receiving...\n");
	result = recv (s, buf, 0, 0);
	printf ("result = %d\n", result);

	printf ("closing...\n");
#if (_AXL_ENV == AXL_ENV_WIN)
	closesocket (s);
#elif (_AXL_ENV == AXL_ENV_POSIX)
	close (s);
#endif

	printf ("done.\n");
}

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

void
printWinErrorDescription (
	dword_t code,
	uint_t langId,
	uint_t subLangId
	)
{
	wchar_t* message = NULL;
	
	::FormatMessageW ( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_MAX_WIDTH_MASK, // no line breaks please
		NULL,
		code,
		MAKELANGID (langId, subLangId),
		(LPWSTR) &message, 
		0, 
		NULL
		);

	sl::String description = message;
	::LocalFree (message);

	printf ("%d/%d: %s\n", langId, subLangId, description.cc ());
}

void 
testWinError ()
{
	dword_t error = ERROR_ACCESS_DENIED;
	
	printWinErrorDescription (error, LANG_NEUTRAL, SUBLANG_NEUTRAL);
	printWinErrorDescription (error, LANG_NEUTRAL, SUBLANG_DEFAULT);
	printWinErrorDescription (error, LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);

	printWinErrorDescription (error, LANG_SYSTEM_DEFAULT, SUBLANG_NEUTRAL);
	printWinErrorDescription (error, LANG_SYSTEM_DEFAULT, SUBLANG_DEFAULT);
	printWinErrorDescription (error, LANG_SYSTEM_DEFAULT, SUBLANG_SYS_DEFAULT);
	
	printWinErrorDescription (error, LANG_ENGLISH, SUBLANG_NEUTRAL);
	printWinErrorDescription (error, LANG_ENGLISH, SUBLANG_DEFAULT);
	printWinErrorDescription (error, LANG_ENGLISH, SUBLANG_SYS_DEFAULT);
	printWinErrorDescription (error, LANG_ENGLISH, SUBLANG_ENGLISH_US);
	printWinErrorDescription (error, LANG_ENGLISH, SUBLANG_ENGLISH_UK);
}

#endif

void
testTimestamp ()
{
	for (int i = 0; i < 20; i++)
	{
		uint64_t t1 = sys::getTimestamp ();
		printf ("%s: %llu\n", sys::Time (t1).format ("%h:%m:%s.%l.%c").cc (), t1);
	}
}

//.............................................................................

#ifdef _AXL_IO_USB

sl::String 
getUsbStringDescriptorText (
	io::UsbDevice* device,
	size_t index
	)
{
	sl::String text;

	if (!device->isOpen ())
	{
		text = "NOT OPENED";
	}
	else
	{	
		bool result = device->getStringDesrciptor (index, &text);
		if (!result)
			text.format ("ERROR (%s)", err::getLastErrorDescription ().cc ());
	}

	return text;
}

void
printUsbIfaceDesc (const libusb_interface_descriptor* ifaceDesc)
{
	printf ("    Interface:   %d\n", ifaceDesc->bInterfaceNumber);
	printf ("    Alt setting: %d\n", ifaceDesc->bAlternateSetting);
	printf ("    Class:       %s\n", io::getUsbClassCodeString ((libusb_class_code) ifaceDesc->bInterfaceClass));
	printf ("    Subclass:    %d\n", ifaceDesc->bInterfaceSubClass);
	printf ("    Protocol:    %d\n", ifaceDesc->bInterfaceProtocol);
	printf ("    Endpoints:   %d\n", ifaceDesc->bNumEndpoints);

	for (size_t i = 0; i < ifaceDesc->bNumEndpoints; i++)
	{
		const libusb_endpoint_descriptor* endpointDesc = &ifaceDesc->endpoint [i];

		printf ("\n");

		printf ("      Endpoint:        0x%02x\n", endpointDesc->bEndpointAddress);
		printf ("      Direction:       %s\n", (endpointDesc->bEndpointAddress & LIBUSB_ENDPOINT_IN) ? "In" : "Out");
		printf ("      Type:            %s\n", io::getUsbTransferTypeString ((libusb_transfer_type) (endpointDesc->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK)));
		printf ("      Max packet size: %d\n", endpointDesc->wMaxPacketSize);
	}
}

void
printUsbConfigDesc (const libusb_config_descriptor* configDesc)
{
	printf ("  Configuration: %d\n", configDesc->bConfigurationValue);
	printf ("  Max power:     %d mA\n", configDesc->MaxPower * 2);
	printf ("  Interfaces:    %d\n", configDesc->bNumInterfaces);

	for (size_t i = 0; i < configDesc->bNumInterfaces; i++)
	{
		const libusb_interface* iface = &configDesc->interface [i];

		if (!iface->num_altsetting)
		{
			printf ("\n    Interface #%d is not configured\n", i);
		}
		else for (size_t j = 0; j < (size_t) iface->num_altsetting; j++)
		{
			printf ("\n");
			printUsbIfaceDesc (&iface->altsetting [j]);
		}
	}
}

void
printUsbDevice (io::UsbDevice* device)
{
	bool result;

	libusb_device_descriptor deviceDesc;
	result = device->getDeviceDescriptor (&deviceDesc);
	if (!result)
	{
		printf ("Cannot get device descriptor (%s)\n", err::getLastErrorDescription ().cc ());
		return;
	}

	printf ("HWID:           VID_%04x&PID_%04x\n", deviceDesc.idVendor, deviceDesc.idProduct);
	printf ("Class:          %s\n", io::getUsbClassCodeString ((libusb_class_code) deviceDesc.bDeviceClass));
	printf ("Manufacturer:   %s\n", getUsbStringDescriptorText (device, deviceDesc.iManufacturer).cc ());
	printf ("Product name:   %s\n", getUsbStringDescriptorText (device, deviceDesc.iProduct).cc ());
	printf ("Serial number:  %s\n", getUsbStringDescriptorText (device, deviceDesc.iSerialNumber).cc ());

	printf ("Address:        %d\n", device->getDeviceAddress ());
	printf ("Bus:            %d\n", device->getBusNumber ());
	printf ("Port:           %d\n", device->getPortNumber ());
	printf ("Speed:          %s\n", io::getUsbSpeedString (device->getDeviceSpeed ()));
	printf ("Port path:      ");

	uint8_t path [8];
	size_t pathLength = device->getPortPath (path, countof (path));
	if (pathLength == -1)
	{
		printf ("ERROR (%s)\n", err::getLastErrorDescription ().cc ());
	}
	else if (pathLength != -1)
	{
		for (size_t i = 0; i < pathLength; i++)
			printf ("-> %d", path [i]);

		printf ("\n");
	}

	printf ("Configurations: %d\n", deviceDesc.bNumConfigurations);

	for (size_t i = 0; i < deviceDesc.bNumConfigurations; i++)
	{
		printf ("\n");

		io::UsbConfigDescriptor configDesc;
		bool result = device->getConfigDescriptor (i, &configDesc);
		if (!result)
			printf ("  Cannot get config descriptor #%d (%s)\n", i, err::getLastErrorDescription ().cc ());
		else
			printUsbConfigDesc (configDesc);
	}
}

void 
testUsb ()
{
	bool result;

	io::registerUsbErrorProvider ();
	io::UsbContext context;

	io::UsbDeviceList deviceList;
	size_t count = deviceList.enumerateDevices (context);
	if (count == -1)
	{
		printf ("Cannot enumerate USB devices (%s)\n", err::getLastErrorDescription ().cc ());
		return;
	}

	libusb_device** pp = deviceList;
	for (size_t i = 0; *pp; pp++, i++)
	{
		printf ("----------------------\nDevice #%d\n", i);

		io::UsbDevice device;
		device.setDevice (*pp);		

		result = device.open ();
		if (!result)
			printf ("Cannot open device (%s)\n", err::getLastErrorDescription ().cc ());		

		printUsbDevice (&device); // even if not opened
	}
}

#endif

//.............................................................................

#if (_AXL_ENV == AXL_ENV_POSIX)

class MyThread: public sys::ThreadImpl <MyThread>
{
public:
	volatile bool m_terminateFlag;

	MyThread ()
	{
		m_terminateFlag = false;
	}

	void 
	threadFunc ()
	{
		uint64_t tid = getThreadId ();

		for (;;)
		{
			sys::sleep (1000);
			printf ("MyThread -- TID: %lld\n", tid);

			if (m_terminateFlag)
				break;
		}

		printf ("MyThread -- TID: %lld -- TERMINATE\n", tid);
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
	_exit (-1);
}

#endif

void* suspendThread (pthread_t thread)
{
	return NULL;
}

bool resumeThread (pthread_t thread)
{
	return false;
}

void testSuspendThread ()
{
	MyThread thread1;
	MyThread thread2;

	thread1.start ();
	sys::sleep (500);
	thread2.start ();

	sys::sleep (3000);

	suspendThread (thread2.m_thread);
	printf ("thread2 is suspended...\n");

	sys::sleep (3000);

	printf ("thread2 is resumed...\n");
	resumeThread (thread2.m_thread);

	sys::sleep (3000);

	printf ("terminating threads...\n");

	thread1.m_terminateFlag = true;
	thread2.m_terminateFlag = true;

	thread1.waitAndClose ();
	thread2.waitAndClose ();

/*
	sigaction action = { 0 };
	action.sa_sigaction = signal_segv;
	action.sa_flags = SA_SIGINFO;

	sigaction(SIGSEGV, &action, NULL);
		perror("sigaction");


	threadArray [i].suspend ();
*/

}

#endif

//.............................................................................

class Gc;

Gc* g_gc = NULL;

#if (_AXL_ENV == AXL_ENV_POSIX)
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

	sl::Array <uint64_t> m_threadArray;
	sl::HashTableMap <uint64_t, bool, sl::HashId <uint64_t> > m_threadMap;

	volatile HandshakeKind m_handshakeKind;
	volatile int32_t m_handshakeCounter;

#if (_AXL_POSIX == AXL_POSIX_DARWIN)
	sys::drw::Semaphore m_handshakeSem;
#else
	sys::psx::Sem m_handshakeSem;
#endif

	sigset_t m_signalWaitMask;

public:
	Gc ()
	{
		m_guardPage.map (
			NULL,
			4 * 1024, // typical page size -- OS will not give us less than that, anyway
			PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS,
			-1,
			0
			);

		m_handshakeKind = HandshakeKind_None;
		installSignalHandlers ();
	}

	void
	gcSafePoint ()
	{
		*(volatile int*) m_guardPage.p () = 0;
	}

	void
	stopTheWorld ()
	{
		m_handshakeKind = HandshakeKind_StopTheWorld;
		m_handshakeCounter = m_threadMap.getCount ();
		m_guardPage.protect (PROT_NONE);
		m_handshakeSem.wait ();
		m_handshakeKind = HandshakeKind_None;
	}

	void
	resumeTheWorld ()
	{
		m_guardPage.protect (PROT_READ | PROT_WRITE);

		m_handshakeKind = HandshakeKind_ResumeTheWorld;
		m_handshakeCounter = m_threadArray.getCount ();
		for (size_t i = 0; i < m_handshakeCounter; i++)
			pthread_kill ((pthread_t) m_threadArray [i], SIGUSR1); // resume

		m_handshakeSem.wait ();
		m_handshakeKind = HandshakeKind_None;
	}

	bool
	registerThread (uint64_t threadId)
	{
		sl::HashTableMapIterator <uint64_t, bool> it = m_threadMap.visit (threadId);
		if (it->m_value)
			return false;

		m_threadArray.append (threadId);
		it->m_value = true;
		return true;
	}

protected:
	bool
	installSignalHandlers ()
	{
		sigemptyset (&m_signalWaitMask); // don't block any signals when servicing SIGSEGV

		struct sigaction sigAction = { 0 };
		sigAction.sa_flags = SA_SIGINFO;
		sigAction.sa_sigaction = signalHandler_SIGSEGV;
		sigAction.sa_mask = m_signalWaitMask;

		struct sigaction prevSigAction;
		int result = sigaction (SIGSEGV, &sigAction, &prevSigAction);
		ASSERT (result == 0);

		sigAction.sa_flags = 0;
		sigAction.sa_handler = signalHandler_SIGUSR1;
		result = sigaction (SIGUSR1, &sigAction, &prevSigAction);
		ASSERT (result == 0);

		return true;
	}

	static
	void
	signalHandler_SIGSEGV (
		int signal,
		siginfo_t* signalInfo,
		void* context
		)
	{
		if (signal != SIGSEGV || 
			signalInfo->si_addr != g_gc->m_guardPage ||
			g_gc->m_handshakeKind != HandshakeKind_StopTheWorld)
			return; // ignore

		int32_t count = sys::atomicDec (&g_gc->m_handshakeCounter);
		if (!count)
			g_gc->m_handshakeSem.signal ();

		do
		{
			sigsuspend (&g_gc->m_signalWaitMask);
		} while (g_gc->m_handshakeKind != HandshakeKind_ResumeTheWorld);

		count = sys::atomicDec (&g_gc->m_handshakeCounter);
		if (!count)
			g_gc->m_handshakeSem.signal ();
	}

	static
	void
	signalHandler_SIGUSR1 (int signal)
	{
		// do nothing (we handshake manually). but we still need a handler
	}
};

#define GC_BEGIN()
#define GC_END()

#elif (_AXL_ENV == AXL_ENV_WIN)

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

	sl::HashTableMap <uint64_t, bool, sl::HashId <uint64_t> > m_threadMap;

	volatile HandshakeKind m_handshakeKind;
	volatile int32_t m_handshakeCounter;
	sys::Event m_handshakeEvent;
	sys::NotificationEvent m_resumeEvent;

public:
	Gc ()
	{
		m_guardPage.alloc (4 * 1024);
		m_handshakeKind = HandshakeKind_None;
	}

	~Gc ()
	{
	}

	void
	gcSafePoint ()
	{
		sys::atomicXchg ((volatile int*) m_guardPage.p (), 0);
	}

	void
	stopTheWorld ()
	{
		m_handshakeKind = HandshakeKind_StopTheWorld;
		m_handshakeCounter = m_threadMap.getCount ();
		m_resumeEvent.reset ();
		m_guardPage.protect (PAGE_NOACCESS);
		m_handshakeEvent.wait ();
		m_handshakeKind = HandshakeKind_None;
	}

	void
	resumeTheWorld ()
	{
		m_guardPage.protect (PAGE_READWRITE);

		m_handshakeKind = HandshakeKind_ResumeTheWorld;
		m_handshakeCounter = m_threadMap.getCount ();
		m_resumeEvent.signal ();
		m_handshakeEvent.wait ();
		m_handshakeKind = HandshakeKind_None;
		m_resumeEvent.reset ();
	}

	bool
	registerThread (uint64_t threadId)
	{
		sl::HashTableMapIterator <uint64_t, bool> it = m_threadMap.visit (threadId);
		if (it->m_value)
			return false;

		it->m_value = true;
		return true;
	}

	int 
	handleException (
		uint_t code, 
		EXCEPTION_POINTERS* exceptionPointers
		) 
	{
		if (code != EXCEPTION_ACCESS_VIOLATION || 
		   exceptionPointers->ExceptionRecord->ExceptionInformation [1] != (uintptr_t) m_guardPage.p () ||
		   m_handshakeKind != HandshakeKind_StopTheWorld) 
		  return EXCEPTION_CONTINUE_SEARCH;

		int32_t count = sys::atomicDec (&g_gc->m_handshakeCounter);
		if (!count)
			g_gc->m_handshakeEvent.signal ();

		do
		{
			g_gc->m_resumeEvent.wait ();
		} while (m_handshakeKind != HandshakeKind_ResumeTheWorld);

		count = sys::atomicDec (&g_gc->m_handshakeCounter);
		if (!count)
			g_gc->m_handshakeEvent.signal ();

		return EXCEPTION_CONTINUE_EXECUTION;
	}
};

#define GC_BEGIN() __try {
#define GC_END() } __except (g_gc->handleException (GetExceptionCode (), GetExceptionInformation ())) { }

#endif

class MutatorThread: public sys::ThreadImpl <MutatorThread>
{
protected:
	volatile bool m_terminateFlag;

public:
	MutatorThread ()
	{
		m_terminateFlag = false;
	}

	void
	stop ()
	{
		m_terminateFlag = true;
		waitAndClose ();
		m_terminateFlag = false;
	}

	void
	threadFunc ()
	{
		uint64_t threadId = sys::getCurrentThreadId ();

		GC_BEGIN ()

		for (;;)
		{
			if (m_terminateFlag)
				break;

			printf ("  mutator thread TID:%lld is working...\n", threadId);
			sys::sleep (200);
			g_gc->gcSafePoint ();
		}
		
		printf ("  mutator thread TID:%lld is finished.\n", threadId);
		
		GC_END ()
	}
};

void
testGcSafePoints ()
{
	Gc gc;

	g_gc = &gc;

	printf ("starting mutator threads...\n");

	MutatorThread thread1;
	MutatorThread thread2;

	thread1.start ();
	thread2.start ();

	gc.registerThread (thread1.getThreadId ());
	gc.registerThread (thread2.getThreadId ());

	sys::sleep (1000);

	printf ("stopping the world...\n");
	gc.stopTheWorld ();
	printf ("the world is stopped.\n");

	sys::sleep (2000);

	printf ("resuming the world...\n");
	gc.resumeTheWorld ();

	sys::sleep (1000);

	printf ("stopping mutator threads...\n");

	thread1.stop ();
	thread2.stop ();

	printf ("done.\n");

	g_gc = NULL;
}

//.............................................................................

struct IfaceHdr
{
	const char* m_p1;
	const char* m_p2;
	const char* m_p3;
};

class Foo: public IfaceHdr
{
public:
	Foo ()
	{
		printf ("m_p1 = %s; m_p2 = %s; m_p3 = %s\n", m_p1, m_p2, m_p3);
	}
};

void
testInheritance ()
{
	char buffer [sizeof (Foo)];
	IfaceHdr* iface = (IfaceHdr*) buffer;
	iface->m_p1 = "hui";
	iface->m_p2 = "govno";
	iface->m_p3 = "muravei";

	new (buffer) Foo ();
}

//.............................................................................

void
testTimestamps ()
{
	uint64_t ts1 = sys::getTimestamp ();
	uint64_t ts2 = sys::getPreciseTimestamp ();

	printf ("time0.1 = %s\n", sys::Time (ts1).format ("%h:%m:%s.%l.%c").cc ());
	printf ("time0.2 = %s\n", sys::Time (ts2).format ("%h:%m:%s.%l.%c").cc ());

	uint64_t t0 = sys::getPreciseTimestamp ();

	for (size_t i = 0; i < 1000000; i++)
	{
		sys::getTimestamp ();
	}

	uint64_t t2 = sys::getPreciseTimestamp ();
	uint64_t d1 = t2 - t0;
	printf ("time1 = %s\n", sys::Time (d1, 0).format ("%h:%m:%s.%l.%c").cc ());

	t0 = sys::getPreciseTimestamp ();

	for (size_t i = 0; i < 1000000; i++)
	{
		sys::getPreciseTimestamp ();
	}

	t2 = sys::getPreciseTimestamp ();
	uint64_t d2 = t2 - t0;
	printf ("time2 = %s\n", sys::Time (d2, 0).format ("%h:%m:%s.%l.%c").cc ());

	ts1 = sys::getTimestamp ();
	ts2 = sys::getPreciseTimestamp ();

	printf ("time0.1 = %s\n", sys::Time (ts1).format ("%h:%m:%s.%l.%c").cc ());
	printf ("time0.2 = %s\n", sys::Time (ts2).format ("%h:%m:%s.%l.%c").cc ());
}

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)
void
testProcess ()
{
	sl::Array <char> output;
	dword_t exitCode;

	const wchar_t* cmdLine = L"C:\\Projects\\ioninja\\devmon\\build\\msvc10\\bin\\Debug\\tdevmon.exe --start-core-service";

	bool result = sys::win::syncExec (cmdLine, &output, &exitCode);
	if (!result)
	{
		printf ("sys::win::syncExec failed: %s\n", err::getLastErrorDescription ().cc ());
		return;
	}

	output.append (0);
	printf ("process returned %d:\n%s\n", exitCode, output.ca ());
}
#endif

//.............................................................................

void
testSharedMemoryTransport ()
{
	bool result;

	io::SharedMemoryReader reader;
	result = reader.open (
		"c:/9141b219-d4af-4f40-96b9-92da376bf3c9.shm", 
		"a51aa9f2-f6d0-43f0-bce2-099a9f780a2e", 
		"00c34a43-45a4-4137-965e-3b172d3a54ed", 
		io::SharedMemoryTransportFlag_Create | io::SharedMemoryTransportFlag_Message | io::SharedMemoryTransportFlag_DeleteOnClose
		);

	if (!result)
	{
		printf ("can't open shared memory reader: %s\n", err::getLastErrorDescription ().cc ());
		return;
	}

	io::SharedMemoryWriter writer;
	result = writer.open (
		"c:/9141b219-d4af-4f40-96b9-92da376bf3c9.shm", 
		"a51aa9f2-f6d0-43f0-bce2-099a9f780a2e", 
		"00c34a43-45a4-4137-965e-3b172d3a54ed", 
		io::SharedMemoryTransportFlag_Message
		);

	if (!result)
	{
		printf ("can't open shared memory writer: %s\n", err::getLastErrorDescription ().cc ());
		return;
	}

	char data [] = "hui, govno i muravei";

	writer.write (data, sizeof (data));
	writer.close ();

	sl::Array <char> a = reader.read ();
	printf ("reader.read () returned: %s\n", a.ca ());
}

//.............................................................................

void
testZip ()
{
	bool result;

	zip::ZipReader reader;

	result = reader.openFile ("c:/projects/projects.zip");
	if (!result)
	{
		printf ("can't open file: %s\n", err::getLastErrorDescription ().cc ());
		return;
	}

	sl::Array <char> buffer;

	size_t count = reader.getFileCount ();
	for (size_t i = 0; i < count; i++)
	{
		sl::String fileName = reader.getFileName (i);

		zip::ZipFileInfo fileInfo;
		reader.getFileInfo (i, &fileInfo);

		bool isDir = reader.isDirectoryFile (i);

		printf (
			"%-10s         %s\n"
			"compressed size:   %d\n"
			"uncompressed size: %d\n"
			"timestamp:         %s\n"
			"method:            %d\n",
			isDir ? "directory:" : "file:",
			fileName.cc (),
			(size_t) fileInfo.m_compressedSize,
			(size_t) fileInfo.m_uncompressedSize,
			sys::Time (fileInfo.m_timestamp).format ().cc (),
			fileInfo.m_method
			);

		if (!isDir)
		{
			reader.extractFileToMem (i, &buffer);
			printf ("<<<\n%s\n>>>\n", buffer.ca ());
		}

		printf ("\n");
	}
}

//.............................................................................

void
testEnumSerial ()
{
	sl::StdList <io::SerialPortDesc> portList;
	io::createSerialPortDescList (&portList);

	sl::Iterator <io::SerialPortDesc> it = portList.getHead ();
	for (; it; it++)
		printf ("device name: %s\ndescription: %s\n\n", it->getDeviceName ().cc (), it->getDescription ().cc ());

	printf ("%d ports total\n", portList.getCount ());
}

//.............................................................................

void
testEncoding ()
{
	sl::String s;
	enc::EscapeEncoding::encode (&s, "\\\\.\\pipe\\mypipe");
	printf ("%s\n", s.cc ());
}

//.............................................................................

// AXL_NO_ASAN
void
testAddressSanitizer ()
{
	char c = 10;
	char* p = &c;
	p++;
	*p = 0;
}

int foo (jmp_buf buf)
{
	return 0;
}

//.............................................................................

#pragma pack ()

struct F0
{
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack (1)

struct F1
{
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack (2)

struct F2
{
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack (4)

struct F4
{
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

#pragma pack (8)

struct F8  
{
	uint8_t m_int8_1;
	uint64_t m_int64;
	uint8_t m_int8_2;
} AXL_GCC_MSC_STRUCT;

void
testPacking ()
{
	size_t s0 = sizeof (F0);
	size_t s1 = sizeof (F1);
	size_t s2 = sizeof (F2);
	size_t s4 = sizeof (F4);
	size_t s8 = sizeof (F8);

	printf ("s0 = %d; s1 = %d; s2 = %d; s4 = %d; s8 = %d\n", s0, s1, s2, s4, s8);
}

//.............................................................................

uint16_t crc16_ansi (
	void const* p, 
	size_t size,
	uint16_t seed = 0
	)
{
	static uint16_t const crcTable [256] = 
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
		crc = (crc >> 8) ^ crcTable [j];
	}

	return crc;
}

uint16_t crc16_ccit (
	uint8_t const* p,
	size_t size,
	uint16_t seed = 0
	)
{
	static uint16_t const crcTable [256] =
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
		crc = (crc << 8) ^ crcTable [j];
	}

	return (uint16_t) crc;
}

void
testModBus ()
{
	uint8_t packet [] = 
	{
		0x01, 0x03, 0x00, 0x30, 0x00, 0x01, // 0x00, 0x00,
	};

	printf (
		"crc16        = %04x\n"
		"crc16 modbus = %04x\n"
		"crc16 xmodem = %04x\n"
		"crc16 ffff   = %04x\n", 
		crc16_ansi (packet, sizeof (packet), 0),
		crc16_ansi (packet, sizeof (packet), 0xffff),
		crc16_ccit (packet, sizeof (packet), 0),
		crc16_ccit (packet, sizeof (packet), 0xffff)
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

	sl::String s_1_5 = sys::Time (timeout_1_5, false).format ("%s.%l.%c");
	sl::String s_3_5 = sys::Time (timeout_3_5, false).format ("%s.%l.%c");

	printf ("t_1_5 = %s\nt_3_5 = %s\n", s_1_5.cc (), s_3_5.cc ());
}

//.............................................................................

uint_t calcChecksum16 (
	const uint8_t* p0, 
	size_t size
	) 
{
	uint16_t const* p = (uint16_t const*) p0;
	void const* end = p0 + (size & ~1);

	uint_t checksum = 0;
	
	for (; p < end; p++)
		checksum += htons (*p);
  
	if (size & 1) 
		checksum += *(uint8_t const*) p << 8;

	return checksum;
}

uint16_t adjustIpCheckSum (uint_t checksum) 
{
	checksum = (checksum >> 16) + (checksum & 0xffff);
	checksum += checksum >> 16;
	return ~checksum;
}

uint16_t calcIpHdrChecksum (const uint8_t* ipHdr, size_t size)
{
	uint_t checksum = calcChecksum16 (ipHdr, size);
	return adjustIpCheckSum (checksum);
}

void
testIpChecksum ()
{
	uint8_t data1_0 [] = 
	{
		0x45, 0x00, 0x00, 0x1e, 0x7e, 0x8b, 0x00, 0x00, 0x80, 0x11, 0x37, 0x7d, 0xc0, 0xa8, 0x01, 0x77,
		0xc0, 0xa8, 0x01, 0xff
	};

	uint8_t data1 [] = 
	{
		0x45, 0x00, 0x00, 0x1e, 0x7e, 0x8b, 0x00, 0x00, 0x80, 0x11, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x77,
		0xc0, 0xa8, 0x01, 0xff
	};

	uint8_t data2_0 [] = 
	{
		0x45, 0x00, 0x00, 0x73, 0x00, 0x00, 0x40, 0x00, 0x40, 0x11, 0xb8, 0x61, 0xc0, 0xa8, 0x00, 0x01,
		0xc0, 0xa8, 0x00, 0xc7
	};

	uint8_t data2 [] = 
	{
		0x45, 0x00, 0x00, 0x73, 0x00, 0x00, 0x40, 0x00, 0x40, 0x11, 0x00, 0x00, 0xc0, 0xa8, 0x00, 0x01,
		0xc0, 0xa8, 0x00, 0xc7
	};

	uint16_t x = calcIpHdrChecksum (data2, sizeof (data2));
	printf ("%x\n", x);
}

size_t
encodeNetBiosName (
	char* buffer,
	const char* name,
	char paddingChar
	)
{
	size_t i = 0;
	size_t j = 0;

	for (; i < 16; i++)
	{
		uchar_t c = name [i];
		if (!c)
			break;
		
		c = toupper (c);

		buffer [j++] = 'A' + (c >> 4);
		buffer [j++] = 'A' + (c & 0x0f);
	}

	char paddingByteHi = 'A' + (paddingChar >> 4);
	char paddingByteLo = 'A' + (paddingChar & 0x0f);

	while (j < 32)
	{
		buffer [j++] = paddingByteHi;
		buffer [j++] = paddingByteLo;
	}

	return j;
}	

size_t
decodeNetBiosName (
	char* buffer,
	const char* name,
	size_t length
	)
{
	ASSERT (!(length & 1)); // must be even

	size_t i = 0;
	size_t j = 0;

	while (i < length)
	{
		uchar_t hi = name [i++] - 'A';
		uchar_t lo = name [i++] - 'A';
		
		buffer [j++] = lo + (hi << 4);
	}

	return j;
}

void
testNetBios ()
{
	const char* name = "*";

	char buffer [1024] = "CKAAAAAAA" "AAAAAAAAAAAAAAAA" "AAAAAAA"; //{ 0 };
	size_t length = 32; //encodeNetBiosName (buffer, name);
	printf ("%s\n", buffer);

	char buffer2 [1024] = { 0 };	
	decodeNetBiosName (buffer2, buffer, length);
	printf ("%s\n", buffer2);
}

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

BOOL WriteSlot(HANDLE hSlot, const char* lpszMessage)
{
   BOOL fResult; 
   DWORD cbWritten; 
 
   fResult = WriteFile(hSlot, 
	 lpszMessage, 
	 strlen (lpszMessage),
	 &cbWritten, 
	 (LPOVERLAPPED) NULL); 
 
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
 
   char buffer [256];

   fResult = ReadFile (hSlot, buffer, sizeof (buffer) - 1, &cbRead, NULL);
   if (!fResult) 
   { 
	  printf("ReadFile failed with %d.\n", GetLastError()); 
	  return FALSE;
   } 
 
   buffer [cbRead] = 0;
   printf("Read from slot: %s\n", buffer); 

   return TRUE;
}

void
testMailSlot ()
{ 
	printf ("1 for server, 2 for client:\n");
	char buffer [256];
	gets (buffer);
	
	if (buffer [0] == '1')
	{
	char slotName [256];
	printf ("slot name (e.g. \\\\.\\mailslot\\foo):\n");
	gets (slotName);

		HANDLE hFile = CreateMailslotA (slotName, 0, -1, NULL);
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

	char slotName [256];
	printf ("slot name (e.g. \\\\*\\mailslot\\foo):\n");
	gets (slotName);

		HANDLE hFile; 

	   hFile = CreateFileA (
		   slotName, 
		 GENERIC_READ | GENERIC_WRITE, 
		 FILE_SHARE_READ | FILE_SHARE_WRITE,
		 (LPSECURITY_ATTRIBUTES) NULL, 
		 OPEN_EXISTING, 
		 FILE_ATTRIBUTE_NORMAL, 
		 (HANDLE) NULL); 
 
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
#endif
	
	testNetBios ();

	return 0;
}

//.............................................................................
