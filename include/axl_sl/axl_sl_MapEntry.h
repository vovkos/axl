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

#include "axl_sl_ListBase.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename Key,
	typename Value
	>
struct MapEntry: ListLink
{
	Key m_key;
	Value m_value;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Key,
	typename Value
	>
class MapIterator: public Iterator <MapEntry <Key, Value> >
{
public:
	MapIterator ()
	{
	}

	template <typename T>
	MapIterator (const Iterator <T>& src)
	{
		this->m_p = src.getEntry ();
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
