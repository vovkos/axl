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

#define _AXL_SL_BOXLIST_H

#include "axl_sl_List.h"
#include "axl_sl_ArgType.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
class BoxListEntry: public ListLink {
public:
	T m_value;

public:
	BoxListEntry() {}

	BoxListEntry(const T& value) {
		m_value = value;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Compare
>
class CompareBoxListEntry {
protected:
	Compare m_compare;

public:
	CompareBoxListEntry(Compare compare):
		m_compare(compare) {}

	bool
	operator () (
		const BoxListEntry<T>& entry1,
		const BoxListEntry<T>& entry2
	) const {
		return m_compare(entry1.m_value, entry2.m_value);
	}

	bool
	operator () (
		const BoxListEntry<T>* entry1,
		const BoxListEntry<T>* entry2
	) const {
		return m_compare(entry1->m_value, entry2->m_value);
	}
};

//..............................................................................

template <
	typename T,
	typename Value,
	typename Entry,
	typename Link
>
class BoxIteratorImpl: public IteratorBase<
	T,
	Entry,
	Link,
	ImplicitCast<Entry*, Link*>
> {
public:
	Value&
	operator * () const {
		return r();
	}

	Value*
	operator -> () const {
		return p();
	}

	Value&
	r() const {
		ASSERT(this->m_p);
		return this->m_p->m_value;
	}

	Value*
	p() const {
		return this->m_p ? &this->m_p->m_value : NULL;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class BoxIterator: public BoxIteratorImpl<
	BoxIterator<T>,
	T,
	BoxListEntry<T>,
	ListLink
> {
public:
	BoxIterator() {}

	BoxIterator(BoxListEntry<T>* p) {
		this->m_p = p;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class ConstBoxIterator: public BoxIteratorImpl<
	ConstBoxIterator<T>,
	const T,
	const BoxListEntry<T>,
	const ListLink
> {
public:
	ConstBoxIterator() {}

	ConstBoxIterator(const BoxListEntry<T>* p) {
		this->m_p = p;
	}

	ConstBoxIterator(const BoxIterator<T>& it) {
		this->m_p = it.getEntry();
	}
};

//..............................................................................

template <
	typename T,
	typename ValueArg = typename ArgType<T>::Type
>
class BoxList: public OwningListBase<
	BoxListEntry<T>,
	ImplicitPtrCast<BoxListEntry<T>, ListLink>,
	BoxIterator<T>,
	ConstBoxIterator<T>,
	typename mem::Delete<BoxListEntry<T> >
> {
public:
	typedef OwningListBase<
		BoxListEntry<T>,
		ImplicitPtrCast<BoxListEntry<T>, ListLink>,
		BoxIterator<T>,
		ConstBoxIterator<T>,
		typename mem::Delete<BoxListEntry<T> >
	> BaseType;

	typedef typename BaseType::Entry Entry;
	typedef typename BaseType::Iterator Iterator;

public:
	template <typename L>
	void
	copy(const L& list) {
		this->clear();

		typename L::Iterator it = list.getHead();
		for (; it; it++)
			insertTail(*it);
	}

	T
	remove(Iterator it) {
		T value = *it;
		BaseType::erase(it);
		return value;
	}

	T
	removeHead() {
		return this->m_head ? remove(this->m_head) : T();
	}

	T
	removeTail() {
		return this->m_tail ? remove(this->m_tail) : T();
	}

	Iterator
	insertHead(ValueArg value) {
		Entry* entry = new Entry;
		entry->m_value = value;
		return insertHeadEntry(entry);
	}

	Iterator
	insertTail(ValueArg value) {
		Entry* entry = new Entry;
		entry->m_value = value;
		return insertTailEntry(entry);
	}

	Iterator
	insertBefore(
		ValueArg value,
		Iterator before
	) {
		Entry* entry = new Entry;
		entry->m_value = value;
		return insertBeforeEntry(entry, before);
	}

	Iterator
	insertAfter(
		ValueArg value,
		Iterator after
	) {
		Entry* entry = new Entry;
		entry->m_value = value;
		return insertAfterEntry(entry, after);
	}

	// empty element insertion

	Iterator
	insertHead() {
		Entry* entry = new Entry;
		return insertHeadEntry(entry);
	}

	Iterator
	insertTail() {
		Entry* entry = new Entry;
		return insertTailEntry(entry);
	}

	Iterator
	insertBefore(Iterator before) {
		Entry* entry = new Entry;
		return insertBeforeEntry(entry, before);
	}

	Iterator
	insertAfter(Iterator after) {
		Entry* entry = new Entry;
		return insertAfterEntry(entry, after);
	}

	// direct access to entries

	Entry*
	removeEntry(Iterator it) {
		return BaseType::remove(it);
	}

	Entry*
	removeHeadEntry() {
		return BaseType::removeHead();
	}

	Entry*
	removeTailEntry() {
		return BaseType::removeTail();
	}

	Iterator
	insertHeadEntry(Entry* entry) {
		return BaseType::insertHead(entry);
	}

	Iterator
	insertTailEntry(Entry* entry) {
		return BaseType::insertTail(entry);
	}

	Iterator
	insertBeforeEntry(
		Entry* entry,
		Iterator before
	) {
		return BaseType::insertBefore(entry, before);
	}

	Iterator
	insertAfterEntry(
		Entry* entry,
		Iterator after
	) {
		return BaseType::insertAfter(entry, after);
	}

	bool
	sort() {
		return sort(Lt<T, ValueArg>());
	}

	template <typename Compare>
	bool
	sort(Compare compare) {
		return BaseType::sort(CompareBoxListEntry<T, Compare>(compare));
	}
};

//..............................................................................

template <typename T>
class ConstBoxList: public ConstListBase<
	BoxListEntry<T>,
	ImplicitPtrCast<BoxListEntry<T>, ListLink>,
	ConstBoxIterator<T>
> {
public:
	ConstBoxList() {}

	template <typename ValueArg>
	ConstBoxList(const BoxList<T, ValueArg>& list) {
		this->m_listData = list.getListData();
	}

	template <typename Delete>
	ConstBoxList(
		const List<
			BoxListEntry<T>,
			typename ConstBoxList::GetLink,
			Delete
		>& list
	) {
		this->m_listData = list.getListData();
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
