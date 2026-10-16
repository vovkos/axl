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

namespace {

//..............................................................................

struct IdentityHash {
	size_t
	operator () (uint_t key) const {
		return key;
	}
};

template <
	typename Key,
	typename Value,
	typename KeyValueArgs
>
class HashTableMap: public sl::HashTable<
	Key,
	Value,
	IdentityHash,
	sl::Eq<Key>,
	KeyValueArgs
> {
};

template <
	typename Key,
	typename Value,
	typename KeyValueArgs
>
class RobinHashTableMap: public sl::RobinHashTable<
	Key,
	Value,
	IdentityHash,
	sl::Eq<Key>,
	KeyValueArgs
> {
};

template <
	typename Key,
	typename Value,
	typename KeyValueArgs
>
class QuadraticHashTableMap: public sl::QuadraticHashTable<
	Key,
	(Key)-1,
	(Key)-2,
	Value,
	IdentityHash,
	sl::Eq<Key>,
	KeyValueArgs
> {
};

template <typename Base>
class TestCache: public Base {
public:
	typedef Base BaseType;
	typedef typename BaseType::Entry Entry;
	typedef typename BaseType::Iterator Iterator;

	TestCache(
		size_t probationCapacity,
		size_t protectedCapacity
	):
		BaseType(probationCapacity, protectedCapacity) {}

	size_t
	getProbationCount() const {
		return this->m_probationList.getCount();
	}

	size_t
	getProtectedCount() const {
		return this->m_protectedList.getCount();
	}

	bool
	contains(uint_t key) const {
		return (bool)this->m_map.find(key);
	}

	void
	assertProbationKeys(
		const uint_t* keyArray,
		size_t count
	) const {
		AXL_TEST_ASSERT(this->m_probationList.getCount() == count);

		auto it = this->m_probationList.getHead();
		for (size_t i = 0; i < count; i++, it++) {
			AXL_TEST_ASSERT(it);
			AXL_TEST_ASSERT(it->getKey() == keyArray[i]);
		}

		AXL_TEST_ASSERT(!it);
	}

	void
	assertProtectedKeys(
		const uint_t* keyArray,
		size_t count
	) const {
		AXL_TEST_ASSERT(this->m_protectedList.getCount() == count);

		auto it = this->m_protectedList.getHead();
		for (size_t i = 0; i < count; i++, it++) {
			AXL_TEST_ASSERT(it);
			AXL_TEST_ASSERT(it->getKey() == keyArray[i]);
		}

		AXL_TEST_ASSERT(!it);
	}
};

template <typename Cache>
void
assertProbationKeys(
	Cache* cache,
	const uint_t* keyArray,
	size_t count
) {
	cache->assertProbationKeys(keyArray, count);
}

template <typename Cache>
void
assertProtectedKeys(
	Cache* cache,
	const uint_t* keyArray,
	size_t count
) {
	cache->assertProtectedKeys(keyArray, count);
}

template <typename Cache>
void
assertKeys(
	Cache* cache,
	const uint_t* keyArray,
	size_t count
) {
	AXL_TEST_ASSERT(cache->getCount() == count);

	typename Cache::ConstIterator it = cache->getHead();
	for (size_t i = 0; i < count; i++, it++) {
		AXL_TEST_ASSERT(it);
		AXL_TEST_ASSERT(it->getKey() == keyArray[i]);
	}

	AXL_TEST_ASSERT(!it);

	if (count) {
		AXL_TEST_ASSERT(cache->getHead()->getKey() == keyArray[0]);
		AXL_TEST_ASSERT(cache->getTail()->getKey() == keyArray[count - 1]);
	} else {
		AXL_TEST_ASSERT(!cache->getHead());
		AXL_TEST_ASSERT(!cache->getTail());
	}
}

template <typename Cache>
void
test_BasicPromotionAndEviction() {
	Cache cache(4, 2);

	for (uint_t i = 1; i <= 4; i++) {
		typename Cache::Iterator it = cache.add(i, i * 10);
		AXL_TEST_ASSERT(it);
	}

	AXL_TEST_ASSERT(cache.getCount() == 4);

	{
		uint_t keys[] = { 1, 2, 3, 4 };
		uint_t probation[] = { 4, 3, 2, 1 };
		assertKeys(&cache, keys, countof(keys));
		assertProbationKeys(&cache, probation, countof(probation));
		assertProtectedKeys(&cache, (const uint_t*)NULL, 0);
	}

	AXL_TEST_ASSERT(cache.find(2));
	{
		uint_t keys[] = { 1, 2, 3, 4 };
		uint_t probation[] = { 4, 3, 2, 1 };
		assertKeys(&cache, keys, countof(keys));
		assertProbationKeys(&cache, probation, countof(probation));
		assertProtectedKeys(&cache, (const uint_t*)NULL, 0);
	}

	AXL_TEST_ASSERT(cache.visit(2));
	{
		uint_t keys[] = { 1, 2, 3, 4 };
		uint_t probation[] = { 4, 3, 1 };
		uint_t protected_[] = { 2 };
		assertKeys(&cache, keys, countof(keys));
		assertProbationKeys(&cache, probation, countof(probation));
		assertProtectedKeys(&cache, protected_, countof(protected_));
	}

	AXL_TEST_ASSERT(cache.visit(3));
	{
		uint_t keys[] = { 1, 2, 3, 4 };
		uint_t probation[] = { 4, 1 };
		uint_t protected_[] = { 3, 2 };
		assertKeys(&cache, keys, countof(keys));
		assertProbationKeys(&cache, probation, countof(probation));
		assertProtectedKeys(&cache, protected_, countof(protected_));
	}

	AXL_TEST_ASSERT(cache.visit(4));
	{
		uint_t keys[] = { 1, 2, 3, 4 };
		uint_t probation[] = { 2, 1 };
		uint_t protected_[] = { 4, 3 };
		assertKeys(&cache, keys, countof(keys));
		assertProbationKeys(&cache, probation, countof(probation));
		assertProtectedKeys(&cache, protected_, countof(protected_));
	}

	AXL_TEST_ASSERT(cache.add(5, 50));
	AXL_TEST_ASSERT(cache.getCount() == 5);
	AXL_TEST_ASSERT(cache.contains(1));
	AXL_TEST_ASSERT(cache.contains(2));
	AXL_TEST_ASSERT(cache.contains(3));
	AXL_TEST_ASSERT(cache.contains(4));
	AXL_TEST_ASSERT(cache.contains(5));

	{
		uint_t keys[] = { 1, 2, 3, 4, 5 };
		uint_t probation[] = { 5, 2, 1 };
		uint_t protected_[] = { 4, 3 };
		assertKeys(&cache, keys, countof(keys));
		assertProbationKeys(&cache, probation, countof(probation));
		assertProtectedKeys(&cache, protected_, countof(protected_));
	}

	AXL_TEST_ASSERT(cache.add(6, 60));
	AXL_TEST_ASSERT(cache.add(7, 70));
	AXL_TEST_ASSERT(cache.getCount() == 6);
	AXL_TEST_ASSERT(!cache.contains(1));
	AXL_TEST_ASSERT(cache.contains(2));
	AXL_TEST_ASSERT(cache.contains(3));
	AXL_TEST_ASSERT(cache.contains(4));
	AXL_TEST_ASSERT(cache.contains(5));
	AXL_TEST_ASSERT(cache.contains(6));
	AXL_TEST_ASSERT(cache.contains(7));

	{
		uint_t keys[] = { 2, 3, 4, 5, 6, 7 };
		uint_t probation[] = { 7, 6, 5, 2 };
		uint_t protected_[] = { 4, 3 };
		assertKeys(&cache, keys, countof(keys));
		assertProbationKeys(&cache, probation, countof(probation));
		assertProtectedKeys(&cache, protected_, countof(protected_));
	}
}

template <typename Cache>
void
test_UpdateEraseClearAndResize() {
	Cache cache(3, 1);

	AXL_TEST_ASSERT(cache.add(1, 10));
	AXL_TEST_ASSERT(cache.add(2, 20));
	AXL_TEST_ASSERT(cache.add(3, 30));
	AXL_TEST_ASSERT(cache.getCount() == 3);

	typename Cache::Iterator it = cache.add(2, 200);
	AXL_TEST_ASSERT(it);
	AXL_TEST_ASSERT(it->m_value == 200);
	AXL_TEST_ASSERT(cache.getCount() == 3);
	AXL_TEST_ASSERT(cache.getProtectedCount() == 1);

	AXL_TEST_ASSERT(cache.eraseKey(1));
	AXL_TEST_ASSERT(!cache.eraseKey(1));
	AXL_TEST_ASSERT(cache.getCount() == 2);
	AXL_TEST_ASSERT(!cache.contains(1));

	AXL_TEST_ASSERT(cache.add(4, 40));
	AXL_TEST_ASSERT(cache.add(5, 50));
	AXL_TEST_ASSERT(cache.getCount() == 4);
	AXL_TEST_ASSERT(cache.contains(3));

	cache.setCapacity(2, 1);
	AXL_TEST_ASSERT(cache.getCount() == 3);
	AXL_TEST_ASSERT(!cache.contains(3));
	AXL_TEST_ASSERT(cache.getProbationCapacity() == 2);
	AXL_TEST_ASSERT(cache.getProtectedCapacity() == 1);

	cache.clear();
	AXL_TEST_ASSERT(cache.isEmpty());
	AXL_TEST_ASSERT(cache.getCount() == 0);
	AXL_TEST_ASSERT(!cache.getHead());
	AXL_TEST_ASSERT(!cache.getTail());
	AXL_TEST_ASSERT(cache.getProbationCount() == 0);
	AXL_TEST_ASSERT(cache.getProtectedCount() == 0);
	AXL_TEST_ASSERT(!cache.contains(2));
}

template <typename Cache>
void
test_ZeroCapacity() {
	Cache cache(1, 1);

	AXL_TEST_ASSERT(cache.isEmpty());
	AXL_TEST_ASSERT(cache.add(1, 10));
	AXL_TEST_ASSERT(cache.getCount() == 1);
	AXL_TEST_ASSERT(cache.add(2, 20));
	AXL_TEST_ASSERT(cache.getCount() == 1);
	AXL_TEST_ASSERT(!cache.contains(1));
	AXL_TEST_ASSERT(cache.contains(2));

	AXL_TEST_ASSERT(cache.visit(2));
	AXL_TEST_ASSERT(cache.getProbationCount() == 0);
	AXL_TEST_ASSERT(cache.getProtectedCount() == 1);

	AXL_TEST_ASSERT(cache.add(3, 30));
	AXL_TEST_ASSERT(cache.getCount() == 2);
	AXL_TEST_ASSERT(cache.contains(2));
	AXL_TEST_ASSERT(cache.contains(3));
}

template <typename Cache>
void
test_ProtectedFifo() {
	Cache cache(2, 2);

	AXL_TEST_ASSERT(cache.add(1, 10));
	AXL_TEST_ASSERT(cache.add(2, 20));
	AXL_TEST_ASSERT(cache.visit(1));
	AXL_TEST_ASSERT(cache.visit(2));
	{
		uint_t protected_[] = { 2, 1 };
		assertProtectedKeys(&cache, protected_, countof(protected_));
	}

	AXL_TEST_ASSERT(cache.visit(1));
	{
		uint_t protected_[] = { 2, 1 };
		assertProtectedKeys(&cache, protected_, countof(protected_));
	}

	AXL_TEST_ASSERT(cache.add(3, 30));
	AXL_TEST_ASSERT(cache.visit(3));
	{
		uint_t probation[] = { 1 };
		uint_t protected_[] = { 3, 2 };
		assertProbationKeys(&cache, probation, countof(probation));
		assertProtectedKeys(&cache, protected_, countof(protected_));
	}
}

template <typename Cache>
void
test_ProtectedLru() {
	Cache cache(2, 2);

	AXL_TEST_ASSERT(cache.add(1, 10));
	AXL_TEST_ASSERT(cache.add(2, 20));
	AXL_TEST_ASSERT(cache.visit(1));
	AXL_TEST_ASSERT(cache.visit(2));
	{
		uint_t protected_[] = { 2, 1 };
		assertProtectedKeys(&cache, protected_, countof(protected_));
	}

	AXL_TEST_ASSERT(cache.visit(1));
	{
		uint_t protected_[] = { 1, 2 };
		assertProtectedKeys(&cache, protected_, countof(protected_));
	}

	AXL_TEST_ASSERT(cache.add(3, 30));
	AXL_TEST_ASSERT(cache.visit(3));
	{
		uint_t probation[] = { 2 };
		uint_t protected_[] = { 3, 1 };
		assertProbationKeys(&cache, probation, countof(probation));
		assertProtectedKeys(&cache, protected_, countof(protected_));
	}
}

template <typename Cache>
void
test_CacheType() {
	test_BasicPromotionAndEviction<Cache>();
	test_UpdateEraseClearAndResize<Cache>();
	test_ZeroCapacity<Cache>();
	test_ProtectedFifo<Cache>();
}

void
run() {
	typedef TestCache<sl::SlruCache<uint_t, uint_t, HashTableMap> > HashTableCache;
	typedef TestCache<sl::SlruCache<uint_t, uint_t, RobinHashTableMap> > RobinHashTableCache;
	typedef TestCache<sl::SlruCache<uint_t, uint_t, QuadraticHashTableMap> > QuadraticHashTableCache;
	typedef TestCache<sl::SlruCache<uint_t, uint_t, HashTableMap, true> > HashTableLruCache;
	typedef TestCache<sl::SlruCache<uint_t, uint_t, RobinHashTableMap, true> > RobinHashTableLruCache;
	typedef TestCache<sl::SlruCache<uint_t, uint_t, QuadraticHashTableMap, true> > QuadraticHashTableLruCache;

	test_CacheType<HashTableCache>();
	test_CacheType<RobinHashTableCache>();
	test_CacheType<QuadraticHashTableCache>();
	test_ProtectedLru<HashTableLruCache>();
	test_ProtectedLru<RobinHashTableLruCache>();
	test_ProtectedLru<QuadraticHashTableLruCache>();
}

//..............................................................................

AXL_TEST_ADD("test_SlruCache", run)

} // namespace
