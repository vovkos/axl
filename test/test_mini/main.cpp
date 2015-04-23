#include "pch.h"

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
	char const* src [] = 
	{
		"'\\x02' V '\\r'",
	};

	fsm::RegExp regExp;	
	fsm::RegExpCompiler regExpCompiler (&regExp);

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
#endif
	
	testAddrInfoIp6 ();
	return 0;
}

//.............................................................................
