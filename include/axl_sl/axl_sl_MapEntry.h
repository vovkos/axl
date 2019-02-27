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

#define _AXL_SL_MAPENTRY_H

#include "axl_sl_Iterator.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Key,
	typename Value
	>
struct MapEntry: ListLink
{
protected:
	Key m_key;

public:
	Value m_value;

public:
	const Key&
	getKey() const
	{
		return m_key;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value
	>
class MapIterator: public Iterator<MapEntry<Key, Value> >
{
public:
	MapIterator()
	{
	}

	template <typename T>
	MapIterator(const Iterator<T>& src)
	{
		this->m_p = src.getEntry();
	}
};

template <
	typename Key,
	typename Value
	>
class ConstMapIterator: public ConstIterator<MapEntry<Key, Value> >
{
public:
	ConstMapIterator()
	{
	}

	template <typename T>
	ConstMapIterator(const ConstIterator<T>& src)
	{
		this->m_p = src.getEntry();
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
