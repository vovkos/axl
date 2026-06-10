//===- llvm/ADT/DenseMap.h - Dense probed hash table ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file defines the DenseMap class.
///
/// The hash table is linear-probing open addressing with tombstone-free
/// deletion (Knuth TAOCP 6.4 Algorithm R), power-of-two capacity, and a 0.75
/// maximum load factor. No sentinel key. Occupancy is stored in a packed
/// 1-bit-per-bucket "used" array.
///
/// `SmallDenseMap` adds an inline small buffer optimization.
///
//===----------------------------------------------------------------------===//
//
// Adapted for standalone use outside LLVM: all llvm/* header dependencies are
// replaced with self-contained stubs below.  Only the DenseMap and
// DenseMapInfo portions are retained; SmallDenseMap and the iterator are kept
// so the API is identical to the original.
//

#pragma once

// Trimmed for C++11: only the API exercised by the hash-table benchmark is kept
// live (default ctor, operator[], find, end, iterator, dtor). The rest of the
// LLVM DenseMap surface — which relies on C++14/17 (auto returns, if constexpr,
// structured bindings, _v traits, inline vars) — is fenced out with `#if 0`.

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <new>
#include <tuple>
#include <type_traits>
#include <utility>

//..............................................................................
// Stubs for LLVM-specific macros and utilities

#if defined(_MSC_VER)
#  define LLVM_ATTRIBUTE_NOINLINE   __declspec(noinline)
#  define LLVM_ATTRIBUTE_ALWAYS_INLINE __forceinline
#  include <intrin.h>
#else
#  define LLVM_ATTRIBUTE_NOINLINE   __attribute__((noinline))
#  define LLVM_ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline)) inline
#endif

#if defined(__GNUC__) || defined(__clang__)
#  define LLVM_LIKELY(x)   __builtin_expect(!!(x), 1)
#  define LLVM_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#  define LLVM_LIKELY(x)   (x)
#  define LLVM_UNLIKELY(x) (x)
#endif

namespace llvm {

//..............................................................................
// DebugEpochBase — no-op stub (full version adds iterator invalidation checks)

class DebugEpochBase {
public:
	void incrementEpoch() {}

	class HandleBase {
	public:
		HandleBase() {}
		HandleBase(const DebugEpochBase*) {}
		HandleBase(const HandleBase&) {}
		HandleBase& operator=(const HandleBase&) { return *this; }
		bool isHandleInSync() const { return true; }
		const DebugEpochBase* getEpochAddress() const { return nullptr; }
	};
};

//..............................................................................
// Math helpers (subset of llvm/Support/MathExtras.h)

inline uint64_t NextPowerOf2(uint64_t A) {
	A |= (A >> 1);
	A |= (A >> 2);
	A |= (A >> 4);
	A |= (A >> 8);
	A |= (A >> 16);
	A |= (A >> 32);
	return A + 1;
}

constexpr bool isPowerOf2_64(uint64_t Value) {
	return Value != 0 && (Value & (Value - 1)) == 0;
}

inline unsigned Log2_32_Ceil(uint32_t Value) {
	if (Value <= 1) return 0;
#if defined(_MSC_VER)
	unsigned long idx;
	_BitScanReverse(&idx, Value - 1);
	return (unsigned)(idx + 1);
#else
	return 32 - (unsigned)__builtin_clz(Value - 1);
#endif
}

// count trailing zeros (uint32_t)
inline unsigned countr_zero(uint32_t Val) {
#if defined(_MSC_VER)
	unsigned long idx;
	_BitScanForward(&idx, Val);
	return idx;
#else
	return __builtin_ctz(Val);
#endif
}

//..............................................................................
// Memory helpers (from llvm/Support/MemAlloc.h)

inline void* allocate_buffer(size_t Size, size_t Align) {
#if defined(_MSC_VER)
	return _aligned_malloc(Size, Align);
#else
	void* p = nullptr;
	if (posix_memalign(&p, Align, Size) != 0)
		p = nullptr;
	return p;
#endif
}

inline void deallocate_buffer(void* Ptr, size_t /*Size*/, size_t /*Align*/) {
#if defined(_MSC_VER)
	_aligned_free(Ptr);
#else
	free(Ptr);
#endif
}

//..............................................................................
// Minimal iterator_range + reverse (from llvm/ADT/STLExtras.h)

template <typename IterT>
class iterator_range {
	IterT B, E;
public:
	iterator_range(IterT b, IterT e) : B(b), E(e) {}
	IterT begin() const { return B; }
	IterT end()   const { return E; }
};

template <typename IterT>
iterator_range<IterT> make_range(IterT b, IterT e) {
	return {b, e};
}

#if 0 // unused: reverse()/map_range()/adl_* rely on C++14 auto-return deduction; the live
      // iterator never reverse-iterates (shouldReverseIterate is always false).
template <typename R>
iterator_range<std::reverse_iterator<decltype(std::begin(std::declval<R&>()))>>
reverse(R&& r) {
	using It = decltype(std::begin(r));
	return make_range(std::reverse_iterator<It>(std::end(r)),
	                  std::reverse_iterator<It>(std::begin(r)));
}

template <typename R, typename Fn>
struct mapped_iterator_range {
	R r; Fn fn;
	struct It {
		decltype(std::begin(std::declval<R&>())) it; Fn fn;
		auto operator*()    const { return fn(*it); }
		It& operator++()    { ++it; return *this; }
		bool operator!=(const It& o) const { return it != o.it; }
	};
	It begin() const { return {std::begin(r), fn}; }
	It end()   const { return {std::end(r), fn}; }
};

template <typename R, typename Fn>
auto map_range(R&& r, Fn fn) {
	return mapped_iterator_range<R, Fn>{std::forward<R>(r), fn};
}

template <typename R> auto adl_begin(R&& r) { return std::begin(r); }
template <typename R> auto adl_end(R&& r)   { return std::end(r); }
#endif // reverse / map_range / adl_*

// shouldReverseIterate: false for all types we use
template <typename T> constexpr bool shouldReverseIterate() { return false; }

#if 0 // unused: from_range constructor is #if 0'd below; inline variable is C++17
// from_range_t (range-based constructor tag)
struct from_range_t {};
inline constexpr from_range_t from_range{};
#endif

//..............................................................................
// const_pointer_or_const_ref (from llvm/Support/type_traits.h)

template <typename T, typename Enable = void>
struct const_pointer_or_const_ref { using type = const T&; };
template <typename T>
struct const_pointer_or_const_ref<T*> { using type = const T*; };

//..............................................................................
// DenseMapPair (from llvm/ADT/DenseMap.h detail namespace)

namespace detail {

template <typename KeyT, typename ValueT>
struct DenseMapPair : std::pair<KeyT, ValueT> {
	using std::pair<KeyT, ValueT>::pair;

	KeyT&       getFirst()       { return std::pair<KeyT, ValueT>::first; }
	const KeyT& getFirst()  const { return std::pair<KeyT, ValueT>::first; }
	ValueT&     getSecond()       { return std::pair<KeyT, ValueT>::second; }
	const ValueT& getSecond() const { return std::pair<KeyT, ValueT>::second; }
};

} // namespace detail

//..............................................................................
// DenseMapInfo (from llvm/ADT/DenseMapInfo.h)

namespace densemap {
namespace detail {

inline uint64_t mix(uint64_t x) {
	x *= 0xbf58476d1ce4e5b9u;
	x ^= x >> 31;
	return x;
}

} // namespace detail
} // namespace densemap

namespace detail {

inline unsigned combineHashValue(unsigned a, unsigned b) {
	uint64_t x = (uint64_t)a << 32 | (uint64_t)b;
	return (unsigned)densemap::detail::mix(x);
}

} // end namespace detail

template<typename T, typename Enable = void>
struct DenseMapInfo {
	// static constexpr T getEmptyKey();
	// static unsigned getHashValue(const T &Val);
	// static bool isEqual(const T &LHS, const T &RHS);
};

#if 0 // unused: built-in DenseMapInfo specializations use if constexpr / _v traits (C++17).
      // The benchmark provides its own KeyInfoT (MyDenseMapInfo); the primary template above
      // is enough to satisfy the default template argument.
template<typename T>
struct DenseMapInfo<T*> {
	static constexpr uintptr_t Log2MaxAlign = 12;

	static constexpr T* getEmptyKey() {
		uintptr_t Val = static_cast<uintptr_t>(-1);
		Val <<= Log2MaxAlign;
		return reinterpret_cast<T*>(Val);
	}

	static unsigned getHashValue(const T* PtrVal) {
		return (unsigned)densemap::detail::mix(reinterpret_cast<uintptr_t>(PtrVal));
	}

	static bool isEqual(const T* LHS, const T* RHS) { return LHS == RHS; }
};

template<> struct DenseMapInfo<char> {
	static constexpr char getEmptyKey() { return ~0; }
	static unsigned getHashValue(const char& Val) { return Val * 37U; }
	static bool isEqual(const char& LHS, const char& RHS) { return LHS == RHS; }
};

template <typename T>
struct DenseMapInfo<
    T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, char>>> {
	static constexpr T getEmptyKey() { return std::numeric_limits<T>::max(); }

	static unsigned getHashValue(const T& Val) {
		if constexpr (std::is_unsigned_v<T> && sizeof(T) > sizeof(unsigned))
			return (unsigned)densemap::detail::mix(Val);
		else
			return static_cast<unsigned>(Val * static_cast<std::make_unsigned_t<T>>(37U));
	}

	static bool isEqual(const T& LHS, const T& RHS) { return LHS == RHS; }
};

template<typename T, typename U>
struct DenseMapInfo<std::pair<T, U>> {
	using Pair = std::pair<T, U>;
	using FirstInfo  = DenseMapInfo<T>;
	using SecondInfo = DenseMapInfo<U>;

	static constexpr Pair getEmptyKey() {
		return {FirstInfo::getEmptyKey(), SecondInfo::getEmptyKey()};
	}

	static unsigned getHashValue(const Pair& PairVal) {
		return detail::combineHashValue(FirstInfo::getHashValue(PairVal.first),
		                                SecondInfo::getHashValue(PairVal.second));
	}

	static bool isEqual(const Pair& LHS, const Pair& RHS) {
		return FirstInfo::isEqual(LHS.first, RHS.first) &&
		       SecondInfo::isEqual(LHS.second, RHS.second);
	}
};
#endif // built-in DenseMapInfo specializations

//..............................................................................
// DenseMap core (from llvm/ADT/DenseMap.h — copied verbatim, includes trimmed)

namespace densemap {
namespace detail {
using UsedT = uint32_t;

// Number of used words backing N buckets where N is zero or a power of two.
constexpr size_t usedWords(size_t N) {
	return (N + 31) / 32; // C++11 constexpr: single return (power-of-two assert dropped)
}

inline bool used(const UsedT* U, size_t I) {
	return (U[I >> 5] >> (I & 31)) & 1;
}
inline void setUsed(UsedT* U, size_t I) { U[I >> 5] |= UsedT(1) << (I & 31); }
inline void unsetUsed(UsedT* U, size_t I) {
	U[I >> 5] &= ~(UsedT(1) << (I & 31));
}

template <typename Fn>
LLVM_ATTRIBUTE_ALWAYS_INLINE void forEachUsed(const UsedT* U, unsigned N, Fn Func) {
	const unsigned NW = (unsigned)usedWords(N);
	for (unsigned W = 0; W != NW; ++W) {
		UsedT Bits = U[W];
		while (Bits) {
			Func((W << 5) + countr_zero(Bits));
			Bits &= Bits - 1;
		}
	}
}

template <typename BucketT> constexpr size_t allocAlign() {
	// C++11 constexpr: ternary instead of std::max (not constexpr until C++14)
	return alignof(BucketT) > alignof(UsedT) ? alignof(BucketT) : alignof(UsedT);
}
template <typename BucketT> size_t allocBytes(unsigned Num) {
	return sizeof(BucketT) * static_cast<size_t>(Num) +
	       usedWords(Num) * sizeof(UsedT);
}

} // namespace detail
} // namespace densemap

// Befriended below so DenseMapBase can expose its bucket-relocation callback
// erase to ValueHandleBase, the only caller that caches bucket pointers.
class ValueHandleBase {};

template <typename KeyT, typename ValueT,
          typename KeyInfoT = DenseMapInfo<KeyT>,
          typename Bucket = llvm::detail::DenseMapPair<KeyT, ValueT>,
          bool IsConst = false>
class DenseMapIterator;

template <typename DerivedT, typename KeyT, typename ValueT, typename KeyInfoT,
          typename BucketT>
class DenseMapBase : public DebugEpochBase {
	template <typename T>
	using const_arg_type_t = typename const_pointer_or_const_ref<T>::type;

	using UsedT = llvm::densemap::detail::UsedT;

public:
	using size_type = unsigned;
	using key_type = KeyT;
	using mapped_type = ValueT;
	using value_type = BucketT;

	using iterator = DenseMapIterator<KeyT, ValueT, KeyInfoT, BucketT>;
	using const_iterator = DenseMapIterator<KeyT, ValueT, KeyInfoT, BucketT, true>;

	inline iterator begin() {
		return iterator::makeBegin(getBuckets(), getUsed(), getNumBuckets(),
		                           empty(), *this);
	}
	inline iterator end() {
		return iterator::makeEnd(getBuckets(), getUsed(), getNumBuckets(), *this);
	}
	inline const_iterator begin() const {
		return const_iterator::makeBegin(getBuckets(), getUsed(), getNumBuckets(),
		                                 empty(), *this);
	}
	inline const_iterator end() const {
		return const_iterator::makeEnd(getBuckets(), getUsed(), getNumBuckets(),
		                               *this);
	}

#if 0 // unused: auto return-type deduction (C++14) + the #if 0'd map_range
	inline auto keys() {
		return map_range(*this, [](const BucketT& P) { return P.getFirst(); });
	}
	inline auto values() {
		return map_range(*this, [](const BucketT& P) { return P.getSecond(); });
	}
	inline auto keys() const {
		return map_range(*this, [](const BucketT& P) { return P.getFirst(); });
	}
	inline auto values() const {
		return map_range(*this, [](const BucketT& P) { return P.getSecond(); });
	}
#endif // keys / values

	bool empty() const { return getNumEntries() == 0; }
	unsigned size() const { return getNumEntries(); }

	void reserve(size_type NumEntries) {
		auto NumBuckets = getMinBucketToReserveForEntries(NumEntries);
		incrementEpoch();
		if (NumBuckets > getNumBuckets())
			grow(NumBuckets);
	}

	void clear() {
		incrementEpoch();
		if (getNumEntries() == 0)
			return;

		if (getNumEntries() * 4 < getNumBuckets() && getNumBuckets() > 64) {
			shrink_and_clear();
			return;
		}

		destroyAll();
		std::memset(getUsed(), 0,
		            llvm::densemap::detail::usedWords(getNumBuckets()) *
		                sizeof(UsedT));
		setNumEntries(0);
	}

	void shrink_and_clear() {
		std::pair<bool, unsigned> Plan = derived().planShrinkAndClear();
		bool Reallocate = Plan.first;
		unsigned NewNumBuckets = Plan.second;
		destroyAll();
		if (!Reallocate) {
			initEmpty();
			return;
		}
		derived().deallocateBuckets();
		initWithExactBucketCount(NewNumBuckets);
	}

	bool contains(const_arg_type_t<KeyT> Val) const {
		return doFind(Val) != nullptr;
	}

	size_type count(const_arg_type_t<KeyT> Val) const {
		return contains(Val) ? 1 : 0;
	}

	iterator find(const_arg_type_t<KeyT> Val) {
		return find_as(Val);
	}
	const_iterator find(const_arg_type_t<KeyT> Val) const {
		return find_as(Val);
	}

	template <class LookupKeyT>
	iterator find_as(const LookupKeyT& Val) {
		if (BucketT* Bucket = doFind(Val))
			return makeIterator(Bucket);
		return end();
	}
	template <class LookupKeyT>
	const_iterator find_as(const LookupKeyT& Val) const {
		if (const BucketT* Bucket = doFind(Val))
			return makeConstIterator(Bucket);
		return end();
	}

	ValueT lookup(const_arg_type_t<KeyT> Val) const {
		if (const BucketT* Bucket = doFind(Val))
			return Bucket->getSecond();
		return ValueT();
	}

	template <typename U = typename std::remove_cv<ValueT>::type>
	ValueT lookup_or(const_arg_type_t<KeyT> Val, U&& Default) const {
		if (const BucketT* Bucket = doFind(Val))
			return Bucket->getSecond();
		return Default;
	}

	ValueT& at(const_arg_type_t<KeyT> Val) {
		auto Iter = this->find(std::move(Val));
		assert(Iter != this->end() && "DenseMap::at failed due to a missing key");
		return Iter->second;
	}
	const ValueT& at(const_arg_type_t<KeyT> Val) const {
		auto Iter = this->find(std::move(Val));
		assert(Iter != this->end() && "DenseMap::at failed due to a missing key");
		return Iter->second;
	}

	std::pair<iterator, bool> insert(const std::pair<KeyT, ValueT>& KV) {
		return try_emplace_impl(KV.first, KV.second);
	}
	std::pair<iterator, bool> insert(std::pair<KeyT, ValueT>&& KV) {
		return try_emplace_impl(std::move(KV.first), std::move(KV.second));
	}

	template <typename... Ts>
	std::pair<iterator, bool> try_emplace(KeyT&& Key, Ts&&... Args) {
		return try_emplace_impl(std::move(Key), std::forward<Ts>(Args)...);
	}
	template <typename... Ts>
	std::pair<iterator, bool> try_emplace(const KeyT& Key, Ts&&... Args) {
		return try_emplace_impl(Key, std::forward<Ts>(Args)...);
	}

	template <typename LookupKeyT>
	std::pair<iterator, bool> insert_as(std::pair<KeyT, ValueT>&& KV,
	                                    const LookupKeyT& Val) {
		BucketT* TheBucket;
		if (LookupBucketFor(Val, TheBucket))
			return {makeIterator(TheBucket), false};

		TheBucket = findBucketForInsertion(Val, TheBucket);
		::new (&TheBucket->getFirst()) KeyT(std::move(KV.first));
		::new (&TheBucket->getSecond()) ValueT(std::move(KV.second));
		return {makeIterator(TheBucket), true};
	}

	template <typename InputIt> void insert(InputIt I, InputIt E) {
		for (; I != E; ++I) insert(*I);
	}
#if 0 // unused: references the #if 0'd adl_begin/adl_end
	template <typename Range> void insert_range(Range&& R) {
		insert(adl_begin(R), adl_end(R));
	}
#endif

	template <typename V>
	std::pair<iterator, bool> insert_or_assign(const KeyT& Key, V&& Val) {
		auto Ret = try_emplace(Key, std::forward<V>(Val));
		if (!Ret.second) Ret.first->second = std::forward<V>(Val);
		return Ret;
	}
	template <typename V>
	std::pair<iterator, bool> insert_or_assign(KeyT&& Key, V&& Val) {
		auto Ret = try_emplace(std::move(Key), std::forward<V>(Val));
		if (!Ret.second) Ret.first->second = std::forward<V>(Val);
		return Ret;
	}

	void eraseFromFilledBucket(BucketT* TheBucket) {
		eraseFromFilledBucket(TheBucket, [](BucketT&) {});
	}

	bool erase(const KeyT& Val) {
		BucketT* TheBucket = doFind(Val);
		if (!TheBucket) return false;
		eraseFromFilledBucket(TheBucket);
		return true;
	}
	void erase(iterator I) { eraseFromFilledBucket(&*I); }

	template <typename Predicate> bool remove_if(Predicate Pred) {
		UsedT* U = getUsed();
		unsigned NumBuckets = getNumBuckets();
		BucketT* B = getBuckets();
		bool Removed = false;
		for (unsigned I = 0; I != NumBuckets; ++I) {
			if (!llvm::densemap::detail::used(U, I)) continue;
			if (Pred(B[I])) {
				B[I].getSecond().~ValueT();
				B[I].getFirst().~KeyT();
				llvm::densemap::detail::unsetUsed(U, I);
				decrementNumEntries();
				Removed = true;
			}
		}
		if (Removed) {
			incrementEpoch();
			this->grow(NumBuckets);
		}
		return Removed;
	}

	ValueT& operator[](const KeyT& Key) {
		return lookupOrInsertIntoBucket(Key).first->second;
	}
	ValueT& operator[](KeyT&& Key) {
		return lookupOrInsertIntoBucket(std::move(Key)).first->second;
	}

	bool isPointerIntoBucketsArray(const void* Ptr) const {
		return Ptr >= getBuckets() && Ptr < getBucketsEnd();
	}
	const void* getPointerIntoBucketsArray() const {
		return getBuckets();
	}

	void swap(DerivedT& RHS) {
		this->incrementEpoch();
		RHS.incrementEpoch();
		derived().swapImpl(RHS);
	}

protected:
	DenseMapBase() = default;

	struct ExactBucketCount {};

	struct Rep {
		const BucketT* Buckets;
		const UsedT*   Used;
		unsigned       NumBuckets;
	};

	void initWithExactBucketCount(unsigned NewNumBuckets) {
		if (derived().allocateBuckets(NewNumBuckets))
			initEmpty();
		else
			setNumEntries(0);
	}

	void destroyAll() {
		if (std::is_trivially_destructible<KeyT>::value &&
		    std::is_trivially_destructible<ValueT>::value)
			return;

		if (getNumBuckets() == 0)
			return;

		BucketT* B = getBuckets();
		const UsedT* U = getUsed();
		const unsigned E = getNumBuckets();
		llvm::densemap::detail::forEachUsed(U, E, [&](unsigned I) {
			B[I].getSecond().~ValueT();
			B[I].getFirst().~KeyT();
		});
	}

	void initEmpty() {
		setNumEntries(0);
		assert((getNumBuckets() & (getNumBuckets() - 1)) == 0 &&
		       "# initial buckets must be a power of two!");
		if (getNumBuckets()) {
			std::memset(getUsed(), 0,
			            llvm::densemap::detail::usedWords(getNumBuckets()) *
			                sizeof(UsedT));
		}
	}

	unsigned getMinBucketToReserveForEntries(unsigned NumEntries) {
		if (NumEntries == 0) return 0;
		return (unsigned)NextPowerOf2(NumEntries * 4 / 3 + 1);
	}

	LLVM_ATTRIBUTE_NOINLINE void moveFrom(DerivedT& Other) {
		assert(getNumEntries() == 0 && "moveFrom requires an empty destination");
		BucketT* OtherB = Other.getBuckets();
		UsedT* OtherU = Other.getUsed();
		const unsigned E = Other.getNumBuckets();
		UsedT* U = getUsed();
		BucketT* B = getBuckets();
		const unsigned Mask = getNumBuckets() - 1;
		llvm::densemap::detail::forEachUsed(OtherU, E, [&](unsigned I) {
			unsigned BucketNo = KeyInfoT::getHashValue(OtherB[I].getFirst()) & Mask;
			while (llvm::densemap::detail::used(U, BucketNo))
				BucketNo = (BucketNo + 1) & Mask;
			BucketT* DestBucket = B + BucketNo;
			::new (&DestBucket->getFirst()) KeyT(std::move(OtherB[I].getFirst()));
			::new (&DestBucket->getSecond()) ValueT(std::move(OtherB[I].getSecond()));
			llvm::densemap::detail::setUsed(U, BucketNo);
			OtherB[I].getSecond().~ValueT();
			OtherB[I].getFirst().~KeyT();
		});
		setNumEntries(Other.getNumEntries());
		Other.derived().kill();
	}

	LLVM_ATTRIBUTE_NOINLINE void copyFrom(const DerivedT& other) {
		this->destroyAll();
		derived().deallocateBuckets();
		setNumEntries(0);
		if (!derived().allocateBuckets(other.getNumBuckets()))
			return;

		assert(&other != this);
		assert(getNumBuckets() == other.getNumBuckets());

		setNumEntries(other.getNumEntries());

		BucketT* Buckets = getBuckets();
		const BucketT* OtherBuckets = other.getBuckets();
		const unsigned NumBuckets = getNumBuckets();
		UsedT* U = getUsed();
		const UsedT* OtherU = other.getUsed();
		std::memcpy(U, OtherU,
		            llvm::densemap::detail::usedWords(NumBuckets) * sizeof(UsedT));
		if (std::is_trivially_copyable<KeyT>::value &&
		    std::is_trivially_copyable<ValueT>::value) {
			memcpy(reinterpret_cast<void*>(Buckets), OtherBuckets,
			       NumBuckets * sizeof(BucketT));
		} else {
			llvm::densemap::detail::forEachUsed(U, NumBuckets, [&](unsigned I) {
				::new (&Buckets[I].getFirst()) KeyT(OtherBuckets[I].getFirst());
				::new (&Buckets[I].getSecond()) ValueT(OtherBuckets[I].getSecond());
			});
		}
	}

private:
	friend class ValueHandleBase;

	template <typename OnMovedT>
	LLVM_ATTRIBUTE_NOINLINE void eraseFromFilledBucket(BucketT* TheBucket,
	                                                   OnMovedT&& OnMoved) {
		incrementEpoch();
		TheBucket->getSecond().~ValueT();
		TheBucket->getFirst().~KeyT();
		decrementNumEntries();

		BucketT* BucketsPtr = getBuckets();
		UsedT* U = getUsed();
		const unsigned Mask = getNumBuckets() - 1;
		unsigned I = (unsigned)(TheBucket - BucketsPtr);
		unsigned J = I;
		while (true) {
			J = (J + 1) & Mask;
			BucketT& BJ = BucketsPtr[J];
			if (!llvm::densemap::detail::used(U, J))
				break;
			auto Ideal = KeyInfoT::getHashValue(BJ.getFirst());
			if (((I - Ideal) & Mask) < ((J - Ideal) & Mask)) {
				BucketT& BI = BucketsPtr[I];
				::new (&BI.getFirst()) KeyT(std::move(BJ.getFirst()));
				::new (&BI.getSecond()) ValueT(std::move(BJ.getSecond()));
				BJ.getSecond().~ValueT();
				BJ.getFirst().~KeyT();
				OnMoved(BI);
				I = J;
			}
		}
		llvm::densemap::detail::unsetUsed(U, I);
	}

	DerivedT& derived() { return *static_cast<DerivedT*>(this); }
	const DerivedT& derived() const { return *static_cast<const DerivedT*>(this); }

	template <typename KeyArgT, typename... Ts>
	std::pair<BucketT*, bool> lookupOrInsertIntoBucket(KeyArgT&& Key, Ts&&... Args) {
		BucketT* TheBucket = nullptr;
		if (LookupBucketFor(Key, TheBucket))
			return {TheBucket, false};
		TheBucket = findBucketForInsertion(Key, TheBucket);
		::new (&TheBucket->getFirst()) KeyT(std::forward<KeyArgT>(Key));
		::new (&TheBucket->getSecond()) ValueT(std::forward<Ts>(Args)...);
		return {TheBucket, true};
	}

	template <typename KeyArgT, typename... Ts>
	std::pair<iterator, bool> try_emplace_impl(KeyArgT&& Key, Ts&&... Args) {
		std::pair<BucketT*, bool> R = lookupOrInsertIntoBucket(
		    std::forward<KeyArgT>(Key), std::forward<Ts>(Args)...);
		return {makeIterator(R.first), R.second};
	}

	iterator makeIterator(BucketT* TheBucket) {
		return iterator::makeIterator(TheBucket, getBuckets(), getUsed(),
		                              getNumBuckets(), *this);
	}
	const_iterator makeConstIterator(const BucketT* TheBucket) const {
		return const_iterator::makeIterator(TheBucket, getBuckets(), getUsed(),
		                                    getNumBuckets(), *this);
	}

	unsigned getNumEntries() const { return derived().getNumEntries(); }
	void setNumEntries(unsigned Num) { derived().setNumEntries(Num); }
	void incrementNumEntries() { setNumEntries(getNumEntries() + 1); }
	void decrementNumEntries() { setNumEntries(getNumEntries() - 1); }
	const BucketT* getBuckets() const { return derived().getBuckets(); }
	BucketT* getBuckets() { return derived().getBuckets(); }
	Rep getRep() const { return derived().getRep(); }
	const UsedT* getUsed() const { return derived().getUsed(); }
	UsedT* getUsed() { return derived().getUsed(); }
	unsigned getNumBuckets() const { return derived().getNumBuckets(); }
	BucketT* getBucketsEnd() { return getBuckets() + getNumBuckets(); }
	const BucketT* getBucketsEnd() const { return getBuckets() + getNumBuckets(); }

	LLVM_ATTRIBUTE_NOINLINE void grow(unsigned MinNumBuckets) {
		unsigned NumBuckets = DerivedT::roundUpNumBuckets(MinNumBuckets);
		DerivedT Tmp(NumBuckets, ExactBucketCount{});
		Tmp.moveFrom(derived());
		if (derived().maybeMoveFast(std::move(Tmp)))
			return;
		initWithExactBucketCount(NumBuckets);
		moveFrom(Tmp);
	}

	template <typename LookupKeyT>
	BucketT* findBucketForInsertion(const LookupKeyT& Lookup, BucketT* TheBucket) {
		incrementEpoch();

		unsigned NewNumEntries = getNumEntries() + 1;
		unsigned NumBuckets = getNumBuckets();
		if (LLVM_UNLIKELY(NewNumEntries * 4 >= NumBuckets * 3)) {
			this->grow(NumBuckets * 2);
			LookupBucketFor(Lookup, TheBucket);
		}
		assert(TheBucket);

		llvm::densemap::detail::setUsed(getUsed(), TheBucket - getBuckets());
		incrementNumEntries();
		return TheBucket;
	}

	template <typename LookupKeyT>
	const BucketT* doFind(const LookupKeyT& Val) const {
		Rep R = getRep();
		const BucketT* BucketsPtr = R.Buckets;
		const UsedT* U = R.Used;
		unsigned NumBuckets = R.NumBuckets;
		if (NumBuckets == 0)
			return nullptr;

		const unsigned Mask = NumBuckets - 1;
		unsigned BucketNo = KeyInfoT::getHashValue(Val) & Mask;
		while (true) {
			if (LLVM_LIKELY(!llvm::densemap::detail::used(U, BucketNo)))
				return nullptr;
			const BucketT* Bucket = BucketsPtr + BucketNo;
			if (LLVM_LIKELY(KeyInfoT::isEqual(Val, Bucket->getFirst())))
				return Bucket;
			BucketNo = (BucketNo + 1) & Mask;
		}
	}

	template <typename LookupKeyT> BucketT* doFind(const LookupKeyT& Val) {
		return const_cast<BucketT*>(
		    static_cast<const DenseMapBase*>(this)->doFind(Val));
	}

	template <typename LookupKeyT>
	bool LookupBucketFor(const LookupKeyT& Val, BucketT*& FoundBucket) {
		Rep R = getRep();
		const UsedT* U = R.Used;
		unsigned NumBuckets = R.NumBuckets;
		if (NumBuckets == 0) {
			FoundBucket = nullptr;
			return false;
		}
		BucketT* BucketsPtr = const_cast<BucketT*>(R.Buckets);

		const unsigned Mask = NumBuckets - 1;
		unsigned BucketNo = KeyInfoT::getHashValue(Val) & Mask;
		while (true) {
			BucketT* ThisBucket = BucketsPtr + BucketNo;
			if (LLVM_LIKELY(!llvm::densemap::detail::used(U, BucketNo))) {
				FoundBucket = ThisBucket;
				return false;
			}
			if (LLVM_LIKELY(KeyInfoT::isEqual(Val, ThisBucket->getFirst()))) {
				FoundBucket = ThisBucket;
				return true;
			}
			BucketNo = (BucketNo + 1) & Mask;
		}
	}

public:
	size_t getMemorySize() const {
		return llvm::densemap::detail::allocBytes<BucketT>(getNumBuckets());
	}
};

template <typename KeyT, typename ValueT,
          typename KeyInfoT = DenseMapInfo<KeyT>,
          typename BucketT = llvm::detail::DenseMapPair<KeyT, ValueT>>
class DenseMap : public DenseMapBase<DenseMap<KeyT, ValueT, KeyInfoT, BucketT>,
                                     KeyT, ValueT, KeyInfoT, BucketT> {
	friend class DenseMapBase<DenseMap, KeyT, ValueT, KeyInfoT, BucketT>;

	using BaseT = DenseMapBase<DenseMap, KeyT, ValueT, KeyInfoT, BucketT>;
	using UsedT = llvm::densemap::detail::UsedT;

	BucketT* Buckets = nullptr;
	UsedT*   Used    = nullptr;
	unsigned NumEntries  = 0;
	unsigned NumBuckets  = 0;

	explicit DenseMap(unsigned NumBuckets, typename BaseT::ExactBucketCount) {
		this->initWithExactBucketCount(NumBuckets);
	}

public:
	explicit DenseMap(unsigned NumElementsToReserve = 0)
	    : DenseMap(BaseT::getMinBucketToReserveForEntries(NumElementsToReserve),
	               typename BaseT::ExactBucketCount{}) {}

	DenseMap(const DenseMap& other) : DenseMap() { this->copyFrom(other); }
	DenseMap(DenseMap&& other) : DenseMap() { this->swap(other); }

	template <typename InputIt>
	DenseMap(const InputIt& I, const InputIt& E)
	    : DenseMap((unsigned)std::distance(I, E)) {
		this->insert(I, E);
	}

#if 0 // unused: depends on the #if 0'd from_range_t tag and adl_begin/adl_end
	template <typename RangeT>
	DenseMap(llvm::from_range_t, const RangeT& Range)
	    : DenseMap(adl_begin(Range), adl_end(Range)) {}
#endif

	DenseMap(std::initializer_list<typename BaseT::value_type> Vals)
	    : DenseMap(Vals.begin(), Vals.end()) {}

	~DenseMap() {
		this->destroyAll();
		deallocateBuckets();
	}

	DenseMap& operator=(const DenseMap& other) {
		if (&other != this) this->copyFrom(other);
		return *this;
	}
	DenseMap& operator=(DenseMap&& other) {
		this->destroyAll();
		deallocateBuckets();
		this->initWithExactBucketCount(0);
		this->swap(other);
		return *this;
	}

private:
	void swapImpl(DenseMap& RHS) {
		std::swap(Buckets, RHS.Buckets);
		std::swap(Used, RHS.Used);
		std::swap(NumEntries, RHS.NumEntries);
		std::swap(NumBuckets, RHS.NumBuckets);
	}

	unsigned getNumEntries() const { return NumEntries; }
	void setNumEntries(unsigned Num) { NumEntries = Num; }

	BucketT* getBuckets() const { return Buckets; }

	typename BaseT::Rep getRep() const { return {Buckets, Used, NumBuckets}; }

	UsedT* getUsed() const { return Used; }

	unsigned getNumBuckets() const { return NumBuckets; }

	void deallocateBuckets() {
		if (NumBuckets == 0) return;
		deallocate_buffer(Buckets,
		                  llvm::densemap::detail::allocBytes<BucketT>(NumBuckets),
		                  llvm::densemap::detail::allocAlign<BucketT>());
		Buckets = nullptr;
		Used    = nullptr;
		NumBuckets = 0;
	}

	bool allocateBuckets(unsigned Num) {
		NumBuckets = Num;
		if (NumBuckets == 0) {
			Buckets = nullptr;
			Used    = nullptr;
			return false;
		}
		auto* Storage = static_cast<char*>(
		    allocate_buffer(llvm::densemap::detail::allocBytes<BucketT>(NumBuckets),
		                    llvm::densemap::detail::allocAlign<BucketT>()));
		Buckets = reinterpret_cast<BucketT*>(Storage);
		assert(sizeof(BucketT) * NumBuckets % alignof(UsedT) == 0 &&
		       "used array would be misaligned");
		Used = reinterpret_cast<UsedT*>(Storage + sizeof(BucketT) * NumBuckets);
		return true;
	}

	void kill() { deallocateBuckets(); }

	static unsigned roundUpNumBuckets(unsigned MinNumBuckets) {
		return std::max(64u, static_cast<unsigned>(NextPowerOf2(MinNumBuckets - 1)));
	}

	bool maybeMoveFast(DenseMap&& Other) {
		swapImpl(Other);
		return true;
	}

	std::pair<bool, unsigned> planShrinkAndClear() const {
		unsigned NewNumBuckets = 0;
		if (NumEntries)
			NewNumBuckets = std::max(64u, 1u << (Log2_32_Ceil(NumEntries) + 1));
		if (NewNumBuckets == NumBuckets)
			return {false, 0};
		return {true, NewNumBuckets};
	}
};

//..............................................................................
// DenseMapIterator

template <typename KeyT, typename ValueT, typename KeyInfoT, typename Bucket,
          bool IsConst>
class DenseMapIterator : DebugEpochBase::HandleBase {
	friend class DenseMapIterator<KeyT, ValueT, KeyInfoT, Bucket, true>;
	friend class DenseMapIterator<KeyT, ValueT, KeyInfoT, Bucket, false>;

	using UsedT = llvm::densemap::detail::UsedT;

public:
	using difference_type   = ptrdiff_t;
	using value_type        = typename std::conditional<IsConst, const Bucket, Bucket>::type;
	using pointer           = value_type*;
	using reference         = value_type&;
	using iterator_category = std::forward_iterator_tag;

private:
	// shouldReverseIterate<KeyT>() is always false, so the bucket cursor is a plain pointer.
	using BucketItTy = pointer;

	BucketItTy Ptr    = {};
	BucketItTy End    = {};
	pointer    Buckets = {};
	const UsedT* Used  = {};

	DenseMapIterator(BucketItTy Pos, BucketItTy E, pointer BucketsBase,
	                 const UsedT* U, const DebugEpochBase& Epoch)
	    : DebugEpochBase::HandleBase(&Epoch), Ptr(Pos), End(E),
	      Buckets(BucketsBase), Used(U) {
		assert(isHandleInSync() && "invalid construction!");
	}

public:
	DenseMapIterator() = default;

	static DenseMapIterator makeBegin(pointer Buckets, const UsedT* Used,
	                                  unsigned NumBuckets, bool IsEmpty,
	                                  const DebugEpochBase& Epoch) {
		if (IsEmpty)
			return makeEnd(Buckets, Used, NumBuckets, Epoch);
		auto R = maybeReverse(make_range(Buckets, Buckets + NumBuckets));
		DenseMapIterator Iter(R.begin(), R.end(), Buckets, Used, Epoch);
		Iter.AdvancePastEmptyBuckets();
		return Iter;
	}

	static DenseMapIterator makeEnd(pointer Buckets, const UsedT* Used,
	                                unsigned NumBuckets,
	                                const DebugEpochBase& Epoch) {
		auto R = maybeReverse(make_range(Buckets, Buckets + NumBuckets));
		return DenseMapIterator(R.end(), R.end(), Buckets, Used, Epoch);
	}

	static DenseMapIterator makeIterator(pointer P, pointer Buckets,
	                                     const UsedT* Used, unsigned NumBuckets,
	                                     const DebugEpochBase& Epoch) {
		auto R = maybeReverse(make_range(Buckets, Buckets + NumBuckets));
		constexpr int Offset = shouldReverseIterate<KeyT>() ? 1 : 0;
		return DenseMapIterator(BucketItTy(P + Offset), R.end(), Buckets, Used, Epoch);
	}

	template <bool IsConstSrc, typename = typename std::enable_if<!IsConstSrc && IsConst>::type>
	DenseMapIterator(
	    const DenseMapIterator<KeyT, ValueT, KeyInfoT, Bucket, IsConstSrc>& I)
	    : DebugEpochBase::HandleBase(I), Ptr(I.Ptr), End(I.End),
	      Buckets(I.Buckets), Used(I.Used) {}

	reference operator*() const {
		assert(isHandleInSync() && "invalid iterator access!");
		assert(Ptr != End && "dereferencing end() iterator");
		return *Ptr;
	}
	pointer operator->() const { return &operator*(); }

	friend bool operator==(const DenseMapIterator& LHS,
	                                     const DenseMapIterator& RHS) {
		return LHS.Ptr == RHS.Ptr;
	}
	friend bool operator!=(const DenseMapIterator& LHS,
	                                     const DenseMapIterator& RHS) {
		return !(LHS == RHS);
	}

	inline DenseMapIterator& operator++() {
		assert(isHandleInSync() && "invalid iterator access!");
		assert(Ptr != End && "incrementing end() iterator");
		++Ptr;
		AdvancePastEmptyBuckets();
		return *this;
	}
	DenseMapIterator operator++(int) {
		DenseMapIterator tmp = *this;
		++*this;
		return tmp;
	}

private:
	void AdvancePastEmptyBuckets() {
		// forward-only (shouldReverseIterate<KeyT>() is always false)
		const size_t N = (size_t)(End - Buckets);
		size_t I = (size_t)(Ptr - Buckets);
		if (I >= N) { Ptr = End; return; }
		const size_t NW = llvm::densemap::detail::usedWords(N);
		size_t W = I >> 5;
		UsedT Bits = Used[W] & (~UsedT(0) << (I & 31));
		while (Bits == 0) {
			if (++W == NW) { Ptr = End; return; }
			Bits = Used[W];
		}
		Ptr = Buckets + ((W << 5) + countr_zero(Bits));
	}

	static iterator_range<pointer> maybeReverse(iterator_range<pointer> Range) {
		return Range; // never reverse-iterates
	}
};

template <typename KeyT, typename ValueT, typename KeyInfoT>
inline size_t
capacity_in_bytes(const DenseMap<KeyT, ValueT, KeyInfoT>& X) {
	return X.getMemorySize();
}

} // end namespace llvm

//..............................................................................

namespace axl {
namespace sl {

template <
	typename KeyT,
	typename ValueT,
	typename KeyInfoT = llvm::DenseMapInfo<KeyT>,
	typename BucketT = llvm::detail::DenseMapPair<KeyT, ValueT>
>
using LlvmDenseMap = llvm::DenseMap<KeyT, ValueT, KeyInfoT, BucketT>;

using llvm::DenseMapInfo;

} // namespace sl
} // namespace axl
