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

#pragma once

#define _AXL_ERR_ERROR_H

#include "axl_rc_Buf.h"
#include "axl_err_SystemError.h"

namespace axl {
namespace err {

//..............................................................................

// axl std errors

extern AXL_SELECT_ANY const sl::Guid g_stdErrorGuid = sl::g_nullGuid;

enum StdErrorCode {
	StdErrorCode_NoError,
	StdErrorCode_String,
	StdErrorCode_Stack,
};

//..............................................................................

// POD structure

struct ErrorHdr {
	uint32_t m_size;
	sl::Guid m_guid;
	uint32_t m_code;

	bool
	isKindOf(
		const sl::Guid& guid,
		uint_t code
	) const {
		return m_guid == guid && m_code == code;
	}

	bool
	isNoError() const {
		return isKindOf(g_stdErrorGuid, StdErrorCode_NoError);
	}

	bool
	isStack() const {
		return
			m_size >= sizeof(ErrorHdr)* 2 &&
			isKindOf(g_stdErrorGuid, StdErrorCode_Stack);
	}

	bool
	isStackTopKindOf(
		const sl::Guid& guid,
		uint_t code
	) const {
		return isStack() && (this + 1)->isKindOf(guid, code);
	}

	sl::StringRef
	getDescription() const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class SizeOfError {
public:
	size_t
	operator () (const ErrorHdr* error) const {
		return AXL_MAX(error->m_size, sizeof(ErrorHdr));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

extern AXL_SELECT_ANY const ErrorHdr g_noError = {
	sizeof(ErrorHdr),
	g_stdErrorGuid,
	StdErrorCode_NoError,
};

//..............................................................................

class ErrorRef: public rc::BufRef<ErrorHdr, SizeOfError> {
public:
	typedef rc::BufRef<ErrorHdr, SizeOfError> BaseType;

public:
	ErrorRef() {}

	ErrorRef(ErrorRef&& src):
		BaseType(std::move(src)) {}

	ErrorRef(const ErrorRef& src):
		BaseType(src) {}

	ErrorRef(const ErrorHdr* src):
		BaseType(src) {}

	ErrorRef(uint_t code);
	ErrorRef(const sl::StringRef& string);
	ErrorRef(const char* string);

	ErrorRef(
		const sl::Guid& guid,
		uint_t code
	);

	ErrorRef&
	operator = (ErrorRef&& src) {
		move(std::move(src));
		return *this;
	}

	ErrorRef&
	operator = (const ErrorRef& src) {
		attach(src);
		return *this;
	}

	bool
	isKindOf(
		const sl::Guid& guid,
		uint_t code
	) const {
		return !isEmpty() && m_p->isKindOf(guid, code);
	}

	bool
	isNoError() const {
		return isEmpty() || m_p->isNoError();
	}

	sl::StringRef
	getDescription() const;
};

//..............................................................................

class Error: public rc::Buf<ErrorHdr, SizeOfError, ErrorRef> {
public:
	Error() {}

	Error(Error&& src) {
		move(std::move(src));
	}

	Error(ErrorRef&& src) {
		move(std::move(src));
	}

	Error(const Error& src) {
		copy(src);
	}

	Error(const ErrorRef& src) {
		copy(src);
	}

	Error(const ErrorHdr* src) {
		copy(src);
	}

	Error(uint_t code) {
		create(code);
	}

	Error(
		const sl::Guid& guid,
		uint_t code
	) {
		create(guid, code);
	}

	Error(const sl::StringRef& string) {
		create(string);
	}

	Error(const char* string) {
		create(string);
	}

	Error(
		rc::BufKind kind,
		void* p,
		size_t size
	) {
		setBuffer(kind, p, size);
	}

	Error&
	operator = (Error&& src) {
		move(std::move(src));
		return *this;
	}

	Error&
	operator = (ErrorRef&& src) {
		move(std::move(src));
		return *this;
	}

	Error&
	operator = (const Error& src) {
		copy(src);
		return *this;
	}

	Error&
	operator = (const ErrorRef& src) {
		copy(src);
		return *this;
	}

	Error&
	operator = (const ErrorHdr* src) {
		copy(src);
		return *this;
	}

	Error&
	operator = (uint_t code) {
		create(code);
		return *this;
	}

	Error&
	operator = (const sl::StringRef& string) {
		create(string);
		return *this;
	}

	Error&
	operator = (const char* string) {
		create(string);
		return *this;
	}

	// generic errors

	size_t
	create(
		const sl::Guid& guid,
		uint_t code
	);

	size_t
	create(uint_t code) {
		return create(g_systemErrorGuid, code);
	}

	size_t
	push(const ErrorRef& error);

	size_t
	push(
		const sl::Guid& guid,
		uint_t code
	);

	// string errors

	size_t
	create(
		const sl::Guid& guid,
		uint_t code,
		const sl::StringRef& string
	);

	size_t
	create(const sl::StringRef& string) {
		return create(g_stdErrorGuid, StdErrorCode_String, string);
	}

	size_t
	create(const char* string) {
		return create(g_stdErrorGuid, StdErrorCode_String, string);
	}

	size_t
	format_va(
		const char* formatString,
		axl_va_list va
	);

	size_t
	format(
		const char* formatString,
		...
	) {
		AXL_VA_DECL(va, formatString);
		return format_va(formatString, va);
	}

	size_t
	push(
		const sl::Guid& guid,
		uint_t code,
		const sl::StringRef& string
	);

	size_t
	push(const sl::StringRef& string) {
		return push(g_stdErrorGuid, StdErrorCode_String, string);
	}

	size_t
	push(const char* string) {
		return push(g_stdErrorGuid, StdErrorCode_String, string);
	}

	size_t
	pushFormat_va(
		const char* formatString,
		axl_va_list va
	);

	size_t
	pushFormat(
		const char* formatString,
		...
	) {
		AXL_VA_DECL(va, formatString);
		return pushFormat_va(formatString, va);
	}

	// error packing

	template <typename Pack>
	size_t
	pack_va(
		const sl::Guid& guid,
		uint_t code,
		axl_va_list va
	);

	template <typename Pack>
	size_t
	pack(
		const sl::Guid& guid,
		uint_t code,
		...
	) {
		AXL_VA_DECL(va, code);
		return pack_va<Pack>(guid, code, va);
	}

	template <typename Pack>
	size_t
	pushPack_va(
		const sl::Guid& guid,
		uint_t code,
		axl_va_list va
	);

	template <typename Pack>
	size_t
	pushPack(
		const sl::Guid& guid,
		uint_t code,
		...
	) {
		AXL_VA_DECL(va, code);
		return pushPack_va<Pack>(guid, code, va);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
size_t
Error::push(
	const sl::Guid& guid,
	uint_t code
) {
	if (!m_p)
		return create(guid, code);

	Error error;
	error.create(guid, code);
	return push(error);
}

inline
size_t
Error::push(
	const sl::Guid& guid,
	uint_t code,
	const sl::StringRef& string
) {
	if (!m_p)
		return create(guid, code, string);

	Error error;
	error.create(guid, code, string);
	return push(error);
}

inline
size_t
Error::pushFormat_va(
	const char* formatString,
	axl_va_list va
) {
	if (!m_p)
		return format_va(formatString, va);

	Error error;
	error.format_va(formatString, va);
	return push(error);
}

template <typename Pack>
size_t
Error::pack_va(
	const sl::Guid& guid,
	uint_t code,
	axl_va_list va
) {
	size_t packSize;
	Pack()(NULL, &packSize, va);

	size_t size = sizeof(ErrorHdr) + packSize;
	createBuffer(size);
	if (!m_p)
		return -1;

	m_p->m_size = size;
	m_p->m_guid = guid;
	m_p->m_code = code;

	Pack()(m_p + 1, &packSize, va);
	return size;
}

template <typename Pack>
size_t
Error::pushPack_va(
	const sl::Guid& guid,
	uint_t code,
	axl_va_list va
) {
	if (!m_p)
		return pack_va<Pack>(guid, code, va);

	Error error;
	error.pack_va<Pack>(guid, code, va);
	return push(error);
}

//..............................................................................

inline
ErrorRef::ErrorRef(uint_t code):
	BaseType(Error(code)) {}

inline
ErrorRef::ErrorRef(const sl::StringRef& string):
	BaseType(Error(string)) {}

inline
ErrorRef::ErrorRef(const char* string):
	BaseType(Error(string)) {}

inline
ErrorRef::ErrorRef(
	const sl::Guid& guid,
	uint_t code
):
	BaseType(Error(guid, code)) {}

//..............................................................................

// error getters

ErrorRef
getLastError();

inline
sl::String
getLastErrorDescription() {
	return getLastError().getDescription();
}

inline
Error
getLastSystemError() {
	return Error(getLastSystemErrorCode());
}

inline
sl::String
getLastSystemErrorDescription() {
	return getLastSystemError().getDescription();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// simple error

size_t
setError(const ErrorRef& error);

inline
size_t
setError(
	const sl::Guid& guid,
	uint_t code
) {
	return setError(Error(guid, code));
}

size_t
pushError(const ErrorRef& error);

inline
size_t
pushError(
	const sl::Guid& guid,
	uint_t code
) {
	return pushError(Error(guid, code));
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// string error formatting

inline
Error
formatError_va(
	const char* formatString,
	axl_va_list va
) {
	Error error;
	error.format_va(formatString, va);
	return error;
}

inline
Error
formatError(
	const char* formatString,
	...
) {
	AXL_VA_DECL(va, formatString);
	return formatError_va(formatString, va);
}

inline
size_t
setError(const sl::StringRef& string) {
	return setError(Error(string));
}

inline
size_t
setError(const char* string) {
	return setError(Error(string));
}

inline
size_t
setError_va(
	const char* formatString,
	axl_va_list va
) {
	return setError(formatError_va(formatString, va));
}

template <
	typename A,
	typename... V
>
size_t
setError(
	const char* formatString,
	A arg,
	V... args
) {
	return setError(formatError(formatString, arg, args...));
}

inline
size_t
pushError(const sl::StringRef& string) {
	return pushError(Error(string));
}

inline
size_t
pushError(const char* string) {
	return pushError(Error(string));
}

inline
size_t
pushError_va(
	const char* formatString,
	axl_va_list va
) {
	return pushError(formatError_va(formatString, va));
}

template <
	typename A,
	typename... V
>
size_t
pushError(
	const char* formatString,
	A arg,
	V... args
) {
	return pushError(formatError(formatString, arg, args...));
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// pack

template <typename Pack>
size_t
setPackError_va(
	const sl::Guid& guid,
	uint_t code,
	axl_va_list va
) {
	Error error;
	size_t result = error.pack_va<Pack>(guid, code, va);
	return result != -1 ? setError(error) : -1;
}

template <typename Pack>
size_t
setPackError(
	const sl::Guid& guid,
	uint_t code,
	...
) {
	AXL_VA_DECL(va, code);
	return setPackError_va<Pack>(guid, code, va);
}

template <typename Pack>
size_t
pushPackError_va(
	const sl::Guid& guid,
	uint_t code,
	axl_va_list va
) {
	Error error;
	size_t result = error.pack_va<Pack>(guid, code, va);
	return result != -1 ? pushError(error) : -1;
}

template <typename Pack>
size_t
pushPackError(
	const sl::Guid& guid,
	uint_t code,
	...
) {
	AXL_VA_DECL(va, code);
	return pushPackError_va<Pack>(guid, code, va);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// failing with a one-liner

template <typename T>
T
fail(
	T failResult,
	const ErrorRef& error
) {
	setError(error);
	return failResult;
}

inline
bool
fail(const ErrorRef& error) {
	return fail<bool>(false, error);
}

// format string must be a literal -- that's what distinguishes
// fail("abc: %s", "def") vs fail(failResult, error)

template <
	size_t N,
	typename A,
	typename... V
>
bool
fail(
	const char (&formatString)[N],
	A arg,
	V... args
) {
	setError(formatString, arg, args...);
	return false;
}

template <
	typename T,
	size_t N,
	typename A,
	typename... V
>
T
fail(
	T failResult,
	const char (&formatString)[N],
	A arg,
	V... args
) {
	setError(formatString, arg, args...);
	return failResult;
}

template <typename T>
T
fail_va(
	T failResult,
	const char* formatString,
	axl_va_list va
) {
	setError_va(formatString, va);
	return failResult;
}

inline
bool
fail_va(
	const char* formatString,
	axl_va_list va
) {
	return fail_va<bool>(false, formatString, va);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// system errors

inline
size_t
setLastSystemError() {
	return setError(getLastSystemErrorCode());
}

template <typename T>
T
failWithLastSystemError(T failResult) {
	setLastSystemError();
	return failResult;
}

inline
bool
failWithLastSystemError() {
	return failWithLastSystemError<bool>(false);
}

template <typename T>
T
completeWithSystemError(
	T result,
	T failResult,
	uint_t errorCode
) {
	if (result == failResult)
		setError(errorCode);

	return result;
}

inline
bool
completeWithSystemError(
	int result,
	uint_t errorCode
) {
	return completeWithSystemError<bool>(result != 0, false, errorCode);
}

template <typename T>
T
complete(
	T result,
	T failResult
) {
	if (result == failResult)
		setLastSystemError();

	return result;
}

inline
bool
complete(bool_t result) {
	return complete<bool>(result != 0, false);
}

//..............................................................................

// providers

class ErrorProvider {
public:
	virtual
	sl::StringRef
	getErrorDescription(const ErrorRef& error) = 0;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class StdErrorProvider: public ErrorProvider {
public:
	virtual
	sl::StringRef
	getErrorDescription(const ErrorRef& error);

protected:
	sl::String
	getStackErrorDescription(const ErrorRef& error);
};

//..............................................................................

} // namespace err
} // namespace axl
