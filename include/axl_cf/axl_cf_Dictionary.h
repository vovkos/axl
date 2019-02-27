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

#define _AXL_CF_DICTIONARY_H

#include "axl_cf_Type.h"

namespace axl {
namespace cf {

//..............................................................................

template <typename T>
class DictionaryRefBase: public TypeRefBase<T>
{
public:
	DictionaryRefBase()
	{
	}

	DictionaryRefBase(const DictionaryRefBase& src)
	{
		this->copy(src);
	}

	DictionaryRefBase(
		CFDictionaryRef p,
		bool isAttach = false
		)
	{
		isAttach ? this->copy(p) : this->attach(p);
	}

	DictionaryRefBase&
	operator = (const DictionaryRefBase& src)
	{
		this->copy(src);
		return *this;
	}

	DictionaryRefBase&
	operator = (CFDictionaryRef p)
	{
		this->copy(p);
		return *this;
	}

	bool
	create(CFDictionaryRef src)
	{
		CFDictionaryRef newDictionary = ::CFDictionaryCreateCopy(kCFAllocatorDefault, src);
		if (!newDictionary)
			return err::fail(err::SystemErrorCode_InsufficientResources);

		this->attach(newDictionary);
		return true;
	}

	size_t
	getCount() const
	{
		return ::CFDictionaryGetCount(this->m_p);
	}

	size_t
	getCountOfKey(const void* key) const
	{
		return ::CFDictionaryGetCountOfKey(this->m_p, key);
	}

	size_t
	getCountOfValue(const void* value) const
	{
		return ::CFDictionaryGetCountOfKey(this->m_p, value);
	}

	const void*
	getValue(const void* key) const
	{
		return ::CFDictionaryGetValue(this->m_p, key);
	}

	bool
	getValueIfPresent(
		const void* key,
		const void** value
		) const
	{
		return ::CFDictionaryGetValueIfPresent(this->m_p, key, value);
	}

	void
	getKeysAndValues(
		const void** keyArray,
		const void** valueArray
		) const
	{
		return ::CFDictionaryGetKeysAndValues(this->m_p, keyArray, valueArray);
	}

	bool
	getKeysAndValues(
		sl::Array<const void*>* keyArray,
		sl::Array<const void*>* valueArray
		) const
	{
		bool result;
		size_t count = getCount();

		const void** keyBuffer = NULL;
		const void** valueBuffer = NULL;

		if (keyArray)
		{
			result = keyArray->setCount(count);
			if (!result)
				return false;

			keyBuffer = *keyArray;
		}

		if (valueArray)
		{
			result = valueArray->setCount(count);
			if (!result)
				return false;

			valueBuffer = *valueArray;
		}

		::CFDictionaryGetKeysAndValues(this->m_p, keyBuffer, valueBuffer);
		return true;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef DictionaryRefBase<CFDictionaryRef> DictionaryRef;

//..............................................................................

class MutableDictionaryRef: public DictionaryRefBase<CFMutableDictionaryRef>
{
public:
	MutableDictionaryRef()
	{
	}

	MutableDictionaryRef(const MutableDictionaryRef& src)
	{
		copy(src);
	}

	MutableDictionaryRef(
		CFMutableDictionaryRef p,
		bool isAttach = false
		)
	{
		isAttach ? copy(p) : attach(p);
	}

	MutableDictionaryRef&
	operator = (const MutableDictionaryRef& src)
	{
		copy(src);
		return *this;
	}

	MutableDictionaryRef&
	operator = (CFMutableDictionaryRef p)
	{
		copy(p);
		return *this;
	}

	bool
	create(CFDictionaryRef src);

	void
	addValue(
		const void* key,
		const void* value
		)
	{
		::CFDictionaryAddValue(m_p, key, value);
	}

	void
	removeValue(const void* key)
	{
		::CFDictionaryRemoveValue(m_p, key);
	}

	void
	removeAllValues()
	{
		::CFDictionaryRemoveAllValues(m_p);
	}

	void
	setValue(
		const void* key,
		const void* value
		)
	{
		::CFDictionarySetValue(m_p, key, value);
	}

	void
	replaceValue(
		const void* key,
		const void* value
		)
	{
		::CFDictionaryReplaceValue(m_p, key, value);
	}
};

//..............................................................................

} // namespace cf
} // namespace axl
