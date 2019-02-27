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

#define _AXL_SL_PACKER_H

#include "axl_sl_Pack.h"
#include "axl_sl_Array.h"
#include "axl_sl_SimpleSingleton.h"

namespace axl {
namespace sl {

//..............................................................................

class Packer
{
public:
	virtual
	axl_va_list
	pack_va(
		void* p,
		size_t* size,
		axl_va_list va
		) = 0;

	void
	pack(
		void* p,
		size_t* size,
		...
		)
	{
		AXL_VA_DECL(va, size);
		pack_va(p, size, va);
	}

	size_t
	count_va(axl_va_list va)
	{
		size_t size = 0;
		pack_va(NULL, &size, va);
		return size;
	}

	size_t
	count(
		int unused,
		...
		)
	{
		AXL_VA_DECL(va, unused);
		return count_va(va);
	}

	ref::Ptr<mem::Block>
	createPackage_va(axl_va_list va)
	{
		size_t size = 0;
		pack_va(NULL, &size, va);

		if (size == -1)
			return ref::g_nullPtr;

		typedef ref::Box<mem::Block> Package;
		ref::Ptr<Package> package = AXL_REF_NEW_EXTRA(Package, size);
		pack_va(package + 1, &size, va);
		package->m_p = package + 1;
		package->m_size = size;

		return package;
	}

	ref::Ptr<mem::Block>
	createPackage(
		int unused,
		...
		)
	{
		AXL_VA_DECL(va, unused);
		return createPackage_va(va);
	}
};

//..............................................................................

template <typename Pack>
class PackerImpl: public Packer
{
public:
	virtual
	axl_va_list
	pack_va(
		void* p,
		size_t* size,
		axl_va_list va
		)
	{
		return Pack() (p, size, va);
	}

	static
	PackerImpl*
	getSingleton()
	{
		return sl::getSimpleSingleton<PackerImpl> ();
	}
};

//..............................................................................

// run-time sequencing

class PackerSeq: public Packer
{
protected:
	sl::Array<Packer*> m_sequence;

public:
	virtual
	axl_va_list
	pack_va(
		void* p,
		size_t* size,
		axl_va_list va
		);

	void
	clear()
	{
		m_sequence.clear();
	}

	size_t
	append(Packer* packer)
	{
		m_sequence.append(packer);
		return m_sequence.getCount();
	}

	template <typename T>
	size_t
	append()
	{
		return append(PackerImpl<T>::getSingleton());
	}

	// often times it is more convenient to use printf-like format string for sequencing

	size_t
	appendFormat(const char* formatString);

	size_t
	format(const char* formatString)
	{
		clear();
		return appendFormat(formatString);
	}
};

//..............................................................................

inline
ref::Ptr<mem::Block>
formatPackage_va(
	const char* formatString,
	axl_va_list va
	)
{
	PackerSeq packer;
	packer.format(formatString);
	return packer.createPackage_va(va);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ref::Ptr<mem::Block>
formatPackage(
	const char* formatString,
	...
	)
{
	AXL_VA_DECL(va, formatString);
	return formatPackage_va(formatString, va);
}

//..............................................................................

// package:

class Package
{
protected:
	axl::sl::Array<uchar_t> m_buffer;

public:
	void
	clear()
	{
		m_buffer.clear();
	}

	const uchar_t*
	getBuffer()
	{
		return m_buffer;
	}

	size_t
	getSize()
	{
		return m_buffer.getCount();
	}

	size_t
	append_va(
		Packer* pack,
		axl_va_list va
		);

	template <typename Pack>
	size_t
	append_va(axl_va_list va)
	{
		Packer* pack = PackerImpl<Pack>::getSingleton();
		return append_va(pack, va);
	}

	size_t
	append(
		const void* p,
		size_t size
		);
/*
	template <typename T>
	size_t
	append(const T& data)
	{
		Packer* pack = PackerImpl<Pack<T> >::getSingleton();
		return pack(&data, sizeof(data));
	}
*/
	size_t
	appendFormat_va(
		const char* formatString,
		axl_va_list va
		)
	{
		PackerSeq packer;
		packer.format(formatString);
		return append_va(&packer, va);
	}

	size_t
	appendFormat(
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL(va, formatString);
		return appendFormat_va(formatString, va);
	}

	size_t
	format_va(
		const char* formatString,
		axl_va_list va
		)
	{
		clear();
		return appendFormat_va(formatString, va);
	}

	size_t
	format(
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL(va, formatString);
		return format_va(formatString, va);
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
