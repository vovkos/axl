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
	ErrorRef(const char* string); // for overload resolving

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
		createSystemError(code);
	}

	Error(
		const sl::Guid& guid,
		uint_t code
	) {
		createSimpleError(guid, code);
	}

	Error(const sl::StringRef& string) {
		createStringError(string);
	}

	Error(const char* string) { // for overload resolving
		createStringError(string);
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
		createSystemError(code);
		return *this;
	}

	size_t
	push(const ErrorRef& error);

	// pack

	template <typename Pack>
	size_t
	pack_va(
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
	) {
		if (!m_p)
			return pack_va<Pack>(guid, code, va);

		Error error;
		error.pack_va<Pack>(guid, code, va);
		return push(error);
	}

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

	// system error (push is irrelevant for system errors)

	size_t
	createSystemError(uint_t code) {
		return createSimpleError(g_systemErrorGuid, code);
	}

	// simple error

	size_t
	createSimpleError(
		const sl::Guid& guid,
		uint_t code
	);

	size_t
	pushSimpleError(
		const sl::Guid& guid,
		uint_t code
	) {
		if (!m_p)
			return createSimpleError(guid, code);

		Error error;
		error.createSimpleError(guid, code);
		return push(error);
	}

	// string error

	size_t
	createStringError(
		const sl::Guid& guid,
		uint_t code,
		const sl::StringRef& string
	);

	size_t
	createStringError(const sl::StringRef& string) {
		return createStringError(g_stdErrorGuid, StdErrorCode_String, string);
	}

	size_t
	pushStringError(const sl::StringRef& string) {
		if (!m_p)
			return createStringError(string);

		Error error;
		error.createStringError(string);
		return push(error);
	}

	size_t
	formatStringError_va(
		const char* formatString,
		axl_va_list va
	);

	size_t
	formatStringError(
		const char* formatString,
		...
	) {
		AXL_VA_DECL(va, formatString);
		return formatStringError_va(formatString, va);
	}

	size_t
	pushFormatStringError_va(
		const char* formatString,
		axl_va_list va
	) {
		if (!m_p)
			return formatStringError_va(formatString, va);

		Error error;
		error.formatStringError_va(formatString, va);
		return push(error);
	}

	size_t
	pushFormatStringError(
		const char* formatString,
		...
	) {
		AXL_VA_DECL(va, formatString);
		return pushFormatStringError_va(formatString, va);
	}
};

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

// utility functions

ErrorRef
getLastError();

size_t
setError(const ErrorRef& error);

size_t
pushError(const ErrorRef& error);

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

// simple error

inline
size_t
setError(
	const sl::Guid& guid,
	uint_t code
) {
	return setError(Error(guid, code));
}

inline
size_t
pushError(
	const sl::Guid& guid,
	uint_t code
) {
	return pushError(Error(guid, code));
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// string error

size_t
setFormatStringError_va(
	const char* formatString,
	axl_va_list va
);

inline
size_t
setFormatStringError(
	const char* formatString,
	...
) {
	AXL_VA_DECL(va, formatString);
	return setFormatStringError_va(formatString, va);
}

size_t
pushFormatStringError_va(
	const char* formatString,
	axl_va_list va
);

inline
size_t
pushFormatStringError(
	const char* formatString,
	...
) {
	AXL_VA_DECL(va, formatString);
	return pushFormatStringError_va(formatString, va);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// convenient helpers

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// we need to correctly select the proper overload in cases like:
// failWithStringError("abc: %s", "def");

template <typename T>
struct IsFailWithStringErrorResult: std::integral_constant<
	bool,
	!std::is_convertible<T, const sl::StringRef&>::value ||
	std::is_same<T, std::nullptr_t>::value
> {};

template <typename T>
using FailWithStringErrorResult = typename std::enable_if<
	IsFailWithStringErrorResult<T>::value, T
>::type;

template <typename T>
FailWithStringErrorResult<T>
failWithStringError(
	T failResult,
	const sl::StringRef& string
) {
	setError(string);
	return failResult;
}

inline
bool
failWithStringError(const sl::StringRef& string) {
	return failWithStringError<bool>(false, string);
}

template <typename T>
FailWithStringErrorResult<T>
failWithStringError_va(
	T failResult,
	const char* formatString,
	axl_va_list va
) {
	setFormatStringError_va(formatString, va);
	return failResult;
}

inline
bool
failWithStringError_va(
	const char* formatString,
	axl_va_list va
) {
	return failWithStringError_va<bool>(false, formatString, va);
}

template <typename T>
FailWithStringErrorResult<T>
failWithStringError(
	T failResult,
	const char* formatString,
	...
) {
	AXL_VA_DECL(va, formatString);
	return failWithStringError_va(failResult, formatString, va);
}

inline
bool
failWithStringError(
	const char* formatString,
	...
) {
	AXL_VA_DECL(va, formatString);
	return failWithStringError_va(formatString, va);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
