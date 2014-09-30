#include "pch.h"

using namespace axl;

namespace test_Sock { 

//.............................................................................

/*

void 
onAccept (ref::CPtrT <io::CConnectionSock> connectionSock)
{
	io::TSockAddrU localAddr;
	io::TSockAddrU peerAddr;

	connectionSock->getLocalAddress (&localAddr);
	connectionSock->getPeerAddress (&peerAddr);
	
	printf ("accepted connection from %s\n", peerAddr.toString ());
}

void run_Listen ()
{
	io::CListenerSock sock;	

	io::CSockAddrIp addr (1001);

	bool result = 
		sock.open (io::ESockProto_Tcp, addr) &&
		sock.listen (32, &exe::CFunctionT <
			exe::CArgT <void>, 
			exe::CArgT <ref::CPtrT <io::CConnectionSock> > > (onAccept));;
	
	if (!result)
	{
		printf ("cannot listen on %s: %s\n", addr.toString (), err::getError ()->getDescription ());
		return;
	}

	printf ("listening on %s\n", addr.toString ());
	
	char s [32];
	gets (s);
}

//.............................................................................

void 
onConnect (
	io::CConnectionSock* sock,
	const err::CError& error
	)
{
	io::TSockAddrU localAddr;
	io::TSockAddrU peerAddr;

	sock->getLocalAddress (&localAddr);
	sock->getPeerAddress (&peerAddr);

	if (!error)
		printf ("established connection to %s from %s\n", peerAddr.toString (), localAddr.toString ());
	else
		printf ("cannot establish connection: %s\n", error->getDescription ());
}

void 
run_Connect ()
{
	exe::CWorkerThread workerThread;
	workerThread.start ();

	io::CConnectionSock sock;	

	io::CSockAddrIp addr (0x7f000001, 1001);

	exe::CScheduledFunctionT <
		exe::CArgT <io::CConnectionSock*>, 
		io::CConnectionSock::COnConnectCompleteArg> 
		onComplete (&workerThread, onConnect, &sock);

	bool result = 
		sock.open (io::ESockProto_Tcp, io::ESockAddr_Ip) &&
		sock.syncConnect (&addr, 1000);
	
	if (!result)
	{
		printf ("cannot connect to %s: %s\n", addr.toString (), err::getError ()->getDescription ());
		return;
	}

	char s [32];
	gets (s);
}

//.............................................................................

char buffer [1024] = { 0 };

void 
onRecvFrom (
	io::CDgramSock* sock,
	const err::CError& error,
	io::TSockAddr* addr,
	size_t actualSize
	)
{
	if (error)
	{
		printf ("error during recvfrom: %s\n", error->getDescription ());
		return;
	}

	printf ("received %d bytes from %s: %s\n", actualSize, addr->toString (), buffer);

	exe::CFunctionT <
		exe::CArgT <io::CDgramSock*>, 
		io::CDgramSock::COnSendRecvCompleteArg> 
		onComplete (onRecvFrom, sock);

	sock->recvFrom (buffer, 1023, &onComplete);
}

void 
run_Dgram ()
{
	io::CDgramSock sock;	

	io::CSockAddrIp addr (0, 1001);

	bool result = sock.open (io::ESockProto_Udp, &addr);
	if (!result)
	{
		printf ("cannot open %s: %s\n", addr.toString (), err::getError ()->getDescription ());
		return;
	}

	printf ("listening on UDP %s\n", addr.toString ());

//	io::CSockAddrIp AddrTo (0x7f000001, 1002);
//	size_t x = Sock.SyncSendTo ("hui!", 4, &AddrTo);

	exe::CFunctionT <
		exe::CArgT <io::CDgramSock*>, 
		io::CDgramSock::COnSendRecvCompleteArg> 
		onComplete (onRecvFrom, &sock);

	sock.recvFrom (buffer, 1023, &onComplete);

	char s [32];
	gets (s);
}

*/

//.............................................................................

} // namespace test_Sock
