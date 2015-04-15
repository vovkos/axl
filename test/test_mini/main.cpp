#include "pch.h"
#include "axl_fsm_RegExp.h"
#include "axl_rtl_AutoPtrArray.h"
#include "axl_mt_LongJmpTry.h"
#include "axl_mt_DynamicLibrary.h"
#include "axl_io_Socket.h"

//.............................................................................

void bar ()
{
	printf ("bar -- throwing...\n");
	AXL_MT_LONG_JMP_THROW ();	
}

void foo ()
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
	bool result;

	WSADATA wsaData;
	WSAStartup (0x0202, &wsaData);
	
#pragma comment (lib, "ws2_32.lib")

	printf ("main ()\n");

	sockaddr_in6 addr = { 0 };
	addr.sin6_family = AF_INET6;

	for (size_t i = 0;; i++)
	{
		addr.sin6_addr.u.Word [0] = rand () % 2 ? rand () : 0;
		addr.sin6_addr.u.Word [1] = rand () % 2 ? rand () : 0;
		addr.sin6_addr.u.Word [2] = rand () % 2 ? rand () : 0;
		addr.sin6_addr.u.Word [3] = rand () % 2 ? rand () : 0;
		addr.sin6_addr.u.Word [4] = rand () % 2 ? rand () : 0;
		addr.sin6_addr.u.Word [5] = rand () % 2 ? rand () : 0;
		addr.sin6_addr.u.Word [6] = rand () % 2 ? rand () : 0;
		addr.sin6_addr.u.Word [7] = rand () % 2 ? rand () : 0;
		addr.sin6_port = rand () % 2 ? rand () : 0;
		addr.sin6_scope_id = rand () % 2 ? rand () : 0;

		char addrString [1024] = { 0 };
		dword_t size = sizeof (addrString);

		WSAAddressToStringA ((sockaddr*) &addr, sizeof (addr), NULL, addrString, &size);

		rtl::String addrString2 = io::formatSockAddr ((const sockaddr*) &addr).cc ();

		printf ("addr1 = %s\n", addrString);
		printf ("addr2 = %s\n\n", addrString2);

		ASSERT (addrString2.cmp (addrString) == 0);

		sockaddr_in6 addr2;

		result = io::parseSockAddr ((sockaddr*) &addr2, sizeof (addr2), addrString2);
		ASSERT (result && memcmp (&addr, &addr2, sizeof (addr)) == 0);
	}

	const char* s = "fe80::f075:7b94:7c50:9565%14";

	int addrSize = sizeof (addr);
	WSAStringToAddressA ("[fe80::f075:0:9565%10]:1001", AF_INET6, NULL, (sockaddr*) &addr, &addrSize);

	return -1;

	mt::DynamicLibrary dl;
	
	result = dl.load ("libc.so.6");
	if (!result)
	{
		printf ("dl.load failed: %s\n", err::getLastError ().getDescription ().cc ());
		return -1;
	}

	typedef int Printf (const char*, ...);
	Printf* prn = (Printf*) dl.getFunction ("printf");
	if (!prn)
	{
		printf ("dl.load failed: %s\n", err::getLastError ().getDescription ().cc ());
		return -1;
	}

	prn ("hui govno i muravei %d\n", 123);

	foo ();

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
			return -1;
		}
	}

	regExpCompiler.finalize ();

	regExp.print ();
	return 0;
}

//.............................................................................
