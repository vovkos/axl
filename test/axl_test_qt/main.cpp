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
#include "axl_sys_Thread.h"
#include "axl_re_Regex.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include "axl_sys_DynamicLib.h"

//..............................................................................

#if 0

bool
trySudo() {
	static char idPass[] = "55b828e362e44b9c8189e73d8eb01778";
	static char idEcho[] = "bb05199f9ad34eb0876dac63300e2b62";

	QString exeName = "/usr/bin/sudo";

	QStringList argList;
	argList.append("-S");
	argList.append("-p");
	argList.append(idPass);
	argList.append("echo");
	argList.append(idEcho);

	for (;;) {
		QProcess process;
		process.setReadChannelMode(QProcess::MergedChannels);
		process.start(
			exeName,
			argList
		);

		process.waitForStarted();

		QByteArray output;
		do {
			process.waitForReadyRead();
			output += process.readAll();
		} while (output.size() < sizeof(idEcho) - 1);

		if (memcmp(output.cbegin(), idEcho, sizeof(idEcho) - 1) == 0) {
			printf("already a superuser\n");
			process.waitForFinished();
			return true;
		}

		printf("enter password: \n");

		char password[256];
		scanf("%s", password);
		size_t length = strlen(password);

		if (length == 0) {
			printf("cancelled\n");
			process.waitForFinished();
			return false;
		}

		password[length++] = '\n';

		process.write(password, length);
		process.closeWriteChannel();
		process.waitForFinished();

		output = process.readAll();
		length = output.size();

		if (length >= sizeof(idEcho) - 1 &&
			memcmp(output.cbegin(), idEcho, sizeof(idEcho) - 1) == 0) {
			printf("successfully authorized\n");
			return true;
		}
	}
}

#endif

#if 0

#include <sys/un.h>

int sendFd(
	int sock,
	int fd
) {
	enum {
		ControlBufferLength = CMSG_LEN(sizeof(int))
	};

	char controlBuffer[ControlBufferLength];
	char dummyData = '.';

	iovec dummyIov;
	dummyIov.iov_base = &dummyData;
	dummyIov.iov_len = sizeof(dummyData);

	msghdr msg;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &dummyIov;
	msg.msg_iovlen = 1;
	msg.msg_flags = 0;
	msg.msg_control = controlBuffer;
	msg.msg_controllen = ControlBufferLength;

	cmsghdr* cmsg;
	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_len = CMSG_LEN(sizeof(int));
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	*(int*)CMSG_DATA(cmsg) = fd;

	return sendmsg(sock, &msg, 0);
}

int recvFd(int sock) {
	enum {
		ControlBufferLength = CMSG_LEN(sizeof(int))
	};

	char controlBuffer[ControlBufferLength];
	char dummyData = '.';

	iovec dummyIov;
	dummyIov.iov_base = &dummyData;
	dummyIov.iov_len = sizeof(dummyData);

	msghdr msg;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &dummyIov;
	msg.msg_iovlen = 1;
	msg.msg_flags = 0;
	msg.msg_control = controlBuffer;
	msg.msg_controllen = ControlBufferLength;

	cmsghdr* cmsg;
	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_len = CMSG_LEN(sizeof(int));
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;

	int result = recvmsg(sock, &msg, 0);
	if (result == -1)
		return -1;

	return *(int*)CMSG_DATA(cmsg);
}
#endif

#if 0

static char g_transmitData[] = "abcdefghijklmnopqrstuvwxyz";

class ReaderThread: public mt::ThreadImpl<ReaderThread> {
protected:
	io::ShmtReader m_reader;

public:
	void threadProc() {
		bool result = m_reader.open(
			"/home/vladimir/test-transport",
			"test-transport-read",
			"test-transport-write",
			io::ShmtFlag_Message | io::FileFlag_OpenExisting
		);

		if (!result) {
			printf("cannot open reader (%s)\n", err::getLastErrorDescription().sz());
			return;
		}

		size_t offset = 0;

		sl::Array<char> buffer;

		for (;;) {
			size_t size = m_reader.read(&buffer);
			if (size == -1) {
				printf("cannot read (%s)\n", err::getLastErrorDescription().sz());
				return;
			}

			int cmpResult = memcmp(g_transmitData + offset, buffer, size);
			if (cmpResult != 0) {
				printf("wrong data!\n");
				return;
			}

			offset += size;
			if (offset >= sizeof(g_transmitData))
				offset = 0;
		}
	}
};

class WriterThread: public mt::ThreadImpl<WriterThread> {
protected:
	io::ShmtWriter m_writer;

public:
	void threadProc() {
		unlink("/home/vladimir/test-transport");

		bool result = m_writer.open(
			"/home/vladimir/test-transport",
			"test-transport-read",
			"test-transport-write",
			io::ShmtFlag_Message
		);

		if (!result) {
			printf("cannot open writer (%s)\n", err::getLastErrorDescription().sz());
			return;
		}

		size_t offset = 0;

		for (;;) {
			const char* p = g_transmitData + offset;
			size_t size = 1 + rand() % (sizeof(g_transmitData) - 1);

			if (offset + size < sizeof(g_transmitData)) {
				offset += size;
			} else {
				size = sizeof(g_transmitData) - offset;
				offset = 0;
			}

			size_t result = m_writer.write(p, size);
			if (result == -1) {
				printf("cannot write (%s)\n", err::getLastErrorDescription().sz());
				return;
			}
		}
	}
};

#endif

void
testUtf() {
	setbuf(stdout, NULL);

	printf("Testing UTF...\n");

	for (uint_t c = 0; c < 0x10ffff; c++) {
		if (axl::enc::isDigit(c) != QChar::isDigit(c))
			printf("%x\taxl::enc::isDigit -> %d\tQChar::isDigit -> %d\n", c, axl::enc::isDigit (c), QChar::isDigit (c));

		if (axl::enc::isLetter(c) != QChar::isLetter(c))
			printf("%x\taxl::enc::isLetter -> %d\tQChar::isLetter -> %d\n", c, axl::enc::isLetter (c), QChar::isLetter (c));

		if (axl::enc::isNumber(c) != QChar::isNumber(c))
			printf("%x\taxl::enc::isNumber -> %d\tQChar::isNumber -> %d\n", c, axl::enc::isNumber (c), QChar::isNumber (c));

		if (axl::enc::isLetterOrDigit(c) != (QChar::isLetter(c) || QChar::isDigit(c)))
			printf("%x\taxl::enc::isLetterOrDigit -> %d\tQChar::isLetter -> %d\tQChar::isDigit -> %d\n", c, axl::enc::isLetterOrDigit (c), QChar::isLetter (c), QChar::isDigit (c));

		if (axl::enc::isLetterOrNumber(c) != QChar::isLetterOrNumber(c))
			printf("%x\taxl::enc::isLetterOrNumber -> %d\tQChar::isLetterOrNumber  -> %d\n", c, axl::enc::isLetterOrNumber (c), QChar::isLetterOrNumber  (c));

		if (axl::enc::isPunctuation(c) != QChar::isPunct(c))
			printf("%x\taxl::enc::isPunctuation -> %d\tQChar::isPunct -> %d\n", c, axl::enc::isPunctuation (c), QChar::isPunct (c));

		if (axl::enc::isSpace(c) != QChar::isSpace(c))
			printf("%x\taxl::enc::isSpace -> %d\tQChar::isDigit -> %d\n", c, axl::enc::isSpace (c), QChar::isSpace (c));

		if (axl::enc::isPrintable(c) != QChar::isPrint(c))
			printf("%x\taxl::enc::isPrintable -> %d\tQChar::isPrint -> %d\n", c, axl::enc::isPrintable (c), QChar::isPrint (c));

		if (axl::enc::isPrintableNonMark(c) != (QChar::isPrint(c) && !QChar::isMark(c)))
			printf("%x\taxl::enc::isPrintableNonMark -> %d\tQChar::isPrint -> %d\tQChar::isMark -> %d\n", c, axl::enc::isPrintableNonMark (c), QChar::isPrint (c), QChar::isMark (c));

		if (axl::enc::isLowerCase(c) != QChar::isLower(c))
			printf("%x\taxl::enc::isLowerCase -> %d\tQChar::isLower -> %d\n", c, axl::enc::isLowerCase (c), QChar::isLower (c));

		if (axl::enc::isUpperCase(c) != QChar::isUpper(c))
			printf("%x\taxl::enc::isUpperCase -> %d\tQChar::isUpper -> %d\n", c, axl::enc::isUpperCase (c), QChar::isUpper (c));

		if (axl::enc::toLowerCase(c) != QChar::toLower(c))
			printf("%x\taxl::enc::toLowerCase -> %x\tQChar::toLower -> %x\n", c, axl::enc::toLowerCase (c), QChar::toLower (c));

		if (axl::enc::toUpperCase(c) != QChar::toUpper(c))
			printf("%x\taxl::enc::toUpperCase -> %x\tQChar::toUpper -> %x\n", c, axl::enc::toUpperCase (c), QChar::toUpper (c));

		if (axl::enc::toCaseFolded(c) != QChar::toCaseFolded(c))
			printf("%x\taxl::enc::toCaseFolded -> %x\tQChar::toCaseFolded -> %x\n", c, axl::enc::toCaseFolded (c), QChar::toCaseFolded (c));
	}

	printf("Done\n");
}

void ulltoa(quint64 l, char* buff, size_t size) {
	size_t last = size - 1;
	memset(buff, 'a', last);
	buff[last] = 0;

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

size_t formatIntegerWithThousandSep_stl(std::string* string, quint64 l) {
	char buff[65];
	ulltoa(l, buff, sizeof(buff));

	*string = buff;

	intptr_t i = string->length() - 3;
	for (; i > 0; i -= 3)
		string->insert(i, 1, ',');

	return string->length();
}

#define _RETURN_STRING 1

#if (_RETURN_STRING)
std::string
#else
void
#endif
formatIntegerWithThousandSep_stl(quint64 value) {
	std::string string;
	formatIntegerWithThousandSep_stl(&string, value);
#if (_RETURN_STRING)
	return string;
#endif
}

size_t formatIntegerWithThousandSep_qt(QString* string, quint64 l) {
	char buff[65];
	ulltoa(l, buff, sizeof(buff));

	*string = QString::fromUtf8(buff, sizeof(buff) - 1);

	int i = string->length() - 3;
	for (; i > 0; i -= 3)
		string->insert(i, ',');

	return string->length();
}

#if (_RETURN_STRING)
QString
#else
void
#endif
formatIntegerWithThousandSep_qt(quint64 value) {
	QString string;
	formatIntegerWithThousandSep_qt(&string, value);
#if (_RETURN_STRING)
	return string;
#endif
}

size_t formatIntegerWithThousandSep_axl(sl::String* string, quint64 l) {
	char buff[65];
	ulltoa(l, buff, sizeof(buff));

	string->copy(buff, sizeof(buff) - 1);

	intptr_t i = string->getLength() - 3;
	for (; i > 0; i -= 3)
		string->insert(i, ',');

	return string->getLength();
}

#if (_RETURN_STRING)
sl::String
#else
void
#endif
formatIntegerWithThousandSep_axl(quint64 value) {
	sl::String string;
	formatIntegerWithThousandSep_axl(&string, value);
#if (_RETURN_STRING)
	return string;
#endif
}

inline
size_t
getMinPower2Gt(size_t size) {
	ASSERT((intptr_t)size >= 0); // hi-bit is already set
	return size ? 2 << sl::getHiBitIdx(size) : 1;
}

inline
size_t
getMinPower2Ge(size_t size) {
	return !(size & (size - 1)) ? size : getMinPower2Gt(size);
}

void benchFormat() {
	enum {
		IterationCount = 32 * 1024 * 1024,
		InterlockedIterationCount = 32 * 1024,
		BitIterationCount = 64 * 1024,
	};

	printf("__cplusplus = %d\n", __cplusplus);

#ifdef AXL_CPP_MSC_VERSION
	printf("AXL_CPP_MSC_VERSION = %x\n", AXL_CPP_MSC_VERSION);
#endif

	std::string stlString;
	QString qtString;
	sl::String axlString;

	printf("sizeof(std::string) = %d\n", sizeof(std::string));
	printf("sizeof(axl::sl::String) = %d\n", sizeof(axl::sl::String));

	uint64_t baseTimestamp;
	uint64_t time;
	size_t counter;

	printf("Testing Hacker's delight...\n");
	baseTimestamp = sys::getTimestamp();

	counter = 0;

	for (size_t i = 0; i < BitIterationCount; i++)
	for (size_t j = 0; j < BitIterationCount; j++) {
		counter += sl::getPowerOf2Ge(j);
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %d %s\n", counter, sys::Time(time, 0).format("%m:%s.%l").sz());

	printf("Testing old index-based search...\n");
	baseTimestamp = sys::getTimestamp();

	counter = 0;

	for (size_t i = 0; i < BitIterationCount; i++)
	for (size_t j = 0; j < BitIterationCount; j++) {
		counter += getMinPower2Ge(j);
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %d %s\n", counter, sys::Time(time, 0).format("%m:%s.%l").sz());

#if (_AXL_OS_WIN)
	printf("Testing Interlocked (intrinsic, i32)...\n");
	baseTimestamp = sys::getTimestamp();

	volatile long i32;
	volatile long long i64;

	for (size_t i = 0; i < InterlockedIterationCount; i++)
	for (size_t j = 0; j < InterlockedIterationCount; j++) {
		_InterlockedIncrement(&i32);
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m:%s.%l").sz());

	printf("Testing Interlocked (winapi, i32)...\n");
	baseTimestamp = sys::getTimestamp();

	for (size_t i = 0; i < InterlockedIterationCount; i++)
	for (size_t j = 0; j < InterlockedIterationCount; j++) {
		::InterlockedIncrement(&i32);
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m:%s.%l").sz());

#if (_AXL_CPU_AMD64)
	printf("Testing Interlocked (intrinsic, i64)...\n");
	baseTimestamp = sys::getTimestamp();

	for (size_t i = 0; i < InterlockedIterationCount; i++)
	for (size_t j = 0; j < InterlockedIterationCount; j++) {
		_InterlockedIncrement64(&i64);
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m:%s.%l").sz());
#endif

	printf("Testing Interlocked (winapi, i64)...\n");
	baseTimestamp = sys::getTimestamp();

	for (size_t i = 0; i < InterlockedIterationCount; i++)
	for (size_t j = 0; j < InterlockedIterationCount; j++) {
		::InterlockedIncrement64(&i64);
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m:%s.%l").sz());
#endif

	printf("Testing STL...\n");
	baseTimestamp = sys::getTimestamp();

	for (size_t i = 0; i < IterationCount; i++) {
//		std::wstring s (L"abcdefghijklmnopqrstuvwxyz");
#if (_RETURN_STRING)
		stlString = formatIntegerWithThousandSep_stl(i);
#else
		formatIntegerWithThousandSep_stl(i);
#endif
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m:%s.%l").sz());

	printf("Testing STL (no alloc)...\n");
	baseTimestamp = sys::getTimestamp();

	for (size_t i = 0; i < IterationCount; i++) {
		formatIntegerWithThousandSep_stl(&stlString, i);
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m:%s.%l").sz());

	printf("Testing QT...\n");
	baseTimestamp = sys::getTimestamp();

	for (size_t i = 0; i < IterationCount; i++) {
//		QString s ((QChar*) L"abcdefghijklmnopqrstuvwxyz");
#if (_RETURN_STRING)
		qtString = formatIntegerWithThousandSep_qt(i);
#else
		formatIntegerWithThousandSep_qt(i);
#endif
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m:%s.%l").sz());

	printf("Testing QT (no alloc)...\n");
	baseTimestamp = sys::getTimestamp();

	for (size_t i = 0; i < IterationCount; i++) {
		formatIntegerWithThousandSep_qt(&qtString, i);
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m:%s.%l").sz());

	printf("Testing AXL...\n");
	baseTimestamp = sys::getTimestamp();

	for (size_t i = 0; i < IterationCount; i++) {
//		sl::String_w s (L"abcdefghijklmnopqrstuvwxyz");
#if (_RETURN_STRING)
		axlString = formatIntegerWithThousandSep_axl(i);
#else
		formatIntegerWithThousandSep_axl(i);
#endif
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m:%s.%l").sz());

	printf("Testing AXL (no alloc)...\n");
	baseTimestamp = sys::getTimestamp();

	for (size_t i = 0; i < IterationCount; i++) {
		formatIntegerWithThousandSep_axl(&axlString, i);
	}

	time = sys::getTimestamp() - baseTimestamp;
	printf("Done: %s\n", sys::Time(time, 0).format("%m:%s.%l").sz());
}

void
addRootCaCertificates() {
	char data[] =
		"-----BEGIN CERTIFICATE-----\n"
		"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\n"
		"-----END CERTIFICATE-----\n";

	char digicert[] =
		"-----BEGIN CERTIFICATE-----\n"
		"MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n"
		"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
		"d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n"
		"ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n"
		"MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n"
		"LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n"
		"RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n"
		"+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n"
		"PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n"
		"xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n"
		"Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n"
		"hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n"
		"EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n"
		"MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n"
		"FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n"
		"nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n"
		"eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n"
		"hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n"
		"Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n"
		"vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n"
		"+OkuE6N36B9K\n"
		"-----END CERTIFICATE-----\n";

	char data2[] =
		"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\n";

	QByteArray decoded = QByteArray::fromBase64(QByteArray::fromRawData(data2, sizeof(data2)));
	const unsigned char *p = (const unsigned char *)decoded.data();
	X509* x509 = d2i_X509(0, &p, decoded.size());
	ASN1_TIME *nbef = X509_get_notBefore(x509);
	ASN1_TIME *naft = X509_get_notAfter(x509);

	long versionNumber = QSslSocket::sslLibraryVersionNumber();
	QString versionString = QSslSocket::sslLibraryVersionString();
	printf("SSL used by QT: 0x%lx %s (%d)\n", versionNumber, versionString.toUtf8().data(), versionString.length());

	QByteArray pem = QByteArray::fromRawData(data, lengthof(data));
	QList<QSslCertificate> list = QSslCertificate::fromData(pem, QSsl::Pem);
	QList<QSslCertificate>::ConstIterator it = list.begin();
	for (; it != list.end(); it++)
		QSslSocket::addDefaultCaCertificate(*it);
}

int
main(
	int argc,
	char *argv[]
) {
#if (_AXL_OS_POSIX)
	setvbuf(stdout, NULL, _IOLBF, 1024);
#endif

#if (0)
	benchFormat();
	return 0;
#endif

#if (0)
	testUtf();
	return 0;
#endif

#if (0)

//	char const* src = "abc";
//	char const* src = "a*(b|cd?)+";
	char const* src = "[ABCDefghijkl](b|c)*";

	re::Regex regex;
	bool result = regex.compile(src);
	if (!result) {
		printf("error: %s\n", err::getLastErrorDescription().sz());
		return -1;
	}

	regex.print();
#endif

#if 0
	WriterThread writerThread;
	writerThread.start();

	ReaderThread readerThread;
	readerThread.start();

	char s[256];
	scanf("%s", s);
	return -1;
#endif

#if 0
	int fd = open("/home/vladimir/suka-bla", O_RDWR | O_CREAT, 0666);
	if (fd == -1) {
		printf("error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	int result;

	char s[] = "hui govno i muravei";
	write(fd, s, sizeof(s));
	lseek(fd, 0, SEEK_SET);

	char sockName[] = "/home/vladimir/hui-govno-i-muravei-2";
	char fifoName[] = "/home/vladimir/hui-govno-i-muravei-3";

	mode_t mode = 0;

	result = mkfifo(fifoName, 0666);
	if (result == -1) {
		printf("mkfifo error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	int sockClient = socket(AF_UNIX, SOCK_STREAM, 0);
	int sockServer = socket(AF_UNIX, SOCK_STREAM, 0);
	ASSERT(sockClient != -1 && sockServer != -1);

	sockaddr_un addr = { 0 };
	addr.sun_family = AF_UNIX;
	memcpy(addr.sun_path, sockName, sizeof(sockName));

	result = bind(sockServer, (const sockaddr*) &addr, sizeof(addr));
	if (result == -1) {
		printf("bind error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	result = listen(sockServer, 8);
	if (result == -1) {
		printf("listen error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	result = connect(sockClient, (const sockaddr*) &addr, sizeof(addr));
	if (result == -1) {
		printf("connect error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	socklen_t len = sizeof(addr);
	int sockAccept = accept(sockServer, (sockaddr*) &addr, &len);
	if (sockAccept == -1) {
		printf("accept error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	result = sendFd(sockClient, fd);
	if (result == -1) {
		printf("sendmsg error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	int fd2 = recvFd(sockAccept);
	if (fd2 == -1) {
		printf("recvmsg error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	char buffer[256] = { 0 };
	result = read(fd2, buffer, sizeof(buffer));
	if (result == -1) {
		printf("read error (%d): %s\n", errno, strerror (errno));
		return -1;
	}

	printf("read: %s\n", buffer);
#endif

#if 0
	trySudo();
	trySudo();
#endif

	QApplication app(argc, argv);

	addRootCaCertificates();

	/*
	QStringList items;
	items.append("Hui");
	items.append("Govno");
	items.append("Muravei");

	QPlainTextEdit edit;
	QCheckBox checkBox("Check box");
	QComboBox comboBox;
	comboBox.addItems(items);

	QDialogButtonBox buttons;
	buttons.setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	QVBoxLayout l;
	l.addWidget(&edit);
	l.addWidget(&checkBox);
	l.addWidget(&comboBox);
	l.addWidget(&buttons);

	QDialog dlg;
	dlg.setWindowTitle("Hui");
	dlg.setLayout(&l);
	dlg.exec();
	*/

	MainWindow mainWindow;
	mainWindow.show();
	return app.exec();
}

//..............................................................................
