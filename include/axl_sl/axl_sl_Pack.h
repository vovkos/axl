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

#define _AXL_SL_PACK_H

#include "axl_sl_String.h"
#include "axl_err_Error.h"
#include "axl_mem_Block.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
class Pack
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		axl_va_list va
		)
	{
		T a = va.arg<T> ();

		*size = sizeof(T);

		if (p)
			*(T*)p = a;

		return va;
	}
};

//..............................................................................

template <typename T>
class PackIntTrunc
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		axl_va_list va
		)
	{
		int n = va.arg<int> ();

		*size = sizeof(T);

		if (p)
			*(T*)p = (T)n;

		return va;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class Pack<char>: public PackIntTrunc<char>
{
};

template <>
class Pack<wchar_t>: public PackIntTrunc<wchar_t>
{
};

template <>
class Pack<int8_t>: public PackIntTrunc<int8_t>
{
};

template <>
class Pack<uint8_t>: public PackIntTrunc<uint8_t>
{
};

template <>
class Pack<int16_t>: public PackIntTrunc<int16_t>
{
};

template <>
class Pack<uint16_t>: public PackIntTrunc<uint16_t>
{
};

//..............................................................................

// pack string directly into buffer

template <typename T>
class PackStringBase
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		axl_va_list va
		)
	{
		T* string = va.arg<T*> ();

		size_t length = StringDetailsBase<T>::calcLength(string);
		size_t stringSize = (length + 1) * sizeof(T);

		*size = stringSize;

		if (p)
		{
			if (string)
				memcpy(p, string, stringSize);
			else
				*(T**) p = NULL;
		}

		return va;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef PackStringBase<char>    PackString;
typedef PackStringBase<wchar_t> PackString_w;
typedef PackStringBase<utf8_t>  PackString_utf8;
typedef PackStringBase<utf16_t> PackString_utf16;
typedef PackStringBase<utf32_t> PackString_utf32;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class Pack<const char*>: public PackStringBase<char>
{
};

template <>
class Pack<char*>: public PackStringBase<char>
{
};

template <>
class Pack<const wchar_t*>: public PackStringBase<wchar_t>
{
};

template <>
class Pack<wchar_t*>: public PackStringBase<wchar_t>
{
};

//..............................................................................

// general pointer

template <typename T>
class Pack<T*>
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		axl_va_list va
		)
	{
		T* obj = va.arg<T*> ();

		*size = sizeof(T);

		if (p)
		{
			if (obj)
				memcpy(p, obj, sizeof(T));
			else
				memset(p, 0, sizeof(T));
		}

		return va;
	}
};

//..............................................................................

// pointer to a self-sized object

template <
	typename T,
	typename SizeOf = sl::SizeOf<T>
	>
class PackSelfSizedPtr
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		axl_va_list va
		)
	{
		T* obj = va.arg<T*> ();

		size_t objSize = obj ? SizeOf() (obj) : sizeof(T);

		*size = objSize;

		if (p)
		{
			if (obj)
				memcpy(p, obj, objSize);
			else
				memset(p, 0, objSize);
		}

		return va;
	}
};

//..............................................................................

// pack error last

class PackLastError
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		axl_va_list va
		)
	{
		err::Error error = err::getLastError();

		*size = error->m_size;

		if (p)
			memcpy(p, error, error->m_size);

		return va;
	}
};

//..............................................................................

// specialization for errors

template <>
class Pack<err::ErrorHdr*>: public PackSelfSizedPtr<
	err::ErrorHdr,
	err::SizeOfError
	>
{
};

template <>
class Pack<const err::ErrorHdr*>: public PackSelfSizedPtr<
	err::ErrorHdr,
	err::SizeOfError
	>
{
};

//..............................................................................

// pack object referenced by (void* p, size_t size) pair

class PackPtrSize
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		axl_va_list va
		)
	{
		void* obj = va.arg<void*> ();
		size_t objSize = va.arg<size_t> ();

		*size = objSize;

		if (p)
		{
			if (obj)
				memcpy(p, obj, objSize);
			else
				memset(p, 0, objSize);
		}

		return va;
	}
};

//..............................................................................

// compile-time sequencing

template <
	typename Pack1,
	typename Pack2
	>
class PackSeqEx
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		axl_va_list va
		)
	{
		size_t size1 = 0;
		size_t size2 = 0;

		if (!p)
		{
			va = Pack1() (NULL, &size1, va);
			va = Pack2() (NULL, &size2, va);
		}
		else
		{
			va = Pack1() (p, &size1, va);
			va = Pack2() ((uchar_t*)p + size1, &size2, va);
		}

		*size = size1 + size2;
		return va;
	}
};

//..............................................................................

//  helper typedefs for packing up to 6 elements

template <
	typename Pack1,
	typename Pack2
	>
class PackSeqEx_2: public PackSeqEx<Pack1, Pack2>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Pack1,
	typename Pack2,
	typename Pack3
	>
class PackSeqEx_3: public PackSeqEx<PackSeqEx<Pack1, Pack2>, Pack3>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Pack1,
	typename Pack2,
	typename Pack3,
	typename Pack4
	>
class PackSeqEx_4: public PackSeqEx<PackSeqEx_3<Pack1, Pack2, Pack3>, Pack4>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Pack1,
	typename Pack2,
	typename Pack3,
	typename Pack4,
	typename Pack5
	>
class PackSeqEx_5: public PackSeqEx<PackSeqEx_4<Pack1, Pack2, Pack3, Pack4>, Pack5>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Pack1,
	typename Pack2,
	typename Pack3,
	typename Pack4,
	typename Pack5,
	typename Pack6
	>
class PackSeqEx_6: public PackSeqEx<PackSeqEx_5<Pack1, Pack2, Pack3, Pack4, Pack5>, Pack6>
{
};

//..............................................................................

//  helper typedefs for packing up to 6 simple elements

template <
	typename T1,
	typename T2
	>
class PackSeq_2: public PackSeqEx<
	Pack<T1>,
	Pack<T2>
	>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1,
	typename T2,
	typename T3
	>
class PackSeq_3: public PackSeqEx_3<
	Pack<T1>,
	Pack<T2>,
	Pack<T3>
	>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1,
	typename T2,
	typename T3,
	typename T4
	>
class PackSeq_4: public PackSeqEx_4<
	Pack<T1>,
	Pack<T2>,
	Pack<T3>,
	Pack<T4>
	>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1,
	typename T2,
	typename T3,
	typename T4,
	typename T5
	>
class PackSeq_5: public PackSeqEx_5<
	Pack<T1>,
	Pack<T2>,
	Pack<T3>,
	Pack<T4>,
	Pack<T5>
	>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1,
	typename T2,
	typename T3,
	typename T4,
	typename T5,
	typename T6
	>
class PackSeq_6: public PackSeqEx_6<
	Pack<T1>,
	Pack<T2>,
	Pack<T3>,
	Pack<T4>,
	Pack<T5>,
	Pack<T6>
	>
{
};

//..............................................................................

template <typename T>
ref::Ptr<mem::Block>
createPackage_va(axl_va_list va)
{
	size_t size = 0;
	T() (NULL, &size, va);

	typedef ref::Box<mem::Block> Package;

	ref::Ptr<Package> package = AXL_REF_NEW_EXTRA(Package, size);
	package->m_p = package + 1;
	package->m_size = size;

	T() (package + 1, &size, va);

	return package;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
ref::Ptr<mem::Block>
createPackage(
	int unused,
	...
	)
{
	AXL_VA_DECL(va, unused);
	return createPackage_va<T> (va);
}

//..............................................................................

} // namespace sl
} // namespace axl
