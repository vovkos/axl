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
class DictionaryBase: public TypeBase<T>
{
public:
	DictionaryBase()
	{
	}

	DictionaryBase(const DictionaryBase& src):
		TypeBase<T>(src)
	{
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	DictionaryBase(DictionaryBase&& src):
		TypeBase<T>(std::move(src))
	{
	}
#endif

	DictionaryBase(
		T p,
		bool isAttach = false
		)
	{
		isAttach ? this->copy(p) : this->attach(p);
	}

	DictionaryBase&
	operator = (const DictionaryBase& src)
	{
		this->copy(src);
		return *this;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	DictionaryBase&
	operator = (DictionaryBase&& src)
	{
		move(std::move(src));
		return *this;
	}
#endif

	DictionaryBase&
	operator = (T p)
	{
		this->copy(p);
		return *this;
	}

	bool
	clone(CFDictionaryRef src)
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
		ASSERT(this->m_p);
		return ::CFDictionaryGetCount(this->m_p);
	}

	size_t
	getCountOfKey(const void* key) const
	{
		ASSERT(this->m_p);
		return ::CFDictionaryGetCountOfKey(this->m_p, key);
	}

	size_t
	getCountOfValue(const void* value) const
	{
		ASSERT(this->m_p);
		return ::CFDictionaryGetCountOfKey(this->m_p, value);
	}

	const void*
	getValue(const void* key) const
	{
		ASSERT(this->m_p);
		return ::CFDictionaryGetValue(this->m_p, key);
	}

	bool
	getValueIfPresent(
		const void* key,
		const void** value
		) const
	{
		ASSERT(this->m_p);
		return ::CFDictionaryGetValueIfPresent(this->m_p, key, value);
	}

	void
	getKeysAndValues(
		const void** keyArray,
		const void** valueArray
		) const
	{
		ASSERT(this->m_p);
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

typedef DictionaryBase<CFDictionaryRef> Dictionary;

//..............................................................................

class MutableDictionary: public DictionaryBase<CFMutableDictionaryRef>
{
public:
	MutableDictionary()
	{
	}

	MutableDictionary(const MutableDictionary& src):
		DictionaryBase<CFMutableDictionaryRef>(src)
	{
	}

	MutableDictionary(
		CFMutableDictionaryRef p,
		bool isAttach = false
		):
		DictionaryBase<CFMutableDictionaryRef>(p, isAttach)
	{
	}

	MutableDictionary&
	operator = (const MutableDictionary& src)
	{
		copy(src);
		return *this;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	MutableDictionary&
	operator = (MutableDictionary&& src)
	{
		move(std::move(src));
		return *this;
	}
#endif

	MutableDictionary&
	operator = (CFMutableDictionaryRef p)
	{
		copy(p);
		return *this;
	}

	bool
	clone(CFDictionaryRef src);

	void
	addValue(
		const void* key,
		const void* value
		)
	{
		ASSERT(m_p);
		::CFDictionaryAddValue(m_p, key, value);
	}

	void
	removeValue(const void* key)
	{
		ASSERT(m_p);
		::CFDictionaryRemoveValue(m_p, key);
	}

	void
	removeAllValues()
	{
		ASSERT(m_p);
		::CFDictionaryRemoveAllValues(m_p);
	}

	void
	setValue(
		const void* key,
		const void* value
		)
	{
		ASSERT(m_p);
		::CFDictionarySetValue(m_p, key, value);
	}

	void
	replaceValue(
		const void* key,
		const void* value
		)
	{
		ASSERT(m_p);
		::CFDictionaryReplaceValue(m_p, key, value);
	}
};

//..............................................................................

} // namespace cf
} // namespace axl
