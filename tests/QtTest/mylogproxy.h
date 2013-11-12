#pragma once

#define MYLOGPROXY_H

//.............................................................................

class MyLogProxy: public log::CPeer
{
protected:
	friend class SocketThread;
	friend class Packetizer;

	class SocketThread: public mt::CThreadImplT <SocketThread>
	{
	public:
		void ThreadProc ()
		{
			AXL_CONTAINING_RECORD (this, MyLogProxy, m_socketThread)->socketThreadProc ();
		}
	};

	class Packetizer: public rtl::CPacketizerT <Packetizer>
	{
	public:
		void OnPacket (const void* p, size_t size)
		{
			AXL_CONTAINING_RECORD (this, MyLogProxy, m_packetizer)->onPacket (p, size);
		}
	};

protected:
	log::CPeer* m_target;
	SocketThread m_socketThread;
	Packetizer m_packetizer;
	SOCKET m_listenSocket;
	SOCKET m_connectSocket;
	
public:
	rtl::CString m_Tag;

public:
	MyLogProxy ();
	
	~MyLogProxy ()
	{
		close ();
	}

	void close ();
	
	bool listen (log::CPeer* target, int port);
	bool connect (log::CPeer* target, int ip, int port);

	virtual void SendMsg (const log::TMsg* msg);

protected:
	void onPacket (const void* p, size_t size);
	void socketThreadProc ();
};

//.............................................................................
