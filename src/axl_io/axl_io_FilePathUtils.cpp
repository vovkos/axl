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
#include "axl_io_FilePathUtils.h"
#include "axl_err_Error.h"

#if (_AXL_OS_WIN)
#	include "axl_io_File.h"
#	include "axl_sys_win_NtDll.h"
#	include "axl_sys_win_NtStatus.h"
#endif

//..............................................................................

#if (_AXL_OS_DARWIN)
char*
get_current_dir_name() {
	size_t size = 128;

	for (;;) {
		char* buffer = (char*)::malloc(size);
		char* result = ::getcwd(buffer, size);
		if (result == buffer)
			return buffer;

		::free(buffer);

		if (errno != ERANGE)
			return NULL;

		size *= 2;
	}
}
#endif

namespace axl {
namespace io {

//..............................................................................

#if (_AXL_OS_WIN)
sl::String_w
getTempDir_w() {
	wchar_t buffer_w[MAX_PATH + 1];
	buffer_w[MAX_PATH] = 0;
	::GetTempPathW(countof(buffer_w) - 1, buffer_w);
	return buffer_w;
}
#endif

sl::String
getTempDir() {
#if (_AXL_OS_WIN)
	return getTempDir_w();
#else
	sl::String tmpDir = ::getenv("TMPDIR");
	return !tmpDir.isEmpty() ? tmpDir : "/tmp";
#endif
}

sl::String
getHomeDir() {
#if (_AXL_OS_WIN)
	wchar_t buffer_w[MAX_PATH + 1];
	HRESULT result = ::SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, buffer_w);
	return SUCCEEDED(result) ? sl::String(buffer_w) : NULL;
#else
	sl::String homeDir = getenv("HOME");
	if (!homeDir.isEmpty())
		return homeDir;

	struct passwd* pw = ::getpwuid(::getuid());
	return pw->pw_dir;
#endif
}

sl::String
createTempFile(
	const sl::StringRef& dir,
	const sl::StringRef& prefix
) {
#if (_AXL_OS_WIN)
	wchar_t buffer_w[MAX_PATH + 1];
	::GetTempFileNameW(
		!dir.isEmpty() ? dir.s2().sz() : getTempDir_w().sz(),
		prefix.s2().szn(),
		0,
		buffer_w
	);

	return buffer_w;
#else
	sl::String fileName = !dir.isEmpty() ? dir : getTempDir();
	if (!fileName.isSuffix('/'))
		fileName += '/';

	if (!prefix.isEmpty()) {
		fileName += prefix;
		fileName += "XXXXXX";
	} else {
		fileName += "tmpXXXXXX";
	}

	int fd = ::mkstemp(fileName.p());
	if (fd == -1)
		return sl::String();

	close(fd);
	return fileName;
#endif
}

sl::String
getCurrentDir() {
#if (_AXL_OS_WIN)
	wchar_t dir[1024] = { 0 };
	::GetCurrentDirectoryW(countof(dir) - 1, dir);
	return dir;
#elif (_AXL_OS_POSIX)
	char* p = ::get_current_dir_name();
	sl::String dir = p;
	::free(p);
	return dir;
#endif
}

bool
setCurrentDir(const sl::StringRef& dir) {
#if (_AXL_OS_WIN)
	bool_t result = ::SetCurrentDirectoryW(dir.s2().sz());
	return err::complete(result);
#elif (_AXL_OS_POSIX)
	int result = ::chdir(dir.sz());
	return err::complete(result == 0);
#endif
}

sl::String
getExeFilePath() {
#if (_AXL_OS_WIN)
	wchar_t buffer[1024] = { 0 };
	::GetModuleFileNameW(::GetModuleHandle(NULL), buffer, countof(buffer) - 1);
	return buffer;
#elif (_AXL_OS_LINUX)
	return getSymbolicLinkTarget("/proc/self/exe");
#elif (_AXL_OS_DARWIN)
	uint32_t size = 0;
	_NSGetExecutablePath(NULL, &size);

	char buffer[256];
	sl::Array<char> path(rc::BufKind_Stack, buffer, sizeof(buffer));
	bool result = path.setCount(size);
	if (!result)
		return sl::String();

	_NSGetExecutablePath(path.p(), &size);
	return getFullFilePath(path.cp());
#else
#	error unsupported OS
#endif
}

sl::String
getExeDir() {
	return io::getDir(io::getExeFilePath());
}

bool
doesFileExist(const sl::StringRef& fileName) {
#if (_AXL_OS_WIN)
	char buffer[256];
	sl::String_w fileName_w(rc::BufKind_Stack, buffer, sizeof(buffer));
	fileName_w = fileName;
	dword_t attributes = ::GetFileAttributesW(fileName_w);
	return attributes != INVALID_FILE_ATTRIBUTES;
#elif (_AXL_OS_POSIX)
	return ::access(fileName.sz(), F_OK) != -1;
#endif
}

#if (_AXL_OS_WIN)
bool
isDir(const sl::StringRef_w& fileName) {
	dword_t attributes = ::GetFileAttributesW(fileName.sz());
	return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool
isDir(const sl::StringRef& fileName) {
	char buffer[256];
	sl::String_w fileName_w(rc::BufKind_Stack, buffer, sizeof(buffer));
	return isDir(fileName_w);
}

bool
ensureDirExists(const sl::StringRef& fileName) {
	char buffer[256] = { 0 };
	sl::String_w fileName_w(rc::BufKind_Stack, buffer, sizeof(buffer));
	fileName_w = fileName;

	if (fileName_w.isEmpty() || isDir(fileName_w))
		return true;

	wchar_t* p = fileName_w.getBuffer();
	if (p[1] == ':')
		p += 2;

	while (*p == '\\' || *p == '/') // skip root
		p++;

	while (*p) {
		wchar_t* p2 = p + 1;
		while (*p2 && *p2 != '\\' && *p2 != '/')
			p2++;

		wchar_t c = *p2; // save
		*(wchar_t*)p2 = 0;

		if (!isDir(fileName_w)) {
			bool_t result = ::CreateDirectoryW(fileName_w, NULL);
			if (!result)
				return err::failWithLastSystemError();
		}

		if (!c)
			break;

		*p2 = c; // restore

		p = p2 + 1;
		while (*p == '\\' || *p == '/') // skip separators
			p++;
	}

	return true;
}

#elif (_AXL_OS_POSIX)

bool
isDir(const sl::StringRef& fileName) {
	struct stat st;

	int result = ::stat(fileName.sz(), &st);
	return result == 0 && S_ISDIR(st.st_mode);
}

bool
ensureDirExists(const sl::StringRef& fileName0) {
	if (fileName0.isEmpty() || isDir(fileName0))
		return true;

	sl::String fileName = fileName0;

	char* p = fileName.getBuffer(); // ensure exclusive buffer (we're going to modify it)
	while (*p == '/') // skip root
		p++;

	while (*p) {
		char* p2 = p + 1;
		while (*p2 && *p2 != '/')
			p2++;

		char c = *p2; // save
		*p2 = 0;

		if (!isDir(fileName)) {
			int result = ::mkdir(fileName, S_IRWXU);
			if (result != 0)
				return err::failWithLastSystemError();
		}

		if (!c)
			break;

		*p2 = c; // restore

		p = p2 + 1;
		while (*p == '/') // skip separators
			p++;
	}

	return true;
}

#endif

sl::String
getFullFilePath(const sl::StringRef& fileName) {
#if (_AXL_OS_WIN)
	char buffer[256];
	sl::String_w fileName_w(rc::BufKind_Stack, buffer, sizeof(buffer));
	fileName_w = fileName;

	size_t bufferLength = ::GetFullPathNameW(fileName_w, 0, NULL, NULL);
	if (!bufferLength)
		return err::failWithLastSystemError(NULL);

	sl::String_w filePath;

	for (;;) {
		wchar_t* p = filePath.createBuffer(bufferLength);
		if (!p)
			return NULL;

		size_t actualLength = ::GetFullPathNameW(fileName_w, bufferLength, p, NULL);
		ASSERT(actualLength != bufferLength); // on success, it should be bufferLength - 1

		if (!actualLength)
			return err::failWithLastSystemError(NULL);

		if (actualLength <= bufferLength)
			return sl::String(filePath, actualLength);

		bufferLength = actualLength; // try again with a bigger buffer
	}

#elif (_AXL_OS_POSIX)
	char fullPath[PATH_MAX];
	char* p = ::realpath(fileName.sz(), fullPath);
	return err::complete<char*>(p, NULL);
#endif
}

sl::String
getDir(const sl::StringRef& filePath) {
#if (_AXL_OS_WIN)
	char buffer[256];
	sl::String_w filePath_w(rc::BufKind_Stack, buffer, sizeof(buffer));
	filePath_w = filePath;

	wchar_t drive[4] = { 0 };
	wchar_t dir[1024] = { 0 };

	_wsplitpath_s(
		filePath_w,
		drive, countof(drive) - 1,
		dir, countof(dir) - 1,
		NULL, 0,
		NULL, 0
	);

	sl::String string = drive;
	string.append(dir);
	return string;

#elif (_AXL_OS_POSIX)
	size_t i = filePath.reverseFind('/');
	return i != -1 ? filePath.getLeftSubString(i + 1) : NULL;
#endif
}

sl::String
getFileName(const sl::StringRef& filePath) {
#if (_AXL_OS_WIN)
	char buffer[256];
	sl::String_w filePath_w(rc::BufKind_Stack, buffer, sizeof(buffer));
	filePath_w = filePath;

	wchar_t fileName[1024] = { 0 };
	wchar_t extension[1024] = { 0 };

	_wsplitpath_s(
		filePath_w,
		NULL, 0,
		NULL, 0,
		fileName, countof(fileName) - 1,
		extension, countof(extension) - 1
	);

	sl::String string = fileName;
	string.append(extension);
	return string;
#elif (_AXL_OS_POSIX)
	size_t i = filePath.reverseFind('/');
	return i != -1 ? filePath.getSubString(i + 1) : filePath;
#endif
}

sl::String
getExtension(const sl::StringRef& filePath) {
	size_t i = filePath.find('.');
	return i != -1 ? filePath.getSubString(i) : NULL;
}

sl::String
concatFilePath(
	sl::String* filePath,
	const sl::StringRef& fileName
) {
	if (filePath->isEmpty()) {
		*filePath = fileName;
		return *filePath;
	}

	char last = (*filePath)[filePath->getLength() - 1];

#if (_AXL_OS_WIN)
	if (last != '\\' && last != '/')
		filePath->append('\\');
#elif (_AXL_OS_POSIX)
	if (last != '/')
		filePath->append('/');
#endif

	filePath->append(fileName);
	return *filePath;
}

sl::String
findFilePath(
	const sl::StringRef& fileName,
	const sl::StringRef& firstDir,
	const sl::BoxList<sl::String>* dirList,
	bool doFindInCurrentDir
) {
	sl::String filePath;

	if (!firstDir.isEmpty()) {
		filePath = concatFilePath(firstDir, fileName);
		if (doesFileExist(filePath))
			return getFullFilePath(filePath);
	}

	if (doFindInCurrentDir)
		if (doesFileExist(fileName))
			return getFullFilePath(fileName);

	if (dirList) {
		sl::ConstBoxIterator<sl::String> dir = dirList->getHead();
		for (; dir; dir++) {
			filePath.forceCopy(*dir);
			concatFilePath(&filePath, fileName);
			if (doesFileExist(filePath))
				return getFullFilePath(filePath);
		}
	}

	return sl::String();
}

#if (_AXL_OS_WIN)

namespace win {

using namespace sys::win;

bool
isSymbolicLink(const sl::StringRef_w& fileName) {
	UNICODE_STRING uniLinkName;
	uniLinkName.Buffer = (WCHAR*)fileName.sz();
	uniLinkName.Length = fileName.getLength() * sizeof(WCHAR);
	uniLinkName.MaximumLength = uniLinkName.Length + sizeof(WCHAR);

	OBJECT_ATTRIBUTES oa = { 0 };
	oa.Length = sizeof(oa);
	oa.RootDirectory = NULL;
	oa.ObjectName = &uniLinkName;

	Handle link; // NT uses NULL for invalid handle value
	NTSTATUS status = ntOpenSymbolicLinkObject(link.p(), GENERIC_READ, &oa);
	return NT_SUCCESS(status);
}

bool
getSymbolicLinkTarget(
	sl::String_w* targetName,
	const sl::StringRef_w& linkName
) {
	NTSTATUS status;

	UNICODE_STRING uniLinkName;
	uniLinkName.Buffer = (WCHAR*)linkName.sz();
	uniLinkName.Length = linkName.getLength() * sizeof(WCHAR);
	uniLinkName.MaximumLength = uniLinkName.Length + sizeof(WCHAR);

	OBJECT_ATTRIBUTES oa = { 0 };
	oa.Length = sizeof(oa);
	oa.RootDirectory = NULL;
	oa.ObjectName = &uniLinkName;

	Handle link; // NT uses NULL for invalid handle value
	status = ntOpenSymbolicLinkObject(link.p(), GENERIC_READ, &oa);
	if (status < 0) {
		err::setError(NtStatus(status));
		return false;
	}

	UNICODE_STRING uniTarget = { 0 };
	ULONG bufferSize = 0;

	status = ntQuerySymbolicLinkObject(link, &uniTarget, &bufferSize);
	if (status != STATUS_BUFFER_TOO_SMALL) {
		err::setError(NtStatus(status));
		return false;
	}

	wchar_t* p = targetName->createBuffer(bufferSize / sizeof(WCHAR));

	uniTarget.Buffer = p;
	uniTarget.Length = 0;
	uniTarget.MaximumLength = (USHORT)bufferSize;

	status = ntQuerySymbolicLinkObject(link, &uniTarget, &bufferSize);
	if (status < 0) {
		err::setError(NtStatus(status));
		return false;
	}

	ASSERT(uniTarget.Length <= uniTarget.MaximumLength);
	targetName->overrideLength(uniTarget.Length / sizeof(WCHAR));
	return true;
}

} // namespace win

#elif (_AXL_OS_POSIX)

bool
isSymbolicLink(const sl::StringRef& fileName) {
	struct stat fileStat;
	int result = ::lstat(fileName.sz(), &fileStat);
	return result != -1 && S_ISLNK(fileStat.st_mode);
}

bool
getSymbolicLinkTarget(
	sl::String* targetName,
	const sl::StringRef& linkName
) {
	struct stat linkStat = { 0 };

	int result = ::lstat(linkName.sz(), &linkStat);
	if (result == -1)
		return err::failWithLastSystemError();

	if (!S_ISLNK(linkStat.st_mode))
		return err::fail(EINVAL);

	size_t bufferLength = linkStat.st_size ? linkStat.st_size : PATH_MAX;

	char* p = targetName->createBuffer(bufferLength);
	if (!p)
		return false;

	result = readlink(linkName.sz(), p, bufferLength);
	if (result == -1)
		return err::failWithLastSystemError();

	ASSERT(result <= bufferLength);
	targetName->overrideLength(result);
	return true;
}

#endif

//..............................................................................

#if (_AXL_OS_WIN)

namespace win {

sl::String_w
getWindowsDir() {
	uint_t length = ::GetWindowsDirectoryW(NULL, 0);

	sl::String_w dir;
	wchar_t* p = dir.createBuffer(length);
	if (!p)
		return sl::String_w();

	::GetWindowsDirectoryW(p, length + 1);
	dir.updateLength();
	return dir;
}

sl::String_w
getSystemDir() {
	uint_t length = ::GetSystemDirectoryW(NULL, 0);

	sl::String_w dir;
	wchar_t* p = dir.createBuffer(length);
	if (!p)
		return sl::String_w();

	::GetSystemDirectoryW(p, length + 1);
	dir.updateLength();
	return dir;
}

} // namespace win

#endif

//..............................................................................

// based on:
// http://www.drdobbs.com/architecture-and-design/matching-wildcards-an-empirical-way-to-t/240169123

bool
matchWildcard(
	const sl::StringRef& string0,
	const sl::StringRef& wildcard0
) {
	const char* wildcard = wildcard0.sz();

	if (string0.isEmpty()) { // empty input shortcut
		while (*wildcard == '*')
			wildcard++;

		return *wildcard == '\0';
	}

	const char* string = string0.sz();
	const char* stringBookmark = NULL;
	const char* wildcardBookmark = NULL;
	char c;

	for (;;) {
		if (*wildcard == '*') {
			while (*++wildcard == '*')
				;

			if (!*wildcard)
				return true;

			if (*wildcard != '?') {
				while (*string != *wildcard) {
					if (!(*(++string)))
						return false;
				}
			}

			wildcardBookmark = wildcard;
			stringBookmark = string;
		} else {
			c = *string;
			if (c != *wildcard && *wildcard != '?') {
				if (wildcardBookmark) {
					if (wildcard != wildcardBookmark) {
						wildcard = wildcardBookmark;

						if (c != *wildcard) {
							string = ++stringBookmark;
							continue;
						} else {
							wildcard++;
						}
					}

					if (*string) {
						string++;
						continue;
					}
				}

				return false;
			}
		}

		string++;
		wildcard++;

		if (!*string) {
			while (*wildcard == '*')
				wildcard++;

			return !*wildcard;
		}
	}
}

//..............................................................................

} // namespace io
} // namespace axl
