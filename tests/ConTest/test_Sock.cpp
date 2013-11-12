#include "pch.h"

using namespace axl;

namespace test_Sock { 

//.............................................................................

/*

void 
OnAccept (ref::CPtrT <io::CConnectionSock> ConnectionSock)
{
	io::TSockAddrU LocalAddr;
	io::TSockAddrU PeerAddr;

	ConnectionSock->GetLocalAddress (&LocalAddr);
	ConnectionSock->GetPeerAddress (&PeerAddr);
	
	printf ("accepted connection from %s\n", PeerAddr.ToString ());
}

void Run_Listen ()
{
	io::CListenerSock Sock;	

	io::CSockAddrIp Addr (1001);

	bool Result = 
		Sock.Open (io::ESockProto_Tcp, Addr) &&
		Sock.Listen (32, &exe::CFunctionT <
			exe::CArgT <void>, 
			exe::CArgT <ref::CPtrT <io::CConnectionSock> > > (OnAccept));;
	
	if (!Result)
	{
		printf ("cannot listen on %s: %s\n", Addr.ToString (), err::GetError ()->GetDescription ());
		return;
	}

	printf ("listening on %s\n", Addr.ToString ());
	
	char s [32];
	gets (s);
}

//.............................................................................

void 
OnConnect (
	io::CConnectionSock* pSock,
	const err::CError& Error
	)
{
	io::TSockAddrU LocalAddr;
	io::TSockAddrU PeerAddr;

	pSock->GetLocalAddress (&LocalAddr);
	pSock->GetPeerAddress (&PeerAddr);

	if (!Error)
		printf ("established connection to %s from %s\n", PeerAddr.ToString (), LocalAddr.ToString ());
	else
		printf ("cannot establish connection: %s\n", Error->GetDescription ());
}

void 
Run_Connect ()
{
	exe::CWorkerThread WorkerThread;
	WorkerThread.Start ();

	io::CConnectionSock Sock;	

	io::CSockAddrIp Addr (0x7f000001, 1001);

	exe::CScheduledFunctionT <
		exe::CArgT <io::CConnectionSock*>, 
		io::CConnectionSock::COnConnectCompleteArg> 
		OnComplete (&WorkerThread, OnConnect, &Sock);

	bool Result = 
		Sock.Open (io::ESockProto_Tcp, io::ESockAddr_Ip) &&
		Sock.SyncConnect (&Addr, 1000);
	
	if (!Result)
	{
		printf ("cannot connect to %s: %s\n", Addr.ToString (), err::GetError ()->GetDescription ());
		return;
	}

	char s [32];
	gets (s);
}

//.............................................................................

char Buffer [1024] = { 0 };

void 
OnRecvFrom (
	io::CDgramSock* pSock,
	const err::CError& Error,
	io::TSockAddr* pAddr,
	size_t ActualSize
	)
{
	if (Error)
	{
		printf ("error during recvfrom: %s\n", Error->GetDescription ());
		return;
	}

	printf ("received %d bytes from %s: %s\n", ActualSize, pAddr->ToString (), Buffer);

	exe::CFunctionT <
		exe::CArgT <io::CDgramSock*>, 
		io::CDgramSock::COnSendRecvCompleteArg> 
		OnComplete (OnRecvFrom, pSock);

	pSock->RecvFrom (Buffer, 1023, &OnComplete);
}

void 
Run_Dgram ()
{
	io::CDgramSock Sock;	

	io::CSockAddrIp Addr (0, 1001);

	bool Result = Sock.Open (io::ESockProto_Udp, &Addr);
	if (!Result)
	{
		printf ("cannot open %s: %s\n", Addr.ToString (), err::GetError ()->GetDescription ());
		return;
	}

	printf ("listening on UDP %s\n", Addr.ToString ());

//	io::CSockAddrIp AddrTo (0x7f000001, 1002);
//	size_t x = Sock.SyncSendTo ("hui!", 4, &AddrTo);

	exe::CFunctionT <
		exe::CArgT <io::CDgramSock*>, 
		io::CDgramSock::COnSendRecvCompleteArg> 
		OnComplete (OnRecvFrom, &Sock);

	Sock.RecvFrom (Buffer, 1023, &OnComplete);

	char s [32];
	gets (s);
}

*/

//.............................................................................

} // namespace test_Sock
