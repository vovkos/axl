#include "pch.h"
#include "mainwindow.h"
#include "moc_axl_gui_QtWidget.cpp"
#include "axl_mt_Thread.h"
#include "axl_fsm_RegExp.h"

//.............................................................................

#if 0

bool
trySudo ()
{
	static char idPass [] = "55b828e362e44b9c8189e73d8eb01778";
	static char idEcho [] = "bb05199f9ad34eb0876dac63300e2b62";

	QString exeName = "/usr/bin/sudo";

	QStringList argList;
	argList.append ("-S");
	argList.append ("-p");
	argList.append (idPass);
	argList.append ("echo");
	argList.append (idEcho);

	for (;;)
	{
		QProcess process;
		process.setReadChannelMode (QProcess::MergedChannels);
		process.start (
			exeName,
			argList
			);

		process.waitForStarted ();

		QByteArray output;
		do
		{
			process.waitForReadyRead ();
			output += process.readAll ();
		} while (output.size () < sizeof (idEcho) - 1);

		if (memcmp (output.cbegin (), idEcho, sizeof (idEcho) - 1) == 0)
		{
			printf ("already a superuser\n");
			process.waitForFinished ();
			return true;
		}

		printf ("enter password: \n");

		char password [256];
		scanf ("%s", password);
		size_t length = strlen (password);

		if (length == 0)
		{
			printf ("cancelled\n");
			process.waitForFinished ();
			return false;
		}

		password [length++] = '\n';

		process.write (password, length);
		process.closeWriteChannel ();
		process.waitForFinished ();

		output = process.readAll ();
		length = output.size ();

		if (length >= sizeof (idEcho) - 1 &&
			memcmp (output.cbegin (), idEcho, sizeof (idEcho) - 1) == 0)
		{
			printf ("successfully authorized\n");
			return true;
		}
	}
}

#endif

#if 0

#include <sys/un.h>

int sendFd (
	int sock,
	int fd
	)
{
	enum
	{
		ControlBufferLength = CMSG_LEN (sizeof (int))
	};

	char controlBuffer [ControlBufferLength];
	char dummyData = '.';

	iovec dummyIov;
	dummyIov.iov_base = &dummyData;
	dummyIov.iov_len = sizeof (dummyData);

	msghdr msg;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &dummyIov;
	msg.msg_iovlen = 1;
	msg.msg_flags = 0;
	msg.msg_control = controlBuffer;
	msg.msg_controllen = ControlBufferLength;

	cmsghdr* cmsg;
	cmsg = CMSG_FIRSTHDR (&msg);
	cmsg->cmsg_len = CMSG_LEN (sizeof (int));
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	*(int*) CMSG_DATA (cmsg) = fd;

	return sendmsg (sock, &msg, 0);
}

int recvFd (int sock)
{
	enum
	{
		ControlBufferLength = CMSG_LEN (sizeof (int))
	};

	char controlBuffer [ControlBufferLength];
	char dummyData = '.';

	iovec dummyIov;
	dummyIov.iov_base = &dummyData;
	dummyIov.iov_len = sizeof (dummyData);

	msghdr msg;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &dummyIov;
	msg.msg_iovlen = 1;
	msg.msg_flags = 0;
	msg.msg_control = controlBuffer;
	msg.msg_controllen = ControlBufferLength;

	cmsghdr* cmsg;
	cmsg = CMSG_FIRSTHDR (&msg);
	cmsg->cmsg_len = CMSG_LEN (sizeof (int));
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;

	int result = recvmsg (sock, &msg, 0);
	if (result == -1)
		return -1;

	return *(int*) CMSG_DATA (cmsg);
}
#endif

#if 0

static char g_transmitData [] = "abcdefghijklmnopqrstuvwxyz";

class ReaderThread: public mt::ThreadImpl <ReaderThread>
{
protected:
	io::SharedMemoryReader m_reader;

public:
	void threadProc ()
	{
		bool result = m_reader.open (
			"/home/vladimir/test-transport",
			"test-transport-read",
			"test-transport-write",
			io::SharedMemoryTransportFlag_Message
			);

		if (!result)
		{
			printf ("cannot open reader (%s)\n", err::getLastError ()->getDescription ().cc ());
			return;
		}

		size_t offset = 0;

		rtl::Array <char> buffer;

		for (;;)
		{
			size_t size = m_reader.read (&buffer);
			if (size == -1)
			{
				printf ("cannot read (%s)\n", err::getLastError ()->getDescription ().cc ());
				return;
			}

			int cmpResult = memcmp (g_transmitData + offset, buffer, size);
			if (cmpResult != 0)
			{
				printf ("wrong data!\n");
				return;
			}

			offset += size;
			if (offset >= sizeof (g_transmitData))
				offset = 0;
		}
	}
};

class WriterThread: public mt::ThreadImpl <WriterThread>
{
protected:
	io::SharedMemoryWriter m_writer;

public:
	void threadProc ()
	{
		unlink ("/home/vladimir/test-transport");

		bool result = m_writer.open (
			"/home/vladimir/test-transport",
			"test-transport-read",
			"test-transport-write",
			io::SharedMemoryTransportFlag_Create | io::SharedMemoryTransportFlag_Message
			);

		if (!result)
		{
			printf ("cannot open writer (%s)\n", err::getLastError ()->getDescription ().cc ());
			return;
		}

		size_t offset = 0;

		for (;;)
		{
			const char* p = g_transmitData + offset;
			size_t size = 1 + rand () % (sizeof (g_transmitData) - 1);

			if (offset + size < sizeof (g_transmitData))
			{
				offset += size;
			}
			else
			{
				size = sizeof (g_transmitData) - offset;
				offset = 0;
			}

			size_t result = m_writer.write (p, size);
			if (result == -1)
			{
				printf ("cannot write (%s)\n", err::getLastError ()->getDescription ().cc ());
				return;
			}
		}
	}
};

#endif

int
main (
	int argc,
	char *argv[]
	)
{
	printf ("main ()\n");

//	char const* src = "abc";
//	char const* src = "a*(b|cd?)+";
	char const* src = "[ABCDefghijkl](b|c)*";

	fsm::RegExp regExp;
	bool result = regExp.compile (src);
	if (!result)
	{
		printf ("error: %s\n", err::getLastError ().getDescription ());
		return -1;
	}

	regExp.print ();

#if 0
	WriterThread writerThread;
	writerThread.start ();

	ReaderThread readerThread;
	readerThread.start ();

	char s [256];
	scanf ("%s", s);
	return -1;

	QApplication app (argc, argv);

	MainWindow mainWindow;
	mainWindow.show ();
	return app.exec ();
#endif

#if 0
	int fd = open ("/home/vladimir/suka-bla", O_RDWR | O_CREAT, 0666);
	if (fd == -1)
	{
		printf ("error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	int result;

	char s [] = "hui govno i muravei";
	write (fd, s, sizeof (s));
	lseek (fd, 0, SEEK_SET);

	char sockName [] = "/home/vladimir/hui-govno-i-muravei-2";
	char fifoName [] = "/home/vladimir/hui-govno-i-muravei-3";

	mode_t mode = 0;

	result = mkfifo (fifoName, 0666);
	if (result == -1)
	{
		printf ("mkfifo error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	int sockClient = socket (AF_UNIX, SOCK_STREAM, 0);
	int sockServer = socket (AF_UNIX, SOCK_STREAM, 0);
	ASSERT (sockClient != -1 && sockServer != -1);

	sockaddr_un addr = { 0 };
	addr.sun_family = AF_UNIX;
	memcpy (addr.sun_path, sockName, sizeof (sockName));

	result = bind (sockServer, (const sockaddr*) &addr, sizeof (addr));
	if (result == -1)
	{
		printf ("bind error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	result = listen (sockServer, 8);
	if (result == -1)
	{
		printf ("listen error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	result = connect (sockClient, (const sockaddr*) &addr, sizeof (addr));
	if (result == -1)
	{
		printf ("connect error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	socklen_t len = sizeof (addr);
	int sockAccept = accept (sockServer, (sockaddr*) &addr, &len);
	if (sockAccept == -1)
	{
		printf ("accept error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	result = sendFd (sockClient, fd);
	if (result == -1)
	{
		printf ("sendmsg error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	int fd2 = recvFd (sockAccept);
	if (fd2 == -1)
	{
		printf ("recvmsg error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	char buffer [256] = { 0 };
	result = read (fd2, buffer, sizeof (buffer));
	if (result == -1)
	{
		printf ("read error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	printf ("read: %s\n", buffer);
#endif

//	trySudo ();
//	trySudo ();

	return 0;
}

//.............................................................................
