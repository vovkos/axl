#include "pch.h"

namespace test_Sock { 

//.............................................................................

/*

void 
onAccept (ref::Ptr <io::ConnectionSock> connectionSock)
{
	io::SockAddrU localAddr;
	io::SockAddrU peerAddr;

	connectionSock->getLocalAddress (&localAddr);
	connectionSock->getPeerAddress (&peerAddr);
	
	printf ("accepted connection from %s\n", peerAddr.toString ());
}

void run_Listen ()
{
	io::ListenerSock sock;	

	io::SockAddrIp addr (1001);

	bool result = 
		sock.open (io::SockProtoKind_Tcp, addr) &&
		sock.listen (32, &exe::Function <
			exe::Arg <void>, 
			exe::Arg <ref::Ptr <io::ConnectionSock> > > (onAccept));;
	
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
	io::ConnectionSock* sock,
	const err::Error& error
	)
{
	io::SockAddrU localAddr;
	io::SockAddrU peerAddr;

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
	exe::Workerhread workerThread;
	workerThread.start ();

	io::ConnectionSock sock;	

	io::SockAddrIp addr (0x7f000001, 1001);

	exe::ScheduledFunction <
		exe::Arg <io::ConnectionSock*>, 
		io::ConnectionSock::OnConnectCompleteArg> 
		onComplete (&workerThread, onConnect, &sock);

	bool result = 
		sock.open (io::SockProtoKind_Tcp, io::SockAddrKind_Ip) &&
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
	io::DgramSock* sock,
	const err::Error& error,
	io::SockAddr* addr,
	size_t actualSize
	)
{
	if (error)
	{
		printf ("error during recvfrom: %s\n", error->getDescription ());
		return;
	}

	printf ("received %d bytes from %s: %s\n", actualSize, addr->toString (), buffer);

	exe::Function <
		exe::Arg <io::DgramSock*>, 
		io::DgramSock::OnSendRecvCompleteArg> 
		onComplete (onRecvFrom, sock);

	sock->recvFrom (buffer, 1023, &onComplete);
}

void 
run_Dgram ()
{
	io::DgramSock sock;	

	io::SockAddrIp addr (0, 1001);

	bool result = sock.open (io::SockProtoKind_Udp, &addr);
	if (!result)
	{
		printf ("cannot open %s: %s\n", addr.toString (), err::getError ()->getDescription ());
		return;
	}

	printf ("listening on UDP %s\n", addr.toString ());

//	io::CSockAddrIp AddrTo (0x7f000001, 1002);
//	size_t x = Sock.SyncSendTo ("hui!", 4, &AddrTo);

	exe::Function <
		exe::Arg <io::DgramSock*>, 
		io::DgramSock::OnSendRecvCompleteArg> 
		onComplete (onRecvFrom, &sock);

	sock.recvFrom (buffer, 1023, &onComplete);

	char s [32];
	gets (s);
}

*/

//.............................................................................

} // namespace test_Sock
