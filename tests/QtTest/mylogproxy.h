#pragma once

#define MYLOGPROXY_H

//.............................................................................

class myLogProxy: public log::CPeer
{
protected:
	friend class socketThread;
	friend class packetizer;

	class socketThread: public mt::CThreadImplT <socketThread>
	{
	public:
		void threadProc ()
		{
			AXL_CONTAINING_RECORD (this, myLogProxy, m_socketThread)->socketThreadProc ();
		}
	};

	class packetizer: public rtl::CPacketizerT <packetizer>
	{
	public:
		void onPacket (const void* p, size_t size)
		{
			AXL_CONTAINING_RECORD (this, myLogProxy, m_packetizer)->onPacket (p, size);
		}
	};

protected:
	log::CPeer* m_target;
	socketThread m_socketThread;
	packetizer m_packetizer;
	SOCKET m_listenSocket;
	SOCKET m_connectSocket;
	
public:
	rtl::CString m_tag;

public:
	myLogProxy ();
	
	~myLogProxy ()
	{
		close ();
	}

	void close ();
	
	bool listen (log::CPeer* target, int port);
	bool connect (log::CPeer* target, int ip, int port);

	virtual void sendMsg (const log::TMsg* msg);

protected:
	void onPacket (const void* p, size_t size);
	void socketThreadProc ();
};

//.............................................................................
