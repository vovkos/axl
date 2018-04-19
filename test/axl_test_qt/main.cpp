//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "mainwindow.h"
#include "moc_axl_gui_QtWidget.cpp"
#include "axl_sys_Thread.h"
#include "axl_fsm_Regex.h"

//..............................................................................

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
			printf ("cannot open reader (%s)\n", err::getLastErrorDescription ().sz ());
			return;
		}

		size_t offset = 0;

		sl::Array <char> buffer;

		for (;;)
		{
			size_t size = m_reader.read (&buffer);
			if (size == -1)
			{
				printf ("cannot read (%s)\n", err::getLastErrorDescription ().sz ());
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
			printf ("cannot open writer (%s)\n", err::getLastErrorDescription ().sz ());
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
				printf ("cannot write (%s)\n", err::getLastErrorDescription ().sz ());
				return;
			}
		}
	}
};

#endif

void
testUtf ()
{
	setbuf(stdout, NULL);

	printf ("Testing UTF...\n");

	for (uint_t c = 0; c < 0x10ffff; c++)
	{
		if (axl::enc::utfIsDigit (c) != QChar::isDigit (c))
			printf ("%x\taxl::enc::utfIsDigit -> %d\tQChar::isDigit -> %d\n", c, axl::enc::utfIsDigit (c), QChar::isDigit (c));

		if (axl::enc::utfIsLetter (c) != QChar::isLetter (c))
			printf ("%x\taxl::enc::utfIsLetter -> %d\tQChar::isLetter -> %d\n", c, axl::enc::utfIsLetter (c), QChar::isLetter (c));

		if (axl::enc::utfIsNumber (c) != QChar::isNumber (c))
			printf ("%x\taxl::enc::utfIsNumber -> %d\tQChar::isNumber -> %d\n", c, axl::enc::utfIsNumber (c), QChar::isNumber (c));

		if (axl::enc::utfIsLetterOrDigit (c) != (QChar::isLetter (c) || QChar::isDigit (c)))
			printf ("%x\taxl::enc::utfIsLetterOrDigit -> %d\tQChar::isLetter -> %d\tQChar::isDigit -> %d\n", c, axl::enc::utfIsLetterOrDigit (c), QChar::isLetter (c), QChar::isDigit (c));

		if (axl::enc::utfIsLetterOrNumber (c) != QChar::isLetterOrNumber (c))
			printf ("%x\taxl::enc::utfIsLetterOrNumber -> %d\tQChar::isLetterOrNumber  -> %d\n", c, axl::enc::utfIsLetterOrNumber (c), QChar::isLetterOrNumber  (c));

		if (axl::enc::utfIsPunctuation (c) != QChar::isPunct (c))
			printf ("%x\taxl::enc::utfIsPunctuation -> %d\tQChar::isPunct -> %d\n", c, axl::enc::utfIsPunctuation (c), QChar::isPunct (c));

		if (axl::enc::utfIsSpace (c) != QChar::isSpace (c))
			printf ("%x\taxl::enc::utfIsSpace -> %d\tQChar::isDigit -> %d\n", c, axl::enc::utfIsSpace (c), QChar::isSpace (c));

		if (axl::enc::utfIsPrintable (c) != QChar::isPrint (c))
			printf ("%x\taxl::enc::utfIsPrintable -> %d\tQChar::isPrint -> %d\n", c, axl::enc::utfIsPrintable (c), QChar::isPrint (c));

		if (axl::enc::utfIsPrintableNonMark (c) != (QChar::isPrint (c) && !QChar::isMark (c)))
			printf ("%x\taxl::enc::utfIsPrintableNonMark -> %d\tQChar::isPrint -> %d\tQChar::isMark -> %d\n", c, axl::enc::utfIsPrintableNonMark (c), QChar::isPrint (c), QChar::isMark (c));

		if (axl::enc::utfIsLowerCase (c) != QChar::isLower (c))
			printf ("%x\taxl::enc::utfIsLowerCase -> %d\tQChar::isLower -> %d\n", c, axl::enc::utfIsLowerCase (c), QChar::isLower (c));

		if (axl::enc::utfIsUpperCase (c) != QChar::isUpper (c))
			printf ("%x\taxl::enc::utfIsUpperCase -> %d\tQChar::isUpper -> %d\n", c, axl::enc::utfIsUpperCase (c), QChar::isUpper (c));

		if (axl::enc::utfToLowerCase (c) != QChar::toLower (c))
			printf ("%x\taxl::enc::utfToLowerCase -> %x\tQChar::toLower -> %x\n", c, axl::enc::utfToLowerCase (c), QChar::toLower (c));

		if (axl::enc::utfToUpperCase (c) != QChar::toUpper (c))
			printf ("%x\taxl::enc::utfToUpperCase -> %x\tQChar::toUpper -> %x\n", c, axl::enc::utfToUpperCase (c), QChar::toUpper (c));

		if (axl::enc::utfToCaseFolded (c) != QChar::toCaseFolded (c))
			printf ("%x\taxl::enc::utfToCaseFolded -> %x\tQChar::toCaseFolded -> %x\n", c, axl::enc::utfToCaseFolded (c), QChar::toCaseFolded (c));
	}

	printf ("Done\n");
}

void ulltoa (quint64 l, char* buff, size_t size)
{
	size_t last = size - 1;
	memset (buff, 'a', last);
	buff [last] = 0;

	char *p = buff + last;

	while (l != 0) {
		int c = l % 10;

		--p;

		if (c < 10)
			*p = '0' + c;
		else
			*p = c - 10 + 'a';

		l /= 10;
	}
}

size_t formatIntegerWithThousandSep_stl (std::string* string, quint64 l)
{
	char buff [65];
	ulltoa (l, buff, sizeof (buff));

	*string = buff;

	intptr_t i = string->length () - 3;
	for (; i > 0; i -= 3)
		string->insert (i, 1, ',');

	return string->length ();
}

#define _RETURN_STRING 1

#if (_RETURN_STRING)
std::string
#else
void
#endif
formatIntegerWithThousandSep_stl (quint64 value)
{
	std::string string;
	formatIntegerWithThousandSep_stl (&string, value);
#if (_RETURN_STRING)
	return string;
#endif
}

size_t formatIntegerWithThousandSep_qt (QString* string, quint64 l)
{
	char buff [65];
	ulltoa (l, buff, sizeof (buff));

	*string = QString::fromUtf8 (buff, sizeof (buff) - 1);

	int i = string->length () - 3;
	for (; i > 0; i -= 3)
		string->insert (i, ',');

	return string->length ();
}

#if (_RETURN_STRING)
QString
#else
void
#endif
formatIntegerWithThousandSep_qt (quint64 value)
{
	QString string;
	formatIntegerWithThousandSep_qt (&string, value);
#if (_RETURN_STRING)
	return string;
#endif
}

size_t formatIntegerWithThousandSep_axl (sl::String* string, quint64 l)
{
	char buff [65];
	ulltoa (l, buff, sizeof (buff));

	string->copy (buff, sizeof (buff) - 1);

	intptr_t i = string->getLength () - 3;
	for (; i > 0; i -= 3)
		string->insert (i, ',');

	return string->getLength ();
}

#if (_RETURN_STRING)
sl::String
#else
void 
#endif
formatIntegerWithThousandSep_axl (quint64 value)
{
	sl::String string;
	formatIntegerWithThousandSep_axl (&string, value);
#if (_RETURN_STRING)
	return string;
#endif
}

inline
size_t
getMinPower2Gt (size_t size)
{
	ASSERT ((intptr_t) size >= 0); // hi-bit is already set
	return size ? 2 << sl::getHiBitIdx (size) : 1;
}

inline
size_t
getMinPower2Ge (size_t size)
{
	return !(size & (size - 1)) ? size : getMinPower2Gt (size);
}

void benchFormat ()
{
	enum 
	{
		IterationCount = 8 * 1024 * 1024,
		InterlockedIterationCount = 32 * 1024,
		BitIterationCount = 256 * 1024 * 1024,
	};

	printf ("__cplusplus = %d\n", __cplusplus);
	printf ("AXL_CPP_MSC_VERSION = %x\n", AXL_CPP_MSC_VERSION);
	
	std::string stlString;
	QString qtString;
	sl::String axlString;

	printf ("sizeof (std::string) = %d\n", sizeof (std::string));
	printf ("sizeof (axl::sl::String) = %d\n", sizeof (axl::sl::String));

	uint64_t baseTimestamp;
	uint64_t time;
	size_t counter;

	printf ("Testing Hacker's delight...\n");
	baseTimestamp = sys::getTimestamp ();

	counter = 0;

	for (size_t i = 0; i < InterlockedIterationCount; i++)
	for (size_t j = 0; j < InterlockedIterationCount; j++)
	{
		counter += sl::getHiBit (j);
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %d %s\n", counter, sys::Time (time, 0).format ("%m:%s.%l").sz ());

	printf ("Testing old index-based search...\n");
	baseTimestamp = sys::getTimestamp ();

	counter = 0;

	for (size_t i = 0; i < InterlockedIterationCount; i++)
	for (size_t j = 0; j < InterlockedIterationCount; j++)
	{
		counter += getMinPower2Ge (j);
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %d %s\n", counter, sys::Time (time, 0).format ("%m:%s.%l").sz ());

	printf ("Testing Interlocked (intrinsic, i32)...\n");
	baseTimestamp = sys::getTimestamp ();

	volatile long i32;
	volatile long long i64;

	for (size_t i = 0; i < InterlockedIterationCount; i++)
	for (size_t j = 0; j < InterlockedIterationCount; j++)
	{
		_InterlockedIncrement (&i32);
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %s\n", sys::Time (time, 0).format ("%m:%s.%l").sz ());

	printf ("Testing Interlocked (winapi, i32)...\n");
	baseTimestamp = sys::getTimestamp ();

	for (size_t i = 0; i < InterlockedIterationCount; i++)
	for (size_t j = 0; j < InterlockedIterationCount; j++)
	{
		::InterlockedIncrement (&i32);
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %s\n", sys::Time (time, 0).format ("%m:%s.%l").sz ());

#if (_AXL_CPU_AMD64)
	printf ("Testing Interlocked (intrinsic, i64)...\n");
	baseTimestamp = sys::getTimestamp ();

	for (size_t i = 0; i < InterlockedIterationCount; i++)
	for (size_t j = 0; j < InterlockedIterationCount; j++)
	{
		_InterlockedIncrement64 (&i64);
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %s\n", sys::Time (time, 0).format ("%m:%s.%l").sz ());
#endif

	printf ("Testing Interlocked (winapi, i64)...\n");
	baseTimestamp = sys::getTimestamp ();

	for (size_t i = 0; i < InterlockedIterationCount; i++)
	for (size_t j = 0; j < InterlockedIterationCount; j++)
	{
		::InterlockedIncrement64 (&i64);
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %s\n", sys::Time (time, 0).format ("%m:%s.%l").sz ());

	printf ("Testing STL...\n");
	baseTimestamp = sys::getTimestamp ();

	for (size_t i = 0; i < IterationCount; i++)
	{
//		std::wstring s (L"abcdefghijklmnopqrstuvwxyz");
#if (_RETURN_STRING)
		stlString = formatIntegerWithThousandSep_stl (i);
#else
		formatIntegerWithThousandSep_stl (i);
#endif
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %s\n", sys::Time (time, 0).format ("%m:%s.%l").sz ());
/*
	printf ("Testing STL (no alloc)...\n");
	baseTimestamp = sys::getTimestamp ();

	for (size_t i = 0; i < IterationCount; i++)
	{
		formatIntegerWithThousandSep_stl (&stlString, i);
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %s\n", sys::Time (time, 0).format ("%m:%s.%l").sz ());
*/
	printf ("Testing QT...\n");
	baseTimestamp = sys::getTimestamp ();

	for (size_t i = 0; i < IterationCount; i++)
	{
//		QString s ((QChar*) L"abcdefghijklmnopqrstuvwxyz");
#if (_RETURN_STRING)
		qtString = formatIntegerWithThousandSep_qt (i);
#else
		formatIntegerWithThousandSep_qt (i);
#endif
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %s\n", sys::Time (time, 0).format ("%m:%s.%l").sz ());
/*
	printf ("Testing QT (no alloc)...\n");
	baseTimestamp = sys::getTimestamp ();

	for (size_t i = 0; i < IterationCount; i++)
	{
		formatIntegerWithThousandSep_qt (&qtString, i);
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %s\n", sys::Time (time, 0).format ("%m:%s.%l").sz ());
*/
	printf ("Testing AXL...\n");
	baseTimestamp = sys::getTimestamp ();

	for (size_t i = 0; i < IterationCount; i++)
	{
//		sl::String_w s (L"abcdefghijklmnopqrstuvwxyz");
#if (_RETURN_STRING)
		axlString = formatIntegerWithThousandSep_axl (i);
#else
		formatIntegerWithThousandSep_axl (i);
#endif
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %s\n", sys::Time (time, 0).format ("%m:%s.%l").sz ());
/*
	printf ("Testing AXL (no alloc)...\n");
	baseTimestamp = sys::getTimestamp ();

	for (size_t i = 0; i < IterationCount; i++)
	{
		formatIntegerWithThousandSep_axl (&axlString, i);
	}

	time = sys::getTimestamp () - baseTimestamp;
	printf ("Done: %s\n", sys::Time (time, 0).format ("%m:%s.%l").sz ());
*/
}


int
main (
	int argc,
	char *argv[]
	)
{
	g::getModule ()->setTag ("axl_test_qt");
	benchFormat ();
	return 0;

#if 0
	testUtf ();
	return 0;
#endif

#if 0

//	char const* src = "abc";
//	char const* src = "a*(b|cd?)+";
	char const* src = "[ABCDefghijkl](b|c)*";

	fsm::Regex regex;
	bool result = regex.compile (src);
	if (!result)
	{
		printf ("error: %s\n", err::getLastErrorDescription ().sz ());
		return -1;
	}

	regex.print ();
#endif

#if 0
	WriterThread writerThread;
	writerThread.start ();

	ReaderThread readerThread;
	readerThread.start ();

	char s [256];
	scanf ("%s", s);
	return -1;
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

#if 0
	trySudo ();
	trySudo ();
#endif

	QApplication app (argc, argv);

	MainWindow mainWindow;
	mainWindow.show ();
	return app.exec ();
}

//..............................................................................
